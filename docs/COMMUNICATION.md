# Serial, MQTT and HTTP interfaces

The following are **observed in the selected source**, not independently exercised in the lab.

## Arduino ↔ Raspberry Pi
- Default serial device: `/dev/ttyUSB0` (override through `CHESS_ARDUINO_PORT`).
- Baud rate: **9600**.
- Mode reports include the prefix `MODE:` and the labels `HvH` and `HvAI`.
- Move strings are four characters using source and destination squares, e.g. `e2e4`.
- Raspberry Pi responses use newline-delimited serial strings.

## MQTT
- The ESP and Pi use `ESP-TOPIC` and `RPI-TOPIC` in the selected baseline.
- Messages carry move strings; verify broker authentication and topic semantics before deploying.
- Original source uses MQTT port **1883**, which ordinarily lacks transport encryption. For real deployment, configure an appropriately secured broker and network.

## ESP HTTP interface
- `GET /` serves the index page.
- `GET /game` serves the game UI.
- `POST /move` reads a `move` form argument.
- `GET /getmove` returns a pending move, or HTTP 204 when none is present.

The standalone HTML files under `web/standalone/` are additional copies for examination; the ESP firmware embeds its active pages in `Pages.h`. They are not automatically synchronized.
