# Public GitHub publication checklist

## Completed in this curated package

- [x] Deduplicated the major integrated Arduino/Pi/ESP source snapshots; alternative ESP version documented instead of silently substituted.
- [x] Documented the game architecture, sensor system, mechanisms, PCB/wiring reference, component inventory, firmware selection, and known implementation differences.
- [x] Added hardware photos, a full video, an edited excerpt, and an animated clickable README preview.
- [x] Equal team credit for Ahmad Mansour, Majd Nasrallah and Ziad Hamade; supervisor named from original materials.
- [x] Removed hard-coded Wi-Fi/MQTT credentials from the **staged source** in favor of example configuration; original archived secrets should be rotated if still active.
- [x] Removed student purchasing invoices, full raw reports with personal identifiers, and non-project reference papers from the public package.
- [x] Static Python parse and internal link checks (see `docs/STATIC_AUDIT_RESULTS.md`).
- [x] No new blanket license assigned to all shared team code/media.

## Manual review before public release

- [ ] **All contributors approve** public sharing of code, circuit source, photos, and recordings, and agree on a future license if desired.
- [ ] Double-check the photographs and videos do not reveal private information or other people without permission.
- [ ] Confirm that any third-party pictures, fonts, chess assets or dependencies are used consistently with their own licenses.
- [ ] Rotate or invalidate the original network/broker credentials if still active.
- [ ] Verify the ESP firmware version, and review the missing HTTP success response on `POST /move`.
- [ ] Rebuild Arduino and ESP firmware with known board/library versions; verify a suitable Pi OS/GPIO environment.
- [ ] Re-run the integrated system on hardware, document serial/MQTT behavior, calibration and movement safety if equipment is available.

**Publication choice:** The repo can be presented as an **archived capstone implementation** with these caveats; it should not be advertised as a plug-and-play or currently production-ready chessboard.
