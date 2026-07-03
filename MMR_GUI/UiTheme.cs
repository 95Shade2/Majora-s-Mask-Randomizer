using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public sealed class ThemePalette
    {
        public Font BaseFont { get; set; }
        public Font HintFont { get; set; }
        public Color FormBackColor { get; set; }
        public Color PanelBackColor { get; set; }
        public Color PrimaryButtonBackColor { get; set; }
        public Color PrimaryButtonForeColor { get; set; }
        public int ButtonMinHeight { get; set; }
        public Padding ControlMargin { get; set; }
        public Padding GroupPadding { get; set; }
    }

    public static class UiTheme
    {
        public static ThemePalette LightTheme { get; } = new ThemePalette
        {
            BaseFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
            HintFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
            FormBackColor = Color.FromArgb(245, 245, 247),
            PanelBackColor = Color.FromArgb(250, 250, 252),
            PrimaryButtonBackColor = Color.FromArgb(0, 103, 192),
            PrimaryButtonForeColor = Color.White,
            ButtonMinHeight = 30,
            ControlMargin = new Padding(4),
            GroupPadding = new Padding(8, 4, 8, 8)
        };

        public static ThemePalette DarkTheme { get; } = new ThemePalette
        {
            BaseFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
            HintFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
            FormBackColor = Color.FromArgb(32, 32, 32),
            PanelBackColor = Color.FromArgb(45, 45, 48),
            PrimaryButtonBackColor = Color.FromArgb(0, 122, 204),
            PrimaryButtonForeColor = Color.White,
            ButtonMinHeight = 30,
            ControlMargin = new Padding(4),
            GroupPadding = new Padding(8, 4, 8, 8)
        };

        public static ThemePalette Current => LightTheme;

        public static void ApplyToForm(Form form)
        {
            ThemePalette theme = Current;
            form.AutoScaleMode = AutoScaleMode.Dpi;
            form.Font = theme.BaseFont;
            form.BackColor = theme.FormBackColor;
            ApplyRecursive(form, theme);
        }

        public static void ApplyRecursive(Control root)
        {
            ApplyRecursive(root, Current);
        }

        private static void ApplyRecursive(Control root, ThemePalette theme)
        {
            if (root is Form)
            {
                // already set on form
            }
            else if (root is Panel || root is GroupBox || root is TabPage)
            {
                root.BackColor = theme.PanelBackColor;
            }

            if (root is Label label)
            {
                bool isHint = label.Text.StartsWith("(") || label.Name == "label20";
                Font targetFont = isHint ? theme.HintFont : theme.BaseFont;
                if (label.Font.Size < targetFont.Size ||
                    label.Font.FontFamily.Name != targetFont.FontFamily.Name)
                {
                    label.Font = targetFont;
                }
            }
            else if (!(root is Form) && !(root is Button) && !(root is MenuStrip) && !(root is ToolStrip))
            {
                root.Font = theme.BaseFont;
            }

            if (root is Button button && button.Name != "Randomize_Button")
            {
                button.MinimumSize = new Size(button.MinimumSize.Width, theme.ButtonMinHeight);
                button.Margin = theme.ControlMargin;
            }
            else if (root is GroupBox groupBox)
            {
                groupBox.Padding = theme.GroupPadding;
            }
            else if (root is ComboBox || root is TextBox || root is NumericUpDown)
            {
                root.Margin = theme.ControlMargin;
            }

            foreach (Control child in root.Controls)
            {
                ApplyRecursive(child, theme);
            }
        }

        public static void StylePrimaryButton(Button btn)
        {
            ThemePalette theme = Current;
            btn.Font = new Font(theme.BaseFont.FontFamily, theme.BaseFont.Size, FontStyle.Bold);
            btn.BackColor = theme.PrimaryButtonBackColor;
            btn.ForeColor = theme.PrimaryButtonForeColor;
            btn.FlatStyle = FlatStyle.Flat;
            btn.FlatAppearance.BorderSize = 0;
            btn.MinimumSize = new Size(0, theme.ButtonMinHeight + 6);
            btn.Cursor = Cursors.Hand;
            btn.UseVisualStyleBackColor = false;
        }

        public static void ApplySplitLayout(
            SplitContainer split,
            int preferredDistance,
            int panel1MinSize,
            int panel2MinSize)
        {
            split.Panel1MinSize = panel1MinSize;
            split.Panel2MinSize = panel2MinSize;

            int available = split.Orientation == Orientation.Vertical
                ? split.Width
                : split.Height;
            int maxDistance = available - panel2MinSize - split.SplitterWidth;
            if (maxDistance < panel1MinSize)
            {
                return;
            }

            int distance = Math.Max(panel1MinSize, Math.Min(preferredDistance, maxDistance));
            split.SplitterDistance = distance;
        }

        public static void ScheduleSplitLayout(
            Form form,
            SplitContainer split,
            int preferredDistance,
            int panel1MinSize,
            int panel2MinSize)
        {
            void OnLoad(object sender, EventArgs e)
            {
                form.Load -= OnLoad;
                ApplySplitLayout(split, preferredDistance, panel1MinSize, panel2MinSize);
            }

            form.Load += OnLoad;
        }

        public static void EnableDoubleBuffer(Control control)
        {
            if (control == null)
            {
                return;
            }

            typeof(Control).InvokeMember(
                "DoubleBuffered",
                System.Reflection.BindingFlags.NonPublic |
                System.Reflection.BindingFlags.Instance |
                System.Reflection.BindingFlags.SetProperty,
                null,
                control,
                new object[] { true });
        }
    }
}
