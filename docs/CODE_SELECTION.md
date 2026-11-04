# Integrated source-code selection (audit)

This repository uses the **matching integrated copies** found under `FYP-Chess-Codes` rather than publishing redundant copies. The corresponding Raspberry Pi, Arduino, and ESP files under `FYP-Codes` are byte-identical in the original archive.

| Component | Source in supplied FYP archive | Decision |
|---|---|---|
| Raspberry Pi controller, chess agent, motion utility | `FYP-Chess-Codes/Rpi/` | Baseline |
| Arduino board sensing/menu | `FYP-Chess-Codes/Arduino/` | Baseline |
| ESP gateway and web pages | `FYP-Chess-Codes/ESP/ESP-Chess/` | Provisional integrated baseline |
| Standalone browser pages | `FYP-Codes/FYP-IoT/ChessWebApp/` | Include as reference interface |
| Motor utility and selected bring-up tests | `FYP-Codes/` and `Test Codes/` | Separate from main app |

**Unresolved revision:** The ESP files in `FYP-Iot/FYP-IoT/ESP-Chess/` differ from the integrated baseline. They are deliberately excluded until protocol compatibility is independently checked. The standalone AI experiment has a missing import; it is also excluded.

**Only intentional code adjustments:** configure MQTT/Wi-Fi values via nontracked local configuration instead of embedded secrets, allow Raspberry Pi serial-port override, and rename Arduino sketches to match their folders. Core algorithms, move processing, and hardware logic were not rewritten.

**Not yet confirmed:** identical final flashed firmware, successful fresh build, end-to-end hardware test, and quantitative test results.
