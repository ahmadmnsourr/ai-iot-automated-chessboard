# As-built component inventory and historical purchasing notes

> **Primary source:** FYP implementation report, June 2, 2025, pp. 29–38. **Secondary source:** `Material-List.xlsx` in the original FYP archive (not republished because it contains student identification data and an outdated purchasing plan). No inventory or prices have been re-verified on the physical build.

## Components supported by the final implementation report

| Item | Quantity in report | Purpose / notes |
|---|---:|---|
| Raspberry Pi 5 | 1 | High-level control and chess agent |
| Arduino Mega 2560 | 1 | Sensor matrix and LCD/menu controller |
| ESP Wi-Fi gateway | 1 (software role) | Remote game interface; precise board model to confirm |
| Hall-effect sensors | **72** | 64 beneath board plus 8 promotion positions |
| Cascaded multiplexers | **5** | Sensor scan circuitry; verify exact IC/module part numbers |
| NEMA 17 stepper motors | **2** | X–Y transport |
| DRV8825 stepper drivers | **2** | One motor driver per axis |
| Electromagnet | 1 | Magnetic piece movement |
| L298N driver | 1 | Electromagnet polarity control |
| 12 V, 11 A DC power supply | 1 | Motor/coil power per report |
| Raspberry Pi official supply | 1 | Separate Pi power |
| Glass sheet, 2 mm | 1 sheet | Playing surface; report says earlier acrylic plan was changed |
| Aluminium guidance rails, belts/pulleys, carriage | Several | See mechanical design documentation; quantities/specifications need physical confirmation |
| N28 neodymium piece magnets | Mounted on pieces | Exact number/spare quantity not verified |
| LCD, push buttons, wires, connectors | As needed | Some appear in purchasing sheet; precise final counts unverified |

## Purchasing sheet reconciliation

The original `Material-List.xlsx` is useful as **historical budgeting/procurement documentation**, not a verified final bill of materials:

- It lists **72 reed switches**, whereas the implementation report says the final sensor choice was **72 linear Hall-effect sensors** after unsuccessful reed-switch experiments.
- It lists a **Raspberry Pi 4 with quantity 0** and a later entry for a **Raspberry Pi 5 with quantity 1**.
- It includes a **16-channel multiplexer quantity of 1** in an earlier list, while the final report explains **five cascaded multiplexers**.
- It contains two purchasing sections with a combined listed amount of **$345.55**. That is an **old planning/procurement figure**, not a final project cost and not an independently audited total.
- Other materials (wooden enclosure, glass, aluminium rails, belts, full sensor procurement, fabricated parts) may not be fully represented in that workbook.

**Do not claim a current total build cost** from this spreadsheet; prices, purchase quantities, replacements, and omitted parts are not reconciled. For a future replicable BOM, the team would need to record actual quantities, exact part identifiers and verified vendors.

## Next evidence needed for a reproducible BOM

- Photos or markings of the actual sensor IC and multiplexer type
- Full mechanical parts list including pulleys, belts, bearings, rails and fasteners
- Exact electromagnet electrical rating and final wiring/driver protection
- Confirmed ESP board revision
- Final purchasing ledger / order quantities if a true as-built cost is desired
