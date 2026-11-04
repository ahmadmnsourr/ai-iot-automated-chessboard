# Implementation report versus available source: evidence register

This file prevents treating planned designs, later report claims, and existing source snapshots as if they were identical. It is an engineering **reconstruction audit**, not a criticism of the team’s original demonstration.

| Topic | Final report (June 2, 2025) | Available source / purchasing file | Documentation decision |
|---|---|---|---|
| Sensors | 72 Hall-effect sensors, mentions UGN3503 | `5-Mux.h` comments mention **SS49E**; original BOM lists reed switches | Specify Hall sensors as implemented; exact physical part number unverified |
| Multiplexing | **5** cascaded multiplexer modules | `5-Mux.h` has five analog outputs and 16-channel indexing | Consistent at architecture level; exact module/IC unknown |
| Raspberry Pi | **Pi 5** used | Historical BOM includes unused Pi 4 entry plus later Pi 5 | Document Pi 5 as built |
| Chess surface | **Single 2 mm glass**, 5 cm squares | Earlier plans considered acrylic and 6 cm squares | Use final dimensions and explain design change |
| Chess agent | Report discusses Minimax **and alpha-beta pruning** | Available integrated `ChessAgent.py` exposes `MiniMax`, `MinValue`, `MaxValue`; no verified pruning | Claim **depth-limited Minimax**, not implemented alpha-beta |
| Belt calibration | Two stepper axes and pulley-based motion | `Main.py` uses diameter **1.27 cm**; utility `Motors.py` uses **1.4848 cm** | Keep code unchanged, document discrepancy and require mechanical calibration |
| Sensor threshold | Magnetic Hall sensing | Fixed ADC neutral band **420–560** in firmware | Indicate calibration values are unverified for other hardware |
| ESP gateway | IoT remote Human-vs-Human | Two distinct source snapshots | Integrated revision provisional; audit in ESP_REVISION_AUDIT.md |
| Computer-controlled movement | Demonstrated physical chessboard | GPIO steps, electromagnet logic and separate tests present | Published source exists; no independent live test of rebuilt package |

## Source traceability

- **Implementation report:** `FYP/Spring 2024-2025/Chess-FYP-Report.pdf` in the original private archive, pp. 29–47.
- **Software:** `FYP/FYP-Chess-Codes/` and the additional web/test folders in the original private archive.
- **Historical purchasing sheet:** `FYP/Material-List.xlsx` (contains student IDs; **not republished**).
- **Electronics design:** `hardware/electronics/AutomatedChessboard.fzz` (editable Fritzing file).
- **Photos and demo:** user-supplied physical prototype recordings.

The report and purchasing workbook are not included in this public-facing reconstruction draft. This avoids publishing identifiers or unreconciled claims; team members may choose to include sanitized materials in a later release.
