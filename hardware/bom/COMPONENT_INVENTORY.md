# Prototype component inventory

The following inventory describes the principal subsystems represented by the completed automated-chessboard design. Some component types and quantities are recoverable from the technical material, while exact manufacturer part numbers, board revision and procurement totals are not fully established.

| Component | Reported quantity | Role |
|---|---:|---|
| Raspberry Pi 5 | 1 | Central chess coordination, AI, motor control and network messaging |
| Arduino Mega 2560 | 1 | Sensor scanning, local LCD/buttons and serial transport |
| ESP Wi-Fi gateway | 1 | Browser HTTP server and MQTT bridge; exact development board variant unresolved |
| Hall-effect sensors | 72 | 64 chess squares and 8 auxiliary positions |
| 16-channel analog multiplexers | 5 | Consolidated acquisition of 72 sensing positions |
| NEMA 17 stepper motors | 2 | Cartesian carriage travel |
| DRV8825 stepper drivers | 2 | Step/direction and microstepping interface |
| Electromagnet | 1 | Magnetic coupling for piece transport |
| L298N H-bridge driver | 1 | Electromagnet polarity control |
| Actuation power supply | 1 | 12 V, 11 A described supply |
| Raspberry Pi power adapter | 1 | Separate controller supply |
| Glass playing surface | 1 | Approximately 2 mm thickness; 5 cm playing squares |
| Aluminium guide rails, belts, pulleys and carriage | Assembly | Two-axis mechanical transport |
| Neodymium magnets in pieces | Set | Magnetic pickup with the carriage |
| LCD, buttons, wiring and connectors | Assembly | Local interface and electrical integration |

Earlier procurement iterations included reed-switch trials and other controller and surface options. The described prototype uses **Hall-effect sensing, Raspberry Pi 5 and a glass top**. Procurement prices and exact final purchase quantities are not presented as verified performance or cost data.

[Mechanical architecture](../mechanics/MECHANICAL_DESIGN.md) · [Electronics and pin mapping](../electronics/ELECTRONICS_AND_PINOUT.md)
