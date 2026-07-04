using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class ThemeToggleSlider : Control
    {
        private static readonly Color TrackOnColor = Color.FromArgb(52, 120, 246);
        private static readonly Color TrackOffColorDark = Color.FromArgb(96, 96, 102);
        private static readonly Color TrackOffColorLight = Color.FromArgb(209, 209, 214);
        private static readonly Color ThumbColor = Color.White;
        private static readonly Color ThumbShadowColor = Color.FromArgb(48, 0, 0, 0);

        private bool _checked;

        public event EventHandler CheckedChanged;

        public bool Checked
        {
            get => _checked;
            set
            {
                if (_checked == value)
                {
                    return;
                }

                _checked = value;
                Invalidate();
                CheckedChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        public ThemeToggleSlider()
        {
            Size = new Size(48, 28);
            MinimumSize = Size;
            MaximumSize = Size;
            Cursor = Cursors.Hand;
            TabStop = false;
            SetStyle(
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.UserPaint |
                ControlStyles.OptimizedDoubleBuffer |
                ControlStyles.ResizeRedraw |
                ControlStyles.SupportsTransparentBackColor,
                true);
            ApplyTheme();
        }

        public void ApplyTheme()
        {
            BackColor = UiTheme.Current.TabStripBackColor;
            Invalidate();
        }

        protected override void OnClick(EventArgs e)
        {
            Checked = !Checked;
            base.OnClick(e);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            ThemePalette theme = UiTheme.Current;
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.PixelOffsetMode = PixelOffsetMode.HighQuality;

            using (SolidBrush backBrush = new SolidBrush(theme.TabStripBackColor))
            {
                g.FillRectangle(backBrush, ClientRectangle);
            }

            Rectangle track = new Rectangle(0, 0, Width, Height);
            Color trackColor = Checked
                ? TrackOnColor
                : (UiTheme.IsDark ? TrackOffColorDark : TrackOffColorLight);

            using (GraphicsPath trackPath = CreateRoundedRect(track, track.Height / 2))
            using (SolidBrush trackBrush = new SolidBrush(trackColor))
            {
                g.FillPath(trackBrush, trackPath);
            }

            const int thumbInset = 2;
            int thumbDiameter = track.Height - (thumbInset * 2);
            int thumbTravel = track.Width - thumbDiameter - (thumbInset * 2);
            int thumbX = Checked ? track.X + thumbInset + thumbTravel : track.X + thumbInset;
            int thumbY = track.Y + thumbInset;
            Rectangle thumb = new Rectangle(thumbX, thumbY, thumbDiameter, thumbDiameter);

            Rectangle shadow = thumb;
            shadow.Offset(0, 1);
            shadow.Inflate(0, 1);
            using (SolidBrush shadowBrush = new SolidBrush(ThumbShadowColor))
            {
                g.FillEllipse(shadowBrush, shadow);
            }

            using (SolidBrush thumbBrush = new SolidBrush(ThumbColor))
            {
                g.FillEllipse(thumbBrush, thumb);
            }
        }

        private static GraphicsPath CreateRoundedRect(Rectangle bounds, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int diameter = radius * 2;
            path.AddArc(bounds.X, bounds.Y, diameter, diameter, 180, 90);
            path.AddArc(bounds.Right - diameter, bounds.Y, diameter, diameter, 270, 90);
            path.AddArc(bounds.Right - diameter, bounds.Bottom - diameter, diameter, diameter, 0, 90);
            path.AddArc(bounds.X, bounds.Bottom - diameter, diameter, diameter, 90, 90);
            path.CloseFigure();
            return path;
        }
    }
}
