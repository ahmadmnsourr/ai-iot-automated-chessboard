const int stepPin = 3;
const int dirPin = 4;
const int enablePin = 5;  // Add this line

const float pitchDiameter = 1.2;           // cm
const float circumference = 3.1416 * pitchDiameter;
const int stepsPerRevolution = 200;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);              // Setup enable pin

  digitalWrite(enablePin, LOW);            // Enable driver

  int stepsToMove = getStepsForDistance(10);
  moveStepper(stepsToMove, true);

  delay(1000);

  stepsToMove = getStepsForDistance(10);
  moveStepper(stepsToMove, false);

  digitalWrite(enablePin, HIGH);           // Disable driver after movement
}

void loop() {
  // Idle
}

int getStepsForDistance(float distance_cm) {
  float revolutions = distance_cm / circumference;
  return int(revolutions * stepsPerRevolution);
}

void moveStepper(int steps, bool clockwise) {
  digitalWrite(enablePin, LOW);                      // Enable before moving
  digitalWrite(dirPin, clockwise ? LOW : HIGH);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }

  digitalWrite(enablePin, HIGH);                     // Disable after moving
}
