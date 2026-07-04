using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class BingoCardDialog
    {
        private Panel _gridHost;

        private void InitializeComponent()
        {
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
            _gridHost = new Panel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                MinimumSize = new Size(860, 420)
            };
            _gridHost.Controls.Add(_grid);
            _gridHost.Resize += (s, e) => CenterGridInHost(_gridHost);
            CenterGridInHost(_gridHost);
            _statusLabel = new Label
            {
                AutoSize = true,
                Dock = DockStyle.Fill,
                Padding = new Padding(0, 8, 0, 0)
            };

            root.Controls.Add(metaPanel, 0, 0);
            root.Controls.Add(controlsPanel, 0, 1);
            root.Controls.Add(_gridHost, 0, 2);
            root.Controls.Add(_statusLabel, 0, 3);
            Controls.Add(root);
            FormClosed += (s, e) => ClosePopouts();
            UiTheme.ApplyToForm(this);
        }
    }
}
