#!/usr/bin/env python3
"""Generate standard_presets.json with MMSETTINGS_V1 hashes for shipped presets.

Re-run whenever files in presets/ change.
"""

import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PRESETS_DIR = ROOT / "presets"
OUT_JSON = ROOT / "MMR_GUI" / "Data" / "standard_presets.json"

HASH_VERSION = "MMSETTINGS_V1"
EXCLUDED_SETTINGS = {"Rom", "Seed", "Wad", "WarnPoolBalance"}
SETTINGS_ORDER = [
    "Logic",
    "Kafei",
    "ScrubBeans",
    "Remove_Cutscenes",
    "GC_Hud",
    "BlastMask_Cooldown",
    "RespawnHPs",
    "LikeLikeMirror",
    "KeepRazor",
    "OceanAnyDay",
    "RespawnHCs",
    "Targeting",
    "TradeQuest",
    "Difficulty",
]
SETTINGS_DEFAULTS = {
    "Logic": "",
    "Kafei": "False",
    "ScrubBeans": "False",
    "Remove_Cutscenes": "False",
    "GC_Hud": "False",
    "BlastMask_Cooldown": "310",
    "RespawnHPs": "False",
    "LikeLikeMirror": "False",
    "KeepRazor": "False",
    "OceanAnyDay": "False",
    "RespawnHCs": "False",
    "Targeting": "Switch",
    "TradeQuest": "False",
    "Difficulty": "Medium",
}
WALLET_ORDER = ["Small", "Medium", "Large"]


def parse_ini(text: str) -> dict[str, dict[str, str]]:
    sections: dict[str, dict[str, str]] = {}
    current = None
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line or line.startswith(";"):
            continue
        if line.startswith("[") and line.endswith("]"):
            current = line[1:-1]
            sections.setdefault(current, {})
            continue
        if current is None or "=" not in line:
            continue
        key, value = line.split("=", 1)
        sections[current][key] = value
    return sections


def build_hash_payload(sections: dict[str, dict[str, str]]) -> str:
    parts: list[str] = []

    items = sections.get("items", {})
    if items:
        parts.append("[items]")
        for key in sorted(items.keys()):
            parts.append(f"{key}={items[key]}")

    plando = sections.get("plando", {})
    if plando:
        parts.append("[plando]")
        for key in sorted(plando.keys()):
            parts.append(f"{key}={plando[key]}")

    settings = sections.get("settings", {})
    if settings:
        parts.append("[settings]")
        for key in SETTINGS_ORDER:
            if key in EXCLUDED_SETTINGS:
                continue
            value = settings.get(key, SETTINGS_DEFAULTS[key])
            parts.append(f"{key}={value}")

    wallets = sections.get("wallets", {})
    if wallets:
        parts.append("[wallets]")
        for key in WALLET_ORDER:
            if key in wallets:
                parts.append(f"{key}={wallets[key]}")

    pools = sections.get("pools", {})
    if pools:
        parts.append("[pools]")
        for index in sorted(pools.keys(), key=lambda k: int(k)):
            parts.append(f"{index}={pools[index]}")

    cutscenes = sections.get("cutscenes", {})
    if cutscenes:
        parts.append("[cutscenes]")
        for key in sorted(cutscenes.keys()):
            parts.append(f"{key}={cutscenes[key]}")

    return "\n".join(parts) + "\n"


def compute_hash(payload: str) -> str:
    digest = hashlib.sha256((payload + HASH_VERSION).encode("utf-8")).hexdigest()
    return digest[:16]


def main() -> None:
    preset_entries = []
    for ini_path in sorted(PRESETS_DIR.glob("*.ini")):
        sections = parse_ini(ini_path.read_text(encoding="utf-8"))
        payload = build_hash_payload(sections)
        preset_entries.append(
            {
                "name": ini_path.stem,
                "settingsHash": compute_hash(payload),
            }
        )

    manifest = {"version": "MMPRESET_V1", "presets": preset_entries}
    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    OUT_JSON.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {len(preset_entries)} preset hashes to {OUT_JSON}")


if __name__ == "__main__":
    main()
