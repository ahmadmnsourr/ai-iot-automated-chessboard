# Public code and credentials

Public versions of the Raspberry Pi and ESP configuration do not contain the original Wi-Fi/MQTT credentials found in the archived files.

- **If the original broker credentials are still active, revoke/rotate them**. Removing secrets from a new GitHub upload does not invalidate credentials that have been previously exposed elsewhere.
- The Pi reads MQTT values through environment variables; the ESP requires a local ignored `Config.h` copied from `Config.example.h`.
- The original design uses plain MQTT on port 1883; restrict the network or upgrade to authenticated encrypted transport before using the architecture beyond a trusted lab.
- The HTTP endpoints are demonstration interfaces without a reviewed authorization model. Do not expose the ESP directly to the public internet.

Do not publish images of handwritten passwords or cloud credentials.
