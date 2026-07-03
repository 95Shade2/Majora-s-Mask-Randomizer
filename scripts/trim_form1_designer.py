"""Generate shell-only Form1.Designer.cs (Phase B trim)."""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "scripts" / "Form1.Designer.cs.full"
FALLBACK_SOURCE = ROOT / "MMR_GUI" / "Form1.Designer.cs.full"
OUTPUT = ROOT / "MMR_GUI" / "Form1.Designer.cs"

SHELL_CONTROLS = {
    "Pools_Label",
    "Pool_Tabs",
    "Create_Pool_Textbox",
    "Create_Pool_Label",
    "Create_Pool_Button",
    "Remove_Pool_Combobox",
    "Remove_Pool_Button",
    "Remove_Pool_Label",
    "Change_Pool_Name_Label",
    "Change_Pool_Name_Combobox",
    "Change_Pool_Name_Textbox",
    "Change_Pool_Name_Button",
    "Randomize_Button",
    "Save_Preset_Button",
    "Load_Preset_Button",
    "Open_Base_Rom_Button",
    "Base_Rom_Label",
    "Open_Base_Rom_Dialog",
    "Save_Presets_Dialog",
    "Presets_Combobox",
    "Presets_Label",
    "Seed_Textbox",
    "Seed_Label",
    "label20",
    "Logic_Combobox",
    "Logic_Label",
    "Scrub_Sells_Beans_Tooltip",
    "Tunic_Label",
    "Tunic_ColorDialog",
    "Tunic_Button",
    "Items_Tab",
    "menuStrip1",
    "settingsToolStripMenuItem",
    "createWadToolStripMenuItem",
    "swampScrubSalesBeansToolStripMenuItem",
    "playAsKafeiToolStripMenuItem",
    "removeCutscenesToolStripMenuItem",
    "gCHudToolStripMenuItem",
    "respawnHPsToolStripMenuItem",
    "edibleMirrorShieldToolStripMenuItem",
    "keepRazorSwordOnSoTToolStripMenuItem",
    "oceanSpiderHouseAnyDayToolStripMenuItem",
    "respawnHCsToolStripMenuItem",
    "removeScrubSalesmanAfterTradingToolStripMenuItem",
    "pauseMenuColorsToolStripMenuItem",
    "walletToolStripMenuItem",
    "logicToolStripMenuItem",
    "cutscenesToolStripMenuItem",
    "label5",
    "label6",
    "BlastMaskFrames_Num",
    "BlastMaskSeconds_Label",
    "Targeting_Switch",
    "Targeting_Hold",
}

DECL_ORDER = [
    "Pools_Label",
    "Pool_Tabs",
    "Create_Pool_Textbox",
    "Create_Pool_Label",
    "Create_Pool_Button",
    "Remove_Pool_Combobox",
    "Remove_Pool_Button",
    "Remove_Pool_Label",
    "Change_Pool_Name_Label",
    "Change_Pool_Name_Combobox",
    "Change_Pool_Name_Textbox",
    "Change_Pool_Name_Button",
    "Randomize_Button",
    "Save_Preset_Button",
    "Load_Preset_Button",
    "Open_Base_Rom_Button",
    "Base_Rom_Label",
    "Open_Base_Rom_Dialog",
    "Save_Presets_Dialog",
    "Presets_Combobox",
    "Presets_Label",
    "Seed_Textbox",
    "Seed_Label",
    "label20",
    "Logic_Combobox",
    "Logic_Label",
    "Scrub_Sells_Beans_Tooltip",
    "Tunic_Label",
    "Tunic_ColorDialog",
    "Tunic_Button",
    "Items_Tab",
    "menuStrip1",
    "settingsToolStripMenuItem",
    "createWadToolStripMenuItem",
    "swampScrubSalesBeansToolStripMenuItem",
    "playAsKafeiToolStripMenuItem",
    "removeCutscenesToolStripMenuItem",
    "gCHudToolStripMenuItem",
    "respawnHPsToolStripMenuItem",
    "edibleMirrorShieldToolStripMenuItem",
    "keepRazorSwordOnSoTToolStripMenuItem",
    "oceanSpiderHouseAnyDayToolStripMenuItem",
    "respawnHCsToolStripMenuItem",
    "removeScrubSalesmanAfterTradingToolStripMenuItem",
    "pauseMenuColorsToolStripMenuItem",
    "walletToolStripMenuItem",
    "logicToolStripMenuItem",
    "cutscenesToolStripMenuItem",
    "label5",
    "label6",
    "BlastMaskFrames_Num",
    "BlastMaskSeconds_Label",
    "Targeting_Switch",
    "Targeting_Hold",
]

FIELD_TYPE = {
    "Pools_Label": "Label",
    "Pool_Tabs": "TabControl",
    "Create_Pool_Textbox": "TextBox",
    "Create_Pool_Label": "Label",
    "Create_Pool_Button": "Button",
    "Remove_Pool_Combobox": "ComboBox",
    "Remove_Pool_Button": "Button",
    "Remove_Pool_Label": "Label",
    "Change_Pool_Name_Label": "Label",
    "Change_Pool_Name_Combobox": "ComboBox",
    "Change_Pool_Name_Textbox": "TextBox",
    "Change_Pool_Name_Button": "Button",
    "Randomize_Button": "Button",
    "Save_Preset_Button": "Button",
    "Load_Preset_Button": "Button",
    "Open_Base_Rom_Button": "Button",
    "Base_Rom_Label": "Label",
    "Open_Base_Rom_Dialog": "OpenFileDialog",
    "Save_Presets_Dialog": "SaveFileDialog",
    "Presets_Combobox": "ComboBox",
    "Presets_Label": "Label",
    "Seed_Textbox": "TextBox",
    "Seed_Label": "Label",
    "label20": "Label",
    "Logic_Combobox": "ComboBox",
    "Logic_Label": "Label",
    "Scrub_Sells_Beans_Tooltip": "ToolTip",
    "Tunic_Label": "Label",
    "Tunic_ColorDialog": "ColorDialog",
    "Tunic_Button": "Button",
    "Items_Tab": "TabControl",
    "menuStrip1": "MenuStrip",
    "settingsToolStripMenuItem": "ToolStripMenuItem",
    "createWadToolStripMenuItem": "ToolStripMenuItem",
    "swampScrubSalesBeansToolStripMenuItem": "ToolStripMenuItem",
    "playAsKafeiToolStripMenuItem": "ToolStripMenuItem",
    "removeCutscenesToolStripMenuItem": "ToolStripMenuItem",
    "gCHudToolStripMenuItem": "ToolStripMenuItem",
    "respawnHPsToolStripMenuItem": "ToolStripMenuItem",
    "edibleMirrorShieldToolStripMenuItem": "ToolStripMenuItem",
    "keepRazorSwordOnSoTToolStripMenuItem": "ToolStripMenuItem",
    "oceanSpiderHouseAnyDayToolStripMenuItem": "ToolStripMenuItem",
    "respawnHCsToolStripMenuItem": "ToolStripMenuItem",
    "removeScrubSalesmanAfterTradingToolStripMenuItem": "ToolStripMenuItem",
    "pauseMenuColorsToolStripMenuItem": "ToolStripMenuItem",
    "walletToolStripMenuItem": "ToolStripMenuItem",
    "logicToolStripMenuItem": "ToolStripMenuItem",
    "cutscenesToolStripMenuItem": "ToolStripMenuItem",
    "label5": "Label",
    "label6": "Label",
    "BlastMaskFrames_Num": "NumericUpDown",
    "BlastMaskSeconds_Label": "Label",
    "Targeting_Switch": "RadioButton",
    "Targeting_Hold": "RadioButton",
}


def extract_control_blocks(text: str) -> dict[str, str]:
    lines = text.splitlines()
    blocks: dict[str, list[str]] = {}
    current_name: str | None = None
    current_lines: list[str] = []

    def flush() -> None:
        nonlocal current_name, current_lines
        if current_name and current_name in SHELL_CONTROLS and current_lines:
            blocks[current_name] = "\r\n".join(current_lines) + "\r\n"
        current_name = None
        current_lines = []

    i = 0
    while i < len(lines):
        line = lines[i]
        header = re.match(r"^            // (\w+)$", line)
        if (
            header
            and i + 1 < len(lines)
            and lines[i + 1].strip() == "//"
        ):
            flush()
            current_name = header.group(1)
            current_lines = [line, lines[i + 1]]
            i += 2
            while i < len(lines):
                next_line = lines[i]
                if re.match(r"^            // \w+$", next_line):
                    if i + 1 < len(lines) and lines[i + 1].strip() == "//":
                        break
                current_lines.append(next_line)
                i += 1
            flush()
            continue
        i += 1

    if current_name and current_name in SHELL_CONTROLS and current_lines:
        blocks[current_name] = "\r\n".join(current_lines) + "\r\n"

    for name, block in list(blocks.items()):
        if name == "Items_Tab":
            blocks[name] = re.sub(
                r"            this\.Items_Tab\.Controls\.Add\(this\.\w+\);\r?\n",
                "",
                block,
            )

    return blocks


def extract_tail(text: str) -> str:
    start = text.index("            // Main_Window")
    end = text.index("            this.Load += new System.EventHandler(this.Main_Window_Load);")
    main_window = text[start : end + len("            this.Load += new System.EventHandler(this.Main_Window_Load);\r\n")]
    main_window = main_window.replace(
        "            this.Items_Tab.ResumeLayout(false);\r\n"
        "            this.Items_Items_Tab.ResumeLayout(false);\r\n"
        "            this.Items_Sub_Tab.ResumeLayout(false);\r\n"
        "            this.Items_Sub_Tab_1.ResumeLayout(false);\r\n"
        "            this.Items_Sub_Tab_1.PerformLayout();\r\n"
        "            this.Items_Sub_Tab_2.ResumeLayout(false);\r\n"
        "            this.Items_Sub_Tab_2.PerformLayout();\r\n"
        "            this.Items_Sub_Tab_3.ResumeLayout(false);\r\n"
        "            this.Items_Sub_Tab_3.PerformLayout();\r\n"
        "            this.Items_Sub_Tab_4.ResumeLayout(false);\r\n"
        "            this.Items_Sub_Tab_4.PerformLayout();\r\n"
        "            this.Mask_Page.ResumeLayout(false);\r\n"
        "            this.Mask_Sub_Tab.ResumeLayout(false);\r\n"
        "            this.Mask_Page_1.ResumeLayout(false);\r\n"
        "            this.Mask_Page_1.PerformLayout();\r\n"
        "            this.Mask_Page_2.ResumeLayout(false);\r\n"
        "            this.Mask_Page_2.PerformLayout();\r\n"
        "            this.Mask_Page_3.ResumeLayout(false);\r\n"
        "            this.Mask_Page_3.PerformLayout();\r\n"
        "            this.Bottle_Page.ResumeLayout(false);\r\n"
        "            this.Bottle_Tab.ResumeLayout(false);\r\n"
        "            this.Bottle_Page_1.ResumeLayout(false);\r\n"
        "            this.Bottle_Page_1.PerformLayout();\r\n"
        "            this.Bottle_Page_2.ResumeLayout(false);\r\n"
        "            this.Bottle_Page_2.PerformLayout();\r\n"
        "            this.Song_Page.ResumeLayout(false);\r\n"
        "            this.Song_Page.PerformLayout();\r\n"
        "            this.Rupee_Page.ResumeLayout(false);\r\n"
        "            this.Rupee_Page.PerformLayout();\r\n"
        "            this.Other_Page.ResumeLayout(false);\r\n"
        "            this.Other_Page.PerformLayout();\r\n",
        "",
    )
    return main_window


def main() -> None:
    source_path = SOURCE if SOURCE.exists() else FALLBACK_SOURCE
    if not source_path.exists():
        raise SystemExit(
            "Missing Form1.Designer.cs.full. Export with:\n"
            "  git show HEAD:\"Majora's Mask Randomizer GUI/Form1.Designer.cs\" "
            "> scripts/Form1.Designer.cs.full"
        )
    text = source_path.read_text(encoding="utf-8")
    blocks = extract_control_blocks(text)
    missing = SHELL_CONTROLS - set(blocks)
    if missing:
        raise SystemExit(f"Missing shell control blocks: {sorted(missing)}")

    region_marker = "#region Windows Form Designer generated code"
    header = text.split(region_marker)[0] + "                " + region_marker + "\n\n"
    tail = extract_tail(text)

    init_lines = [
        "                private void InitializeComponent()",
        "                {",
        "            this.components = new System.ComponentModel.Container();",
        "            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main_Window));",
    ]
    for name in DECL_ORDER:
        field_type = FIELD_TYPE[name]
        init_lines.append(
            f"            this.{name} = new System.Windows.Forms.{field_type}();"
        )

    init_lines.append("            this.menuStrip1.SuspendLayout();")
    init_lines.append(
        "            ((System.ComponentModel.ISupportInitialize)(this.BlastMaskFrames_Num)).BeginInit();"
    )
    init_lines.append("            this.SuspendLayout();")

    block_order = [name for name in DECL_ORDER if name in blocks]
    for name in block_order:
        init_lines.append(blocks[name].rstrip())

    init_lines.append(tail.rstrip())
    init_lines.append("            this.menuStrip1.ResumeLayout(false);")
    init_lines.append("            this.menuStrip1.PerformLayout();")
    init_lines.append(
        "            ((System.ComponentModel.ISupportInitialize)(this.BlastMaskFrames_Num)).EndInit();"
    )
    init_lines.append("            this.ResumeLayout(false);")
    init_lines.append("            this.PerformLayout();")
    init_lines.append("")
    init_lines.append("                }")
    init_lines.append("")
    init_lines.append("                #endregion")
    init_lines.append("")

    field_lines = []
    for name in DECL_ORDER:
        field_type = FIELD_TYPE[name]
        field_lines.append(
            f"                private System.Windows.Forms.{field_type} {name};"
        )

    output = (
        header
        + "\n".join(init_lines)
        + "\n"
        + "\n".join(field_lines)
        + "\n    }\n}\n"
    )
    OUTPUT.write_text(output, encoding="utf-8", newline="\r\n")
    print(f"Wrote {OUTPUT} ({len(output.splitlines())} lines)")


if __name__ == "__main__":
    main()
