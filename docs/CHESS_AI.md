# Chess intelligence and move selection

The Python chess agent is implemented in [`ChessAgent.py`](../software/raspberry-pi/ChessAgent.py) and connected to the Raspberry Pi coordinator in [`Main.py`](../software/raspberry-pi/Main.py).

## Agent structure

The agent maintains a chessboard representation, generates candidate moves, applies position-evaluation logic, and searches a bounded game tree. The source exposes `MiniMax`, `MaxValue` and `MinValue`, consistent with a **depth-limited Minimax** implementation.

## Game integration

`Main.py` tracks the active game mode and maintains a shared board state. In **Human vs AI** mode, a human move can trigger agent selection followed by a physical-movement command. In **Human vs Human** mode, move events also participate in the IoT/MQTT exchange.

## Scope of the implementation

The included integrated agent source does **not visibly implement alpha–beta pruning**. The published project description therefore characterizes the implemented search as Minimax rather than claiming pruning or a benchmarked competitive playing strength. No engine rating, empirical win rate, or timing performance is asserted for this archived snapshot.
