using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class BingoLinePopoutDialog : Form
    {
        private readonly BingoCard _card;
        private readonly int[] _indices;
        private readonly Action _onStateChanged;
        private readonly Button[] _cells;

        public BingoLinePopoutDialog(
            BingoCard card,
            int[] indices,
            string title,
            bool horizontal,
            Action onStateChanged)
        {
            _card = card;
            _indices = indices;
            _onStateChanged = onStateChanged;
            _cells = new Button[indices.Length];

            Text = title;
            StartPosition = FormStartPosition.CenterParent;
            Font = UiTheme.Current.BaseFont;
            MinimumSize = horizontal ? new Size(720, 120) : new Size(280, 420);
            FormBorderStyle = FormBorderStyle.SizableToolWindow;

            TableLayoutPanel layout = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                Padding = new Padding(8),
                ColumnCount = horizontal ? indices.Length : 1,
                RowCount = horizontal ? 1 : indices.Length
            };

            for (int i = 0; i < indices.Length; i++)
            {
                if (horizontal)
                {
                    layout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F / indices.Length));
                }
                else
                {
                    layout.RowStyles.Add(new RowStyle(SizeType.Percent, 100F / indices.Length));
                }
            }

            for (int i = 0; i < indices.Length; i++)
            {
                int index = _indices[i];
                Button cell = new Button
                {
                    Text = _card.Goals[index],
                    Dock = DockStyle.Fill,
                    Margin = new Padding(3),
                    TextAlign = ContentAlignment.MiddleCenter,
                    Font = new Font(Font.FontFamily, 8F),
                    Tag = index
                };
                ApplyCellStyle(cell, index);
                cell.MouseDown += Cell_MouseDown;
                _cells[i] = cell;

                if (horizontal)
                {
                    layout.Controls.Add(cell, i, 0);
                }
                else
                {
                    layout.Controls.Add(cell, 0, i);
                }
            }

            Controls.Add(layout);
        }

        private void Cell_MouseDown(object sender, MouseEventArgs e)
        {
            Button cell = (Button)sender;
            int index = (int)cell.Tag;

            if (e.Button == MouseButtons.Left)
            {
                _card.GoalStates[index] = BingoGoalMark.CycleForward(_card.GoalStates[index]);
            }
            else if (e.Button == MouseButtons.Right)
            {
                _card.GoalStates[index] = BingoGoalMark.CycleBackward(_card.GoalStates[index]);
            }
            else
            {
                return;
            }

            ApplyCellStyle(cell, index);
            if (_onStateChanged != null)
            {
                _onStateChanged();
            }
        }

        public void RefreshCells()
        {
            for (int i = 0; i < _indices.Length; i++)
            {
                ApplyCellStyle(_cells[i], _indices[i]);
            }
        }

        private void ApplyCellStyle(Button cell, int index)
        {
            cell.BackColor = BingoGoalMark.GetBackColor(_card.GoalStates[index]);
        }
    }
}
