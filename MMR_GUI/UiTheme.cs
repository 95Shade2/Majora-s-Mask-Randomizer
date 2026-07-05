using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
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
        public Color PrimaryButtonBorderColor { get; set; }
        public Color ForeColor { get; set; }
        public Color HintForeColor { get; set; }
        public Color DisabledForeColor { get; set; }
        public Color ControlBackColor { get; set; }
        public Color ControlForeColor { get; set; }
        public Color BorderColor { get; set; }
        public Color MenuBackColor { get; set; }
        public Color MenuForeColor { get; set; }
        public Color MenuHoverBackColor { get; set; }
        public Color TabSelectedBackColor { get; set; }
        public Color TabForeColor { get; set; }
        public Color TabIconColor { get; set; }
        public Color TabSelectedAccentColor { get; set; }
        public Color ListEvenRowColor { get; set; }
        public Color ListOddRowColor { get; set; }
        public Color ListSelectedBackColor { get; set; }
        public Color ListSelectedForeColor { get; set; }
        public Color ListHeaderBackColor { get; set; }
        public Color ListHeaderForeColor { get; set; }
        public Color TabStripBackColor { get; set; }
        public Color TabUnselectedBackColor { get; set; }
        public Color PlandoRowBackColor { get; set; }
        public Color WarningForeColor { get; set; }
        public Color ErrorForeColor { get; set; }
        public Color CellDefaultBackColor { get; set; }
        public Color ButtonBackColor { get; set; }
        public Color ButtonForeColor { get; set; }
        public Color CheckBoxBackColor { get; set; }
        public Color CheckBoxBorderColor { get; set; }
        public Color CheckBoxCheckedBackColor { get; set; }
        public Color CheckBoxCheckMarkColor { get; set; }
        public Color MarkGreen { get; set; }
        public Color MarkRed { get; set; }
        public Color MarkOrange { get; set; }
        public Color MarkBlue { get; set; }
        public Color MarkPurple { get; set; }
        public int ButtonMinHeight { get; set; }
        public Padding ControlMargin { get; set; }
        public Padding GroupPadding { get; set; }

        public Color GetMarkColor(int state)
        {
            switch (state)
            {
                case 1:
                    return MarkGreen;
                case 2:
                    return MarkRed;
                case 3:
                    return MarkOrange;
                case 4:
                    return MarkBlue;
                case 5:
                    return MarkPurple;
                default:
                    return CellDefaultBackColor;
            }
        }
    }

    public static class UiTheme
    {
        private static bool _isDark = true;
        private const int CheckBoxGlyphSize = 18;
        private const int CheckBoxTextGap = 8;

        private const string BingoMarkGreenHex = "#005511";
        private const string BingoMarkRedHex = "#550011";
        private const string BingoMarkOrangeHex = "#553300";
        private const string BingoMarkBlueHex = "#001155";
        private const string BingoMarkPurpleHex = "#220055";
        private static readonly Color BingoMarkGreenColor = Color.FromArgb(0, 85, 17);
        private static readonly Color BingoMarkRedColor = Color.FromArgb(85, 0, 17);
        private static readonly Color BingoMarkOrangeColor = Color.FromArgb(85, 51, 0);
        private static readonly Color BingoMarkBlueColor = Color.FromArgb(0, 17, 85);
        private static readonly Color BingoMarkPurpleColor = Color.FromArgb(34, 0, 85);

        [DllImport("uxtheme.dll", CharSet = CharSet.Unicode)]
        private static extern int SetWindowTheme(IntPtr hWnd, string pszSubAppName, string pszSubIdList);

        public static bool IsDark => _isDark;

        public static ThemePalette LightTheme { get; } = CreateLightTheme();
        public static ThemePalette DarkTheme { get; } = CreateDarkTheme();

        public static ThemePalette Current => _isDark ? DarkTheme : LightTheme;

        public static void SetDarkMode(bool dark)
        {
            _isDark = dark;
        }

        public static void ApplyToForm(Form form)
        {
            ThemePalette theme = Current;
            form.AutoScaleMode = AutoScaleMode.Dpi;
            form.Font = theme.BaseFont;
            form.ForeColor = theme.ForeColor;
            form.BackColor = theme.FormBackColor;
            ApplyRecursive(form, theme);
        }

        public static void ApplyRecursive(Control root)
        {
            ApplyRecursive(root, Current);
        }

        private static void ApplyRecursive(Control root, ThemePalette theme)
        {
            if (root is Form form)
            {
                form.ForeColor = theme.ForeColor;
            }
            else if (root is Panel || root is GroupBox || root is TabPage ||
                     root is TableLayoutPanel || root is FlowLayoutPanel)
            {
                root.BackColor = theme.PanelBackColor;
                root.ForeColor = theme.ForeColor;
                if (root is TabPage tabPage)
                {
                    tabPage.UseVisualStyleBackColor = false;
                }
            }
            else if (root is SplitContainer split)
            {
                split.BackColor = theme.FormBackColor;
                split.Panel1.BackColor = theme.PanelBackColor;
                split.Panel2.BackColor = theme.PanelBackColor;
            }

            if (root is Label label)
            {
                bool isHint = label.Text.StartsWith("(") ||
                              label.Name == "label20" ||
                              (label.Tag as string) == "Hint";
                Font targetFont = isHint ? theme.HintFont : theme.BaseFont;
                if (label.Font.Size < targetFont.Size ||
                    label.Font.FontFamily.Name != targetFont.FontFamily.Name)
                {
                    label.Font = targetFont;
                }

                ApplyLabelTheme(label, theme, isHint);
            }
            else if (root is TextBox || root is ComboBox || root is ListBox)
            {
                root.Font = theme.BaseFont;
                root.BackColor = theme.ControlBackColor;
                root.ForeColor = theme.ControlForeColor;
                root.Margin = theme.ControlMargin;
                ApplyNativeControlTheme(root);
            }
            else if (root is NumericUpDown numericUpDown)
            {
                ApplyNumericUpDownTheme(numericUpDown, theme);
            }
            else if (root is CheckBox checkBox)
            {
                ApplyCheckBoxTheme(checkBox, theme);
            }
            else if (root is RadioButton radioButton)
            {
                ApplyRadioButtonTheme(radioButton, theme);
            }
            else if (root is MenuStrip || root is ToolStrip || root is StatusStrip)
            {
                if (root is ToolStrip strip)
                {
                    ApplyToolStripTheme(strip, theme);
                }
                else
                {
                    root.BackColor = theme.MenuBackColor;
                    root.ForeColor = theme.MenuForeColor;
                    root.Font = theme.BaseFont;
                }
            }
            else if (root is TabControl tabs)
            {
                ApplyTabControlTheme(tabs, theme);
            }
            else if (root is ListView listView)
            {
                ApplyListViewTheme(listView, theme);
            }
            else if (root is DataGridView dataGridView)
            {
                ApplyDataGridViewTheme(dataGridView, theme);
            }
            else if (root is Button button)
            {
                ApplyButtonTheme(button, theme);
            }
            else if (!(root is Form))
            {
                root.Font = theme.BaseFont;
            }

            if (root is GroupBox groupBox)
            {
                groupBox.Padding = theme.GroupPadding;
            }

            foreach (Control child in root.Controls)
            {
                ApplyRecursive(child, theme);
            }
        }

        private static void ApplyNumericUpDownTheme(NumericUpDown numericUpDown, ThemePalette theme)
        {
            // Do not call SetWindowTheme on NumericUpDown — UpDownBase still uses
            // VisualStyleRenderer and throws when the theme handle is stripped.
            numericUpDown.Font = theme.BaseFont;
            numericUpDown.BackColor = theme.ControlBackColor;
            numericUpDown.ForeColor = theme.ControlForeColor;
            numericUpDown.BorderStyle = BorderStyle.FixedSingle;
            numericUpDown.Margin = theme.ControlMargin;
        }

        public static void ApplyNativeControlTheme(Control control)
        {
            if (control == null)
            {
                return;
            }

            if (control.IsHandleCreated)
            {
                SetWindowTheme(control.Handle, string.Empty, string.Empty);
            }
            else
            {
                control.HandleCreated += OnControlHandleCreatedForNativeTheme;
            }
        }

        private static void OnControlHandleCreatedForNativeTheme(object sender, EventArgs e)
        {
            Control control = (Control)sender;
            control.HandleCreated -= OnControlHandleCreatedForNativeTheme;
            SetWindowTheme(control.Handle, string.Empty, string.Empty);
        }

        public static void ApplyTabControlTheme(TabControl tabs)
        {
            ApplyTabControlTheme(tabs, Current);
        }

        private static void ApplyTabControlTheme(TabControl tabs, ThemePalette theme)
        {
            tabs.BackColor = theme.TabStripBackColor;
            tabs.ForeColor = theme.TabForeColor;
            tabs.Font = theme.BaseFont;
            ApplyNativeControlTheme(tabs);

            foreach (TabPage page in tabs.TabPages)
            {
                page.BackColor = theme.PanelBackColor;
                page.ForeColor = theme.ForeColor;
                page.UseVisualStyleBackColor = false;
            }
        }

        public static void ApplyListViewTheme(ListView listView)
        {
            ApplyListViewTheme(listView, Current);
        }

        private static void ApplyListViewTheme(ListView listView, ThemePalette theme)
        {
            listView.BackColor = theme.ListEvenRowColor;
            listView.ForeColor = theme.ForeColor;
            listView.Font = theme.BaseFont;
            ApplyNativeControlTheme(listView);
        }

        public static void ApplyDataGridViewTheme(DataGridView grid)
        {
            ApplyDataGridViewTheme(grid, Current);
        }

        private static void ApplyDataGridViewTheme(DataGridView grid, ThemePalette theme)
        {
            grid.Font = theme.BaseFont;
            grid.EnableHeadersVisualStyles = false;
            grid.BackgroundColor = theme.PanelBackColor;
            grid.GridColor = theme.BorderColor;
            grid.BorderStyle = BorderStyle.None;

            grid.DefaultCellStyle.BackColor = theme.ListEvenRowColor;
            grid.DefaultCellStyle.ForeColor = theme.ForeColor;
            grid.DefaultCellStyle.SelectionBackColor = theme.ListSelectedBackColor;
            grid.DefaultCellStyle.SelectionForeColor = theme.ListSelectedForeColor;

            grid.AlternatingRowsDefaultCellStyle.BackColor = theme.ListOddRowColor;
            grid.AlternatingRowsDefaultCellStyle.ForeColor = theme.ForeColor;
            grid.AlternatingRowsDefaultCellStyle.SelectionBackColor = theme.ListSelectedBackColor;
            grid.AlternatingRowsDefaultCellStyle.SelectionForeColor = theme.ListSelectedForeColor;

            grid.ColumnHeadersDefaultCellStyle.BackColor = theme.ListHeaderBackColor;
            grid.ColumnHeadersDefaultCellStyle.ForeColor = theme.ListHeaderForeColor;
            grid.ColumnHeadersDefaultCellStyle.SelectionBackColor = theme.ListHeaderBackColor;
            grid.ColumnHeadersDefaultCellStyle.SelectionForeColor = theme.ListHeaderForeColor;

            grid.RowHeadersDefaultCellStyle.BackColor = theme.ListHeaderBackColor;
            grid.RowHeadersDefaultCellStyle.ForeColor = theme.ListHeaderForeColor;

            ApplyNativeControlTheme(grid);
        }

        private static void ApplyCheckBoxTheme(CheckBox checkBox, ThemePalette theme)
        {
            checkBox.Font = theme.BaseFont;
            checkBox.ForeColor = theme.ForeColor;
            checkBox.UseVisualStyleBackColor = false;
            checkBox.FlatStyle = FlatStyle.Flat;
            checkBox.BackColor = theme.PanelBackColor;
            checkBox.FlatAppearance.BorderSize = 0;
            checkBox.FlatAppearance.BorderColor = theme.PanelBackColor;
            checkBox.FlatAppearance.MouseDownBackColor = theme.PanelBackColor;
            checkBox.FlatAppearance.MouseOverBackColor = theme.PanelBackColor;
            checkBox.FlatAppearance.CheckedBackColor = theme.CheckBoxCheckedBackColor;

            if (!string.IsNullOrEmpty(checkBox.Text) && checkBox.Dock != DockStyle.Fill)
            {
                Size preferredSize = MeasureLabeledCheckBoxSize(checkBox);
                checkBox.AutoSize = false;
                checkBox.Size = preferredSize;
            }

            EnableUserPaint(checkBox);
            checkBox.Paint -= PaintThemedCheckBox;
            checkBox.Paint += PaintThemedCheckBox;
            checkBox.Invalidate();
        }

        private static Size MeasureLabeledCheckBoxSize(CheckBox checkBox)
        {
            int glyphSize = CheckBoxGlyphSize;
            int height = Math.Max(20, glyphSize + 4);
            Size textSize = TextRenderer.MeasureText(
                checkBox.Text,
                checkBox.Font,
                new Size(int.MaxValue, height),
                TextFormatFlags.SingleLine | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);

            int width = glyphSize + CheckBoxTextGap + textSize.Width + 2;
            if (checkBox.MinimumSize.Width > 0)
            {
                width = Math.Max(width, checkBox.MinimumSize.Width);
            }

            if (checkBox.MinimumSize.Height > 0)
            {
                height = Math.Max(height, checkBox.MinimumSize.Height);
            }

            return new Size(width, height);
        }

        private static void EnableUserPaint(Control control)
        {
            ControlStyles styles = ControlStyles.UserPaint |
                                   ControlStyles.AllPaintingInWmPaint |
                                   ControlStyles.OptimizedDoubleBuffer |
                                   ControlStyles.ResizeRedraw |
                                   ControlStyles.Opaque;

            typeof(Control).InvokeMember(
                "SetStyle",
                BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.InvokeMethod,
                null,
                control,
                new object[] { styles, true });
        }

        private static void PaintThemedCheckBox(object sender, PaintEventArgs e)
        {
            CheckBox checkBox = (CheckBox)sender;
            ThemePalette theme = Current;
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.PixelOffsetMode = PixelOffsetMode.HighQuality;

            bool glyphOnly = string.IsNullOrEmpty(checkBox.Text);
            Color rowBack = checkBox.BackColor;
            Rectangle client = checkBox.ClientRectangle;

            using (SolidBrush rowBrush = new SolidBrush(rowBack))
            {
                g.FillRectangle(rowBrush, client);
            }

            if (glyphOnly && checkBox.Checked)
            {
                using (SolidBrush boxBrush = new SolidBrush(theme.CheckBoxCheckedBackColor))
                {
                    g.FillRectangle(boxBrush, client);
                }

                using (Pen borderPen = new Pen(theme.PrimaryButtonBorderColor, 1f))
                {
                    g.DrawRectangle(borderPen, 0, 0, client.Width - 1, client.Height - 1);
                }

                Rectangle markBox = GetCheckBoxGlyphBounds(checkBox);
                Color markColor = checkBox.Enabled ? theme.CheckBoxCheckMarkColor : theme.DisabledForeColor;
                DrawCheckMark(g, markBox, markColor);
                return;
            }

            if (glyphOnly)
            {
                Color uncheckedBorder = checkBox.Enabled ? theme.CheckBoxBorderColor : theme.DisabledForeColor;
                using (SolidBrush boxBrush = new SolidBrush(theme.CheckBoxBackColor))
                {
                    g.FillRectangle(boxBrush, client);
                }

                using (Pen borderPen = new Pen(uncheckedBorder, 1f))
                {
                    g.DrawRectangle(borderPen, 0, 0, client.Width - 1, client.Height - 1);
                }

                return;
            }

            Rectangle box = GetCheckBoxGlyphBounds(checkBox);

            Rectangle glyphArea = new Rectangle(0, box.Y, box.Right + 1, box.Height);
            using (SolidBrush clearBrush = new SolidBrush(rowBack))
            {
                g.FillRectangle(clearBrush, glyphArea);
            }

            Color boxBack = checkBox.Checked ? theme.CheckBoxCheckedBackColor : theme.CheckBoxBackColor;
            Color boxBorder = checkBox.Enabled ? theme.CheckBoxBorderColor : theme.DisabledForeColor;
            if (!checkBox.Enabled)
            {
                boxBack = Color.FromArgb(
                    (boxBack.R + rowBack.R) / 2,
                    (boxBack.G + rowBack.G) / 2,
                    (boxBack.B + rowBack.B) / 2);
            }

            using (SolidBrush boxBrush = new SolidBrush(boxBack))
            {
                g.FillRectangle(boxBrush, box);
            }

            if (!checkBox.Checked)
            {
                using (Pen borderPen = new Pen(boxBorder, 1f))
                {
                    g.DrawRectangle(borderPen, box.X, box.Y, box.Width - 1, box.Height - 1);
                }
            }
            else
            {
                using (Pen borderPen = new Pen(theme.PrimaryButtonBorderColor, 1f))
                {
                    g.DrawRectangle(borderPen, box.X + 1, box.Y + 1, box.Width - 3, box.Height - 3);
                }
            }

            if (checkBox.Checked)
            {
                Color markColor = checkBox.Enabled ? theme.CheckBoxCheckMarkColor : theme.DisabledForeColor;
                DrawCheckMark(g, box, markColor);
            }

            if (!glyphOnly)
            {
                Color textColor = checkBox.Enabled ? checkBox.ForeColor : theme.DisabledForeColor;
                Rectangle textBounds = new Rectangle(
                    box.Right + CheckBoxTextGap,
                    0,
                    Math.Max(0, checkBox.ClientSize.Width - box.Right - CheckBoxTextGap),
                    checkBox.ClientSize.Height);

                TextFormatFlags textFlags = TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.NoPrefix;
                if (checkBox.Dock == DockStyle.Fill)
                {
                    textFlags |= TextFormatFlags.EndEllipsis;
                }

                TextRenderer.DrawText(
                    g,
                    checkBox.Text,
                    checkBox.Font,
                    textBounds,
                    textColor,
                    textFlags);
            }
        }

        private static int GetCheckBoxGlyphSize(CheckBox checkBox)
        {
            int availableHeight = Math.Max(12, checkBox.ClientSize.Height - 2);
            if (!string.IsNullOrEmpty(checkBox.Text))
            {
                return Math.Max(14, Math.Min(CheckBoxGlyphSize, availableHeight));
            }

            int availableWidth = checkBox.ClientSize.Width - 2;
            int size = Math.Min(CheckBoxGlyphSize, Math.Min(availableWidth, availableHeight));
            return Math.Max(14, size);
        }

        private static Rectangle GetCheckBoxGlyphBounds(CheckBox checkBox)
        {
            int size = GetCheckBoxGlyphSize(checkBox);
            int x = string.IsNullOrEmpty(checkBox.Text)
                ? Math.Max(0, (checkBox.ClientSize.Width - size) / 2)
                : 0;
            int y = Math.Max(0, (checkBox.ClientSize.Height - size) / 2);
            return new Rectangle(x, y, size, size);
        }

        private static void DrawCheckMark(Graphics graphics, Rectangle box, Color color)
        {
            float penWidth = Math.Max(1.8f, Math.Min(2.4f, box.Width * 0.14f));
            using (Pen pen = new Pen(color, penWidth))
            {
                pen.StartCap = LineCap.Round;
                pen.EndCap = LineCap.Round;
                pen.LineJoin = LineJoin.Round;

                float bottomInset = box.Height * 0.24f;
                float bottomY = box.Bottom - bottomInset;
                float left = box.Left + box.Width * 0.22f;
                float midX = box.Left + box.Width * 0.42f;
                float midY = box.Top + box.Height * 0.58f;
                float right = box.Right - box.Width * 0.18f;
                float top = box.Top + box.Height * 0.26f;

                graphics.DrawLine(pen, left, midY, midX, bottomY);
                graphics.DrawLine(pen, midX, bottomY, right, top);
            }
        }

        private static void ApplyRadioButtonTheme(RadioButton radioButton, ThemePalette theme)
        {
            radioButton.Font = theme.BaseFont;
            radioButton.ForeColor = theme.ForeColor;
            radioButton.UseVisualStyleBackColor = false;
            radioButton.FlatStyle = FlatStyle.Flat;
            radioButton.BackColor = theme.PanelBackColor;
            radioButton.FlatAppearance.BorderSize = 0;
            radioButton.FlatAppearance.BorderColor = theme.PanelBackColor;
            radioButton.FlatAppearance.MouseDownBackColor = theme.PanelBackColor;
            radioButton.FlatAppearance.MouseOverBackColor = theme.PanelBackColor;
            radioButton.FlatAppearance.CheckedBackColor = theme.CheckBoxCheckedBackColor;

            if (!string.IsNullOrEmpty(radioButton.Text))
            {
                Size preferredSize = MeasureLabeledRadioButtonSize(radioButton);
                radioButton.AutoSize = false;
                radioButton.Size = preferredSize;
            }

            EnableUserPaint(radioButton);
            radioButton.Paint -= PaintThemedRadioButton;
            radioButton.Paint += PaintThemedRadioButton;
            radioButton.Invalidate();
        }

        private static Size MeasureLabeledRadioButtonSize(RadioButton radioButton)
        {
            int glyphSize = CheckBoxGlyphSize;
            int height = Math.Max(22, glyphSize + 4);
            Size textSize = TextRenderer.MeasureText(
                radioButton.Text,
                radioButton.Font,
                new Size(int.MaxValue, height),
                TextFormatFlags.SingleLine | TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);

            int width = glyphSize + CheckBoxTextGap + textSize.Width + 2;
            if (radioButton.MinimumSize.Width > 0)
            {
                width = Math.Max(width, radioButton.MinimumSize.Width);
            }

            if (radioButton.MinimumSize.Height > 0)
            {
                height = Math.Max(height, radioButton.MinimumSize.Height);
            }

            return new Size(width, height);
        }

        private static void PaintThemedRadioButton(object sender, PaintEventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            ThemePalette theme = Current;
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.PixelOffsetMode = PixelOffsetMode.HighQuality;

            Color rowBack = radioButton.BackColor;
            Rectangle client = radioButton.ClientRectangle;

            using (SolidBrush rowBrush = new SolidBrush(rowBack))
            {
                g.FillRectangle(rowBrush, client);
            }

            Rectangle circle = GetRadioButtonGlyphBounds(radioButton);
            Color circleBorder = radioButton.Enabled ? theme.CheckBoxBorderColor : theme.DisabledForeColor;

            using (SolidBrush circleBrush = new SolidBrush(rowBack))
            {
                g.FillEllipse(circleBrush, circle);
            }

            if (radioButton.Checked)
            {
                Color checkedColor = radioButton.Enabled ? theme.CheckBoxCheckedBackColor : theme.DisabledForeColor;
                using (Pen borderPen = new Pen(checkedColor, 1.6f))
                {
                    g.DrawEllipse(borderPen, circle.X + 1, circle.Y + 1, circle.Width - 3, circle.Height - 3);
                }

                int dotSize = Math.Max(8, circle.Width - 10);
                int dotX = circle.X + (circle.Width - dotSize) / 2;
                int dotY = circle.Y + (circle.Height - dotSize) / 2;
                using (SolidBrush dotBrush = new SolidBrush(checkedColor))
                {
                    g.FillEllipse(dotBrush, dotX, dotY, dotSize, dotSize);
                }
            }
            else
            {
                using (Pen borderPen = new Pen(circleBorder, 1.6f))
                {
                    g.DrawEllipse(borderPen, circle.X + 1, circle.Y + 1, circle.Width - 3, circle.Height - 3);
                }
            }

            if (!string.IsNullOrEmpty(radioButton.Text))
            {
                Color textColor = radioButton.Enabled ? radioButton.ForeColor : theme.DisabledForeColor;
                Rectangle textBounds = new Rectangle(
                    circle.Right + CheckBoxTextGap,
                    0,
                    Math.Max(0, radioButton.ClientSize.Width - circle.Right - CheckBoxTextGap),
                    radioButton.ClientSize.Height);

                TextRenderer.DrawText(
                    g,
                    radioButton.Text,
                    radioButton.Font,
                    textBounds,
                    textColor,
                    TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.NoPrefix);
            }
        }

        private static Rectangle GetRadioButtonGlyphBounds(RadioButton radioButton)
        {
            int size = Math.Max(14, Math.Min(CheckBoxGlyphSize, Math.Max(12, radioButton.ClientSize.Height - 4)));
            int x = 0;
            int y = Math.Max(0, (radioButton.ClientSize.Height - size) / 2);
            return new Rectangle(x, y, size, size);
        }

        private static void ApplyToolStripItems(ToolStripItemCollection items, ThemePalette theme)
        {
            foreach (ToolStripItem item in items)
            {
                item.ForeColor = item.Enabled ? theme.MenuForeColor : theme.DisabledForeColor;
                if (item is ToolStripDropDownItem dropDown)
                {
                    ApplyToolStripItems(dropDown.DropDownItems, theme);
                }
            }
        }

        private static void ApplyToolStripTheme(ToolStrip strip, ThemePalette theme)
        {
            strip.BackColor = theme.MenuBackColor;
            strip.ForeColor = theme.MenuForeColor;
            strip.Font = theme.BaseFont;
            strip.RenderMode = ToolStripRenderMode.Professional;
            strip.Renderer = new ThemedToolStripRenderer(theme);
            ApplyToolStripItems(strip.Items, theme);
        }

        public static void ApplyContextMenuStrip(ContextMenuStrip menu)
        {
            ApplyToolStripTheme(menu, Current);
        }

        public static void SyncToolStripItemColors(ToolStripItem item)
        {
            if (item == null)
            {
                return;
            }

            ThemePalette theme = Current;
            item.ForeColor = item.Enabled ? theme.MenuForeColor : theme.DisabledForeColor;
            item.Invalidate();
        }

        private sealed class ThemedToolStripRenderer : ToolStripProfessionalRenderer
        {
            private readonly ThemePalette _theme;

            public ThemedToolStripRenderer(ThemePalette theme)
                : base(new ThemedMenuColorTable(theme))
            {
                _theme = theme;
            }

            protected override void OnRenderMenuItemBackground(ToolStripItemRenderEventArgs e)
            {
                ToolStripItem item = e.Item;
                Rectangle bounds = new Rectangle(Point.Empty, item.Size);
                bool highlight = item.Selected || item.Pressed;
                Color back = highlight ? _theme.MenuHoverBackColor : _theme.MenuBackColor;

                using (SolidBrush brush = new SolidBrush(back))
                {
                    e.Graphics.FillRectangle(brush, bounds);
                }
            }

            protected override void OnRenderItemText(ToolStripItemTextRenderEventArgs e)
            {
                e.TextColor = e.Item.Enabled ? _theme.MenuForeColor : _theme.DisabledForeColor;
                base.OnRenderItemText(e);
            }

            protected override void OnRenderToolStripBackground(ToolStripRenderEventArgs e)
            {
                using (SolidBrush brush = new SolidBrush(_theme.MenuBackColor))
                {
                    e.Graphics.FillRectangle(brush, e.AffectedBounds);
                }
            }

            protected override void OnRenderToolStripBorder(ToolStripRenderEventArgs e)
            {
                if (e.ToolStrip is ToolStripDropDown)
                {
                    using (Pen pen = new Pen(_theme.BorderColor))
                    {
                        Rectangle bounds = e.AffectedBounds;
                        bounds.Width -= 1;
                        bounds.Height -= 1;
                        e.Graphics.DrawRectangle(pen, bounds);
                    }
                }
            }
        }

        private sealed class ThemedMenuColorTable : ProfessionalColorTable
        {
            private readonly ThemePalette _theme;

            public ThemedMenuColorTable(ThemePalette theme)
            {
                _theme = theme;
            }

            public override Color MenuBorder => _theme.BorderColor;

            public override Color ToolStripDropDownBackground => _theme.MenuBackColor;

            public override Color ImageMarginGradientBegin => _theme.MenuBackColor;

            public override Color ImageMarginGradientMiddle => _theme.MenuBackColor;

            public override Color ImageMarginGradientEnd => _theme.MenuBackColor;

            public override Color MenuItemBorder => _theme.MenuHoverBackColor;

            public override Color MenuItemSelected => _theme.MenuHoverBackColor;

            public override Color MenuItemSelectedGradientBegin => _theme.MenuHoverBackColor;

            public override Color MenuItemSelectedGradientEnd => _theme.MenuHoverBackColor;

            public override Color MenuItemPressedGradientBegin => _theme.MenuHoverBackColor;

            public override Color MenuItemPressedGradientEnd => _theme.MenuHoverBackColor;

            public override Color MenuStripGradientBegin => _theme.MenuBackColor;

            public override Color MenuStripGradientEnd => _theme.MenuBackColor;
        }

        private static void ApplyLabelTheme(Label label, ThemePalette theme, bool isHint)
        {
            string tag = label.Tag as string;
            if (tag == "CustomForeColor")
            {
                return;
            }

            if (tag == "Warning")
            {
                label.ForeColor = theme.WarningForeColor;
                return;
            }

            label.ForeColor = isHint ? theme.HintForeColor : theme.ForeColor;
        }

        private static void ApplyButtonTheme(Button button, ThemePalette theme)
        {
            if (button.Name == "Randomize_Button")
            {
                StylePrimaryButton(button);
                return;
            }

            if (button.Tag as string == "ColorSwatch" || button.Tag as string == "BingoGoalCell" || button.Tag is int || button is BingoGoalButton)
            {
                return;
            }

            button.MinimumSize = new Size(button.MinimumSize.Width, theme.ButtonMinHeight);
            button.Margin = theme.ControlMargin;
            button.BackColor = theme.ButtonBackColor;
            button.ForeColor = theme.ButtonForeColor;
            button.FlatStyle = FlatStyle.Flat;
            button.FlatAppearance.BorderColor = theme.BorderColor;
            button.FlatAppearance.BorderSize = 1;
            button.UseVisualStyleBackColor = false;
        }

        public static void StylePrimaryButton(Button btn)
        {
            ThemePalette theme = Current;
            btn.Font = new Font(theme.BaseFont.FontFamily, theme.BaseFont.Size, FontStyle.Bold);
            btn.BackColor = theme.PrimaryButtonBackColor;
            btn.ForeColor = theme.PrimaryButtonForeColor;
            btn.FlatStyle = FlatStyle.Flat;
            btn.FlatAppearance.BorderSize = _isDark ? 1 : 0;
            btn.FlatAppearance.BorderColor = theme.PrimaryButtonBorderColor;
            btn.MinimumSize = new Size(0, theme.ButtonMinHeight + 6);
            btn.Cursor = Cursors.Hand;
            btn.UseVisualStyleBackColor = false;
        }

        // Hex values aligned with files/Log.css for spoiler log HTML exports.
        public static string BuildExportCss()
        {
            ThemePalette theme = Current;
            StringBuilder css = new StringBuilder();
            css.AppendLine("body{font-family:Segoe UI,Arial,sans-serif;margin:24px;}");
            css.AppendLine("table{border-collapse:collapse;margin:16px 0;}");
            css.AppendLine("td,th{border:1px solid " + ToHex(theme.BorderColor) + ";padding:10px;vertical-align:middle;text-align:center;width:140px;height:80px;}");
            css.AppendLine("th{background:" + ToHex(theme.ListEvenRowColor) + ";}");
            css.AppendLine(".line-header{cursor:pointer;text-decoration:underline;}");
            css.AppendLine(".goal-cell{cursor:pointer;user-select:none;}");
            css.AppendLine(".meta{margin-bottom:12px;line-height:1.5;}");
            css.AppendLine(".mark-green{background:" + ToHex(theme.MarkGreen) + ";}");
            css.AppendLine(".mark-red{background:" + ToHex(theme.MarkRed) + ";}");
            css.AppendLine(".mark-orange{background:" + ToHex(theme.MarkOrange) + ";}");
            css.AppendLine(".mark-blue{background:" + ToHex(theme.MarkBlue) + ";}");
            css.AppendLine(".mark-purple{background:" + ToHex(theme.MarkPurple) + ";}");
            css.AppendLine("#popout-overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.35);z-index:1000;align-items:center;justify-content:center;}");
            css.AppendLine("#popout-overlay.open{display:flex;}");
            css.AppendLine("#popout-panel{background:" + ToHex(theme.PanelBackColor) + ";border:1px solid " + ToHex(theme.BorderColor) + ";padding:12px;max-width:95vw;max-height:90vh;overflow:auto;}");
            css.AppendLine("#popout-title{font-weight:bold;margin-bottom:8px;}");
            css.AppendLine("#popout-grid{display:flex;gap:8px;}");
            css.AppendLine("#popout-grid.vertical{flex-direction:column;}");
            css.AppendLine("#popout-grid.horizontal{flex-direction:row;flex-wrap:wrap;}");
            css.AppendLine(".popout-cell{border:1px solid " + ToHex(theme.BorderColor) + ";padding:12px;min-width:120px;min-height:72px;cursor:pointer;user-select:none;text-align:center;}");

            if (_isDark)
            {
                css.AppendLine("body{background:#121212;color:#959595;}");
                css.AppendLine("h1,h2,p,em{color:#959595;}");
            }
            else
            {
                css.AppendLine("body{background:#fff;color:#222;}");
            }

            css.AppendLine("html{color-scheme:" + (_isDark ? "dark" : "light") + ";}");
            return css.ToString();
        }

        /// <summary>
        /// Standalone bingo card HTML styling (Jimmie1717-style layout/contrast).
        /// </summary>
        public static string BuildBingoExportCss()
        {
            if (_isDark)
            {
                return BuildBingoExportCssDark();
            }

            return BuildBingoExportCssLight();
        }

        private static string BuildBingoExportCssDark()
        {
            StringBuilder css = new StringBuilder();
            css.AppendLine("*{box-sizing:border-box;}");
            css.AppendLine("body{font-family:Segoe UI,Helvetica,Arial,sans-serif;margin:24px;background:#101010;color:#b8b8b8;line-height:1.45;}");
            css.AppendLine("h1{font-size:1.6rem;font-weight:600;color:#ffffff;margin:0 0 12px;}");
            css.AppendLine("h2{font-size:1.15rem;font-weight:600;color:#e8e8e8;margin:24px 0 8px;}");
            css.AppendLine("p{color:#b8b8b8;margin:8px 0;}");
            css.AppendLine("em{color:#909090;font-style:italic;}");
            css.AppendLine(".meta{margin-bottom:16px;line-height:1.6;font-size:14px;color:#a8a8a8;}");
            css.AppendLine("#status{font-weight:600;color:#ffffff;margin-top:8px;}");
            css.AppendLine(".reroll-log{white-space:pre-wrap;font-family:Consolas,Monaco,monospace;font-size:12px;background:#141414;color:#d8d8d8;border:1px solid #333333;padding:12px;border-radius:4px;}");
            css.AppendLine("#bingo-grid{border-collapse:collapse;margin:16px 0;table-layout:fixed;}");
            css.AppendLine("#bingo-grid td,#bingo-grid th{border:1px solid #333333;padding:8px 6px;vertical-align:middle;text-align:center;}");
            css.AppendLine("#bingo-grid th.line-header{width:68px;min-width:68px;max-width:72px;height:auto;min-height:32px;padding:6px 4px;font-size:11px;line-height:1.2;background:#101010;color:#c8c8c8;font-weight:600;}");
            css.AppendLine("#bingo-grid th.grid-pad{width:8px;min-width:8px;max-width:8px;padding:0;border:none;background:transparent;}");
            css.AppendLine("#bingo-grid td.goal-cell{width:148px;min-width:130px;height:84px;font-size:13px;line-height:1.35;cursor:pointer;user-select:none;background:#000000;color:#ffffff;}");
            css.AppendLine(".line-header{cursor:pointer;text-decoration:underline;color:#c8c8c8;}");
            css.AppendLine("#bingo-grid td.goal-cell:hover{background:#0a0a0a;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-green,.popout-cell.mark-green{background:" + BingoMarkGreenHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-red,.popout-cell.mark-red{background:" + BingoMarkRedHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-orange,.popout-cell.mark-orange{background:" + BingoMarkOrangeHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-blue,.popout-cell.mark-blue{background:" + BingoMarkBlueHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-purple,.popout-cell.mark-purple{background:" + BingoMarkPurpleHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-green:hover,.popout-cell.mark-green:hover{background:" + BingoMarkGreenHex + ";}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-red:hover,.popout-cell.mark-red:hover{background:" + BingoMarkRedHex + ";}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-orange:hover,.popout-cell.mark-orange:hover{background:" + BingoMarkOrangeHex + ";}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-blue:hover,.popout-cell.mark-blue:hover{background:" + BingoMarkBlueHex + ";}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-purple:hover,.popout-cell.mark-purple:hover{background:" + BingoMarkPurpleHex + ";}");
            css.AppendLine("#popout-overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.55);z-index:1000;align-items:center;justify-content:center;}");
            css.AppendLine("#popout-overlay.open{display:flex;}");
            css.AppendLine("#popout-panel{background:#141414;border:1px solid #333333;padding:12px;max-width:95vw;max-height:90vh;overflow:auto;color:#ffffff;}");
            css.AppendLine("#popout-title{font-weight:600;margin-bottom:8px;color:#ffffff;}");
            css.AppendLine("#popout-grid{display:flex;gap:8px;}");
            css.AppendLine("#popout-grid.vertical{flex-direction:column;}");
            css.AppendLine("#popout-grid.horizontal{flex-direction:row;flex-wrap:wrap;}");
            css.AppendLine(".popout-cell{border:1px solid #333333;background:#000000;color:#ffffff;padding:12px;min-width:120px;min-height:72px;cursor:pointer;user-select:none;text-align:center;font-size:13px;line-height:1.35;}");
            css.AppendLine("#mark-menu{display:none;position:fixed;z-index:1100;background:#141414;border:1px solid #333333;border-radius:4px;padding:4px;box-shadow:0 6px 18px rgba(0,0,0,.65);min-width:132px;}");
            css.AppendLine("#mark-menu.open{display:flex;flex-direction:column;gap:2px;}");
            css.AppendLine(".mark-menu-item{display:flex;align-items:center;gap:8px;border:none;background:transparent;color:#ffffff;padding:7px 10px;cursor:pointer;text-align:left;font-size:13px;border-radius:3px;width:100%;}");
            css.AppendLine(".mark-menu-item:hover{background:#1f1f1f;}");
            css.AppendLine(".mark-menu-swatch{width:14px;height:14px;border-radius:2px;flex-shrink:0;border:1px solid rgba(255,255,255,.15);}");
            css.AppendLine(".mark-menu-item.mark-menu-clear .mark-menu-swatch{background:#000000;}");
            css.AppendLine(".mark-menu-item.mark-green .mark-menu-swatch{background:" + BingoMarkGreenHex + ";}");
            css.AppendLine(".mark-menu-item.mark-red .mark-menu-swatch{background:" + BingoMarkRedHex + ";}");
            css.AppendLine(".mark-menu-item.mark-orange .mark-menu-swatch{background:" + BingoMarkOrangeHex + ";}");
            css.AppendLine(".mark-menu-item.mark-blue .mark-menu-swatch{background:" + BingoMarkBlueHex + ";}");
            css.AppendLine(".mark-menu-item.mark-purple .mark-menu-swatch{background:" + BingoMarkPurpleHex + ";}");
            css.AppendLine(".mark-menu-label{flex:1;}");
            css.AppendLine("html{color-scheme:dark;}");
            return css.ToString();
        }

        private static string BuildBingoExportCssLight()
        {
            StringBuilder css = new StringBuilder();
            css.AppendLine("*{box-sizing:border-box;}");
            css.AppendLine("body{font-family:Segoe UI,Helvetica,Arial,sans-serif;margin:24px;background:#f4f4f4;color:#222;line-height:1.45;}");
            css.AppendLine("h1{font-size:1.6rem;font-weight:600;color:#111;margin:0 0 12px;}");
            css.AppendLine("h2{font-size:1.15rem;font-weight:600;color:#222;margin:24px 0 8px;}");
            css.AppendLine("p{color:#333;margin:8px 0;}");
            css.AppendLine("em{color:#666;font-style:italic;}");
            css.AppendLine(".meta{margin-bottom:16px;line-height:1.6;font-size:14px;color:#444;}");
            css.AppendLine("#status{font-weight:600;color:#111;margin-top:8px;}");
            css.AppendLine(".reroll-log{white-space:pre-wrap;font-family:Consolas,Monaco,monospace;font-size:12px;background:#fff;color:#222;border:1px solid #ccc;padding:12px;border-radius:4px;}");
            css.AppendLine("#bingo-grid{border-collapse:collapse;margin:16px 0;table-layout:fixed;}");
            css.AppendLine("#bingo-grid td,#bingo-grid th{border:1px solid #888;padding:8px 6px;vertical-align:middle;text-align:center;}");
            css.AppendLine("#bingo-grid th.line-header{width:68px;min-width:68px;max-width:72px;height:auto;min-height:32px;padding:6px 4px;font-size:11px;line-height:1.2;background:#e8e8e8;color:#111;font-weight:600;}");
            css.AppendLine("#bingo-grid th.grid-pad{width:8px;min-width:8px;max-width:8px;padding:0;border:none;background:transparent;}");
            css.AppendLine("#bingo-grid td.goal-cell{width:148px;min-width:130px;height:84px;font-size:13px;line-height:1.35;cursor:pointer;user-select:none;background:#fff;color:#111;}");
            css.AppendLine(".line-header{cursor:pointer;text-decoration:underline;color:#005a9e;}");
            css.AppendLine("#bingo-grid td.goal-cell:hover{background:#f9f9f9;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-green,.popout-cell.mark-green{background:" + BingoMarkGreenHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-red,.popout-cell.mark-red{background:" + BingoMarkRedHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-orange,.popout-cell.mark-orange{background:" + BingoMarkOrangeHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-blue,.popout-cell.mark-blue{background:" + BingoMarkBlueHex + ";color:#ffffff;}");
            css.AppendLine("#bingo-grid td.goal-cell.mark-purple,.popout-cell.mark-purple{background:" + BingoMarkPurpleHex + ";color:#ffffff;}");
            css.AppendLine("#popout-overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.35);z-index:1000;align-items:center;justify-content:center;}");
            css.AppendLine("#popout-overlay.open{display:flex;}");
            css.AppendLine("#popout-panel{background:#fff;border:1px solid #888;padding:12px;max-width:95vw;max-height:90vh;overflow:auto;color:#222;}");
            css.AppendLine("#popout-title{font-weight:600;margin-bottom:8px;}");
            css.AppendLine("#popout-grid{display:flex;gap:8px;}");
            css.AppendLine("#popout-grid.vertical{flex-direction:column;}");
            css.AppendLine("#popout-grid.horizontal{flex-direction:row;flex-wrap:wrap;}");
            css.AppendLine(".popout-cell{border:1px solid #888;background:#fff;color:#111;padding:12px;min-width:120px;min-height:72px;cursor:pointer;user-select:none;text-align:center;}");
            css.AppendLine("#mark-menu{display:none;position:fixed;z-index:1100;background:#fff;border:1px solid #888;border-radius:4px;padding:4px;box-shadow:0 6px 18px rgba(0,0,0,.2);min-width:132px;}");
            css.AppendLine("#mark-menu.open{display:flex;flex-direction:column;gap:2px;}");
            css.AppendLine(".mark-menu-item{display:flex;align-items:center;gap:8px;border:none;background:transparent;color:#222;padding:7px 10px;cursor:pointer;text-align:left;font-size:13px;border-radius:3px;width:100%;}");
            css.AppendLine(".mark-menu-item:hover{background:#f0f0f0;}");
            css.AppendLine(".mark-menu-swatch{width:14px;height:14px;border-radius:2px;flex-shrink:0;border:1px solid rgba(0,0,0,.2);}");
            css.AppendLine(".mark-menu-item.mark-menu-clear .mark-menu-swatch{background:#fff;}");
            css.AppendLine(".mark-menu-item.mark-green .mark-menu-swatch{background:" + BingoMarkGreenHex + ";}");
            css.AppendLine(".mark-menu-item.mark-red .mark-menu-swatch{background:" + BingoMarkRedHex + ";}");
            css.AppendLine(".mark-menu-item.mark-orange .mark-menu-swatch{background:" + BingoMarkOrangeHex + ";}");
            css.AppendLine(".mark-menu-item.mark-blue .mark-menu-swatch{background:" + BingoMarkBlueHex + ";}");
            css.AppendLine(".mark-menu-item.mark-purple .mark-menu-swatch{background:" + BingoMarkPurpleHex + ";}");
            css.AppendLine(".mark-menu-label{flex:1;}");
            css.AppendLine("html{color-scheme:light;}");
            return css.ToString();
        }

        private static string ToHex(Color color)
        {
            return "#" + color.R.ToString("X2") + color.G.ToString("X2") + color.B.ToString("X2");
        }

        private static ThemePalette CreateLightTheme()
        {
            return new ThemePalette
            {
                BaseFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
                HintFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
                FormBackColor = Color.FromArgb(245, 245, 247),
                PanelBackColor = Color.FromArgb(250, 250, 252),
                PrimaryButtonBackColor = Color.FromArgb(0, 103, 192),
                PrimaryButtonForeColor = Color.White,
                PrimaryButtonBorderColor = Color.FromArgb(0, 103, 192),
                ForeColor = Color.FromArgb(32, 32, 34),
                HintForeColor = Color.FromArgb(92, 92, 96),
                DisabledForeColor = Color.FromArgb(160, 160, 165),
                ControlBackColor = SystemColors.Window,
                ControlForeColor = SystemColors.WindowText,
                BorderColor = Color.FromArgb(200, 200, 200),
                MenuBackColor = Color.FromArgb(245, 245, 247),
                MenuForeColor = SystemColors.ControlText,
                MenuHoverBackColor = Color.FromArgb(220, 220, 228),
                TabSelectedBackColor = Color.White,
                TabForeColor = SystemColors.ControlText,
                TabIconColor = Color.FromArgb(55, 55, 60),
                TabSelectedAccentColor = Color.FromArgb(98, 0, 234),
                ListEvenRowColor = Color.White,
                ListOddRowColor = Color.FromArgb(242, 245, 250),
                ListSelectedBackColor = SystemColors.Highlight,
                ListSelectedForeColor = SystemColors.HighlightText,
                ListHeaderBackColor = Color.FromArgb(240, 240, 240),
                ListHeaderForeColor = SystemColors.ControlText,
                TabStripBackColor = Color.FromArgb(245, 245, 247),
                TabUnselectedBackColor = Color.FromArgb(235, 235, 238),
                PlandoRowBackColor = Color.FromArgb(245, 248, 252),
                WarningForeColor = Color.DarkOrange,
                ErrorForeColor = Color.FromArgb(180, 40, 40),
                CellDefaultBackColor = SystemColors.Control,
                ButtonBackColor = Color.FromArgb(240, 240, 240),
                ButtonForeColor = SystemColors.ControlText,
                CheckBoxBackColor = Color.White,
                CheckBoxBorderColor = Color.FromArgb(130, 130, 130),
                CheckBoxCheckedBackColor = Color.FromArgb(98, 0, 234),
                CheckBoxCheckMarkColor = Color.White,
                MarkGreen = BingoMarkGreenColor,
                MarkRed = BingoMarkRedColor,
                MarkOrange = BingoMarkOrangeColor,
                MarkBlue = BingoMarkBlueColor,
                MarkPurple = BingoMarkPurpleColor,
                ButtonMinHeight = 30,
                ControlMargin = new Padding(4),
                GroupPadding = new Padding(8, 4, 8, 8)
            };
        }

        private static ThemePalette CreateDarkTheme()
        {
            return new ThemePalette
            {
                BaseFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
                HintFont = new Font("Segoe UI", 10F, FontStyle.Regular, GraphicsUnit.Point),
                FormBackColor = Color.FromArgb(32, 32, 32),
                PanelBackColor = Color.FromArgb(45, 45, 48),
                PrimaryButtonBackColor = Color.FromArgb(72, 48, 108),
                PrimaryButtonForeColor = Color.FromArgb(235, 230, 245),
                PrimaryButtonBorderColor = Color.FromArgb(100, 70, 140),
                ForeColor = Color.FromArgb(220, 220, 220),
                HintForeColor = Color.FromArgb(150, 150, 150),
                DisabledForeColor = Color.FromArgb(120, 120, 120),
                ControlBackColor = Color.FromArgb(37, 37, 38),
                ControlForeColor = Color.FromArgb(220, 220, 220),
                BorderColor = Color.FromArgb(35, 35, 35),
                MenuBackColor = Color.FromArgb(37, 37, 38),
                MenuForeColor = Color.FromArgb(220, 220, 220),
                MenuHoverBackColor = Color.FromArgb(62, 62, 66),
                TabSelectedBackColor = Color.FromArgb(37, 37, 38),
                TabForeColor = Color.FromArgb(200, 200, 200),
                TabIconColor = Color.FromArgb(180, 180, 185),
                TabSelectedAccentColor = Color.FromArgb(140, 100, 255),
                ListEvenRowColor = Color.FromArgb(21, 21, 21),
                ListOddRowColor = Color.FromArgb(25, 25, 25),
                ListSelectedBackColor = Color.FromArgb(0, 122, 204),
                ListSelectedForeColor = Color.White,
                ListHeaderBackColor = Color.FromArgb(45, 45, 48),
                ListHeaderForeColor = Color.FromArgb(200, 200, 200),
                TabStripBackColor = Color.FromArgb(45, 45, 48),
                TabUnselectedBackColor = Color.FromArgb(55, 55, 60),
                PlandoRowBackColor = Color.FromArgb(40, 44, 52),
                WarningForeColor = Color.FromArgb(255, 160, 80),
                ErrorForeColor = Color.FromArgb(255, 120, 120),
                CellDefaultBackColor = Color.FromArgb(55, 55, 58),
                ButtonBackColor = Color.FromArgb(62, 62, 66),
                ButtonForeColor = Color.FromArgb(220, 220, 220),
                CheckBoxBackColor = Color.FromArgb(50, 50, 54),
                CheckBoxBorderColor = Color.FromArgb(130, 130, 138),
                CheckBoxCheckedBackColor = Color.FromArgb(88, 58, 128),
                CheckBoxCheckMarkColor = Color.White,
                MarkGreen = BingoMarkGreenColor,
                MarkRed = BingoMarkRedColor,
                MarkOrange = BingoMarkOrangeColor,
                MarkBlue = BingoMarkBlueColor,
                MarkPurple = BingoMarkPurpleColor,
                ButtonMinHeight = 30,
                ControlMargin = new Padding(4),
                GroupPadding = new Padding(8, 4, 8, 8)
            };
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
