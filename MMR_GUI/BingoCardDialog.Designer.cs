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
            StartPosition = FormStartPosition.CenterParent;
            Font = UiTheme.Current.BaseFont;
            AutoScroll = false;

            TableLayoutPanel root = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 4,
                Padding = new Padding(12)
            };
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            Panel metaPanel = BuildMetaPanel(
                out _romSeedBox,
                out _effectiveSeedLabel,
                out _poolHashLabel,
                out _settingsHashLabel,
                out _verificationLabel,
                out _raceCodeBox,
                out _rerollLabel,
                out _viewRerollLogButton,
                out _warningLabel);
            Panel controlsPanel = BuildControlsPanel(out _lineMode, out _blackoutMode);
            _grid = BuildGrid();
            _gridHost = new Panel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                AutoScroll = false,
                Padding = new Padding(0, 4, 0, 4)
            };
            _gridHost.Controls.Add(_grid);
            _gridHost.Resize += (s, e) => CenterGridInHost(_gridHost);
            CenterGridInHost(_gridHost);
            _statusLabel = new Label
            {
                AutoSize = true,
                Dock = DockStyle.Top,
                Padding = new Padding(0, 8, 0, 0)
            };

            root.Controls.Add(metaPanel, 0, 0);
            root.Controls.Add(controlsPanel, 0, 1);
            root.Controls.Add(_gridHost, 0, 2);
            root.Controls.Add(_statusLabel, 0, 3);
            Controls.Add(root);
            FormClosed += (s, e) => ClosePopouts();
            Shown += (s, e) => SizeToFitContent(root);
            UiTheme.ApplyToForm(this);

            SizeToFitContent(root);
        }

        private void SizeToFitContent(TableLayoutPanel root)
        {
            root.PerformLayout();
            int width = Math.Max(980, root.PreferredSize.Width + 8);
            int height = Math.Max(GridHeight + 360, root.PreferredSize.Height + 8);
            MinimumSize = new Size(900, height);
            ClientSize = new Size(width, height);
        }
    }
}
