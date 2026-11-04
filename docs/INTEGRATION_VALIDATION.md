# System integration test plan

**Status:** proposed verification plan, **not** a record of passed tests. The real prototype was demonstrated in original media, but this curated repository has not been built, flashed or validated end-to-end independently.

## Safe testing order

1. **Static build:** Compile both microcontroller programs with documented board packages and libraries; run Python syntax/import checks on a suitable Pi environment. Record versions and build output.
2. **Power isolated:** Verify the low-voltage controllers before connecting the motor/coil supply. Check GPIO numbering and current-limiting interfaces against actual driver pin ratings.
3. **Sensor scan:** Confirm all 72 physical Hall locations, neutral values, magnetic sign, calibration thresholds and stable colour classification.
4. **Local user interface:** Confirm LCD startup and both mode-button inputs, including serial `READY` and `MODE:` outputs.
5. **Serial protocol:** Connect Arduino to Pi, verify 9600-baud communication and a single `e2e4`-style move message, without motion energized.
6. **Single-axis jogging:** Verify homing/origin, direction, step scale, and a short safe 5 cm motion separately for each axis; measure repeatability.
7. **Electromagnet:** Measure coil current and temperature, verify polarity change via driver at the correct times, and establish safe duty cycle.
8. **One controlled move:** Start from a calibrated known position, execute one requested physical move and verify the final square.
9. **Human vs AI:** Verify turn order, move legality, selected AI move and physical execution.
10. **Human vs Human remote:** Exercise browser → ESP → MQTT → Pi and the return path with loss/reconnect testing.
11. **Edge cases:** Captures, promotion area, invalid moves, disconnections, program restart, unexpected magnet state, and interruption/recovery.

## Evidence log template

| Test ID | Test / condition | Expected output | Observation | Evidence | Status |
|---|---|---|---|---|---|
| T01 | Firmware compilation | No critical compile failures | Not re-tested | — | Pending |
| T02 | Full sensor-array sweep | 72 positions classified | Not re-tested | — | Pending |
| T03 | Arduino/Pi serial exchange | Move + mode decoded | Not re-tested | — | Pending |
| T04 | Motor motion and calibration | Repeatable 5 cm grid movement | Not re-tested | — | Pending |
| T05 | Magnetic piece transfer | Safe and repeatable pick/drag/release | Not re-tested | — | Pending |
| T06 | Human-vs-AI move | Controller selects and executes one move | Original demo shows gameplay but not instrumented test | Physical demo | Partially evidenced |
| T07 | MQTT remote move | Round-trip move propagated | Not re-tested | — | Pending |
| T08 | Reconnect/restart | No stale moves or unexpected actuation | Not re-tested | — | Pending |

## Suggested metrics — do not fill with invented numbers

- Sensor classification accuracy and false-trigger rate for white, black and empty squares
- Movement time per square / full board; positioning error at reference corners
- Repeatability across at least 10 identical transfers (mean and max error)
- Chess decision latency by search depth and branching state
- End-to-end remote-move latency and recovery after broker drop
- Peak coil and stepper-driver current/temperature during operation

Use the original demo solely as proof that a physical prototype was presented; do **not** infer numeric success rates or a full regression test suite from footage.
