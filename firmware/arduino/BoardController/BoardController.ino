#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "5-Mux.h"  // Multiplexer and board reading logic

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int button1Pin = 6;  // Mode cycle or scan
const int button2Pin = 7;  // Confirm

int currentMode = 1;       // 1: HvAI, 2: HvH
bool configSent = false;

bool lastButton1State = LOW;
bool lastButton2State = LOW;

// Variables for Pi communication
unsigned long lastMessageTime = 0;
const unsigned long MESSAGE_DISPLAY_TIME = 3000;  // 3 seconds
bool showingPiMessage = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  initBoardReader();  // Setup for multiplexer board

  // Show mode selection on startup
  lcd.setCursor(0, 0);
  lcd.print("Select Mode:");
  lcd.setCursor(0, 1);
  lcd.print(currentMode == 1 ? "Mode: HvAI" : "Mode: HvH");

  // Send ready signal to Pi
  Serial.println("READY");
}

bool isAlpha(char c) { return (c >= 'a' && c <= 'h'); }
bool isDigit(char c) { return (c >= '1' && c <= '8'); }

// Apply a move to the board matrix
void applyMoveToBoard(String move) {
  // Expects move in "a2a3" format
  if (move.length() != 4) return;

  int srcCol = 'h' - move[0]; // 'a'-'h' mapped to 7-0
  int srcRow = 8 - (move[1] - '0'); // '1'-'8' mapped to 7-0
  int dstCol = 'h' - move[2];
  int dstRow = 8 - (move[3] - '0');

  // Only update if indices are valid
  if (srcRow >= 0 && srcRow < 8 && srcCol >= 0 && srcCol < 8 &&
      dstRow >= 0 && dstRow < 8 && dstCol >= 0 && dstCol < 8) {
    char movingPiece = board[srcRow][srcCol];
    board[srcRow][srcCol] = '-';
    board[dstRow][dstCol] = movingPiece == '-' ? 'w' : movingPiece; // Assume white if empty
    saveCurrentBoardAsPrevious();
  }
}

// Send board state to Pi (as 8 rows)
void sendBoardStateToPi() {
  for (int row = 0; row < 8; row++) {
    String line = "";
    for (int col = 0; col < 8; col++) {
      line += board[row][col];
    }
    Serial.println(line);
  }
}

void loop() {
  bool button1State = !digitalRead(button1Pin);
  bool button2State = !digitalRead(button2Pin);

  // Handle button presses
  if (button1State && !lastButton1State) {
    handleButton1();
  }
  if (button2State && !lastButton2State) {
    handleButton2();
  }

  // Check for incoming messages from Pi
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    message.trim();

    // If the message is a move (length 4, format like "a2a3")
    if (message.length() == 4 && isAlpha(message[0]) && isDigit(message[1]) && isAlpha(message[2]) && isDigit(message[3])) {
      applyMoveToBoard(message); // Update the board

      sendBoardStateToPi();      // Send the updated board state to Pi

      // Optionally: display the move on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("White moved:");
      lcd.setCursor(0, 1);
      lcd.print(message);

      showingPiMessage = true;
      lastMessageTime = millis();
    } else {
      // Not a move, just show the message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pi Says:");
      lcd.setCursor(0, 1);
      lcd.print(message);
      showingPiMessage = true;
      lastMessageTime = millis();
    }
  }

  // Revert to normal display after 3 seconds
  if (showingPiMessage && (millis() - lastMessageTime >= MESSAGE_DISPLAY_TIME)) {
    redrawNormalDisplay();
    showingPiMessage = false;
  }

  lastButton1State = button1State;
  lastButton2State = button2State;
}

// Button, LCD, and other logic remains unchanged below
void handleButton1() {
  if (!configSent) {
    // Cycle mode before game starts
    currentMode = (currentMode == 1) ? 2 : 1;
    redrawNormalDisplay();
  } else {
    // Game has started: scan move
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scanning board...");
    
    String move = readAndGetMove();
    
    lcd.clear();
    if (move.length() > 0) {
      lcd.setCursor(0, 0);
      lcd.print("Move: ");
      lcd.print(move);
      Serial.println(move);  // Send move to serial
    } else {
      lcd.setCursor(0, 0);
      lcd.print("No move detected");
    }
    delay(1500);
    redrawNormalDisplay();
  }
}

void handleButton2() {
  if (!configSent) {
    // Confirm and send mode, prepare game
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode: ");
    lcd.print(currentMode == 1 ? "HvAI" : "HvH");
    lcd.setCursor(0, 1);
    lcd.print("Ready to play");

    Serial.print("MODE:");
    Serial.println(currentMode == 1 ? "HvAI" : "HvH");
    configSent = true;

    // Baseline board reading
    readBoard();
    saveCurrentBoardAsPrevious();
  }
}

void redrawNormalDisplay() {
  lcd.clear();
  if (!configSent) {
    // Pre-game mode selection
    lcd.setCursor(0, 0);
    lcd.print("Select Mode:");
    lcd.setCursor(0, 1);
    lcd.print(currentMode == 1 ? "Mode: HvAI" : "Mode: HvH");
  } else {
    // In-game display
    lcd.setCursor(0, 0);
    lcd.print("Press to scan");
    lcd.setCursor(0, 1);
    lcd.print("move");
  }
}
