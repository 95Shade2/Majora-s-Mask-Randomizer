using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class pmc
    {
        private const int ColorDialogContentWidth = 396;
        private const int ColorGroupHeight = 204;
        private const int ColorRowHeight = 32;

        private void ApplyModernLayout()
        {
            SuspendLayout();

            MinimumSize = new Size(420, 560);
            ClientSize = new Size(440, 580);
            StartPosition = FormStartPosition.CenterParent;
            AutoScaleMode = AutoScaleMode.Dpi;
            FormBorderStyle = FormBorderStyle.Sizable;

            Control[] toRemove = { groupBox1, groupBox2, Confirm_Button, label6 };
            foreach (Control control in toRemove)
            {
                Controls.Remove(control);
            }

            Panel scrollPanel = new Panel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                Padding = new Padding(0, 0, 4, 0)
            };
            UiTheme.EnableDoubleBuffer(scrollPanel);

            TableLayoutPanel root = new TableLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 1,
                RowCount = 5,
                Dock = DockStyle.Top,
                Padding = new Padding(12)
            };
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            label6.AutoSize = true;
            label6.Tag = "Hint";
            label6.Margin = new Padding(0, 0, 0, 8);
            label6.Text = "Check a box to use a random color instead of the picker.";

            FlowLayoutPanel toggleBar = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Dock = DockStyle.Top,
                Margin = new Padding(0, 0, 0, 8)
            };
            Button checkAllButton = CreateToggleButton("Check All");
            Button uncheckAllButton = CreateToggleButton("Uncheck All");
            checkAllButton.Click += (sender, args) => SetRandomColorChecks(true);
            uncheckAllButton.Click += (sender, args) => SetRandomColorChecks(false);
            toggleBar.Controls.Add(checkAllButton);
            toggleBar.Controls.Add(uncheckAllButton);

            RebuildPauseGroup();
            RebuildTunicGroup();

            Confirm_Button.Dock = DockStyle.None;
            Confirm_Button.Anchor = AnchorStyles.Right;
            Confirm_Button.AutoSize = true;
            Confirm_Button.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            Confirm_Button.Text = "Confirm";
            Confirm_Button.MinimumSize = new Size(100, UiTheme.Current.ButtonMinHeight);
            Confirm_Button.Margin = new Padding(0, 8, 0, 0);

            FlowLayoutPanel buttonBar = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.RightToLeft,
                WrapContents = false,
                Dock = DockStyle.Top,
                Margin = new Padding(0)
            };
            buttonBar.Controls.Add(Confirm_Button);

            root.Controls.Add(label6, 0, 0);
            root.Controls.Add(toggleBar, 0, 1);
            root.Controls.Add(groupBox1, 0, 2);
            root.Controls.Add(groupBox2, 0, 3);
            root.Controls.Add(buttonBar, 0, 4);

            scrollPanel.Controls.Add(root);
            Controls.Add(scrollPanel);

            ResumeLayout(true);
            UiTheme.ApplyToForm(this);

            RestoreColorSwatchButtonLayout();
        }

        private void RebuildPauseGroup()
        {
            Control[] rows = {
                label1, ItemScreen_Checkbox, ItemSelect_Button,
                label2, MapScreen_Checkbox, MapScreen_Button,
                label3, QuestScreen_Checkbox, Quest_Button,
                label4, MaskScreen_Checkbox, MaskScreen_Button,
                label5, NamePlate_Checkbox, NamePlate_Button
            };
            foreach (Control control in rows)
            {
                groupBox1.Controls.Remove(control);
            }

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                AutoSize = false,
                ColumnCount = 3,
                RowCount = 5,
                Padding = new Padding(4),
                Width = ColorDialogContentWidth - 24,
                Height = ColorRowHeight * 5 + 8
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 32F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 36F));
            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.Absolute, ColorRowHeight));
            }

            AddColorRow(table, 0, label1, ItemScreen_Checkbox, ItemSelect_Button);
            AddColorRow(table, 1, label2, MapScreen_Checkbox, MapScreen_Button);
            AddColorRow(table, 2, label3, QuestScreen_Checkbox, Quest_Button);
            AddColorRow(table, 3, label4, MaskScreen_Checkbox, MaskScreen_Button);
            AddColorRow(table, 4, label5, NamePlate_Checkbox, NamePlate_Button);

            groupBox1.Controls.Add(table);
            groupBox1.Dock = DockStyle.Top;
            groupBox1.AutoSize = false;
            groupBox1.Width = ColorDialogContentWidth;
            groupBox1.Height = ColorGroupHeight;
            groupBox1.Padding = new Padding(8, 4, 8, 8);
            groupBox1.Margin = new Padding(0, 0, 0, 8);
        }

        private void RebuildTunicGroup()
        {
            Control[] rows = {
                label8, LinkColor_Checkbox, LinkColor_Button,
                label9, DekuColor_Checkbox, DekuColor_Button,
                label10, GoronColor_Checkbox, GoronColor_Button,
                label11, ZoraColor_Checkbox, ZoraColor_Button,
                label12, FDColor_Checkbox, FDColor_Button
            };
            foreach (Control control in rows)
            {
                groupBox2.Controls.Remove(control);
            }

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                AutoSize = false,
                ColumnCount = 3,
                RowCount = 5,
                Padding = new Padding(4),
                Width = ColorDialogContentWidth - 24,
                Height = ColorRowHeight * 5 + 8
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 32F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 36F));
            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.Absolute, ColorRowHeight));
            }

            AddColorRow(table, 0, label8, LinkColor_Checkbox, LinkColor_Button);
            AddColorRow(table, 1, label9, DekuColor_Checkbox, DekuColor_Button);
            AddColorRow(table, 2, label10, GoronColor_Checkbox, GoronColor_Button);
            AddColorRow(table, 3, label11, ZoraColor_Checkbox, ZoraColor_Button);
            AddColorRow(table, 4, label12, FDColor_Checkbox, FDColor_Button);

            groupBox2.Controls.Add(table);
            groupBox2.Dock = DockStyle.Top;
            groupBox2.AutoSize = false;
            groupBox2.Width = ColorDialogContentWidth;
            groupBox2.Height = ColorGroupHeight;
            groupBox2.Padding = new Padding(8, 4, 8, 8);
            groupBox2.Margin = new Padding(0);
        }

        private static void AddColorRow(
            TableLayoutPanel table,
            int row,
            Label label,
            CheckBox checkBox,
            Button colorButton)
        {
            label.AutoSize = true;
            label.Anchor = AnchorStyles.Left;
            label.Margin = new Padding(0, 6, 4, 0);

            checkBox.AutoSize = false;
            checkBox.Text = string.Empty;
            checkBox.Size = new Size(22, 22);
            checkBox.MinimumSize = new Size(18, 18);
            checkBox.Anchor = AnchorStyles.None;
            checkBox.Margin = Padding.Empty;

            StyleColorSwatchButton(colorButton);

            table.Controls.Add(label, 0, row);
            table.Controls.Add(checkBox, 1, row);
            table.Controls.Add(colorButton, 2, row);
        }

        private static Button CreateToggleButton(string text)
        {
            return new Button
            {
                Text = text,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                Margin = new Padding(0, 0, 6, 0),
                MinimumSize = new Size(0, UiTheme.Current.ButtonMinHeight)
            };
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
