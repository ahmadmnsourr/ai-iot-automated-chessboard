# Electronics, sensor acquisition and controller pinout

> **Evidence basis:** June 2, 2025 FYP implementation report pp. 33–38, `firmware/arduino/BoardController/{BoardController.ino,5-Mux.h}`, `software/raspberry-pi/Main.py`, and the original [`Fritzing schematic`](AutomatedChessboard.fzz). The Fritzing design predates the final report and has **not** been verified against every pin on the finished physical assembly.

## Main controllers and actuation

| Element | Reported role | Count / note |
|---|---|---|
| Raspberry Pi 5 | Game orchestration, chess agent, motion/IO, serial and MQTT | 1 main controller |
| Arduino Mega 2560 | Board sensors, local buttons and LCD | 1 |
| ESP-based IoT gateway | Web UI and MQTT communication | 1 gateway in code; exact board revision unconfirmed |
| NEMA 17 stepper motors | Cartesian X–Y movement | 2 |
| DRV8825 drivers | Independent X–Y motor control | 2 |
| L298N driver | Reverses electromagnet current direction | 1 described |
| Electromagnet | Translates chess pieces by magnetic coupling | 1 |
| Hall-effect sensors | Magnetic piece presence/polarity | **72 total: 64 board + 8 promotion** |
| Cascaded analog multiplexers | Read sensors with fewer Arduino analog inputs | **5**, per final report |
| 16×2 I²C LCD / buttons | Mode selection, local user feedback | See firmware, model-dependent |

## Sensor acquisition: Arduino source pinout

| Arduino connection | Source-defined mapping |
|---|---|
| Multiplexer selectors `S0 S1 S2 S3` | Digital **D2, D3, D4, D5** |
| Multiplexer analog outputs `SIG[]` | **A0, A1, A2, A3, A5** (five arrays) |
| Button 1 / mode | Digital **D6**, `INPUT_PULLUP` |
| Button 2 / confirm | Digital **D7**, `INPUT_PULLUP` |
| LCD | I²C, firmware address **0x27**, 16×2 |
| Arduino serial link | **9600 baud** in the Arduino/Pi source |

In `5-Mux.h`, the sensing routine scans an **8 × 9** grid using `index = col * 8 + row`, `mux = index / 16`, and `ch = index % 16`. Five 16-channel multiplexers provide sufficient capacity for 72 inputs. The 9th column models the additional 8-position area; the move-detection routine checks only the first 8 columns.

The firmware uses thresholds **420** and **560** to classify sensor values; these are existing code thresholds that **must be calibrated** to the installed sensors, magnets, ADC reference and noise level. The source code comment calls the sensor type **SS49E**, while the final report names **UGN3503**. Do not assume they are electrically interchangeable without checking what was physically installed.

## Raspberry Pi GPIO mappings (as written in Main.py)

`gpiozero.DigitalOutputDevice` uses **BCM GPIO numbering** by default. Confirm these pin numbers on the actual Pi and the driver interfaces before enabling motors.

| Signal | BCM GPIO |
|---|---:|
| X-axis step / direction / enable | 13 / 12 / 1 |
| Y-axis step / direction / enable | 21 / 26 / 16 |
| X-axis DRV8825 M0 / M1 / M2 | 18 / 23 / 24 |
| Y-axis DRV8825 M0 / M1 / M2 | 17 / 27 / 22 |
| Electromagnet L298N IN1 / IN2 | 7 / 8 |

**Review flag:** GPIO1 has special functions on Raspberry Pi headers; confirm its actual use and boot behavior before duplicating the motor-enable wiring. These are source-code assignments, **not a verified wiring inspection**.

## Power architecture

The final report says a **12 V, 11 A DC supply** powered the motor/electromagnet system, while a **dedicated official Raspberry Pi adapter** supplied the Pi (rather than the originally proposed DC–DC converter). Validate common ground/reference requirements, isolation, current limits, driver heat dissipation, coil flyback handling, and emergency stop before replicating the circuit.

## Original editable drawing

Open [`AutomatedChessboard.fzz`](AutomatedChessboard.fzz) in Fritzing. Treat it as a **project design artifact**, not guaranteed documentation of every last as-built revision. Photos and reported late-stage changes are more current on several design choices.
