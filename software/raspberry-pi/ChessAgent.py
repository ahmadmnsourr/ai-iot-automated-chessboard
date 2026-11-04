import secrets
import numpy as np
from math import inf as infinity
import time
import copy

class ChessAgent:
    def __init__(self):
        
        self.MAX = "white" 
        self.MIN = "black"
        self.state = [["bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"],
                      ["bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"],
                      ["--", "--", "--", "--", "--", "--", "--", "--"],
                      ["--", "--", "--", "--", "--", "--", "--", "--"],
                      ["--", "--", "--", "--", "--", "--", "--", "--"],
                      ["--", "--", "--", "--", "--", "--", "--", "--"],
                      ["wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"],
                      ["wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"]]
        
        self.initState = copy.deepcopy(self.state)
        self.nbExpandedNodes = 0
        self.moveHistory = []
        self.historyLimit = 6
        
    def convert_to_chess_notation(self, move):
        """Convert move coordinates to chess notation like 'a2a4'"""
        if not move:
            return None
            
        start, end = move
        x1, y1 = start
        x2, y2 = end
        
        # Convert column numbers to letters (0=a, 1=b, etc.)
        col_to_letter = {0: 'a', 1: 'b', 2: 'c', 3: 'd', 
                         4: 'e', 5: 'f', 6: 'g', 7: 'h'}
        
        # Convert row numbers to chess ranks (0=8, 1=7, etc. since chess starts from bottom)
        row_to_rank = {0: '8', 1: '7', 2: '6', 3: '5',
                       4: '4', 5: '3', 6: '2', 7: '1'}
        
        start_square = col_to_letter[y1] + row_to_rank[x1]
        end_square = col_to_letter[y2] + row_to_rank[x2]
        
        return start_square + end_square
        
    def isRepetition(self, move): # Detects if same move is repeated
        recentMoves = self.moveHistory[-self.historyLimit:] if len(self.moveHistory) >= self.historyLimit else self.moveHistory
        count = recentMoves.count(move)
        return count >= 2
    
    def isPieceTypeRepetition(self, move): # Detects if same move is repeated within same piece
        piece = self.state[move[0][0]][move[0][1]]
        if piece == "--":
            return False  
        piece_type = piece[1]  
        piece_count = 0
        for m in self.moveHistory[-self.historyLimit:]:
            if self.state[m[0][0]][m[0][1]] == piece_type:
                piece_count += 1
        
        return piece_count >= 3

    def Reset(self):
        self.state = copy.deepcopy(self.initState)
        self.nbExpandedNodes = 0

    def GetNextPossibleMoves(self, player):
        possibleMoves = []
        for x1 in range(8):
            for y1 in range(8):
                piece = self.state[x1][y1]
                if (player == self.MAX and piece.startswith('w')) or (player == self.MIN and piece.startswith('b')):
                    for x2 in range(8):
                        for y2 in range(8):
                            move = ((x1, y1), (x2, y2))
                            if self.IsValidMove(move, player):
                                possibleMoves.append(move)
        return possibleMoves

    def IsValidMove(self, move, player): # Checks the validity of moves based on pieces types
        start, end = move
        x1, y1 = start
        x2, y2 = end
        piece = self.state[x1][y1]
        target = self.state[x2][y2]
        
        if target != "--" and target[0] == piece[0]:
            return False
        
        dx, dy = x2 - x1, y2 - y1

        if piece[1] == 'p':  # Pawn
            direction = -1 if player == self.MAX else 1
            if dy == 0 and dx == direction and self.state[x2][y2] == "--":
                return True
            if dy in [-1, 1] and dx == direction and self.state[x2][y2] != "--":
                return True
        elif piece[1] == 'R':  # Rook
            if dx == 0 or dy == 0:
                return self.IsPathClear(start, end)
        elif piece[1] == 'B':  # Bishop
            if abs(dx) == abs(dy):
                return self.IsPathClear(start, end)
        elif piece[1] == 'Q':  # Queen
            if dx == 0 or dy == 0 or abs(dx) == abs(dy):
                return self.IsPathClear(start, end)
        elif piece[1] == 'K':  # King
            if abs(dx) <= 1 and abs(dy) <= 1:
                return True
        elif piece[1] == 'N':  # Knight
            if (abs(dx), abs(dy)) in [(2, 1), (1, 2)]:
                return True
        return False

    def IsPathClear(self, start, end): # Detects if path is clear for sliding pieces
        x1, y1 = start
        x2, y2 = end
        dx = x2 - x1
        dy = y2 - y1
        steps = max(abs(dx), abs(dy))
        step_x = dx // steps if steps != 0 else 0
        step_y = dy // steps if steps != 0 else 0

        for step in range(1, steps):
            if self.state[x1 + step * step_x][y1 + step * step_y] != "--":
                return False
        return True

    def Evaluate(self):
        material_score = 0
        piece_position_score = 0
        mobility_score = 0
        board_control_score = 0
        pawn_structure_score = 0
        queen_safety_score = 0

        piece_values = {'p': 1, 'N': 3, 'B': 3, 'R': 5, 'Q': 9, 'K': 1000000}

        # Material score: sum of piece values
        for row in self.state:
            for piece in row:
                if piece != '--':
                    value = piece_values.get(piece[1], 0)
                    material_score += value if piece.startswith('w') else -value

        # Piece Positioning 
        piece_position_table = {
            'P': [[0, 0, 0, 0, 0, 0, 0, 0],  
                  [5, 10, 10, 20, 20, 10, 10, 5],
                  [10, 20, 20, 30, 30, 20, 20, 10],
                  [15, 30, 30, 40, 40, 30, 30, 15],
                  [10, 20, 20, 30, 30, 20, 20, 10],
                  [5, 10, 10, 20, 20, 10, 10, 5],
                  [0, 0, 0, 5, 5, 0, 0, 0],
                  [0, 0, 0, 0, 0, 0, 0, 0]],}
        
        for row in range(8):
            for col in range(8):
                piece = self.state[row][col]
                if piece != '--':
                    piece_type = piece[1]
                    piece_color = piece[0]
                    if piece_color == 'w':
                        piece_position_score += piece_position_table.get(piece_type, [[0]*8]*8)[row][col]
                    else:
                        piece_position_score -= piece_position_table.get(piece_type, [[0]*8]*8)[7-row][7-col]

        # Mobility: Number of legal moves
        mobility_score = len(self.GetNextPossibleMoves(self.MAX)) - len(self.GetNextPossibleMoves(self.MIN))

        # Board Control: Number of central squares controlled 
        central_squares = [(3, 3), (3, 4), (4, 3), (4, 4)]
        for row, col in central_squares:
            piece = self.state[row][col]
            if piece.startswith('w'):
                board_control_score += 1
            elif piece.startswith('b'):
                board_control_score -= 1
                
        # Pawn Structure
        for row in range(8):
            for col in range(8):
                piece = self.state[row][col]
                if piece != '--' and piece[1] == 'p':
                    if piece[0] == 'w':
                        pawn_structure_score += 1
                    else:
                        pawn_structure_score -= 1

        # Queen Safety: Penalize exposed queens
        for row in range(8):
            for col in range(8):
                piece = self.state[row][col]
                if piece != '--' and piece[1] == 'Q':
                    if self.IsQueenExposed((row, col), piece[0]):
                        queen_safety_score -= 50 if piece.startswith('w') else -50

        # Combine scores with weights
        score = (1.0 * material_score 
                 + 0.3 * piece_position_score 
                 + 0.2 * mobility_score 
                 + 0.2 * board_control_score 
                 + 0.4 * pawn_structure_score 
                 + 0.5 * queen_safety_score) 
        return score
    
    def IsQueenExposed(self, position, color):
        x, y = position
        opponent = 'b' if color == 'w' else 'w'
        for start_x in range(8):
            for start_y in range(8):
                piece = self.state[start_x][start_y]
                if piece.startswith(opponent):
                    move = ((start_x, start_y), (x, y))
                    if self.IsValidMove(move, self.MIN if color == 'w' else self.MAX):
                        return True
        return False


    def IsKingInCheck(self, player): # Detects if king is in check position
        king = "wK" if player == self.MAX else "bK"
        king_position = None

        # Find the position of the king
        for row in range(8):
            for col in range(8):
                if self.state[row][col] == king:
                    king_position = (row, col)
                    break
            if king_position:
                break

        # Check if any opponent's piece can attack the king
        opponent = self.MIN if player == self.MAX else self.MAX
        for start_x in range(8):
            for start_y in range(8):
                piece = self.state[start_x][start_y]
                if (opponent == self.MAX and piece.startswith('w')) or (opponent == self.MIN and piece.startswith('b')):
                    for end_x in range(8):
                        for end_y in range(8):
                            move = ((start_x, start_y), (end_x, end_y))
                            if self.IsValidMove(move, opponent):
                                # Check if the move can attack the king
                                if (end_x, end_y) == king_position:
                                    return True
        return False

    def IsKingMovable(self, player):
        # Check if the king has any valid moves to escape
        king = "wK" if player == self.MAX else "bK"
        king_position = None
        for row in range(8):
            for col in range(8):
                if self.state[row][col] == king:
                    king_position = (row, col)
                    break
            if king_position:
                break
        if king_position is None:
            return False

        # Try all 8 possible king moves
        for dx in [-1, 0, 1]:
            for dy in [-1, 0, 1]:
                if dx == 0 and dy == 0:
                    continue
                new_x = king_position[0] + dx
                new_y = king_position[1] + dy
                if 0 <= new_x < 8 and 0 <= new_y < 8:
                    if self.state[new_x][new_y] == "--" or (self.state[new_x][new_y][0] != self.state[king_position[0]][king_position[1]][0]):
                        # Temporarily move the king to check if it is still in check after the move
                        captured_piece = self.state[new_x][new_y]
                        self.state[new_x][new_y] = king
                        self.state[king_position[0]][king_position[1]] = "--"
                        if not self.IsKingInCheck(player):
                            # Undo the move and return True
                            self.state[king_position[0]][king_position[1]] = king
                            self.state[new_x][new_y] = captured_piece
                            return True
                        # Undo the move
                        self.state[king_position[0]][king_position[1]] = king
                        self.state[new_x][new_y] = captured_piece
        return False
    
    def isKingInCheckAfterMove(self, move, player): # Detects if after a certain move the king is still in check
        start, end = move
        x1, y1 = start
        x2, y2 = end
        piece = self.state[x1][y1]
        captured_piece = self.state[x2][y2]
        self.ExecuteMove(move, player)
        is_in_check = self.IsKingInCheck(player)
        self.UndoMove(move, captured_piece)
        return is_in_check

    def IsGameWon(self, player):
        if self.IsKingInCheck(player) and not self.IsKingMovable(player):
            return True
        return False

    def IsDraw(self):
        return not self.GetNextPossibleMoves(self.MAX) or not self.GetNextPossibleMoves(self.MIN)

    def GameOver(self):
        return self.IsGameWon(self.MAX) or self.IsGameWon(self.MIN) or self.IsDraw()

    def ExecuteMove(self, move, player):
        start, end = move
        x1, y1 = start
        x2, y2 = end
        piece = self.state[x1][y1]
        self.state[x2][y2] = piece
        self.state[x1][y1] = '--'
        return self.state

    def UndoMove(self, move, captured_piece):
        start, end = move
        x1, y1 = start
        x2, y2 = end
        self.state[x1][y1] = self.state[x2][y2]
        self.state[x2][y2] = captured_piece

    def MiniMax(self, player, depth):
        self.nbExpandedNodes += 1
        if self.GameOver() or depth == 0:
            return self.Evaluate()

        if player == self.MAX:
            return self.MaxValue(depth)
        else:
            return self.MinValue(depth)

    def MaxValue(self, depth):
        max_eval = -infinity
        for move in self.GetNextPossibleMoves(self.MAX):
            captured_piece = self.state[move[1][0]][move[1][1]]
            self.ExecuteMove(move, self.MAX)
            eval = self.MiniMax(self.MIN, depth - 1)
            self.UndoMove(move, captured_piece)
            max_eval = max(max_eval, eval)
        return max_eval

    def MinValue(self, depth):
        min_eval = infinity
        for move in self.GetNextPossibleMoves(self.MIN):
            captured_piece = self.state[move[1][0]][move[1][1]]
            self.ExecuteMove(move, self.MIN)
            eval = self.MiniMax(self.MAX, depth - 1)
            self.UndoMove(move, captured_piece)
            min_eval = min(min_eval, eval)
        return min_eval

    def GetBestMove(self, player):
        depth = 2  
        best_move = None
        best_score = -infinity if player == self.MAX else infinity
        for move in self.GetNextPossibleMoves(player):
            if self.isRepetition(move) or self.isPieceTypeRepetition(move) or self.isKingInCheckAfterMove(move,player):
                continue
            
            captured_piece = self.state[move[1][0]][move[1][1]]
            self.ExecuteMove(move, player)
            eval = self.MiniMax(self.MIN if player == self.MAX else self.MAX, depth - 1)
            self.UndoMove(move, captured_piece)

            if (player == self.MAX and eval > best_score) or (player == self.MIN and eval < best_score):
                best_score = eval
                best_move = move
        if best_move:
            self.moveHistory.append(best_move)
            if len(self.moveHistory) > self.historyLimit:
                self.moveHistory.pop(0)
        
        # Return the move in chess notation
        chess_notation_move = self.convert_to_chess_notation(best_move)
        return chess_notation_move, best_score

    def DisplayBoard(self):
        print("Expanded nodes:", self.nbExpandedNodes)
        print(np.array(self.state))