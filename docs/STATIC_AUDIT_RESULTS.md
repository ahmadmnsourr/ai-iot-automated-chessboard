# Static audit results

**Preparation date:** 2026-11-04. **Result:** 4 standard-library static tests passed on the curated archive.

Command: `python -m unittest discover -s tests/static -v`

Checks include:

- Presence of the major source, firmware, hardware design, and demo files.
- Python syntax parsing without importing or executing the GPIO code.
- Resolution of local Markdown links and image/video references.
- Non-inclusion of the local ESP `Config.h` and Raspberry Pi `.env` secrets files.

Additionally, the package was checked for expected folder structure, SHA-256 hashes were recorded in `SOURCE_CHECKSUMS.sha256`, the six curated photos have no EXIF/GPS metadata, and the full demo has an encoded duration of approximately 77 seconds.

**Limitations:** These checks do **not** compile the Arduino or ESP firmware, exercise JavaScript/HTML, verify electronic wiring, validate credentials, inspect every image for sensitive visual content, run the AI, or test the complete machine. Credential-checking is necessarily incomplete; review the package manually before public distribution.
