# AI-Driven Automated Chessboard with IoT Integration

**A physical, intelligent chess platform combining magnetic piece detection, two-axis electromechanical motion, a custom Python chess agent, and web-based remote play.**

**Mechatronics Capstone | University of Balamand | 2025**  
**Team:** Ahmad Mansour · Majd Nasrallah · Ziad Hamade — equal contributions

![Completed automated chessboard](media/photos/chessboard-finished.jpg)

## Physical system demonstration

[![Physical chessboard demonstration — animated preview](media/demo/physical-chessboard-preview.gif)](media/demo/physical-chessboard-full-demo.mp4)

[**Full physical demonstration (77 seconds)**](media/demo/physical-chessboard-full-demo.mp4) · [Short demonstration excerpt](media/demo/physical-chessboard-demo.mp4) · [Hardware gallery](docs/VISUAL_WALKTHROUGH.md)

The prototype integrates a glass playing surface, magnetic sensing, a Cartesian carriage and an electromagnet for automated piece transport. A Raspberry Pi coordinates board state, move selection and actuation, while an Arduino Mega handles sensor acquisition and local controls. An ESP-based gateway connects the chess web interface to the physical game through MQTT.

## System capabilities

| Domain | Implemented elements |
|---|---|
| **Embedded sensing** | 64 board positions and 8 auxiliary sensing positions, read using five multiplexer channels/groups and Arduino firmware |
| **Mechatronic actuation** | Two stepper-driven axes, DRV8825 control signals, electromagnet actuation via an H-bridge |
| **Chess intelligence** | Custom Python chess-agent logic using a depth-limited Minimax search and board evaluation |
| **Local gameplay** | Physical piece detection, Arduino menu/LCD interaction, Human-vs-Human and Human-vs-AI mode handling |
| **IoT / remote play** | ESP-hosted HTTP interface, browser-based chessboard, MQTT message exchange with Raspberry Pi |
| **Engineering artifacts** | Fritzing electronics design, source code, physical photos, demonstration videos and technical documentation |

## System architecture

![System architecture showing sensing, controllers, AI, actuation and the IoT gateway](docs/diagrams/system-architecture.svg)

[Physical move sequence](docs/diagrams/physical-move-sequence.svg) · [Remote communication architecture](docs/diagrams/remote-communication.svg) · [Architecture details](docs/ARCHITECTURE.md)

The **Arduino Mega** samples magnetic sensor states and exchanges move/mode messages with the **Raspberry Pi** over USB serial. The Pi maintains chess state, invokes the chess agent when applicable, and controls the X–Y motors and magnetic coupling. The **ESP gateway** hosts the remote chess pages and bridges web requests to MQTT messages exchanged with the Pi.

## IoT chess interface and remote gameplay

The IoT interface is a distinct part of the system, not merely an auxiliary dashboard. A browser connects to an **ESP-hosted HTTP server** to access the game portal, select visual preferences, view chess rules, and participate in a remote chess session. The chess screen represents a complete 8×8 game board with piece graphics, captured and promotion pieces, player timers and turn indication.

### Original ESP-hosted web interface

| Game portal | Chess gameplay screen |
|---|---|
| ![IoT web portal: join game, rules and options](media/iot-interface/chess-portal.png) | ![IoT chess game interface: board, captures, promotion and timers](media/iot-interface/remote-chess-game.png) |

*Screenshots are browser renderings of the HTML/CSS/JavaScript embedded in `firmware/esp32/ChessGateway/Pages.h`. They show the original interface in its initial states, not a live network session or a connected physical-game result.*

**Remote-play data path:**

1. The ESP serves the landing page at `GET /` and the chess interface at `GET /game`.
2. A web move is encoded as a square-to-square string such as `e2e4` and submitted to `POST /move`.
3. The gateway publishes that move to MQTT topic `ESP-TOPIC` for the Raspberry Pi controller.
4. The Pi processes the move and, in the supported mode, issues physical motion commands and updates its state.
5. The Pi publishes moves to `RPI-TOPIC`; the ESP exposes pending updates through `GET /getmove`, which the browser polls periodically.

The browser, ESP gateway and Pi operate as **three separate software layers**. The source includes gameplay-related UI functionality for move selection, client-side legality checks, captured pieces, promotions, timers and theme options; the firmware also exposes the communication endpoints. The selected ESP gateway is a documented integrated revision, with network and firmware compatibility constraints described in [IoT interface and communication](docs/IOT_INTERFACE.md).

## Physical chessboard and control

<details>
<summary><strong>Hardware implementation</strong></summary>

The playing area uses an 8×8 grid of approximately 5 cm squares on a 2 mm glass surface. The design includes an auxiliary eight-position area, magnetic pieces and a lower X–Y gantry that positions an electromagnet under the intended source and destination squares. A pair of NEMA 17 stepper motors drives the orthogonal axes, while DRV8825 signals select direction, step and microstepping. The Arduino Mega samples 72 reported Hall-sensor locations through analog multiplexers. Raspberry Pi GPIO outputs coordinate the motors and magnetic actuator.

See [mechanical design](hardware/mechanics/MECHANICAL_DESIGN.md), [electronics and pinout](hardware/electronics/ELECTRONICS_AND_PINOUT.md) and [component inventory](hardware/bom/COMPONENT_INVENTORY.md).

</details>

## Software and firmware

| Subsystem | Primary implementation | Responsibility |
|---|---|---|
| Raspberry Pi | [`software/raspberry-pi/Main.py`](software/raspberry-pi/Main.py) | Game orchestration, MQTT/serial connections, motion execution |
| Chess agent | [`software/raspberry-pi/ChessAgent.py`](software/raspberry-pi/ChessAgent.py) | Board evaluation and depth-limited Minimax |
| Arduino Mega | [`firmware/arduino/BoardController/`](firmware/arduino/BoardController/) | Mux sensing, mode buttons, LCD, serial reports |
| ESP gateway | [`firmware/esp32/ChessGateway/`](firmware/esp32/ChessGateway/) | HTTP web server, active embedded pages, MQTT bridge |
| Standalone web source | [`web/standalone/`](web/standalone/) | HTML source snapshots of the chess interface |
| Hardware design | [`hardware/electronics/AutomatedChessboard.fzz`](hardware/electronics/AutomatedChessboard.fzz) | Editable Fritzing electronics design |

## Engineering documentation

- [System architecture](docs/ARCHITECTURE.md) — subsystem responsibilities and data flow
- [IoT web interface](docs/IOT_INTERFACE.md) — UI screenshots, browser interactions, ESP endpoints and broker messaging
- [Communication protocols](docs/COMMUNICATION.md) — serial and MQTT messages, HTTP API
- [Chess AI](docs/CHESS_AI.md) — agent structure, decision logic and supported scope
- [Hardware architecture](docs/HARDWARE.md) — mechatronic assembly and sensing
- [Mechanical design](hardware/mechanics/MECHANICAL_DESIGN.md) — gantry, surface and magnetic transport
- [Electronics and pinout](hardware/electronics/ELECTRONICS_AND_PINOUT.md) — controller signals and acquisition
- [Component inventory](hardware/bom/COMPONENT_INVENTORY.md) — as-described hardware and historical component distinctions
- [Software environment](docs/ENVIRONMENT.md) — toolchains, interfaces and configuration variables
- [Engineering verification](docs/TESTING.md) — evidence and verification boundaries
- [Known limitations](docs/KNOWN_LIMITATIONS.md) — implementation-specific constraints
- [Hardware and interface gallery](docs/VISUAL_WALKTHROUGH.md)

## Repository structure

```text
ai-iot-automated-chessboard/
├── firmware/            Arduino sensing and ESP web/MQTT gateway
├── software/            Raspberry Pi coordinator, motion routines, chess AI
├── web/                 Standalone HTML copies of the chess interface
├── hardware/            Fritzing circuit, mechanical design, components
├── docs/                Engineering architecture and implementation details
│   └── diagrams/        Original editable Graphviz diagrams and SVGs
├── media/
│   ├── photos/           Finished board, electronics and mechanism
│   ├── demo/             Physical demonstration and animated preview
│   └── iot-interface/    Captures of the actual ESP-hosted chess pages
├── tests/               Subsystem test sketches and static source checks
├── CONTRIBUTORS.md
├── README.md
└── .gitignore
```

## Project scope and engineering status

This repository documents a **constructed and demonstrated physical prototype** with source files for the principal subsystems. The source has passed packaging and Python syntax checks; the unified firmware and physical machine have not been independently rebuilt or benchmarked from this archived snapshot. The specific ESP firmware revision in service on the final hardware was not recoverable, and hardware-dependent motion constants reflect the source configuration rather than a universal calibration. Quantitative performance claims are therefore deliberately absent.

## Team and attribution

**Ahmad Mansour · Majd Nasrallah · Ziad Hamade** — equal team contributions  
**Academic supervisor:** Dr. Mazen Abi Hussein  
**University:** University of Balamand, Mechatronics

[Contributor information](CONTRIBUTORS.md). External libraries, images and other dependencies retain their respective third-party rights; no blanket license is assigned to shared code without team agreement.
