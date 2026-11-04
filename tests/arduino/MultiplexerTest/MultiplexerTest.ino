// Multiplexer control pins
const int S0 = 2;
const int S1 = 3;
const int S2 = 4;
const int S3 = 5;

// Multiplexer signal pins
const int SIG[] = {A0, A1, A2, A3, A5}; // MUX0 to MUX4

// Thresholds for Hall effect sensor (SS49E)
const int NEUTRAL_MIN = 420;
const int NEUTRAL_MAX = 560;

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
}

void loop() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 9; col++) {
      int index = col * 8 + row; // Column-major indexing
      int mux = index / 16;
      int ch = index % 16;

      selectChannel(ch);
      int value = analogRead(SIG[mux]);

      // Interpret Hall sensor value
      if (value < NEUTRAL_MIN) {
        Serial.print("b"); // Strong south polarity — black piece
      } else if (value > NEUTRAL_MAX) {
        Serial.print("w"); // Strong north polarity — white piece
      } else {
        Serial.print("-"); // Neutral zone — no piece
      }

      Serial.print(col < 8 ? " " : "\n");
    }
  }

  Serial.println(); // Blank line between frames
  delay(1000); // Delay for readability
}

void selectChannel(int channel) {
  digitalWrite(S0, channel & 0x01);
  digitalWrite(S1, (channel >> 1) & 0x01);
  digitalWrite(S2, (channel >> 2) & 0x01);
  digitalWrite(S3, (channel >> 3) & 0x01);
}
