# Chess agent implementation

`software/raspberry-pi/ChessAgent.py` contains a custom chess agent with board-state representation, move-generation/validation routines, an evaluation function, and recursive depth-limited **Minimax** functions (`MiniMax`, `MaxValue`, `MinValue`).

The inspected integrated implementation does **not visibly contain alpha–beta pruning**. Some narrative project materials discuss alpha–beta search; that claim requires a different verified implementation before it should appear as implemented here.

This repository does not claim the chess agent is a certified rules engine or publish unverified win rates, search depths, or performance benchmarks.
