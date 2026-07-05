using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class BingoLinePopoutDialog : Form
    {
        private readonly BingoCard _card;
        private readonly int[] _indices;
        private readonly Action _onStateChanged;
        private readonly BingoGoalButton[] _cells;

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
            _cells = new BingoGoalButton[indices.Length];

            InitializeComponent();
            Text = title;
            MinimumSize = horizontal ? new Size(720, 120) : new Size(280, 420);

            _layout.ColumnCount = horizontal ? indices.Length : 1;
            _layout.RowCount = horizontal ? 1 : indices.Length;
            _layout.ColumnStyles.Clear();
            _layout.RowStyles.Clear();

            for (int i = 0; i < indices.Length; i++)
            {
                if (horizontal)
                {
                    _layout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F / indices.Length));
                }
                else
                {
                    _layout.RowStyles.Add(new RowStyle(SizeType.Percent, 100F / indices.Length));
                }
            }

            for (int i = 0; i < indices.Length; i++)
            {
                int index = _indices[i];
                BingoGoalButton cell = new BingoGoalButton
                {
                    Text = _card.Goals[index],
                    Dock = DockStyle.Fill,
                    Margin = new Padding(3),
                    Font = new Font(Font.FontFamily, 8F),
                    GoalIndex = index,
                    Tag = "BingoGoalCell"
                };
                BingoGoalMark.ApplyCellStyle(cell, _card.GoalStates[index]);
                cell.MouseDown += Cell_MouseDown;
                cell.MouseUp += Cell_MouseUp;
                _cells[i] = cell;

                if (horizontal)
                {
                    _layout.Controls.Add(cell, i, 0);
                }
                else
                {
                    _layout.Controls.Add(cell, 0, i);
                }
            }

            UiTheme.ApplyToForm(this);
            RefreshCells();
        }

        private void Cell_MouseDown(object sender, MouseEventArgs e)
        {
            BingoGoalButton cell = (BingoGoalButton)sender;
            int index = cell.GoalIndex;

            if (e.Button == MouseButtons.Left)
            {
                _card.GoalStates[index] = BingoGoalMark.CycleForward(_card.GoalStates[index]);
                ApplyCellStyle(cell, index);
                if (_onStateChanged != null)
                {
                    _onStateChanged();
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                _card.GoalStates[index] = BingoGoalMark.CycleBackward(_card.GoalStates[index]);
                ApplyCellStyle(cell, index);
                if (_onStateChanged != null)
                {
                    _onStateChanged();
                }
            }
        }

        private void Cell_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Middle)
            {
                return;
            }

            BingoGoalButton cell = (BingoGoalButton)sender;
            int index = cell.GoalIndex;
            BingoGoalMark.ShowMarkContextMenu(cell, e.Location, state =>
            {
                _card.GoalStates[index] = state;
                ApplyCellStyle(cell, index);
                if (_onStateChanged != null)
                {
                    _onStateChanged();
                }
            });
        }

        public void RefreshCells()
        {
            for (int i = 0; i < _indices.Length; i++)
            {
                ApplyCellStyle(_cells[i], _indices[i]);
            }
        }

        private void ApplyCellStyle(BingoGoalButton cell, int index)
        {
            BingoGoalMark.ApplyCellStyle(cell, _card.GoalStates[index]);
        }
    }
}
