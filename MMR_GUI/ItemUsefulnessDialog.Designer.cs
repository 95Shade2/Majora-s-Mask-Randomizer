using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class ItemUsefulnessDialog
    {
        private void InitializeComponent()
        {
            Text = "Item Usefulness";
            ClientSize = new Size(520, 640);
            MinimumSize = new Size(420, 360);
            StartPosition = FormStartPosition.CenterParent;
            Font = UiTheme.Current.BaseFont;

            _statusLabel = new Label
            {
                Dock = DockStyle.Top,
                AutoSize = false,
                Height = 36,
                Padding = new Padding(12, 10, 12, 0),
                TextAlign = ContentAlignment.MiddleLeft
            };

            _grid = new DataGridView
            {
                Dock = DockStyle.Fill,
                ReadOnly = true,
                AllowUserToAddRows = false,
                AllowUserToDeleteRows = false,
                AllowUserToResizeRows = false,
                RowHeadersVisible = false,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect,
                AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill,
                MultiSelect = false,
                EnableHeadersVisualStyles = false
            };

            DataGridViewTextBoxColumn nameColumn = new DataGridViewTextBoxColumn
            {
                Name = "ItemName",
                HeaderText = "Item Name",
                DataPropertyName = "ItemName",
                FillWeight = 180
            };
            DataGridViewTextBoxColumn valueColumn = new DataGridViewTextBoxColumn
            {
                Name = "Usefulness",
                HeaderText = "Usefulness",
                DataPropertyName = "Usefulness",
                FillWeight = 80
            };

            _grid.Columns.Add(nameColumn);
            _grid.Columns.Add(valueColumn);
            _grid.DataSource = _rows;
            _grid.ColumnHeaderMouseClick += Grid_ColumnHeaderMouseClick;

            Controls.Add(_grid);
            Controls.Add(_statusLabel);

            Load += ItemUsefulnessDialog_Load;
            FormClosing += ItemUsefulnessDialog_FormClosing;
        }
    }
}
