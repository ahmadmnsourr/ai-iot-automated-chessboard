from gpiozero import DigitalOutputDevice
import time

# --- Motor 1 Pins ---
STEP_PIN_1   = 13
DIR_PIN_1    = 12

ENABLE_PIN_1 = 1

# --- Motor 2 Pins ---
STEP_PIN_2   = 21
DIR_PIN_2    = 26
ENABLE_PIN_2 = 16

# --- Motor Initialization ---
motor1 = {
    "step":   DigitalOutputDevice(STEP_PIN_1),
    "dir":    DigitalOutputDevice(DIR_PIN_1),
    "enable": DigitalOutputDevice(ENABLE_PIN_1),
    "active": False
}

motor2 = {
    "step":   DigitalOutputDevice(STEP_PIN_2),
    "dir":    DigitalOutputDevice(DIR_PIN_2),
    "enable": DigitalOutputDevice(ENABLE_PIN_2),
    "active": False
}

# --- Stepper Motor Settings ---
PITCH_DIAMETER_CM    = 1.4848
CIRCUMFERENCE_CM     = 3.1415 * PITCH_DIAMETER_CM
STEPS_PER_REVOLUTION = 200

def get_steps_for_distance(distance_cm):
    revolutions = distance_cm / CIRCUMFERENCE_CM
    return int(revolutions * STEPS_PER_REVOLUTION)

def move_stepper(motor, steps, clockwise=True):
    motor["enable"].off()
    motor["active"] = True
    motor["dir"].off() if clockwise else motor["dir"].on()

    for _ in range(steps):
        motor["step"].on()
        time.sleep(0.0008)
        motor["step"].off()
        time.sleep(0.0008)

    motor["enable"].on()
    motor["active"] = False

if __name__ == "__main__":
    try:
        print("Type 'exit' anytime to quit.")
        while True:
            motor_choice = input("Which motor to move? (1 or 2): ").strip()
            if motor_choice.lower() == "exit":
                break

            direction = input("Direction? (r = right / l = left): ").strip().lower()
            if direction == "exit":
                break

            dist_input = input("Distance in cm: ").strip()
            if dist_input.lower() == "exit":
                break

            try:
                distance_cm = float(dist_input)
                motor = motor1 if motor_choice == "1" else motor2
                clockwise = True if direction == "r" else False
                steps = get_steps_for_distance(distance_cm)

                print(f"Moving motor{motor_choice} {'right' if clockwise else 'left'} for {distance_cm} cm...")
                move_stepper(motor, steps, clockwise)
            except ValueError:
                print("Invalid input, try again.")

    except KeyboardInterrupt:
        print("\nInterrupted – stopping all outputs.")
    finally:
        motor1["enable"].on()
        motor2["enable"].on()
        motor1["active"] = False
        motor2["active"] = False
        print("Motors disabled. Exiting.")
