using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class LogicGraphDialog
    {
        private SplitContainer _mainSplit;
        private Panel _searchPanel;
        private Label _searchLabel;
        private TextBox _searchTextBox;
        private ListBox _searchResultsList;
        private Label _statusLabel;
        private Label _detailLabel;

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            _searchPanel = new Panel();
            _searchLabel = new Label();
            _searchTextBox = new TextBox();
            _searchResultsList = new ListBox();
            _statusLabel = new Label();
            _detailLabel = new Label();
            _mainSplit = new SplitContainer();
            ((System.ComponentModel.ISupportInitialize)_mainSplit).BeginInit();
            _mainSplit.SuspendLayout();
            SuspendLayout();

            Text = "Logic Graph";
            ClientSize = new Size(1180, 720);
            MinimumSize = new Size(900, 520);
            StartPosition = FormStartPosition.CenterParent;

            _searchPanel.Dock = DockStyle.Top;
            _searchPanel.Height = 36;
            _searchPanel.Padding = new Padding(8, 6, 8, 4);

            _searchLabel.AutoSize = true;
            _searchLabel.Location = new Point(8, 9);
            _searchLabel.Text = "Search:";

            _searchTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            _searchTextBox.Location = new Point(68, 6);
            _searchTextBox.Size = new Size(1096, 23);
            _searchTextBox.TabIndex = 0;

            _searchResultsList.Dock = DockStyle.Bottom;
            _searchResultsList.Height = 0;
            _searchResultsList.IntegralHeight = false;
            _searchResultsList.TabIndex = 1;
            _searchResultsList.Visible = false;

            _searchPanel.Controls.Add(_searchResultsList);
            _searchPanel.Controls.Add(_searchTextBox);
            _searchPanel.Controls.Add(_searchLabel);

            _statusLabel.Dock = DockStyle.Top;
            _statusLabel.AutoSize = false;
            _statusLabel.Height = 40;
            _statusLabel.Padding = new Padding(12, 6, 12, 0);
            _statusLabel.TextAlign = ContentAlignment.TopLeft;

            _detailLabel.Dock = DockStyle.Bottom;
            _detailLabel.AutoSize = false;
            _detailLabel.Height = 108;
            _detailLabel.Padding = new Padding(12, 8, 12, 8);
            _detailLabel.TextAlign = ContentAlignment.TopLeft;

            _mainSplit.Dock = DockStyle.Fill;
            _mainSplit.Orientation = Orientation.Vertical;
            _mainSplit.FixedPanel = FixedPanel.Panel2;

            Controls.Add(_mainSplit);
            Controls.Add(_detailLabel);
            Controls.Add(_statusLabel);
            Controls.Add(_searchPanel);

            _mainSplit.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)_mainSplit).EndInit();
            ResumeLayout(false);
        }

        #endregion
    }
}
