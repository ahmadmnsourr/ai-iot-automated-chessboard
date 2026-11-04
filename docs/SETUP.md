# Setup and operation (reconstruction guide)

**Status: not independently validated on the original hardware.** Wiring, boards, dependencies and firmware revisions must be confirmed before treating this as a ready-to-run kit.

## Hardware and safety
Use the supplied Fritzing design as a reference, but validate supply voltage, motor-driver wiring, current limits, protective switching, limit positions, and the electromagnet power path before applying power. Do not use the schematic as a verified safety-certified wiring guide. Keep the carriage clear of hands during motion tests.

## Arduino Mega
1. Open `firmware/arduino/BoardController/BoardController.ino` in Arduino IDE.
2. Install a compatible `LiquidCrystal_I2C` library and `Wire` support; select the appropriate board and port.
3. Confirm the multiplexer analog connections and buttons against `5-Mux.h`.
4. Compile and upload, then test serial output at 9600 baud with the motion system disabled.

## ESP gateway
1. Open `firmware/esp32/ChessGateway/ChessGateway.ino`.
2. Copy `Config.example.h` to **`Config.h`** in the same folder and fill *your own* SSID, password and MQTT credentials. `Config.h` is excluded from Git.
3. Install an ESP32 board package and the matching `MQTT.h` library supported by the original code. Compile and upload after selecting the actual board model.
4. Verify `/`, `/game`, `/move`, and `/getmove` on a trusted local network.

## Raspberry Pi
1. Use Raspberry Pi OS on the intended Pi, with a compatible Python interpreter and GPIO interface.
2. Install the dependencies listed in `software/raspberry-pi/requirements.txt` in a local Python environment.
3. Set `CHESS_MQTT_BROKER`, `CHESS_MQTT_USERNAME`, `CHESS_MQTT_PASSWORD`, `CHESS_MQTT_PORT`, and optionally `CHESS_ARDUINO_PORT` in the shell environment. An `.env.example` lists the names; `Main.py` reads environment variables but does **not** automatically load an `.env` file.
4. Confirm the actual GPIO assignments in `Main.py` against your wired motors and magnet driver. Check serial-port permissions.
5. Start with non-energized GPIO/serial checks, then launch `python Main.py` only with supervision and a known safe mechanism state.

## Important limitations
- No turnkey installer or verified automated test suite exists in the supplied materials.
- The Python MQTT callback code was written against older `paho-mqtt` APIs; dependency compatibility needs testing.
- The ESP client uses a blocking MQTT reconnect loop, so a missing broker may prevent responsive operation.
- Configuration examples are intentionally not functional without user-provided credentials and hardware configuration.
