# Communication protocols

The system integrates **USB serial**, **HTTP**, and **MQTT**. Each protocol serves a distinct purpose and uses move strings shared across the controller, ESP gateway and browser.

## Arduino Mega ↔ Raspberry Pi

| Property | Value in source |
|---|---|
| Interface | USB serial (`pyserial` on Pi; Arduino `Serial`) |
| Baud rate | **9600** |
| Default Pi device | `/dev/ttyUSB0` |
| Override variable | `CHESS_ARDUINO_PORT` |
| Mode labels | `HvH` and `HvAI` |
| Message examples | `MODE:HvH`, `MODE:HvAI`, `e2e4` |

The Arduino detects square changes and sends algebraic-coordinate move strings; it also receives Pi-originated messages for local display and board-state updates. The Pi performs higher-level coordination and decides which move sources trigger the physical motors.

## Browser ↔ ESP gateway

The ESP hosts HTTP routes on port **80**:

| Endpoint | Method | Data |
|---|---|---|
| `/` | GET | HTML landing screen |
| `/game` | GET | Chessboard HTML; optional theme query |
| `/move` | POST | URL-encoded `move` field (for example `e2e4`) |
| `/getmove` | GET | Pending move text, otherwise 204 No Content |

A browser game submits local chess moves and polls the gateway for incoming moves. The selected firmware's successful `/move` handler does not explicitly produce an HTTP success response.

## ESP gateway ↔ MQTT broker ↔ Raspberry Pi

| Channel | Publisher | Subscriber | Function |
|---|---|---|---|
| `ESP-TOPIC` | ESP | Pi | Browser-originated chess moves |
| `RPI-TOPIC` | Pi | ESP | Pi-originated moves for web clients |

Both programs use port **1883** in the source configuration. The ESP source uses retained publication for web moves; the Pi attempts to clear retained `ESP-TOPIC` data when its MQTT connection is established. Reconnection and stale-message handling remain implementation considerations.

### Communication sequence

![IoT communication pathways](diagrams/remote-communication.svg)

The broker is an infrastructure component separate from the ESP and Raspberry Pi. Broker hostname and credentials are represented as local configuration values rather than published secrets. The code targets a trusted network environment; TLS transport is not implemented in this snapshot.
