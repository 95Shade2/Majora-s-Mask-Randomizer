using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class BingoCardDialog : Form
    {
        private readonly Main_Window _owner;
        private readonly TextBox _romSeedBox;
        private readonly Label _effectiveSeedLabel;
        private readonly Label _poolHashLabel;
        private readonly Label _rerollLabel;
        private readonly Button _viewRerollLogButton;
        private readonly Label _warningLabel;
        private readonly Label _statusLabel;
        private readonly RadioButton _lineMode;
        private readonly RadioButton _blackoutMode;
        private readonly TableLayoutPanel _grid;
        private readonly Button[,] _cells = new Button[5, 5];

        private BingoCard _card;
        private readonly List<BingoLinePopoutDialog> _openPopouts = new List<BingoLinePopoutDialog>();

        public BingoCardDialog(Main_Window owner)
        {
            _owner = owner;
            Text = "Bingo Card";
            ClientSize = new Size(980, 720);
            MinimumSize = new Size(900, 640);
            StartPosition = FormStartPosition.CenterParent;
            Font = UiTheme.Current.BaseFont;
            AutoScroll = true;

            TableLayoutPanel root = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 4,
                Padding = new Padding(12)
            };
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            Panel metaPanel = BuildMetaPanel(
                out _romSeedBox,
                out _effectiveSeedLabel,
                out _poolHashLabel,
                out _rerollLabel,
                out _viewRerollLogButton,
                out _warningLabel);
            Panel controlsPanel = BuildControlsPanel(out _lineMode, out _blackoutMode);
            _grid = BuildGrid();
            Panel gridHost = new Panel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                MinimumSize = new Size(860, 420)
            };
            gridHost.Controls.Add(_grid);
            gridHost.Resize += (s, e) => CenterGridInHost(gridHost);
            CenterGridInHost(gridHost);
            _statusLabel = new Label
            {
                AutoSize = true,
                Dock = DockStyle.Fill,
                Padding = new Padding(0, 8, 0, 0)
            };

            root.Controls.Add(metaPanel, 0, 0);
            root.Controls.Add(controlsPanel, 0, 1);
            root.Controls.Add(gridHost, 0, 2);
            root.Controls.Add(_statusLabel, 0, 3);
            Controls.Add(root);
            FormClosed += (s, e) => ClosePopouts();
            UiTheme.ApplyToForm(this);
        }

        public void RefreshFromMainWindow()
        {
            _romSeedBox.Text = _owner.GetRomSeedText();
            RegenerateCard();
        }

        private void RegenerateCard()
        {
            try
            {
                ClosePopouts();
                BingoWinMode mode = _blackoutMode.Checked ? BingoWinMode.Blackout : BingoWinMode.Line;
                _card = BingoCardGenerator.Generate(_romSeedBox.Text, mode, _owner);
                UpdateMetaLabels();
                PopulateGrid();
                UpdateStatus();
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message, "Bingo Card", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void ClosePopouts()
        {
            for (int i = _openPopouts.Count - 1; i >= 0; i--)
            {
                if (_openPopouts[i] != null && !_openPopouts[i].IsDisposed)
                {
                    _openPopouts[i].Close();
                }
            }

            _openPopouts.Clear();
        }

        private void UpdateMetaLabels()
        {
            if (_card == null)
            {
                return;
            }

            bool seedRerolled = _card.RerollCount > 0;
            _effectiveSeedLabel.Text = "Generation seed: " + _card.EffectiveSeed
                + (seedRerolled ? " (rerolled)" : "");
            _effectiveSeedLabel.ForeColor = seedRerolled ? UiTheme.Current.WarningForeColor : UiTheme.Current.ForeColor;
            _poolHashLabel.Text = "Pool hash (" + BingoGoalValidator.PoolHashVersion + "): " + _card.PoolHash;
            _rerollLabel.Text = _card.RerollCount > 0
                ? "Rerolls: " + _card.RerollCount
                : "Rerolls: 0";
            _viewRerollLogButton.Enabled = _card.RerollTrace != null && _card.RerollTrace.Count > 0;
            _warningLabel.Text = _card.GoalsSubstituted
                ? "Warning: some goals were substituted after reroll exhaustion."
                : "";
        }

        private void PopulateGrid()
        {
            if (_card == null)
            {
                return;
            }

            for (int row = 0; row < 5; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    int index = row * 5 + col;
                    ApplyCellStyle(_cells[row, col], index);
                    _cells[row, col].Text = _card.Goals[index];
                }
            }

            RefreshOpenPopouts();
        }

        private void ApplyCellStyle(Button cell, int index)
        {
            if (_card == null || _card.GoalStates == null)
            {
                return;
            }

            cell.BackColor = BingoGoalMark.GetBackColor(_card.GoalStates[index]);
        }

        private void RefreshOpenPopouts()
        {
            for (int i = 0; i < _openPopouts.Count; i++)
            {
                if (_openPopouts[i] != null && !_openPopouts[i].IsDisposed)
                {
                    _openPopouts[i].RefreshCells();
                }
            }
        }

        private void SyncFromCard()
        {
            PopulateGrid();
            UpdateStatus();
        }

        private void UpdateStatus()
        {
            if (_card == null)
            {
                _statusLabel.Text = "";
                return;
            }

            int marked = CountMarkedGoals();

            string winText = "";
            if (_card.WinMode == BingoWinMode.Blackout)
            {
                if (marked >= 25)
                {
                    winText = " | Blackout complete!";
                }

                _statusLabel.Text = "Goals: " + marked + " / 25" + winText;
            }
            else
            {
                string line = FindCompletedLine();
                if (line != null)
                {
                    winText = " | Line complete: " + line;
                }

                _statusLabel.Text = "Goals: " + marked + " / 5 needed" + winText;
            }
        }

        private int CountMarkedGoals()
        {
            int marked = 0;
            for (int i = 0; i < _card.GoalStates.Length; i++)
            {
                if (BingoGoalMark.IsMarked(_card.GoalStates[i]))
                {
                    marked++;
                }
            }

            return marked;
        }

        private string FindCompletedLine()
        {
            for (int row = 0; row < 5; row++)
            {
                if (IsLineMarked(row * 5, 1, 5))
                {
                    return "ROW-" + (row + 1);
                }
            }

            for (int col = 0; col < 5; col++)
            {
                if (IsLineMarked(col, 5, 5))
                {
                    return "COL-" + (col + 1);
                }
            }

            if (IsLineMarked(0, 6, 5))
            {
                return "TL-BR";
            }

            if (IsLineMarked(4, 4, 5))
            {
                return "BL-TR";
            }

            return null;
        }

        private bool IsLineMarked(int start, int step, int count)
        {
            int index = start;
            for (int i = 0; i < count; i++)
            {
                if (!BingoGoalMark.IsMarked(_card.GoalStates[index]))
                {
                    return false;
                }

                index += step;
            }

            return true;
        }

        private void CellMouseDown(int row, int col, MouseEventArgs e)
        {
            if (_card == null)
            {
                return;
            }

            int index = row * 5 + col;
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

            ApplyCellStyle(_cells[row, col], index);
            RefreshOpenPopouts();
            UpdateStatus();
        }

        private void OpenLinePopout(string title, int[] indices, bool horizontal)
        {
            if (_card == null)
            {
                return;
            }

            BingoLinePopoutDialog popout = new BingoLinePopoutDialog(
                _card,
                indices,
                title,
                horizontal,
                SyncFromCard);
            popout.FormClosed += (s, e) => _openPopouts.Remove(popout);
            _openPopouts.Add(popout);
            popout.Show(this);
        }

        private static int[] RowIndices(int row)
        {
            int start = row * 5;
            return new int[] { start, start + 1, start + 2, start + 3, start + 4 };
        }

        private static int[] ColIndices(int col)
        {
            return new int[] { col, col + 5, col + 10, col + 15, col + 20 };
        }

        private void LineHeaderMouseDown(string title, int[] indices, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                OpenLinePopout(title, indices, false);
            }
            else if (e.Button == MouseButtons.Right)
            {
                OpenLinePopout(title, indices, true);
            }
        }

        private Panel BuildMetaPanel(
            out TextBox romSeedBox,
            out Label effectiveSeedLabel,
            out Label poolHashLabel,
            out Label rerollLabel,
            out Button viewRerollLogButton,
            out Label warningLabel)
        {
            Panel panel = new Panel { Dock = DockStyle.Top, AutoSize = true };

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                ColumnCount = 3,
                RowCount = 5
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 110));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));

            romSeedBox = new TextBox { Width = 160, Anchor = AnchorStyles.Left };
            effectiveSeedLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            poolHashLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            rerollLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            viewRerollLogButton = new Button { Text = "View reroll log", AutoSize = true, Enabled = false };
            viewRerollLogButton.Click += ViewRerollLogClicked;
            warningLabel = new Label { AutoSize = true, ForeColor = UiTheme.Current.WarningForeColor, Anchor = AnchorStyles.Left };

            table.Controls.Add(new Label { Text = "ROM seed", AutoSize = true }, 0, 0);
            table.Controls.Add(romSeedBox, 1, 0);

            table.Controls.Add(new Label { Text = "Metadata", AutoSize = true }, 0, 1);
            table.Controls.Add(effectiveSeedLabel, 1, 1);
            table.SetColumnSpan(effectiveSeedLabel, 2);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 2);
            table.Controls.Add(poolHashLabel, 1, 2);
            Button copyHash = new Button { Text = "Copy hash", AutoSize = true };
            copyHash.Click += CopyHashClicked;
            table.Controls.Add(copyHash, 2, 2);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 3);
            table.Controls.Add(rerollLabel, 1, 3);
            table.Controls.Add(viewRerollLogButton, 2, 3);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 4);
            table.Controls.Add(warningLabel, 1, 4);
            table.SetColumnSpan(warningLabel, 2);

            panel.Controls.Add(table);
            return panel;
        }

        private Panel BuildControlsPanel(out RadioButton lineMode, out RadioButton blackoutMode)
        {
            Panel panel = new Panel { Dock = DockStyle.Top, AutoSize = true, Padding = new Padding(0, 8, 0, 8) };

            TableLayoutPanel layout = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                ColumnCount = 1,
                RowCount = 2
            };
            layout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            layout.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            FlowLayoutPanel flow = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = true,
                Dock = DockStyle.Fill
            };

            lineMode = new RadioButton { Text = "Line", Checked = true, AutoSize = true };
            blackoutMode = new RadioButton { Text = "Blackout", AutoSize = true };

            Button regenerate = new Button { Text = "Regenerate", AutoSize = true };
            regenerate.Click += (s, e) => RegenerateCard();

            Button exportHtml = new Button { Text = "Export HTML", AutoSize = true };
            exportHtml.Click += ExportHtmlClicked;

            Button exportText = new Button { Text = "Export Text", AutoSize = true };
            exportText.Click += ExportTextClicked;

            flow.Controls.Add(new Label { Text = "Win:", AutoSize = true, Padding = new Padding(0, 6, 0, 0) });
            flow.Controls.Add(lineMode);
            flow.Controls.Add(blackoutMode);
            flow.Controls.Add(regenerate);
            flow.Controls.Add(exportHtml);
            flow.Controls.Add(exportText);

            Label hint = new Label
            {
                Text = "L-click forward / R-click backward on goals. L-click row/col headers for vertical popout, R-click for horizontal.",
                AutoSize = true,
                ForeColor = UiTheme.Current.HintForeColor,
                Padding = new Padding(0, 4, 0, 0),
                MaximumSize = new Size(940, 0)
            };

            layout.Controls.Add(flow, 0, 0);
            layout.Controls.Add(hint, 0, 1);
            panel.Controls.Add(layout);
            return panel;
        }

        private const int HeaderColWidth = 72;
        private const int GoalColWidth = 156;
        private static readonly int GridWidth = HeaderColWidth + GoalColWidth * 5;

        private TableLayoutPanel BuildGrid()
        {
            TableLayoutPanel grid = new TableLayoutPanel
            {
                Anchor = AnchorStyles.Top | AnchorStyles.Left,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 6,
                RowCount = 7,
                Padding = new Padding(0, 8, 0, 0),
                MinimumSize = new Size(GridWidth, 0),
                MaximumSize = new Size(GridWidth, 0),
                Width = GridWidth
            };

            for (int i = 0; i < 6; i++)
            {
                grid.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, i == 0 ? HeaderColWidth : GoalColWidth));
            }

            for (int i = 0; i < 7; i++)
            {
                grid.RowStyles.Add(new RowStyle(SizeType.Absolute, i == 0 ? 28 : (i == 6 ? 28 : 88)));
            }

            Label corner = CreateLineHeaderLabel("CARD");
            corner.MouseDown += (s, e) =>
            {
                int[] all = new int[25];
                for (int i = 0; i < 25; i++)
                {
                    all[i] = i;
                }

                LineHeaderMouseDown("CARD", all, e);
            };
            grid.Controls.Add(corner, 0, 0);

            for (int col = 0; col < 5; col++)
            {
                int c = col;
                Label header = CreateLineHeaderLabel("COL-" + (col + 1));
                header.MouseDown += (s, e) => LineHeaderMouseDown("COL-" + (c + 1), ColIndices(c), e);
                grid.Controls.Add(header, col + 1, 0);
            }

            for (int row = 0; row < 5; row++)
            {
                int r = row;
                Label rowHeader = CreateLineHeaderLabel("ROW-" + (row + 1));
                rowHeader.MouseDown += (s, e) => LineHeaderMouseDown("ROW-" + (r + 1), RowIndices(r), e);
                grid.Controls.Add(rowHeader, 0, row + 1);

                for (int col = 0; col < 5; col++)
                {
                    int rowIndex = row;
                    int colIndex = col;
                    Button cell = new Button
                    {
                        Dock = DockStyle.Fill,
                        Margin = new Padding(2),
                        MinimumSize = Size.Empty,
                        TextAlign = ContentAlignment.MiddleCenter,
                        FlatStyle = FlatStyle.Standard,
                        Font = new Font(Font.FontFamily, 8F),
                        Tag = row * 5 + col,
                        UseVisualStyleBackColor = false
                    };
                    cell.MouseDown += (s, e) => CellMouseDown(rowIndex, colIndex, e);
                    _cells[row, col] = cell;
                    grid.Controls.Add(cell, col + 1, row + 1);
                }
            }

            Label tlbr = CreateLineHeaderLabel("TL-BR");
            tlbr.MouseDown += (s, e) => LineHeaderMouseDown("TL-BR", new int[] { 0, 6, 12, 18, 24 }, e);
            grid.Controls.Add(tlbr, 0, 6);

            Label bltr = CreateLineHeaderLabel("BL-TR");
            bltr.MouseDown += (s, e) => LineHeaderMouseDown("BL-TR", new int[] { 4, 8, 12, 16, 20 }, e);
            grid.Controls.Add(bltr, 5, 6);

            return grid;
        }

        private static void CenterGridInHost(Panel host)
        {
            Control grid = host.Controls.Count > 0 ? host.Controls[0] : null;
            if (grid == null)
            {
                return;
            }

            grid.Left = Math.Max(0, (host.ClientSize.Width - grid.Width) / 2);
        }

        private static Label CreateLineHeaderLabel(string text)
        {
            return new Label
            {
                Text = text,
                TextAlign = ContentAlignment.MiddleCenter,
                Dock = DockStyle.Fill,
                Cursor = Cursors.Hand,
                AutoSize = false,
                Font = new Font(UiTheme.Current.BaseFont.FontFamily, 8F, FontStyle.Underline)
            };
        }

        private void ViewRerollLogClicked(object sender, EventArgs e)
        {
            if (_card == null)
            {
                return;
            }

            BingoRerollLogDialog.ShowForCard(_card, this);
        }

        private void CopyHashClicked(object sender, EventArgs e)
        {
            if (_card != null && !string.IsNullOrEmpty(_card.PoolHash))
            {
                Clipboard.SetText(_card.PoolHash);
            }
        }

        private void ExportHtmlClicked(object sender, EventArgs e)
        {
            if (_card == null)
            {
                return;
            }

            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "HTML files|*.html|All files|*.*";
                dialog.FileName = "Bingo Card - " + _card.EffectiveSeed + ".html";
                if (dialog.ShowDialog(this) == DialogResult.OK)
                {
                    BingoCardExport.SaveHtml(_card, dialog.FileName);
                }
            }
        }

        private void ExportTextClicked(object sender, EventArgs e)
        {
            if (_card == null)
            {
                return;
            }

            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.Filter = "Text files|*.txt|All files|*.*";
                dialog.FileName = "Bingo Card - " + _card.EffectiveSeed + ".txt";
                if (dialog.ShowDialog(this) == DialogResult.OK)
                {
                    BingoCardExport.SavePlainText(_card, dialog.FileName);
                }
            }
        }
    }
}
