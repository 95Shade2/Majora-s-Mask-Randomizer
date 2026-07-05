using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class BingoGoalButton : Button
    {
        public int GoalIndex { get; set; }

        public int MarkState { get; set; }

        public BingoGoalButton()
        {
            FlatStyle = FlatStyle.Flat;
            UseVisualStyleBackColor = false;
            TextAlign = ContentAlignment.MiddleCenter;
            FlatAppearance.BorderSize = 1;
            SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
            UpdateStyles();
        }

        public void ApplyMarkState(int state)
        {
            MarkState = state;
            ForeColor = BingoGoalMark.IsMarked(state) ? Color.White : UiTheme.Current.ControlForeColor;
            FlatAppearance.BorderColor = UiTheme.Current.BorderColor;
            FlatAppearance.MouseOverBackColor = BingoGoalMark.GetBackColor(state);
            FlatAppearance.MouseDownBackColor = BingoGoalMark.GetBackColor(state);
            Invalidate();
        }

        protected override void OnPaint(PaintEventArgs pevent)
        {
            Color backColor = BingoGoalMark.GetBackColor(MarkState);
            pevent.Graphics.Clear(backColor);
            using (Pen border = new Pen(UiTheme.Current.BorderColor))
            {
                Rectangle bounds = ClientRectangle;
                bounds.Width -= 1;
                bounds.Height -= 1;
                pevent.Graphics.DrawRectangle(border, bounds);
            }

            if (!string.IsNullOrEmpty(Text))
            {
                TextRenderer.DrawText(
                    pevent.Graphics,
                    Text,
                    Font,
                    ClientRectangle,
                    ForeColor,
                    TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter | TextFormatFlags.WordBreak);
            }
        }
    }
}
