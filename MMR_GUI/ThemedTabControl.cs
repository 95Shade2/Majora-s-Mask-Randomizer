using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal class ThemedTabControl : TabControl
    {
        public ThemedTabControl()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
        }

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
            UiTheme.ApplyNativeControlTheme(this);
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            ThemePalette theme = UiTheme.Current;
            using (SolidBrush panelBrush = new SolidBrush(theme.PanelBackColor))
            {
                pevent.Graphics.FillRectangle(panelBrush, ClientRectangle);
            }

            PaintTabStrip(pevent.Graphics);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            PaintTabStripGap(e.Graphics);
        }

        private void PaintTabStrip(Graphics graphics)
        {
            if (TabCount == 0)
            {
                return;
            }

            ThemePalette theme = UiTheme.Current;
            int stripHeight = GetTabRect(0).Bottom;
            using (SolidBrush stripBrush = new SolidBrush(theme.TabStripBackColor))
            {
                graphics.FillRectangle(stripBrush, 0, 0, ClientSize.Width, stripHeight);
            }
        }

        private void PaintTabStripGap(Graphics graphics)
        {
            if (TabCount == 0)
            {
                return;
            }

            ThemePalette theme = UiTheme.Current;
            int stripHeight = GetTabRect(0).Bottom;
            int lastTabRight = GetTabRect(TabCount - 1).Right;
            int gapWidth = ClientSize.Width - lastTabRight;
            if (gapWidth <= 0)
            {
                return;
            }

            using (SolidBrush stripBrush = new SolidBrush(theme.TabStripBackColor))
            {
                graphics.FillRectangle(stripBrush, lastTabRight, 0, gapWidth, stripHeight);
            }
        }
    }
}
