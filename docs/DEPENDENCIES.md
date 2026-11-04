# Toolchains and dependency inventory

This is a **source-level reconstruction** of a University of Balamand mechatronics capstone, not a verified one-command install. The following dependency information is based on the selected code imports and original project notes; exact tool versions were not recorded and were **not independently validated** against the physical system.

| Subsystem | Toolchain / package | Reason and status |
|---|---|---|
| Raspberry Pi 5 | Raspberry Pi OS, Python 3 | The main controller is written in Python; OS and Python versions used on the original device are unknown |
| Raspberry Pi Python | `numpy` | Imported by `ChessAgent.py`; version not recorded |
| Raspberry Pi Python | `pyserial` (`import serial`) | Arduino serial link, default 9600 baud |
| Raspberry Pi Python | `gpiozero` | Stepper/magnet GPIO output; check Raspberry Pi 5 GPIO backend compatibility |
| Raspberry Pi Python | `paho-mqtt<2` | Pinning to legacy API semantics used by `Main.py`; compatibility recommendation only, not a tested lockfile |
| Arduino Mega | Arduino IDE + board support | Controller firmware in `firmware/arduino/BoardController/` |
| Arduino Mega | `Wire.h`, `LiquidCrystal_I2C.h` | I2C LCD interface; several libraries use this name, the exact original distribution is unknown |
| ESP web/MQTT gateway | ESP32/ESP-compatible Arduino board package | Confirm the actual development board before flashing |
| ESP web/MQTT gateway | ESP board `WiFi.h`, `WebServer.h` + `MQTT.h` | MQTT API resembles the 256dpi Arduino MQTT library; exact installed version has not been confirmed |
| Electrical design | Fritzing | Editable `.fzz` electronics file |
| Mechanical assembly | Physical hardware including 2 axis motors, drivers, Hall sensors and electromagnetic pick-up | No native CAD assembly found in supplied archive |

## Python environment (code inspection only)

From the repository root on a compatible Raspberry Pi:

```bash
cd software/raspberry-pi
python3 -m venv .venv
. .venv/bin/activate
python -m pip install -r requirements.txt
```

**Do not run `Main.py` blindly.** Its initialization creates actual GPIO outputs and may energize hardware. Verify wiring and follow [`SETUP.md`](SETUP.md) first. Installing dependencies alone does not confirm GPIO, broker, serial, camera or motor compatibility.

## ESP configuration

Copy `firmware/esp32/ChessGateway/Config.example.h` to `Config.h` in the same directory; set **your own** Wi-Fi and broker values. `Config.h` is excluded from Git. The firmware cannot be compiled until this local configuration is provided.

## Web interface

`web/standalone/` is for code review. Its `/move` and `/getmove` endpoints depend on the ESP gateway; opening the pages as local HTML is not a substitute for running the hardware. The rendered ESP pages in `Pages.h` are not automatically synchronized with these standalone copies. The pages may request Google Fonts and a third-party background image when connected to the internet.

## Versions

This repository deliberately does **not** fabricate exact Arduino, firmware, library or Python versions. A reproducer should record their successful versions after testing a matching board and hardware configuration.
