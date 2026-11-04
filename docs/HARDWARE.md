# Hardware overview

This repository documents an **as-reported** physical AI chessboard, not a newly manufactured build. The final University of Balamand implementation report (June 2, 2025, pp. 29–38) describes:

- A single **2 mm glass** playing surface with **5 × 5 cm** squares
- A wooden enclosure and belt-driven, two-stepper Cartesian X–Y carriage
- A reversible carriage-mounted electromagnet
- **72 Hall-effect sensors** through **5 cascaded multiplexers**
- A Raspberry Pi 5, Arduino Mega, ESP-based IoT link, and dedicated power supplies

Read the detailed source-grounded engineering documents:

- [Mechanical design and dimensions](../hardware/mechanics/MECHANICAL_DESIGN.md)
- [Electronics, sensor acquisition and pinout](../hardware/electronics/ELECTRONICS_AND_PINOUT.md)
- [As-built component inventory and procurement caveats](../hardware/bom/COMPONENT_INVENTORY.md)
- [Report-to-code reconciliation](REPORT_TO_CODE_DISCREPANCIES.md)

The original Fritzing file remains at [`hardware/electronics/AutomatedChessboard.fzz`](../hardware/electronics/AutomatedChessboard.fzz). Native mechanical CAD, authoritative wiring signoff and full as-built parts list are **not present** in the shared project materials.
