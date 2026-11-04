# ESP gateway revision audit — provisional implementation

The original submission has **two different ESP gateway revisions**. Version names below are descriptive audit labels only; they are not official release tags.

| Detail | Integrated `FYP-Chess-Codes/ESP/ESP-Chess/` | Alternate `FYP-Iot/FYP-IoT/ESP-Chess/` |
|---|---|---|
| Main sketch | `ESP-Chess.ino` | `ESP-Chess.ino` |
| Web pages header | `Pages.h` (~48.8 KB) | `Pages.h` (~57.7 KB) |
| `client.begin()` | Explicit MQTT port | Broker with default port |
| MQTT subscribe | QoS 1 attempt and log | Simpler subscribe without return check |
| Publishing | Explicit acknowledgment with a retained/QoS1 publish | Simpler publish operation |
| MQTT callback | Saves received move for `/getmove` | Contains an attempt to `server.send()` from callback |
| Serial commands | Handles `pub:` and `sub:` prefixes | Relays generic serial input |
| HTTP endpoints | `/`, `/game`, `/move`, `/getmove` | Similar routes, but handler behavior differs |

**Selected curated repository code:** The integrated version from `FYP-Chess-Codes` (retaining the same communication behavior, with credentials replaced by an ignored `Config.h` template). The ESP alternative is **not published as a competing main version**; it remains in the private original archive.

## Cross-system messages supported in code

- **ESP → Raspberry Pi** through MQTT topic `ESP-TOPIC`, carrying a simple move such as `e2e4`.
- **Raspberry Pi → ESP** through `RPI-TOPIC`, to deliver a move for the browser client.
- **Browser → ESP** via HTTP `POST /move`; the request argument is called `move`.
- **Browser pull** via `GET /getmove`, which returns a waiting move or HTTP 204.
- **Arduino → Pi** mode and move messages via 9600-baud serial (separate physical channel).

## Important integration checks before a firmware release

1. **HTTP completion:** `handleEspMove()` publishes a move but does not visibly call `server.send()` for a successful `POST /move`. Verify that the web browser doesn't wait indefinitely and explicitly send a success response if needed.
2. **Retained MQTT messages:** ESP publishes retained moves; Pi also publishes to clear a retained `ESP-TOPIC` on connection. Test restart/disconnect behavior to avoid replaying stale moves.
3. **Topic and broker consistency:** Confirm Pi and ESP credentials, broker hostname, port, subscriptions and topic names are aligned on the same trusted network.
4. **Callback handling:** Ensure the alternate callback's HTTP response code is not accidentally mixed into the selected sketch.
5. **Page compatibility:** The standalone web pages and the firmware's embedded `Pages.h` differ in form and are not automatically synchronized. The embedded pages are the firmware's active web UI.
6. **Never deploy real credentials** in public source. Set your own `Config.h` locally and check `.gitignore`.

**No end-to-end compatibility test has been run in this reconstruction**. Treat the selected revision as a documented baseline, not proof of the exact firmware flashed on the final prototype.
