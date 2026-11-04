import time
from gpiozero import DigitalOutputDevice

class StepperController:
    def __init__(self):
        # Motor 1 (Horizontal Axis)
        self.STEP_PIN_1 = 13
        self.DIR_PIN_1 = 12
        self.ENABLE_PIN_1 = 1

        # Motor 2 (Vertical Axis)
        self.STEP_PIN_2 = 21
        self.DIR_PIN_2 = 26
        self.ENABLE_PIN_2 = 16

        # Microstepping Pins
        self.MS1_PIN_M1 = 18  # M0
        self.MS2_PIN_M1 = 23  # M1
        self.MS3_PIN_M1 = 24  # M2
        self.MS1_PIN_M2 = 17  # M0
        self.MS2_PIN_M2 = 27  # M1
        self.MS3_PIN_M2 = 22  # M2

        # Motor settings
        self.PITCH_DIAMETER_CM = 1.27
        self.CIRCUMFERENCE_CM = 3.1415 * self.PITCH_DIAMETER_CM
        self.DISTANCE_PER_CELL_CM = 5.0
        self.FIXED_DESTINATION = "a1"  # Destination is always a1

        # Chessboard mapping
        self.letter_to_index = {chr(i): i - 97 for i in range(97, 105)}  # a-h → 0-7
        self.number_to_index = {str(i): i - 1 for i in range(1, 9)}      # 1-8 → 0-7
        
        self.current_position = {'col': 0, 'row': 0}  # Start at a1 by default
        self.initialize_motors()
        print("Stepper motors initialized")

    def initialize_motors(self):
        """Initialize stepper motor control with microstepping"""
        # Motor 1 (Horizontal)
        self.motor1 = {
            "step": DigitalOutputDevice(self.STEP_PIN_1),
            "dir": DigitalOutputDevice(self.DIR_PIN_1),
            "enable": DigitalOutputDevice(self.ENABLE_PIN_1),
            "microsteps": 4
        }
        
        # Motor 2 (Vertical)
        self.motor2 = {
            "step": DigitalOutputDevice(self.STEP_PIN_2),
            "dir": DigitalOutputDevice(self.DIR_PIN_2),
            "enable": DigitalOutputDevice(self.ENABLE_PIN_2),
            "microsteps": 4
        }
        
        # Microstepping setup
        self.ms1_m1 = DigitalOutputDevice(self.MS1_PIN_M1)
        self.ms2_m1 = DigitalOutputDevice(self.MS2_PIN_M1)
        self.ms3_m1 = DigitalOutputDevice(self.MS3_PIN_M1)
        self.ms1_m2 = DigitalOutputDevice(self.MS1_PIN_M2)
        self.ms2_m2 = DigitalOutputDevice(self.MS2_PIN_M2)
        self.ms3_m2 = DigitalOutputDevice(self.MS3_PIN_M2)
        
        # Set 1/4 microstepping (M0=0, M1=1, M2=0)
        self.ms1_m1.off(); self.ms2_m1.on(); self.ms3_m1.off()
        self.ms1_m2.off(); self.ms2_m2.on(); self.ms3_m2.off()

    def get_steps_for_distance(self, distance_cm):
        """Calculate steps needed for distance"""
        revolutions = distance_cm / self.CIRCUMFERENCE_CM
        return int(revolutions * 200 * 4)  # 200 steps/rev * microsteps

    def move_stepper(self, motor, steps, clockwise=True):
        """Move stepper motor with direction control"""
        motor["enable"].off()
        motor["dir"].value = 0 if clockwise else 1
        
        for _ in range(steps):
            motor["step"].on()
            time.sleep(0.0008)
            motor["step"].off()
            time.sleep(0.0008)
        
        motor["enable"].on()

    def move_to_destination(self, start_square):
        """Move from start position to fixed destination (a1)"""
        # Convert start position to indices
        start_col = self.letter_to_index.get(start_square[0])
        start_row = self.number_to_index.get(start_square[1])
        
        if start_col is None or start_row is None:
            print("Invalid starting position!")
            return

        # Set current position to start
        self.current_position = {'col': start_col, 'row': start_row}
        print(f"Starting from position: {start_square}")
        
        # Create move command (start to a1)
        move_command = f"{start_square}{self.FIXED_DESTINATION}"
        self.process_move(move_command)
        print("Movement to a1 completed!")

    def process_move(self, move):
        """Execute movement between two squares"""
        if len(move) != 4:
            print("Invalid move format. Use like 'e2a1'")
            return

        # Parse move coordinates
        col1 = self.letter_to_index[move[0]]
        row1 = self.number_to_index[move[1]]
        col2 = self.letter_to_index[move[2]]
        row2 = self.number_to_index[move[3]]

        # Calculate move distances
        dx = col2 - col1
        dy = row2 - row1

        # Calculate steps needed
        dist_x = abs(dx) * self.DISTANCE_PER_CELL_CM
        dist_y = abs(dy) * self.DISTANCE_PER_CELL_CM
        steps_x = self.get_steps_for_distance(dist_x)
        steps_y = self.get_steps_for_distance(dist_y)

        # Execute moves based on direction
        if dx != 0:
            self.move_stepper(
                self.motor1,
                steps_x,
                clockwise=(dx < 0)  # Direction inverted for horizontal
            )
            time.sleep(0.5)
        
        if dy != 0:
            self.move_stepper(
                self.motor2,
                steps_y,
                clockwise=(dy > 0)
            )
            time.sleep(0.5)
        
        # Update current position
        self.current_position = {'col': col2, 'row': row2}

    def cleanup(self):
        """Clean up GPIO resources"""
        self.motor1["enable"].on()
        self.motor2["enable"].on()
        print("Motors disabled")

if __name__ == "__main__":
    controller = StepperController()
    
    try:
        # Get start position from user
        start_pos = input("Enter starting position (e.g. e2): ").strip().lower()
        if len(start_pos) != 2 or not start_pos[0].isalpha() or not start_pos[1].isdigit():
            print("Invalid input! Use format like 'e2'")
        else:
            controller.move_to_destination(start_pos)
    finally:
        controller.cleanup()