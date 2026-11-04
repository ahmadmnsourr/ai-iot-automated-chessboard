#ifndef BOARD_READER_H
#define BOARD_READER_H

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

// 8x9 board matrix to store piece states
char board[8][9];  // 'b', 'w', or '-'
char previousBoard[8][9]; // Stores previous state
bool firstMove = true;    // Flag for initial board read

void selectChannel(int channel) {
  digitalWrite(S0, channel & 0x01);
  digitalWrite(S1, (channel >> 1) & 0x01);
  digitalWrite(S2, (channel >> 2) & 0x01);
  digitalWrite(S3, (channel >> 3) & 0x01);
}

void initBoardReader() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Initialize boards
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 9; col++) {
      board[row][col] = '-';
      previousBoard[row][col] = '-';
    }
  }
}

void readBoard() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 9; col++) {
      int index = col * 8 + row; // Column-major indexing
      int mux = index / 16;
      int ch = index % 16;

      selectChannel(ch);
      delayMicroseconds(50);  // Allow signal settling
      int value = analogRead(SIG[mux]);

      // Interpret Hall sensor value
      if (value < NEUTRAL_MIN) {
        board[row][col] = 'b';  // Black piece
      } else if (value > NEUTRAL_MAX) {
        board[row][col] = 'w';  // White piece
      } else {
        board[row][col] = '-';  // No piece
      }
    }
  }
}

void saveCurrentBoardAsPrevious() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 9; col++) {
      previousBoard[row][col] = board[row][col];
    }
  }
}

String detectMove() {
  String move = "";
  int srcRow = -1, srcCol = -1;
  int dstRow = -1, dstCol = -1;
  
  // Find the moved piece (changed from piece to empty)
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) { // Only check first 8 columns
      if (previousBoard[row][col] != '-' && board[row][col] == '-') {
        srcRow = row;
        srcCol = col;
      }
      
    }
  }
  
  // Find the destination (changed from empty to piece)
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) { // Only check first 8 columns
      if (previousBoard[row][col] == '-' && board[row][col] != '-') {
        dstRow = row;
        dstCol = col;
      }
    }
  }
  
  // If we found both source and destination, convert to chess notation
  if (srcRow != -1 && dstRow != -1) {
    // Convert columns to files (a-h)
    char srcFile = 'h' - srcCol;
    char dstFile = 'h' - dstCol;
    
    // Convert rows to ranks (1-8, remembering that row 0 is rank 8)
    int srcRank = 8 - srcRow;
    int dstRank = 8 - dstRow;
    
    move += srcFile;
    move += srcRank;
    move += dstFile;
    move += dstRank;
  }
  
  return move;
}


String readAndGetMove() {
  readBoard();
  
  String move = "";
  if (!firstMove) {
    move = detectMove();
  } else {
    firstMove = false;
  }
  
  // Update the previous board
  saveCurrentBoardAsPrevious();
  
  return move;
}

#endif
