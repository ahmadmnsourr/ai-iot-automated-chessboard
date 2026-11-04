# Known limitations and technical debt

The items below are observed in the selected code or documented as unresolved by the original evidence; they are **not claims that the final historical hardware necessarily malfunctioned**.

| Area | Evidence and consequence | Follow-up |
|---|---|---|
| ESP `POST /move` | `handleEspMove()` accepts `move` and publishes MQTT but does not explicitly call `server.send()` for a successful request. Clients may wait or report an unexpected response. | Test with a browser and, if confirmed, send an HTTP success/error response in a future code revision. |
| ESP firmware revision | Alternate `FYP-Iot` sketch differs from integrated source; actual flashed revision unconfirmed. | Compare with original device backup and test protocol compatibility. |
| MQTT robustness | Code uses unencrypted port 1883, retained move publications, and blocking ESP reconnection. | Keep on trusted lab network; test reconnect/replay and secure before external exposure. |
| HTTP access | Demonstration endpoints do not have reviewed authentication and input validation. | Do not expose ESP on the open internet. |
| Raspberry Pi compatibility | GPIO pins and gpiozero backend not independently retested on a Pi 5. | Validate board mapping, permissions and installed GPIO backend before energizing motors. |
| Motion calibration | Different belt/pulley calibration constants are present in `Main.py` and `Motors.py`; no current limit/homing validation in reconstruction. | Calibrate against physical dimensions and add hardware safety checks. |
| Sensor calibration | Thresholds are hard-coded; the exact Hall-sensor part number differs between report/source comments. | Recalibrate for each installed sensor matrix. |
| Chess AI | Available agent visibly implements depth-limited Minimax without verified alpha-beta pruning; completeness of all chess rules not formally tested. | Document actual algorithm and measure search performance before claiming improvements. |
| Dependency reproduction | Exact original versions of Arduino library, ESP board support, OS and Python packages are not known. | Record a known-working environment on real hardware. |
| Web copies | Standalone HTML pages are distinct from the embedded pages header and require server endpoints; not an offline playable chess application. | Test against the gateway and synchronize if modified. |
| Evaluation | Historical physical demonstration exists, but no independent numerical accuracy/latency/reliability testing of this reconstructed bundle. | Use the test protocol and publish actual measurements only. |

No autonomous movement should be triggered on unverified wiring or exposed to people without appropriate motor, thermal, and electrical safeguards.
