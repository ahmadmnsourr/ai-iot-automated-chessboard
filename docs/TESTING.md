# Verification status and remaining validation

## Evidence available
- Five original photos of the completed board and its electronics.
- One original 77-second recording showing physical hardware alongside the chess interface.
- Original project report materials, Arduino/Pi/ESP source code and a Fritzing schematic.
- Static parsing of the Python files in this repository.

## Validation still required
- [ ] Rebuild and upload the Arduino firmware using documented library versions.
- [ ] Build and flash the selected ESP firmware using a local `Config.h`.
- [ ] Run Pi application with the physical board and verify all GPIO pin mappings.
- [ ] Test a human move through the multiplexer/Arduino/Pi path.
- [ ] Test one AI move through motor control and electromagnet actuation.
- [ ] Test remote moves through browser/ESP/MQTT/Pi.
- [ ] Establish motor calibration and limits without binding or colliding.
- [ ] Check error behavior on lost serial/MQTT connectivity and power interruption.
- [ ] Record actual recognition accuracy, move completion time and actuator repeatability (if measured).

The physical demo establishes that a board was constructed and demonstrated. It is **not** evidence of every subsystem passing a repeatable test. Do not infer quantitative metrics or full gameplay correctness from the footage.
