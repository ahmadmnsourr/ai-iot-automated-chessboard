const int button1Pin = 2; // First button connected to digital pin 2
const int button2Pin = 3; // Second button connected to digital pin 3

void setup() {
  pinMode(button1Pin, INPUT_PULLUP); // Enable internal pull-up for button 1
  pinMode(button2Pin, INPUT_PULLUP); // Enable internal pull-up for button 2
  Serial.begin(9600); // Start serial communication
}

void loop() {
  int state1 = digitalRead(button1Pin);
  int state2 = digitalRead(button2Pin);

  if (state1 == LOW) {
    Serial.println("Button 1 Pressed!");
  }

  if (state2 == LOW) {
    Serial.println("Button 2 Pressed!");
  }

  delay(100); // Delay to reduce serial output spam
}
