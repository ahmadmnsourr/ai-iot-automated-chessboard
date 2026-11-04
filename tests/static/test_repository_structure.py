"""Offline packaging checks; intentionally do not import or run hardware code."""
import ast
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

class RepositoryAudit(unittest.TestCase):
    def test_major_components_exist(self):
        expected = [
            "README.md", "CONTRIBUTORS.md", ".gitignore",
            "software/raspberry-pi/Main.py",
            "software/raspberry-pi/ChessAgent.py",
            "software/raspberry-pi/requirements.txt",
            "firmware/arduino/BoardController/BoardController.ino",
            "firmware/arduino/BoardController/5-Mux.h",
            "firmware/esp32/ChessGateway/ChessGateway.ino",
            "firmware/esp32/ChessGateway/Config.example.h",
            "hardware/electronics/AutomatedChessboard.fzz",
            "media/demo/physical-chessboard-full-demo.mp4",
            "media/demo/physical-chessboard-preview.gif",
        ]
        missing = [path for path in expected if not (ROOT / path).is_file()]
        self.assertFalse(missing, f"Missing files: {missing}")

    def test_python_syntax_without_importing_hardware(self):
        for path in ROOT.rglob("*.py"):
            if any(part in (".venv", "__pycache__") for part in path.parts):
                continue
            with self.subTest(file=str(path.relative_to(ROOT))):
                ast.parse(path.read_text(encoding="utf-8-sig"), filename=str(path))

    def test_relative_markdown_links(self):
        pat = re.compile(r"(?<!!)\[[^]]*\]\(([^)]+)\)|!\[[^]]*\]\(([^)]+)\)")
        broken = []
        for md in ROOT.rglob("*.md"):
            for match in pat.finditer(md.read_text(encoding="utf-8")):
                target = match.group(1) or match.group(2)
                target = target.strip().split("#",1)[0].split("?",1)[0]
                if not target or target.startswith(("https://", "http://", "mailto:", "data:")):
                    continue
                if not (md.parent / target).exists():
                    broken.append(f"{md.relative_to(ROOT)} -> {target}")
        self.assertEqual(broken, [], f"Broken relative links: {broken}")

    def test_local_credential_files_not_committed(self):
        self.assertFalse((ROOT / "firmware/esp32/ChessGateway/Config.h").exists())
        self.assertFalse((ROOT / "software/raspberry-pi/.env").exists())
        sketch = (ROOT / "firmware/esp32/ChessGateway/ChessGateway.ino").read_text(encoding="utf-8")
        self.assertIn('#include "Config.h"', sketch)
        template = (ROOT / "firmware/esp32/ChessGateway/Config.example.h").read_text(encoding="utf-8")
        self.assertIn("YOUR_WIFI_PASSWORD", template)
        self.assertIn("YOUR_MQTT_PASSWORD", template)

if __name__ == "__main__":
    unittest.main()
