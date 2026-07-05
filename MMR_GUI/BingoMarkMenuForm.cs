using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class BingoMarkMenuForm : Form
    {
        private readonly Action<int> _setState;

        public BingoMarkMenuForm(Control anchor, Point clientLocation, Action<int> setState)
        {
            _setState = setState;
            ThemePalette theme = UiTheme.Current;

            FormBorderStyle = FormBorderStyle.None;
            ShowInTaskbar = false;
            StartPosition = FormStartPosition.Manual;
            BackColor = theme.BorderColor;
            Padding = new Padding(1);
            AutoSize = true;
            AutoSizeMode = AutoSizeMode.GrowAndShrink;
            TopMost = true;

            FlowLayoutPanel panel = new FlowLayoutPanel
            {
                FlowDirection = FlowDirection.TopDown,
                AutoSize = true,
                WrapContents = false,
                Margin = Padding.Empty,
                Padding = new Padding(2),
                BackColor = theme.MenuBackColor
            };

            AddItem(panel, "Clear", 0, Color.Black);
            AddItem(panel, "Green", 1, theme.MarkGreen);
            AddItem(panel, "Red", 2, theme.MarkRed);
            AddItem(panel, "Orange", 3, theme.MarkOrange);
            AddItem(panel, "Blue", 4, theme.MarkBlue);
            AddItem(panel, "Purple", 5, theme.MarkPurple);
            Controls.Add(panel);
            ClientSize = panel.PreferredSize;

            Rectangle work = Screen.FromControl(anchor).WorkingArea;
            Point screen = anchor.PointToScreen(clientLocation);
            Location = new Point(
                Math.Max(work.Left, Math.Min(screen.X, work.Right - Width)),
                Math.Max(work.Top, Math.Min(screen.Y, work.Bottom - Height)));

            Deactivate += (s, e) =>
            {
                Close();
                Dispose();
            };

            Show(anchor.FindForm());
        }

        private void AddItem(FlowLayoutPanel panel, string text, int state, Color swatchColor)
        {
            MarkMenuRow row = new MarkMenuRow(text, swatchColor);
            row.Click += (s, e) =>
            {
                _setState(state);
                Close();
            };
            panel.Controls.Add(row);
        }

        private sealed class MarkMenuRow : Control
        {
            private const int SwatchSize = 14;
            private const int LeftPadding = 10;
            private const int SwatchGap = 8;

            private readonly string _text;
            private readonly Color _swatchColor;
            private readonly Color _rowBackColor;
            private readonly Color _hoverBackColor;
            private readonly Color _foreColor;
            private bool _hover;

            public MarkMenuRow(string text, Color swatchColor)
            {
                _text = text;
                _swatchColor = swatchColor;
                _rowBackColor = UiTheme.Current.MenuBackColor;
                _hoverBackColor = UiTheme.Current.MenuHoverBackColor;
                _foreColor = UiTheme.Current.MenuForeColor;
                Width = 132;
                Height = 28;
                Margin = new Padding(1);
                Cursor = Cursors.Hand;
                SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.Selectable, true);
                UpdateStyles();
            }

            protected override void OnMouseEnter(EventArgs e)
            {
                base.OnMouseEnter(e);
                _hover = true;
                Invalidate();
            }

            protected override void OnMouseLeave(EventArgs e)
            {
                base.OnMouseLeave(e);
                _hover = false;
                Invalidate();
            }

            protected override void OnPaint(PaintEventArgs e)
            {
                Color rowColor = _hover ? _hoverBackColor : _rowBackColor;
                e.Graphics.Clear(rowColor);

                int swatchTop = Math.Max(0, (Height - SwatchSize) / 2);
                Rectangle swatch = new Rectangle(LeftPadding, swatchTop, SwatchSize, SwatchSize);
                using (SolidBrush swatchBrush = new SolidBrush(_swatchColor))
                {
                    e.Graphics.FillRectangle(swatchBrush, swatch);
                }

                using (Pen swatchBorder = new Pen(Color.FromArgb(160, 160, 160)))
                {
                    e.Graphics.DrawRectangle(swatchBorder, swatch);
                }

                Rectangle textRect = new Rectangle(
                    LeftPadding + SwatchSize + SwatchGap,
                    0,
                    Width - LeftPadding - SwatchSize - SwatchGap - 4,
                    Height);
                TextRenderer.DrawText(
                    e.Graphics,
                    _text,
                    Font,
                    textRect,
                    _foreColor,
                    TextFormatFlags.Left | TextFormatFlags.VerticalCenter);
            }
        }
    }
}
