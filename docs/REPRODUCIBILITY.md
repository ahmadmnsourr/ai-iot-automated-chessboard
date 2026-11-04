# Reproducing and verifying the prototype

## What is available

- Arduino board-sensing firmware, Raspberry Pi controller/AI/motion software, and ESP HTTP/MQTT firmware.
- Fritzing circuit source, interface descriptions, hardware photographs, a physical video demonstration, selected component tests, and engineering documentation.
- A standard-library **static** audit that checks repository structure, Python syntax and Markdown links (does not execute motors, test the firmware or verify correctness).

## 1. Static checks (can run without hardware)

From the repository root:

```bash
python -m unittest discover -s tests/static -v
```

This checks that required files are present, Python source files parse, the ESP local configuration is templated and not committed, and relative Markdown links resolve. This is **not** a runtime, electrical or integration test.

## 2. Verify safe hardware setup

1. Review [`ELECTRONICS_AND_PINOUT.md`](../hardware/electronics/ELECTRONICS_AND_PINOUT.md) and the `.fzz` source; match connections to the actual hardware.
2. Check power polarity, motor driver current limits, switch/coil protection, mechanical clearance, and travel limits **with the motor/coil supply isolated**.
3. Confirm installed controller boards and toolchain libraries, following [`DEPENDENCIES.md`](DEPENDENCIES.md).
4. Flash and verify the Arduino and ESP one at a time. Treat the ESP revision in this repository as provisional until its MQTT/browser behavior is checked.
5. Confirm serial 9600 baud and the broker settings, then test non-energized message exchanges.
6. Confirm axis direction, limit handling and magnet driver behavior before a physical move; test under supervision.
7. Perform full system tests according to [`INTEGRATION_VALIDATION.md`](INTEGRATION_VALIDATION.md) and record actual observations.

## 3. Reproduce a demo

A successful reproduction should show: an Arduino sensor message, Pi interpretation of a physical move, a controller response, verified X–Y motion with safe magnet operation, and a web-to-Pi-to-web remote move. Report each subsystem separately if the combined build cannot be reproduced.

## Important distinction

The original team's physical demonstration is genuine historical project media. No fresh end-to-end test on the original hardware has been performed during this repository audit, and no measured performance metrics have been invented.
