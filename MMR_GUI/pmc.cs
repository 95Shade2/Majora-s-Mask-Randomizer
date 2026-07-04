using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class pmc : Form
    {
        public Main_Window parent;
        public bool showing;

        private Dictionary<CheckBox, Button> Check_To_Button;
        private Dictionary<CheckBox, ColorDialog> Check_To_Color;
        private Dictionary<CheckBox, string> Check_To_ID;

        public pmc()
        {
            InitializeComponent();
            FitCompactLayout();
        }

        private const int ColorDialogContentWidth = 396;
        private const int ColorGroupHeight = 204;
        private const int ColorRowHeight = 32;

        public void FitCompactLayout()
        {
            AutoScaleMode = AutoScaleMode.None;
            AutoSize = false;
            MinimumSize = new Size(420, 560);
            ClientSize = new Size(440, 580);

            scrollPanel.Dock = DockStyle.Fill;

            rootTable.AutoSize = true;
            rootTable.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            rootTable.Dock = DockStyle.Top;
            rootTable.MaximumSize = new Size(ColorDialogContentWidth + 44, 0);

            toggleBar.AutoSize = true;
            toggleBar.Dock = DockStyle.None;
            toggleBar.MaximumSize = new Size(ColorDialogContentWidth, 0);

            ApplyColorGroupLayout(groupBox1, pauseMenuTable, bottomMargin: true);
            ApplyColorGroupLayout(groupBox2, tunicsTable, bottomMargin: false);

            buttonBar.AutoSize = true;
            buttonBar.Dock = DockStyle.None;
            Confirm_Button.AutoSize = true;
            Confirm_Button.Anchor = AnchorStyles.Right;
            Confirm_Button.MinimumSize = new Size(100, 30);

            RestoreColorSwatchButtonLayout();
            PerformLayout();
        }

        private static void ApplyColorGroupLayout(GroupBox groupBox, TableLayoutPanel table, bool bottomMargin)
        {
            groupBox.AutoSize = false;
            groupBox.Dock = DockStyle.Top;
            groupBox.Width = ColorDialogContentWidth;
            groupBox.Height = ColorGroupHeight;
            groupBox.Padding = new Padding(8, 4, 8, 8);
            groupBox.Margin = bottomMargin ? new Padding(0, 0, 0, 8) : Padding.Empty;

            table.AutoSize = false;
            table.Dock = DockStyle.Fill;
            table.Width = ColorDialogContentWidth - 24;
            table.Height = ColorRowHeight * 5 + 8;
            table.ColumnStyles.Clear();
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 32F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 36F));
            table.RowStyles.Clear();
            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.Absolute, ColorRowHeight));
            }
        }

        private void pmc_Load(object sender, EventArgs e)
        {
            UiTheme.ApplyToForm(this);
            FitCompactLayout();
            Check_To_Button = new Dictionary<CheckBox, Button>();
            Check_To_Color = new Dictionary<CheckBox, ColorDialog>();
            Check_To_ID = new Dictionary<CheckBox, string>();
            showing = true;

            //checkbox to buttons
            Check_To_Button.Add(LinkColor_Checkbox, LinkColor_Button);
            Check_To_Button.Add(DekuColor_Checkbox, DekuColor_Button);
            Check_To_Button.Add(GoronColor_Checkbox, GoronColor_Button);
            Check_To_Button.Add(ZoraColor_Checkbox, ZoraColor_Button);
            Check_To_Button.Add(FDColor_Checkbox, FDColor_Button);

            Check_To_Button.Add(ItemScreen_Checkbox, ItemSelect_Button);
            Check_To_Button.Add(MapScreen_Checkbox, MapScreen_Button);
            Check_To_Button.Add(QuestScreen_Checkbox, Quest_Button);
            Check_To_Button.Add(MaskScreen_Checkbox, MaskScreen_Button);
            Check_To_Button.Add(NamePlate_Checkbox, NamePlate_Button);

            //checkbox to color dialog boxes
            Check_To_Color.Add(LinkColor_Checkbox, LinkColor_ColorBox);
            Check_To_Color.Add(DekuColor_Checkbox, DekuColor_ColorBox);
            Check_To_Color.Add(GoronColor_Checkbox, GoronColor_ColorBox);
            Check_To_Color.Add(ZoraColor_Checkbox, ZoraColor_ColorBox);
            Check_To_Color.Add(FDColor_Checkbox, FDColor_ColorBox);

            Check_To_Color.Add(ItemScreen_Checkbox, ItemSelect_ColorBox);
            Check_To_Color.Add(MapScreen_Checkbox, MapScreen_ColorBox);
            Check_To_Color.Add(QuestScreen_Checkbox, Quest_ColorBox);
            Check_To_Color.Add(MaskScreen_Checkbox, MaskScreen_ColorBox);
            Check_To_Color.Add(NamePlate_Checkbox, NamePlate_BolorBox);

            //checkbox to color text ids
            Check_To_ID.Add(LinkColor_Checkbox, "Link");
            Check_To_ID.Add(DekuColor_Checkbox, "Deku");
            Check_To_ID.Add(GoronColor_Checkbox, "Goron");
            Check_To_ID.Add(ZoraColor_Checkbox, "Zora");
            Check_To_ID.Add(FDColor_Checkbox, "FD");

            Check_To_ID.Add(ItemScreen_Checkbox, "Item");
            Check_To_ID.Add(MapScreen_Checkbox, "Map");
            Check_To_ID.Add(QuestScreen_Checkbox, "Quest");
            Check_To_ID.Add(MaskScreen_Checkbox, "Mask");
            Check_To_ID.Add(NamePlate_Checkbox, "Name");


            ItemSelect_Button.BackColor = parent.Game_Colors["Item"];
            MapScreen_Button.BackColor = parent.Game_Colors["Map"];
            Quest_Button.BackColor = parent.Game_Colors["Quest"];
            MaskScreen_Button.BackColor = parent.Game_Colors["Mask"];
            NamePlate_Button.BackColor = parent.Game_Colors["Name"];
            LinkColor_Button.BackColor = parent.Game_Colors["Link"];
            DekuColor_Button.BackColor = parent.Game_Colors["Deku"];
            GoronColor_Button.BackColor = parent.Game_Colors["Goron"];
            ZoraColor_Button.BackColor = parent.Game_Colors["Zora"];
            FDColor_Button.BackColor = parent.Game_Colors["FD"];

            ItemSelect_ColorBox.Color = parent.Game_Colors["Item"];
            MapScreen_ColorBox.Color = parent.Game_Colors["Map"];
            Quest_ColorBox.Color = parent.Game_Colors["Quest"];
            MaskScreen_ColorBox.Color = parent.Game_Colors["Mask"];
            NamePlate_BolorBox.Color = parent.Game_Colors["Name"];
            LinkColor_ColorBox.Color = parent.Game_Colors["Link"];
            DekuColor_ColorBox.Color = parent.Game_Colors["Deku"];
            GoronColor_ColorBox.Color = parent.Game_Colors["Goron"];
            ZoraColor_ColorBox.Color = parent.Game_Colors["Zora"];
            FDColor_ColorBox.Color = parent.Game_Colors["FD"];

            foreach (CheckBox checkBox in Check_To_ID.Keys)
            {
                string id = Check_To_ID[checkBox];
                checkBox.Checked = parent.Game_Color_Randomized.ContainsKey(id) &&
                    parent.Game_Color_Randomized[id];
            }
        }

        private void ItemSelect_Button_Click(object sender, EventArgs e)
        {
            ItemSelect_ColorBox.ShowDialog();
            ItemSelect_Button.BackColor = ItemSelect_ColorBox.Color;
        }

        private void MapScreen_Button_Click(object sender, EventArgs e)
        {
            MapScreen_ColorBox.ShowDialog();
            MapScreen_Button.BackColor = MapScreen_ColorBox.Color;
        }

        private void Quest_Button_Click(object sender, EventArgs e)
        {
            Quest_ColorBox.ShowDialog();
            Quest_Button.BackColor = Quest_ColorBox.Color;
        }

        private void MaskScreen_Button_Click(object sender, EventArgs e)
        {
            MaskScreen_ColorBox.ShowDialog();
            MaskScreen_Button.BackColor = MaskScreen_ColorBox.Color;
        }

        private void Confirm_Button_Click(object sender, EventArgs e)
        {
            Dictionary<CheckBox, Button>.KeyCollection keys = Check_To_Button.Keys;

            for (int k = 0; k < keys.Count; k++)
            {
                CheckBox key = keys.ElementAt(k);
                string id = Check_To_ID[key];

                //random color
                if (key.Checked)
                {
                    parent.Game_Color_Randomized[id] = true;
                }
                //user specified color
                else
                {
                    parent.Game_Color_Randomized[id] = false;
                    parent.Game_Colors[id] = Check_To_Button[key].BackColor;
                }
            }

            //parent.Game_Colors["Item"] = ItemSelect_Button.BackColor;
            //parent.Game_Colors["Map"] = MapScreen_Button.BackColor;
            //parent.Game_Colors["Quest"] = Quest_Button.BackColor;
            //parent.Game_Colors["Mask"] = MaskScreen_Button.BackColor;
            //parent.Game_Colors["Name"] = NamePlate_Button.BackColor;
            //parent.Game_Colors["Link"] = LinkColor_Button.BackColor;
            //parent.Game_Colors["Deku"] = DekuColor_Button.BackColor;
            //parent.Game_Colors["Goron"] = GoronColor_Button.BackColor;
            //parent.Game_Colors["Zora"] = ZoraColor_Button.BackColor;
            //parent.Game_Colors["FD"] = FDColor_Button.BackColor;

            this.Close();
        }

        private void NamePlate_Button_Click(object sender, EventArgs e)
        {
            NamePlate_BolorBox.ShowDialog();
            NamePlate_Button.BackColor = NamePlate_BolorBox.Color;
        }

        private void LinkColor_Button_Click(object sender, EventArgs e)
        {
            LinkColor_ColorBox.ShowDialog();
            LinkColor_Button.BackColor = LinkColor_ColorBox.Color;
        }

        private void DekuColor_Button_Click(object sender, EventArgs e)
        {
            DekuColor_ColorBox.ShowDialog();
            DekuColor_Button.BackColor = DekuColor_ColorBox.Color;
        }

        private void GoronColor_Button_Click(object sender, EventArgs e)
        {
            GoronColor_ColorBox.ShowDialog();
            GoronColor_Button.BackColor = GoronColor_ColorBox.Color;
        }

        private void ZoraColor_Button_Click(object sender, EventArgs e)
        {
            ZoraColor_ColorBox.ShowDialog();
            ZoraColor_Button.BackColor = ZoraColor_ColorBox.Color;
        }

        private void FDColor_Button_Click(object sender, EventArgs e)
        {
            FDColor_ColorBox.ShowDialog();
            FDColor_Button.BackColor = FDColor_ColorBox.Color;
        }

        private void LinkColor_Checkbox_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox CheckboxColor = (CheckBox)sender;
            Button ButtonColor = Check_To_Button[CheckboxColor];
            ColorDialog ColorColor = Check_To_Color[CheckboxColor];

            ButtonColor.Enabled = !CheckboxColor.Checked;

            if (ButtonColor.Enabled)
            {
                ButtonColor.BackColor = ColorColor.Color;
            }
            else
            {
                ButtonColor.BackColor = DefaultBackColor;
            }
        }

        private void pmc_FormClosing(object sender, FormClosingEventArgs e)
        {
            parent.color_menu_form = new pmc();
            showing = false;
        }

        private void checkAllButton_Click(object sender, EventArgs e)
        {
            SetRandomColorChecks(true);
        }

        private void uncheckAllButton_Click(object sender, EventArgs e)
        {
            SetRandomColorChecks(false);
        }

        private void SetRandomColorChecks(bool value)
        {
            CheckBox[] checks =
            {
                ItemScreen_Checkbox,
                MapScreen_Checkbox,
                QuestScreen_Checkbox,
                MaskScreen_Checkbox,
                NamePlate_Checkbox,
                LinkColor_Checkbox,
                DekuColor_Checkbox,
                GoronColor_Checkbox,
                ZoraColor_Checkbox,
                FDColor_Checkbox
            };

            foreach (CheckBox check in checks)
            {
                check.Checked = value;
            }
        }

        private void RestoreColorSwatchButtonLayout()
        {
            Button[] colorButtons =
            {
                ItemSelect_Button,
                MapScreen_Button,
                Quest_Button,
                MaskScreen_Button,
                NamePlate_Button,
                LinkColor_Button,
                DekuColor_Button,
                GoronColor_Button,
                ZoraColor_Button,
                FDColor_Button
            };

            foreach (Button button in colorButtons)
            {
                StyleColorSwatchButton(button);
            }
        }

        private static void StyleColorSwatchButton(Button colorButton)
        {
            colorButton.Tag = "ColorSwatch";
            colorButton.AutoSize = false;
            colorButton.MinimumSize = Size.Empty;
            colorButton.Size = new Size(28, 30);
            colorButton.Margin = Padding.Empty;
            colorButton.Anchor = AnchorStyles.None;
            colorButton.UseVisualStyleBackColor = false;
        }
    }
}
