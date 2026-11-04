# Known implementation boundaries

This archived capstone represents a physical prototype with integrated code and media. The following observations describe its code and evidence boundaries without implying that the entire historical build has been re-executed.

| Area | Observed limitation |
|---|---|
| ESP firmware | Two distinct ESP development revisions exist in the original materials; the repository represents the integrated source copy rather than a verified flashed image |
| HTTP response | The successful `POST /move` path publishes a message but lacks an explicit `server.send(...)` response |
| MQTT | Port 1883 has no in-code TLS configuration; retained publications and reconnect handling can produce stale messages |
| Browser interface | The ESP-embedded pages and standalone HTML snapshots are distinct copies, not an automatically synchronized build pipeline |
| Chess AI | Integrated Python functions implement depth-limited Minimax; alpha–beta pruning is not visible in the supplied agent source |
| Board sensing | The Arduino firmware uses ADC thresholds of 420 and 560; exact installed Hall sensor part number is unresolved (source comments differ from hardware description) |
| Mechanical calibration | The main controller and a separate motor utility use differing pulley diameters; values are source configuration rather than verified measurement |
| Hardware replication | Original CAD assembly files, full final wiring specification and complete qualified BOM were not recovered |
| Performance | Quantitative repeatability, recognition and AI-response metrics are unavailable |

These constraints do not change the recorded fact that the physical chessboard was constructed and demonstrated. They distinguish a complete capstone artifact archive from a guaranteed turnkey hardware replication package.
