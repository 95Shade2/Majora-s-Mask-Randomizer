from pathlib import Path
import importlib.util

ROOT = Path(r"C:/REPOSI~1/MAJORA~1")
p = ROOT / "MMR_GUI" / "Form1.Designer.cs"
spec = importlib.util.spec_from_file_location("t", ROOT / "scripts/trim_form1_designer.py")
t = importlib.util.module_from_spec(spec)
spec.loader.exec_module(t)
t.main()
disk = p.read_text(encoding="utf-8")
print("disk_lines", len(disk.splitlines()))
print("has_maximum", "BlastMaskFrames_Num.Maximum" in disk)
