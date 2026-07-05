using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class BingoCardDialog : Form
    {
        private readonly Main_Window _owner;
        private TextBox _romSeedBox;
        private Label _effectiveSeedLabel;
        private Label _poolHashLabel;
        private Label _settingsHashLabel;
        private Label _logicLabel;
        private Label _placementsHashLabel;
        private Label _verificationLabel;
        private TextBox _raceCodeBox;
        private Label _rerollLabel;
        private Button _viewRerollLogButton;
        private Label _warningLabel;
        private Label _statusLabel;
        private RadioButton _lineMode;
        private RadioButton _blackoutMode;
        private TableLayoutPanel _grid;
        private readonly BingoGoalButton[,] _cells = new BingoGoalButton[5, 5];

        private BingoCard _card;
        private readonly List<BingoLinePopoutDialog> _openPopouts = new List<BingoLinePopoutDialog>();
        private bool _suppressRomSeedEvents;

        public BingoCardDialog(Main_Window owner)
        {
            _owner = owner;
            InitializeComponent();
            _romSeedBox.Text = _owner.GetRomSeedText();
            RegenerateCard();
        }

        public void RefreshFromMainWindow()
        {
            _owner.ClearBingoRaceContext();
            _romSeedBox.Text = _owner.GetRomSeedText();
            RegenerateCard(false);
        }

        private void RegenerateCard(bool clearRaceContext = true)
        {
            try
            {
                if (clearRaceContext)
                {
                    _owner.ClearBingoRaceContext();
                }
                ClosePopouts();
                BingoWinMode mode = _blackoutMode.Checked ? BingoWinMode.Blackout : BingoWinMode.Line;
                _card = BingoCardGenerator.Generate(_romSeedBox.Text, mode, _owner);
                _card.SettingsHash = _owner.ComputeSettingsHash();
                AsyncRaceBuildResult raceCode = AsyncRaceCode.Build(_owner, mode);
                _card.AsyncRaceCode = raceCode.Success ? raceCode.RaceCode : "";
                SyncRomSeedFromCard();
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

        private void SyncRomSeedFromCard()
        {
            if (_card == null || string.IsNullOrEmpty(_card.RomSeed))
            {
                return;
            }

            if (!string.Equals(_romSeedBox.Text?.Trim(), _card.RomSeed, StringComparison.Ordinal))
            {
                _suppressRomSeedEvents = true;
                try
                {
                    _romSeedBox.Text = _card.RomSeed;
                }
                finally
                {
                    _suppressRomSeedEvents = false;
                }
            }

            if (string.IsNullOrWhiteSpace(_owner.GetRomSeedText()))
            {
                _owner.SetRomSeedText(_card.RomSeed);
            }
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
            _settingsHashLabel.Text = "Settings hash (" + SettingsHash.Version + "): " + _card.SettingsHash;
            _logicLabel.Text = "Logic: " + (string.IsNullOrEmpty(_card.LogicName) ? "None" : _card.LogicName);
            _placementsHashLabel.Text = "Placements hash (" + BingoPlacementMap.Version + "): "
                + (string.IsNullOrEmpty(_card.PlacementsHash) ? "(none)" : _card.PlacementsHash);

            string currentHash = _owner.ComputeSettingsHash();
            if (!string.IsNullOrEmpty(_card.SettingsHash)
                && string.Equals(currentHash, _card.SettingsHash, StringComparison.OrdinalIgnoreCase))
            {
                _verificationLabel.Text = "Race settings verified.";
                _verificationLabel.ForeColor = UiTheme.Current.MarkGreen;
            }
            else
            {
                _verificationLabel.Text = "Race settings mismatch — re-apply race code or regenerate from host.";
                _verificationLabel.ForeColor = UiTheme.Current.ErrorForeColor;
            }

            if (_raceCodeBox != null && !string.IsNullOrEmpty(_card.AsyncRaceCode))
            {
                _raceCodeBox.Text = _card.AsyncRaceCode;
            }

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

        private void ApplyCellStyle(BingoGoalButton cell, int index)
        {
            if (_card == null || _card.GoalStates == null)
            {
                return;
            }

            BingoGoalMark.ApplyCellStyle(cell, _card.GoalStates[index]);
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
                return "TLBR";
            }

            if (IsLineMarked(4, 4, 5))
            {
                return "BLTR";
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
            BingoGoalButton cell = _cells[row, col];
            if (e.Button == MouseButtons.Left)
            {
                _card.GoalStates[index] = BingoGoalMark.CycleForward(_card.GoalStates[index]);
                ApplyCellStyle(cell, index);
                RefreshOpenPopouts();
                UpdateStatus();
            }
            else if (e.Button == MouseButtons.Right)
            {
                _card.GoalStates[index] = BingoGoalMark.CycleBackward(_card.GoalStates[index]);
                ApplyCellStyle(cell, index);
                RefreshOpenPopouts();
                UpdateStatus();
            }
        }

        private void CellMouseUp(int row, int col, MouseEventArgs e)
        {
            if (_card == null || e.Button != MouseButtons.Middle)
            {
                return;
            }

            int index = row * 5 + col;
            BingoGoalButton cell = _cells[row, col];
            BingoGoalMark.ShowMarkContextMenu(cell, e.Location, state =>
            {
                _card.GoalStates[index] = state;
                ApplyCellStyle(cell, index);
                RefreshOpenPopouts();
                UpdateStatus();
            });
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
            out Label settingsHashLabel,
            out Label verificationLabel,
            out TextBox raceCodeBox,
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
                RowCount = 10
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 110));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));

            romSeedBox = new TextBox { Width = 160, Anchor = AnchorStyles.Left };
            romSeedBox.TextChanged += (s, e) =>
            {
                if (!_suppressRomSeedEvents)
                {
                    RegenerateCard();
                }
            };
            effectiveSeedLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            poolHashLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            settingsHashLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            _logicLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            _placementsHashLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left };
            verificationLabel = new Label { AutoSize = true, Anchor = AnchorStyles.Left, MaximumSize = new Size(760, 0) };
            raceCodeBox = new TextBox { Anchor = AnchorStyles.Left | AnchorStyles.Right, Width = 420 };
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
            Button copyPoolHash = new Button { Text = "Copy pool hash", AutoSize = true };
            copyPoolHash.Click += CopyHashClicked;
            table.Controls.Add(copyPoolHash, 2, 2);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 3);
            table.Controls.Add(settingsHashLabel, 1, 3);
            Button copySettingsHash = new Button { Text = "Copy settings hash", AutoSize = true };
            copySettingsHash.Click += CopySettingsHashClicked;
            table.Controls.Add(copySettingsHash, 2, 3);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 4);
            table.Controls.Add(_logicLabel, 1, 4);
            table.SetColumnSpan(_logicLabel, 2);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 5);
            table.Controls.Add(_placementsHashLabel, 1, 5);
            table.SetColumnSpan(_placementsHashLabel, 2);

            table.Controls.Add(new Label { Text = "Status", AutoSize = true }, 0, 6);
            table.Controls.Add(verificationLabel, 1, 6);
            table.SetColumnSpan(verificationLabel, 2);

            table.Controls.Add(new Label { Text = "Race code", AutoSize = true }, 0, 7);
            table.Controls.Add(raceCodeBox, 1, 7);
            FlowLayoutPanel raceButtons = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.TopDown,
                WrapContents = false
            };
            Button applyRaceCode = new Button { Text = "Apply race code", AutoSize = true };
            applyRaceCode.Click += ApplyRaceCodeClicked;
            Button copyRaceCode = new Button { Text = "Copy race code", AutoSize = true };
            copyRaceCode.Click += CopyRaceCodeClicked;
            raceButtons.Controls.Add(applyRaceCode);
            raceButtons.Controls.Add(copyRaceCode);
            table.Controls.Add(raceButtons, 2, 7);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 8);
            table.Controls.Add(rerollLabel, 1, 8);
            table.Controls.Add(viewRerollLogButton, 2, 8);

            table.Controls.Add(new Label { Text = "", AutoSize = true }, 0, 9);
            table.Controls.Add(warningLabel, 1, 9);
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
            lineMode.CheckedChanged += WinModeChanged;
            blackoutMode.CheckedChanged += WinModeChanged;

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
        private const int GoalRowHeight = 88;
        private const int HeaderRowHeight = 28;
        private static readonly int GridWidth = HeaderColWidth + GoalColWidth * 5;
        private static readonly int GridHeight = 8 + HeaderRowHeight + (GoalRowHeight * 5) + HeaderRowHeight;

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
                grid.RowStyles.Add(new RowStyle(
                    SizeType.Absolute,
                    i == 0 ? HeaderRowHeight : (i == 6 ? HeaderRowHeight : GoalRowHeight)));
            }

            Label corner = CreateLineHeaderLabel("TLBR");
            corner.MouseDown += (s, e) => LineHeaderMouseDown("TLBR", new int[] { 0, 6, 12, 18, 24 }, e);
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
                    BingoGoalButton cell = new BingoGoalButton
                    {
                        Dock = DockStyle.Fill,
                        Margin = new Padding(2),
                        MinimumSize = Size.Empty,
                        Font = new Font(Font.FontFamily, 8F),
                        GoalIndex = row * 5 + col,
                        Tag = "BingoGoalCell"
                    };
                    cell.MouseDown += (s, e) => CellMouseDown(rowIndex, colIndex, e);
                    cell.MouseUp += (s, e) => CellMouseUp(rowIndex, colIndex, e);
                    _cells[row, col] = cell;
                    grid.Controls.Add(cell, col + 1, row + 1);
                }
            }

            Label bltr = CreateLineHeaderLabel("BLTR");
            bltr.MouseDown += (s, e) => LineHeaderMouseDown("BLTR", new int[] { 4, 8, 12, 16, 20 }, e);
            grid.Controls.Add(bltr, 0, 6);

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

        private void WinModeChanged(object sender, EventArgs e)
        {
            RadioButton button = sender as RadioButton;
            if (button != null && button.Checked)
            {
                RegenerateCard();
            }
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

        private void CopySettingsHashClicked(object sender, EventArgs e)
        {
            if (_card != null && !string.IsNullOrEmpty(_card.SettingsHash))
            {
                Clipboard.SetText(_card.SettingsHash);
            }
        }

        private void CopyRaceCodeClicked(object sender, EventArgs e)
        {
            BingoWinMode mode = _blackoutMode.Checked ? BingoWinMode.Blackout : BingoWinMode.Line;
            AsyncRaceBuildResult result = AsyncRaceCode.Build(_owner, mode);
            if (!result.Success)
            {
                MessageBox.Show(this, result.Message, "Async Race", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            Clipboard.SetText(result.RaceCode);
            if (_raceCodeBox != null)
            {
                _raceCodeBox.Text = result.RaceCode;
            }
        }

        private void ApplyRaceCodeClicked(object sender, EventArgs e)
        {
            if (_raceCodeBox == null)
            {
                return;
            }

            AsyncRaceCodeData parsed;
            string error;
            if (!AsyncRaceCode.TryParse(_raceCodeBox.Text, out parsed, out error))
            {
                MessageBox.Show(this, error, "Async Race", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            AsyncRaceApplyResult applyResult = AsyncRaceCode.Apply(_owner, parsed);
            if (!applyResult.Success)
            {
                MessageBox.Show(this, applyResult.Message, "Async Race", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                UpdateMetaLabels();
                return;
            }

            if (parsed.WinMode == BingoWinMode.Blackout)
            {
                _blackoutMode.Checked = true;
            }
            else
            {
                _lineMode.Checked = true;
            }

            _romSeedBox.Text = parsed.RomSeed;
            _raceCodeBox.Text = AsyncRaceCode.Format(parsed);

            RegenerateCard(false);
            MessageBox.Show(this, applyResult.Message, "Async Race", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
