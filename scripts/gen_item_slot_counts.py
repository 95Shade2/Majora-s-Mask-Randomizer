#!/usr/bin/env python3
"""Extract per-cycle slot counts from Setup_Items() in main.cpp."""

import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN_CPP = ROOT / "src" / "main.cpp"
OUT_JSON = ROOT / "MMR_GUI" / "Data" / "item_slot_counts.json"


def parse_setup_items(text: str) -> dict[str, int]:
    counts: dict[str, int] = {}
    pattern = re.compile(
        r'Items\["([^"]+)"\]\s*=\s*Item\(\s*\n\s*(\d+|0)\s*,',
        re.MULTILINE,
    )
    for match in pattern.finditer(text):
        slot = match.group(1)
        count = int(match.group(2))
        counts[slot] = count
    return counts


def main() -> None:
    text = MAIN_CPP.read_text(encoding="utf-8")
    start = text.find("void Setup_Items()")
    if start < 0:
        raise SystemExit("Setup_Items() not found in main.cpp")
    section = text[start:]
    counts = parse_setup_items(section)
    if not counts:
        raise SystemExit("No item slot counts parsed")
    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    OUT_JSON.write_text(
        json.dumps(counts, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )
    print(f"Wrote {len(counts)} slot counts to {OUT_JSON}")


if __name__ == "__main__":
    main()
