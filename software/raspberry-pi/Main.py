import serial
import time
import paho.mqtt.client as mqtt
import os
from gpiozero import DigitalOutputDevice
from ChessAgent import *

# Motor 1 (Horizontal Axis)
STEP_PIN_1 = 13
DIR_PIN_1 = 12
ENABLE_PIN_1 = 1

# Electromagnet Pins (L298N)
MAGNET_IN1 = 7
MAGNET_IN2 = 8

# Motor 2 (Vertical Axis)
STEP_PIN_2 = 21
DIR_PIN_2 = 26
ENABLE_PIN_2 = 16

# Microstepping Pins (for DRV8825)
MS1_PIN_M1 = 18  # M0
MS2_PIN_M1= 23  # M1
MS3_PIN_M1 = 24  # M2

# Motor 2 Microstepping Pins (added as per your request)
MS1_PIN_M2 = 17  # M0
MS2_PIN_M2 = 27  # M1
MS3_PIN_M2 = 22  # M2

# --- MQTT Configuration ---
MQTT_BROKER = os.getenv("CHESS_MQTT_BROKER", "broker.example.com")
MQTT_PORT = int(os.getenv("CHESS_MQTT_PORT", "1883"))
MQTT_USERNAME = os.getenv("CHESS_MQTT_USERNAME", "")
MQTT_PASSWORD = os.getenv("CHESS_MQTT_PASSWORD", "")
ESP_TOPIC = "ESP-TOPIC"
RPI_TOPIC = "RPI-TOPIC"

# --- Serial Configuration ---
ARDUINO_PORT = os.getenv("CHESS_ARDUINO_PORT", "/dev/ttyUSB0")
BAUD_RATE = 9600

# --- Game State Initialization ---
gameState = [
    ["bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"],
    ["bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"],
    ["wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"]
]

class ChessSystem:
    def __init__(self):
        self.mode = None  # Changed from "" to None to indicate no mode selected
        self.mqtt_client = None
        self.ser = None
        self.setup_complete = False
        self.current_position = {'col': 0, 'row': 0}
        self.magnet_in1 = DigitalOutputDevice(MAGNET_IN1)
        self.magnet_in2 = DigitalOutputDevice(MAGNET_IN2)
        self.chess_agent = ChessAgent()
        self.agent_turn = False
        self.game_active = False  # Added to track if game is active
        self.initialize_motors()  # Initialize motors
        self.letter_to_index = {chr(i): i - 97 for i in range(97, 106)}  # a-i → 0-8
        # Add number_to_index support for row 4
        self.number_to_index = {str(i): i - 1 for i in range(1, 9)} 

    def magnet_on_north(self):
        self.magnet_in1.off()
        self.magnet_in2.on()

    def magnet_off(self):
        self.magnet_in1.off()
        self.magnet_in2.off()

        
    def initialize_motors(self):
        """Initialize stepper motor control"""
        # Motor 1 (Horizontal)
        self.motor1 = {
            "step": DigitalOutputDevice(STEP_PIN_1),
            "dir": DigitalOutputDevice(DIR_PIN_1),
            "enable": DigitalOutputDevice(ENABLE_PIN_1),
            "microsteps": 4  # 1/4 microstepping
        }
        
        # Motor 2 (Vertical)
        self.motor2 = {
            "step": DigitalOutputDevice(STEP_PIN_2),
            "dir": DigitalOutputDevice(DIR_PIN_2),
            "enable": DigitalOutputDevice(ENABLE_PIN_2),
            "microsteps": 4  # Now also using 1/4 microstepping
        }
        
        # Microstepping setup for Motor 1
        self.ms1_m1 = DigitalOutputDevice(MS1_PIN_M1)
        self.ms2_m1 = DigitalOutputDevice(MS2_PIN_M1)
        self.ms3_m1 = DigitalOutputDevice(MS3_PIN_M1)
        
        # Microstepping setup for Motor 2 (added)
        self.ms1_m2 = DigitalOutputDevice(MS1_PIN_M2)
        self.ms2_m2 = DigitalOutputDevice(MS2_PIN_M2)
        self.ms3_m2 = DigitalOutputDevice(MS3_PIN_M2)
        
        # Set 1/4 microstepping mode for both motors:
        # Motor 1: M0=0, M1=1, M2=0 (1/4 microstepping)
        self.ms1_m1.off()
        self.ms2_m1.on()
        self.ms3_m1.off()
        
        # Motor 2: M0=0, M1=1, M2=0 (1/4 microstepping)
        self.ms1_m2.off()
        self.ms2_m2.on()
        self.ms3_m2.off()
        
        # Motor settings
        self.PITCH_DIAMETER_CM = 1.27
        self.CIRCUMFERENCE_CM = 3.1415 * self.PITCH_DIAMETER_CM
        self.DISTANCE_PER_CELL_CM = 5.0
        
        # Chessboard mapping
        self.letter_to_index = {chr(i): i - 97 for i in range(97, 105)}  # a-h → 0-7
        self.number_to_index = {str(i): i - 1 for i in range(1, 9)}      # 1-8 → 0-7
        
        print("Stepper motors initialized with microstepping")
    def setup(self):
        """Initialize all connections"""
        self.connect_mqtt()
        self.connect_arduino()
        
        if self.ser:
            print("Initial chessboard setup:")
            self.print_game_state()
            self.setup_complete = True

    def connect_mqtt(self):
        """Connect to MQTT broker"""
        try:
            self.mqtt_client = mqtt.Client()
            self.mqtt_client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
            self.mqtt_client.on_connect = self.on_mqtt_connect
            self.mqtt_client.on_message = self.on_mqtt_message
            self.mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
            
            # Clear any retained messages
            self.mqtt_client.publish(ESP_TOPIC, payload=None, retain=True)
            
            self.mqtt_client.loop_start()
            print("MQTT connection initialized")
        except Exception as e:
            print(f"MQTT connection failed: {str(e)}")

    def connect_arduino(self):
        """Connect to Arduino via serial"""
        try:
            self.ser = serial.Serial(ARDUINO_PORT, BAUD_RATE)
            time.sleep(2)  # Wait for connection
            print("Arduino connection established")
        except Exception as e:
            print(f"Arduino connection failed: {str(e)}")

    def on_mqtt_connect(self, client, userdata, flags, rc):
        """MQTT connection callback"""
        if rc == 0:
            print(f"Connected to MQTT broker (RC: {rc})")
            client.subscribe(ESP_TOPIC)
        else:
            print(f"MQTT connection failed (RC: {rc})")

    def on_mqtt_message(self, client, userdata, msg):
        """Handle incoming MQTT messages from ESP-TOPIC"""
        message = msg.payload.decode().strip()
        print(f"MQTT message received on {msg.topic}: {message}")

        if msg.topic != ESP_TOPIC:
            return

        # Handle raw 4-character moves like "a5a6"
        if len(message) == 4 and message[0].isalpha() and message[1].isdigit() and message[2].isalpha() and message[3].isdigit():
            self.update_game_state(message, from_esp=True, from_arduino=False, from_ai=False)
        else:
            print(f"Unrecognized MQTT message format: {message}")

    def update_mode(self, new_mode):
        """Update game mode and initialize game accordingly"""
        new_mode = new_mode.strip()
        
        if new_mode not in ["HvH", "HvAI"]:
            print(f"Invalid game mode: {new_mode}")
            return
            
        if self.mode == new_mode:
            return  # No change needed
            
        print(f"Game mode updated: {new_mode}")
        
        # Clean up previous mode connections
        if self.mqtt_client:
            self.mqtt_client.disconnect()
            self.mqtt_client = None
            
        # Set new mode
        self.mode = new_mode
        self.game_active = True
        
        # Initialize mode-specific connections
        if self.mode == "HvH":
            self.connect_mqtt()
            self.agent_turn = False  # No AI in HvH mode
        else:  # HvAI
            self.agent_turn = True   # AI plays first as white
            # Trigger the agent's first move immediately
            self.get_agent_move()
                
        # If we have a serial connection, send mode confirmation
        if self.ser:
            self.ser.write(f"MODE:{self.mode}\n".encode())
            
    def get_agent_move(self):
        """Get a move from the chess agent and execute it"""
        if not self.agent_turn or self.mode != "HvAI":
            print("Not agent's turn or wrong mode")
            return
        print("Chess Agent is thinking...")
        
        # Update the agent's board state
        self.chess_agent.state = copy.deepcopy(gameState)
        
        # Get the best move from the agent (always playing as white/MAX)
        move, score = self.chess_agent.GetBestMove(self.chess_agent.MAX)
        
        if move:
            print(f"Agent plays: {move} (score: {score})")
            # Update game state and move the piece
            self.execute_agent_move(move)
            
        # Toggle turn to human player
        self.agent_turn = False
        
        # In execute_agent_move:
    def execute_agent_move(self, move):
        try:
            from_pos, to_pos = move[:2], move[2:]
            from_row, from_col = 8 - int(from_pos[1]), ord(from_pos[0]) - ord('a')
            to_row, to_col = 8 - int(to_pos[1]), ord(to_pos[0]) - ord('a')

            piece = gameState[from_row][from_col]
            if piece == "--":
                raise ValueError(f"No piece at {from_pos}")

            captured_piece = gameState[to_row][to_col]
            is_capture = captured_piece != "--" and captured_piece[0] == 'b'

            # Update game state and move the piece (pass from_ai=True!)
            self.update_game_state(move, from_esp=False, from_arduino=False, from_ai=True)

        except Exception as e:
            print(f"Error executing agent move: {str(e)}")
            
        
    def update_game_state(self, move, from_esp=False, from_arduino=False, from_ai=False):
        """Process and validate chess moves"""
        if not self.game_active or self.mode is None:
            print("Game not active - cannot process moves")
            return

        if len(move) != 4:
            print(f"Invalid move format: {move}")
            return

        try:
            from_pos, to_pos = move[:2], move[2:]
            from_row, from_col = 8 - int(from_pos[1]), ord(from_pos[0]) - ord('a')
            to_row, to_col = 8 - int(to_pos[1]), ord(to_pos[0]) - ord('a')

            # Validate board positions
            if not all(0 <= x <= 7 for x in [from_row, from_col, to_row, to_col]):
                raise ValueError("Invalid board coordinates")

            piece = gameState[from_row][from_col]
            if piece == "--":
                raise ValueError(f"No piece at {from_pos}")

            # Check for capture
            captured_piece = gameState[to_row][to_col]
            is_white_capture = captured_piece != "--" and piece[0] == 'w' and captured_piece[0] == 'b'

            if captured_piece != "--":
                # Determine which player is capturing
                if piece[0] == 'w':
                    capturing_color = "White"
                    captured_color = "Black"
                else:
                    capturing_color = "Black"
                    captured_color = "White"

                piece_names = {
                    'p': 'Pawn',
                    'R': 'Rook',
                    'N': 'Knight',
                    'B': 'Bishop',
                    'Q': 'Queen',
                    'K': 'King'
                }
                capturing_piece_name = piece_names.get(piece[1], 'Unknown')
                captured_piece_name = piece_names.get(captured_piece[1], 'Unknown')

                print(f"\n⚔️ Capture on {to_pos}:")
                print(f"{capturing_color} {capturing_piece_name} captured {captured_color} {captured_piece_name}")

            # Execute the move
            gameState[from_row][from_col] = "--"
            gameState[to_row][to_col] = piece

            print(f"\n📥 Received move: {move}")
            print(f"Moved {piece} from {from_pos} to {to_pos}")
            self.print_game_state()

            # ====== NEW: Send moves to Arduino if from AI in HvAI or from ESP in HvH ======
            if (self.mode == "HvAI" and from_ai) or (self.mode == "HvH" and from_esp):
                if self.ser:
                    print(f"Sending move to Arduino: {move}")
                    self.send_to_arduino(move)
            # ====== END NEW ======

            # Publish the move to RPI_TOPIC in HvH mode (from either Arduino or ESP)
            if self.mode == "HvH" and self.mqtt_client and (from_arduino or from_esp):
                self.mqtt_client.publish(RPI_TOPIC, move)
                print(f"📤 Published to {RPI_TOPIC}: {move}")

            # Move the physical piece (for ESP moves or AI)
            if from_esp or from_ai:
                if is_white_capture:
                    self.move_by_square(
                        move,
                        from_esp=from_esp,
                        from_ai=from_ai,
                        capture=True,
                        captured_square=move[2:4]
                    )
                else:
                    self.move_by_square(
                        move,
                        from_esp=from_esp,
                        from_ai=from_ai
                    )

            # If in HvAI mode and human moved, it's now the agent's turn
            if self.mode == "HvAI" and (from_esp or from_arduino):
                self.agent_turn = True
                time.sleep(1)  # Small delay before agent moves
                self.get_agent_move()

        except Exception as e:
            print(f"Move processing error: {str(e)}")


    def print_game_state(self):
        """Display the current chessboard"""
        print("\nCurrent Game State:")
        print("  +------------------------+")
        for i in range(8):
            print(f"{8-i} |", end=" ")
            for piece in gameState[i]:
                print(piece, end=" ")
            print("|")
        print("  +------------------------+")
        print("    a  b  c  d  e  f  g  h\n")

    # --- Stepper Motor Control Functions ---
    def get_steps_for_distance(self, distance_cm, circumference_cm, steps_per_rev):
        revolutions = distance_cm / circumference_cm
        return int(revolutions * steps_per_rev)

    def move_stepper(self, motor, steps, clockwise=True, magnet_control=False):
        """Move stepper motor with optional magnet control"""
        motor["enable"].off()
        motor["dir"].off() if clockwise else motor["dir"].on()
        
        # Only control magnet if explicitly requested
        if magnet_control:
            self.magnet_on_north()
        
        for _ in range(steps):
            motor["step"].on()
            time.sleep(0.0008)
            motor["step"].off()
            time.sleep(0.0008)

        # Only control magnet if explicitly requested
        if magnet_control:
            self.magnet_off()
        
        motor["enable"].on()
        
    def move_by_square(self, move, from_esp=False, from_ai=False, capture=False, captured_square=None):
        """
        Execute a chess move on the stepper motors.
        If capture is True:
            1. Go to captured square (to_square), pick up captured piece (magnet ON)
            2. Move captured piece to i4 (graveyard), drop it (magnet OFF)
            3. Return to source square (from_square), magnet OFF
            4. Pick up moving piece (magnet ON)
            5. Move to destination (to_square), drop moving piece (magnet OFF)
        """
        if not (from_esp or from_ai):
            print("Motor movement skipped - command not from ESP or AI")
            return

        if len(move) != 4:
            print("Invalid format. Use a2a4.")
            return

        from_square = move[0:2]
        to_square = move[2:4]

        col2 = self.letter_to_index.get(move[2])
        row2 = self.number_to_index.get(move[3])
        if col2 is None or row2 is None:
            print("Invalid target square notation.")
            return

        if capture and captured_square:
            graveyard = "i4"

            current_square = self.indices_to_square(self.current_position['col'], self.current_position['row'])

            # 1. Go to to_square (captured piece)
            print(f"Capturing: Moving to {to_square} to pick up captured piece.")
            if current_square != to_square:
                self.process_move(
                    f"{current_square}{to_square}",
                    from_esp=from_esp,
                    from_ai=from_ai,
                    magnet_on=False
                )
            self.magnet_on_north()

            # 2. Move captured piece to graveyard (i4) **with flipped magnet**
            print(f"Moving captured piece to graveyard at {graveyard} with magnet flipped to SOUTH.")
            self.magnet_on_south()  # FLIP DIRECTION!
            
            # SPECIAL HANDLING FOR ROW 4 CAPTURES
            if captured_square[1] == '4':  # Check if captured on row 4
                print("Applying 2.5cm offset for row 4 capture")
                offset_cm = 2.5
                offset_steps = self.get_steps_for_distance(offset_cm, self.CIRCUMFERENCE_CM, 200 * self.motor2["microsteps"])
                
                # Lift piece 2.5cm
                self.move_stepper(self.motor2, offset_steps, clockwise=False, magnet_control=False)
                time.sleep(0.2)
                
                # Move horizontally to graveyard
                self.process_move(
                    f"{to_square}{graveyard}",
                    from_esp=from_esp,
                    from_ai=from_ai,
                    magnet_on=True
                )
                
                # Lower piece 2.5cm
                self.move_stepper(self.motor2, offset_steps, clockwise=True, magnet_control=False)
                time.sleep(0.2)
            else:
                # Normal movement for non-row4 captures
                self.process_move(
                    f"{to_square}{graveyard}",
                    from_esp=from_esp,
                    from_ai=from_ai,
                    magnet_on=True
                )
            
            self.magnet_off()

            # 3. Return to from_square
            print(f"Returning to {from_square}.")
            self.process_move(
                f"{graveyard}{from_square}",
                from_esp=from_esp,
                from_ai=from_ai,
                magnet_on=False
            )

            # 4. Pick up moving piece (magnet ON north)
            print(f"Picking up moving piece at {from_square}.")
            self.magnet_on_north()

            # 5. Move to to_square and drop piece
            print(f"Placing moving piece at {to_square}.")
            self.process_move(
                f"{from_square}{to_square}",
                from_esp=from_esp,
                from_ai=from_ai,
                magnet_on=True
            )
            self.magnet_off()

            # Update current position
            self.current_position = {'col': col2, 'row': row2}
            return

    # ... rest of the method remains unchanged ...
        # NORMAL MOVE LOGIC
        current_square = self.indices_to_square(self.current_position['col'], self.current_position['row'])
        if move[0:2] == current_square:
            # Direct move - turn magnet ON immediately
            print(f"Direct move from current position: {move}")
            self.magnet_on_north()  # Turn magnet ON before moving
            self.process_move(move, from_esp=from_esp, from_ai=from_ai, magnet_on=True)
        else:
            # Need to move to starting position first (with magnet OFF)
            intermediate_move = f"{current_square}{move[0:2]}"
            print(f"Moving to starting position: {intermediate_move}")
            self.process_move(intermediate_move, from_esp=from_esp, from_ai=from_ai, magnet_on=False)
            # Then execute main move (with magnet ON)
            self.magnet_on_north()  # Turn magnet ON before moving
            self.process_move(move, from_esp=from_esp, from_ai=from_ai, magnet_on=True)

        # Update current position to the target position
        self.current_position = {'col': col2, 'row': row2}
        # Turn magnet OFF after reaching destination
        self.magnet_off()
        
            
        # 1. Add this helper method to the ChessSystem class if not already present:
    def indices_to_square(self, col, row):
        return f"{chr(col + 97)}{row + 1}"
    
        # 1. Add this to your ChessSystem class:
    def magnet_on_south(self):
        self.magnet_in1.on()
        self.magnet_in2.off()

    def process_move(self, move, from_esp=False, from_ai=False, magnet_on=False):
        """Process a single move command for the stepper motors"""
        if not (from_esp or from_ai):
            return
        # ... rest of the function remains the same ...

        col1 = self.letter_to_index.get(move[0])
        row1 = self.number_to_index.get(move[1])
        col2 = self.letter_to_index.get(move[2])
        row2 = self.number_to_index.get(move[3])

        if None in [col1, row1, col2, row2]:
            print("Invalid square notation.")
            return

        dx = col2 - col1
        dy = row2 - row1

        offset_cm = 2.5
        offset_steps_m1 = self.get_steps_for_distance(offset_cm, self.CIRCUMFERENCE_CM, 200 * self.motor1["microsteps"])
        offset_steps_m2 = self.get_steps_for_distance(offset_cm, self.CIRCUMFERENCE_CM, 200 * self.motor2["microsteps"])

        if dx == 0 and dy != 0:
            # Pure vertical move
            dist_y = abs(dy) * self.DISTANCE_PER_CELL_CM
            steps_y = self.get_steps_for_distance(dist_y, self.CIRCUMFERENCE_CM, 200 * self.motor2["microsteps"])
            cw_y = dy > 0
            self.move_stepper(self.motor2, steps_y, cw_y, magnet_control=False)
            time.sleep(1.0)  # Increased to 1 second delay to ensure piece is centered
            if magnet_on:
                time.sleep(1.0)  # Additional 1 second delay with magnet on

        elif dy == 0 and dx != 0:
            # Pure horizontal move
            dist_x = abs(dx) * self.DISTANCE_PER_CELL_CM
            steps_x = self.get_steps_for_distance(dist_x, self.CIRCUMFERENCE_CM, 200 * self.motor1["microsteps"])
            cw_x = dx < 0
            self.move_stepper(self.motor1, steps_x, cw_x, magnet_control=False)
            time.sleep(1.0)  # Increased to 1 second delay to ensure piece is centered
            if magnet_on:
                time.sleep(1.0)  # Additional 1 second delay with magnet on

        else:
            # Edge-riding move for dx ≠ 0 and dy ≠ 0
            # Step 1: Move 2.5 cm horizontally from center
            self.move_stepper(self.motor1, offset_steps_m1, clockwise=(dx < 0), magnet_control=False)
            time.sleep(0.2)

            # Step 2: Move vertically to (row - 2.5 cm)
            dist_y = abs(dy) * self.DISTANCE_PER_CELL_CM - offset_cm
            steps_y = self.get_steps_for_distance(dist_y, self.CIRCUMFERENCE_CM, 200 * self.motor2["microsteps"])
            cw_y = dy > 0
            self.move_stepper(self.motor2, steps_y, cw_y, magnet_control=False)
            time.sleep(0.2)

            # Step 3: Move horizontally to (column - 2.5 cm)
            dist_x = abs(dx) * self.DISTANCE_PER_CELL_CM - offset_cm
            steps_x = self.get_steps_for_distance(dist_x, self.CIRCUMFERENCE_CM, 200 * self.motor1["microsteps"])
            cw_x = dx < 0
            self.move_stepper(self.motor1, steps_x, cw_x, magnet_control=False)
            time.sleep(0.2)

            # Step 4: Final vertical move into the center of the target square
            self.move_stepper(self.motor2, offset_steps_m2, clockwise=(dy > 0), magnet_control=False)
            time.sleep(1.0)  # Increased to 1 second delay to ensure piece is centered
            if magnet_on:
                time.sleep(1.0)  # Additional 1 second delay with magnet on
                
    def send_to_arduino(self, message):
        """Send messages to Arduino via serial"""
        if self.ser and self.ser.is_open:
            try:
                formatted_message = f"{message}\n"  # Add newline terminator
                self.ser.write(formatted_message.encode())
                print(f"Sent to Arduino: {message}")
                print("We sent a move to Arduino.")  # <-- Added message here
            except serial.SerialException as e:
                print(f"Serial write error: {str(e)}")
            except Exception as e:
                print(f"Unexpected error during send: {str(e)}")

    def run(self):
        """Main execution loop with bidirectional serial communication"""
        if not self.setup_complete:
            print("System not properly initialized!")
            return
            
        try:
            while True:
                # --- Handle incoming Arduino data ---
                if self.ser and self.ser.in_waiting > 0:
                    try:
                        data = self.ser.readline().decode('utf-8').strip()
                        print(f"Arduino data: {data}")
                        
                        if "MODE:" in data:
                            mode = data.split(":")[1].strip()
                            self.update_mode(mode)
                        
                        elif len(data) == 4 and data[0].isalpha() and data[1].isdigit() and data[2].isalpha() and data[3].isdigit():
                            if self.mode is None:
                                print("Ignoring move - no game mode selected")
                            elif self.mode == "HvAI" and self.agent_turn:
                                print("Ignoring move - it's the agent's turn")
                            else:
                                self.update_game_state(data, from_esp=False, from_arduino=True)
                        
                        else:
                            print(f"Unrecognized data: {data}")
                            
                    except UnicodeDecodeError:
                        print("Serial decode error - skipping line")
                    except Exception as e:
                        print(f"Error processing serial data: {str(e)}")

                time.sleep(0.1)  # Maintain CPU efficiency

        except KeyboardInterrupt:
            print("\nShutting down...")
        finally:
            # Cleanup resources
            if self.mqtt_client:
                self.mqtt_client.loop_stop()
                self.mqtt_client.disconnect()
            if self.ser:
                self.ser.close()
            # Disable motors
            self.motor1["enable"].on()
            self.motor2["enable"].on()
            print("System shutdown complete")

if __name__ == "__main__":
    chess_system = ChessSystem()
    chess_system.setup()
    
    if chess_system.setup_complete:
        chess_system.run()
