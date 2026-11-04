# Security and network boundary

The automated chessboard was developed as an integrated local prototype with Wi-Fi and MQTT connectivity. Its source archive excludes the live Wi-Fi and broker credentials present in some development snapshots.

## Configuration separation

- `firmware/esp32/ChessGateway/Config.example.h` describes Wi-Fi and MQTT configuration fields without embedded real secrets.
- `software/raspberry-pi/.env.example` lists the controller's environment variable names.
- `.gitignore` excludes `Config.h`, local `.env` files, private keys and similar local artifacts.

## Network characteristics

The ESP HTTP server is configured for port 80. MQTT traffic uses port 1883 in the source configuration, without MQTT TLS handling in the included code. Access controls and traffic confidentiality therefore depend on the surrounding network and broker configuration. The web endpoints do not constitute an internet-hardened application.

## Reliability boundaries

The ESP gateway uses a blocking reconnect loop for MQTT. Move publication may use retained MQTT messages, and the web `POST /move` handler lacks an explicit success response in the included revision. These behaviors can affect reconnection and client feedback and are characterized in [IoT interface](IOT_INTERFACE.md) and [known limitations](KNOWN_LIMITATIONS.md).
