import re
import json
from pathlib import Path

text = Path("MMR_GUI/Form1.Designer.cs").read_text(encoding="utf-8")

tab_controls = {}
for line in text.splitlines():
    m = re.search(r"this\.(\w+)\.Controls\.Add\(this\.(\w+)\)", line)
    if m:
        parent, child = m.group(1), m.group(2)
        tab_controls.setdefault(parent, []).append(child)

structure = {
    "Items": ["Items_Sub_Tab_1", "Items_Sub_Tab_2", "Items_Sub_Tab_3", "Items_Sub_Tab_4"],
    "Masks": ["Mask_Page_1", "Mask_Page_2", "Mask_Page_3"],
    "Bottles": ["Bottle_Page_1", "Bottle_Page_2"],
    "Songs": ["Song_Page"],
    "Rupees": ["Rupee_Page"],
    "Maps/Others": ["Other_Page"],
}


def get_item_name(ctrl):
    if ctrl.endswith("_Pool") or ctrl.endswith("_Gives") or ctrl.startswith("label"):
        return None
    pat = rf"this\.{re.escape(ctrl)}\.Text = \"([^\"]+)\";"
    m = re.search(pat, text)
    if not m:
        return None
    pat = rf"this\.{re.escape(ctrl)} = new System\.Windows\.Forms\.(\w+)"
    type_m = re.search(pat, text)
    if not type_m or type_m.group(1) != "CheckBox":
        return None
    return m.group(1).replace("\\'", "'")


result = {}
for cat, pagenames in structure.items():
    if len(pagenames) == 1:
        items = []
        for c in tab_controls.get(pagenames[0], []):
            name = get_item_name(c)
            if name:
                items.append(name)
        result[cat] = items
    else:
        result[cat] = {}
        for i, page in enumerate(pagenames, 1):
            items = []
            for c in tab_controls.get(page, []):
                name = get_item_name(c)
                if name:
                    items.append(name)
            result[cat][f"Page {i}"] = items

out = Path("MMR_GUI/item_tabs.json")
out.write_text(json.dumps(result, indent=2), encoding="utf-8")
total = sum(len(v) if isinstance(v, list) else sum(len(x) for x in v.values()) for v in result.values())
print(f"Wrote {out} with {total} items")
