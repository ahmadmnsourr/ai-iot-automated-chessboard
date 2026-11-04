# System architecture

![Subsystem architecture](diagrams/system-architecture.svg)

The automated chessboard consists of four coupled domains: **physical sensing**, **central game coordination**, **electromechanical motion**, and **networked remote play**. The Raspberry Pi coordinates the exchanges across those domains and holds the principal chess state.

## Subsystem responsibilities

| Layer | Components | Function |
|---|---|---|
| Sensor layer | Board magnets, 72 reported Hall-effect positions, five analog multiplexers | Convert piece placement into analog input readings |
| Embedded controller | Arduino Mega, local buttons, LCD, serial interface | Scan sensors, report move detections, display local game status |
| Coordination and AI | Raspberry Pi 5, Python controller, chess agent | Maintain game state, interpret moves, choose AI moves, coordinate physical motion |
| Actuation | DRV8825-controlled X–Y stepper motors, magnetic carriage, L298N-driven electromagnet | Transport pieces on the physical playing surface |
| IoT layer | ESP Wi-Fi gateway, HTTP server, MQTT broker, browser chess UI | Provide a remote chess interface and exchange game moves |

## Local move processing

The Arduino firmware reads the 8×8 active playing grid through its multiplexer scanning routine. Changes in the sensed pattern produce a move identifier, which is sent to the Raspberry Pi over 9600-baud serial. The Pi updates its chess representation and determines whether a response from the chess agent is needed for the active mode. When a motion command originates from the agent or remote interface, the Pi calculates transport motion and controls both stepper axes and the electromagnet.

## Remote move processing

The ESP gateway exposes the browser pages and collects move submissions. MQTT carries move strings between the gateway and Pi; the same browser periodically reads waiting remote moves from the gateway. This provides an interaction path between the physical chessboard and a remote player. The MQTT broker is a separate service, not hosted inside the browser.

![Communication diagram](diagrams/remote-communication.svg)

## Engineering boundaries

The block diagram represents the subsystem and software interfaces of the project; the original Fritzing file in [`hardware/electronics/`](../hardware/electronics/AutomatedChessboard.fzz) records the circuit design. Detailed GPIO assignments, sensor thresholds and mechanism constants are documented separately because those values are tied to the physical implementation.
