# Software environment and deployment configuration

The firmware and software form a multi-device stack rather than one executable. Its runtime environment consists of a Raspberry Pi application, Arduino sensing firmware, ESP gateway firmware, and a reachable MQTT broker.

## Software toolchains

| Component | Implementation / dependency | Role |
|---|---|---|
| Raspberry Pi | Python 3 on Raspberry Pi OS | Central coordinator and motor control |
| Python packages | `gpiozero`, `pyserial`, `numpy`, `paho-mqtt` | GPIO access, serial link, agent data and MQTT |
| Arduino Mega | Arduino IDE-compatible build, `Wire`, `LiquidCrystal_I2C` | Sensor acquisition and LCD/menu |
| ESP | ESP-compatible Arduino toolchain, `WiFi`, `WebServer`, `MQTT.h` | Web server and broker communication |
| Electronics design | Fritzing | Editable circuit project |
| IoT browser | HTML, CSS, JavaScript | Embedded game portal and board interaction |

Exact original library versions and device images were not recorded in the archived source. The Pi program uses legacy `paho-mqtt` callback conventions, so package compatibility is version-sensitive. The ESP `MQTT.h` API is consistent with the Arduino MQTT family but its exact distribution is not established.

## Configuration interface

The ESP firmware includes [`Config.h`](../firmware/esp32/ChessGateway/ChessGateway.ino) by name; the repository contains [`Config.example.h`](../firmware/esp32/ChessGateway/Config.example.h) as a non-secret configuration schema. The required categories are Wi-Fi SSID/password and MQTT host/username/password. Live private credentials are absent from the project archive.

The Pi controller reads the following process-environment variables:

| Variable | Meaning |
|---|---|
| `CHESS_MQTT_BROKER` | MQTT broker hostname |
| `CHESS_MQTT_PORT` | MQTT port (source default 1883) |
| `CHESS_MQTT_USERNAME` | Broker username |
| `CHESS_MQTT_PASSWORD` | Broker password |
| `CHESS_ARDUINO_PORT` | Linux serial path (source default `/dev/ttyUSB0`) |

[`software/raspberry-pi/.env.example`](../software/raspberry-pi/.env.example) provides the variable names; `Main.py` reads process variables directly and does **not** automatically parse `.env` files. The Python dependency list is in [`requirements.txt`](../software/raspberry-pi/requirements.txt).

## Hardware-specific initialization

The Pi controller initializes GPIO output devices for the motion system and electromagnet. The source contains board-specific pin mappings, thresholds, travel parameters and microstepping configurations. Their values document the original configuration but are not generalized to other mechanical assemblies. The ESP gateway connects over Wi-Fi to an external MQTT broker and hosts HTTP on port 80. The Arduino and Pi exchange move and mode data at 9600 baud.
