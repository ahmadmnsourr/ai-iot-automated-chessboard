# Mechanical design — implemented chessboard

> **Evidence basis:** University of Balamand, *Automated Chess Board with IoT Integration and AI-Based Gameplay*, implementation report dated **2 June 2025**, pp. 29–33; project photographs in [`media/photos/`](../../media/photos/); source dimensions in [`Main.py`](../../software/raspberry-pi/Main.py). These notes reconstruct the reported final design; no native mechanical CAD was found in the submitted archive.

## Top surface and playing areas

- **Board:** 8 × 8 chess squares, **5 × 5 cm** each, giving a nominal **40 × 40 cm** playing grid (not necessarily the total glass size).
- **Board surface:** a **single 2 mm glass sheet**. The project initially considered two acrylic sheets, but the team changed to glass for lower sliding friction.
- **Captured-piece area:** one 8-position strip on White's right, described in the report as a graveyard.
- **Promotion area:** one 8-position strip on White's left. These positions have their own sensors.
- **Controls:** timer and button area beside the captured-piece area.

The nominal grid is **not** a verified travel or collision-safe envelope. The exact total enclosure dimensions and limit-switch locations are not available in the shared materials.

## X–Y transport system

The report describes a Cartesian gantry beneath the glass:

1. A **wooden rectangular enclosure** holds the assembly.
2. Two **70 cm fixed aluminium guide rods** (approximately **1.2 cm diameter**) were originally installed, with a **third 70 cm rod added** for support.
3. Two perpendicular **60 cm aluminium moving rails** are described (approximately **2 cm wide**).
4. Each motion axis is driven by a **NEMA 17 stepper motor** via a synchronous/timing belt and pulley mechanism.
5. A carriage carries the **electromagnet**, positioned close to the underside of the playing surface. The vertical gap is adjustable to improve magnetic coupling.

![Underside and transport mechanism](../../media/photos/chessboard-underside.jpg)

## Magnetic interaction

The implementation report says the team selected **N28 neodymium magnets** at the bottom of the pieces after comparing several options. The moving electromagnet's polarity is controlled to distinguish between the two piece polarities. The **L298N** is described as providing the reversing function.

This is a reported construction detail, not a specification for safely energizing the coil. Electromagnet voltage, continuous duty rating, driver thermal limits and protective circuitry must be checked from the actual hardware.

## Motion constants from the published source

The primary Raspberry Pi controller currently sets `DISTANCE_PER_CELL_CM = 5.0` and configures the motor drivers for **quarter-step microstepping**. It also has a drive-pulley diameter parameter of **1.27 cm** and uses an offset of **2.5 cm** in some diagonal/routed moves. These are **software values**, not independently measured mechanical calibration results. A separate `Motors.py` utility contains a **different pulley diameter**; see [`docs/REPORT_TO_CODE_DISCREPANCIES.md`](../../docs/REPORT_TO_CODE_DISCREPANCIES.md).

## What remains unverified

- Carriage origin/homing procedure, end stops, travel limits and reproducible alignment
- Actual mechanism dimensions after assembly (beyond report values)
- Gear/pulley tooth pitch and measured steps-per-centimetre
- Electromagnet lift gap, temperature, and safe duty cycle
- Positioning accuracy and repeatability across a full 8 × 8 sequence

For an engineering demo, keep hands away from the carriage, avoid running with a loose power supply or unrestrained belt, and validate driver and coil ratings before energizing hardware.
