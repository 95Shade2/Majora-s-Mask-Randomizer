using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class ItemUsefulnessDialog : Form
    {
        public bool showing;
        public Main_Window form;

        private readonly BindingList<UsefulnessRow> _rows = new BindingList<UsefulnessRow>();
        private DataGridView _grid;
        private Label _statusLabel;
        private string _nameSortDirection = "asc";
        private string _valueSortDirection = "desc";

        public ItemUsefulnessDialog()
        {
            InitializeComponent();
        }

        private void ItemUsefulnessDialog_Load(object sender, EventArgs e)
        {
            showing = true;
            UiTheme.ApplyToForm(this);
            ReloadData();
        }

        private void ItemUsefulnessDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (form != null)
            {
                form.item_usefulness_dialog = new ItemUsefulnessDialog();
            }

            showing = false;
        }

        internal void ReloadData()
        {
            _rows.Clear();

            if (form == null)
            {
                _statusLabel.Text = "Main window is unavailable.";
                return;
            }

            string logicName = form.GetSelectedLogicName();
            if (string.IsNullOrWhiteSpace(logicName))
            {
                _statusLabel.Text = "Select a logic file to view item usefulness.";
                return;
            }

            LogicUsefulnessResult result = LogicUsefulness.Compute(logicName, form.Item_Names);
            IEnumerable<KeyValuePair<string, int>> entries = result.UnlockCounts
                .Where(pair => form.Item_Names.Contains(pair.Key))
                .OrderByDescending(pair => pair.Value)
                .ThenBy(pair => pair.Key);

            foreach (KeyValuePair<string, int> entry in entries)
            {
                _rows.Add(new UsefulnessRow(entry.Key, entry.Value));
            }

            _statusLabel.Text = "Logic: " + logicName + " — " + _rows.Count + " items";
            ApplySort("Usefulness", "desc");
        }

        private void Grid_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.ColumnIndex < 0)
            {
                return;
            }

            string columnName = _grid.Columns[e.ColumnIndex].Name;
            if (columnName == "ItemName")
            {
                _nameSortDirection = _nameSortDirection == "asc" ? "desc" : "asc";
                ApplySort("ItemName", _nameSortDirection);
            }
            else if (columnName == "Usefulness")
            {
                _valueSortDirection = _valueSortDirection == "asc" ? "desc" : "asc";
                ApplySort("Usefulness", _valueSortDirection);
            }
        }

        private void ApplySort(string column, string direction)
        {
            List<UsefulnessRow> sorted = _rows.ToList();

            if (column == "ItemName")
            {
                sorted = direction == "asc"
                    ? sorted.OrderBy(row => row.ItemName).ToList()
                    : sorted.OrderByDescending(row => row.ItemName).ToList();
            }
            else
            {
                sorted = direction == "asc"
                    ? sorted.OrderBy(row => row.Usefulness).ThenBy(row => row.ItemName).ToList()
                    : sorted.OrderByDescending(row => row.Usefulness).ThenBy(row => row.ItemName).ToList();
            }

            _rows.Clear();
            foreach (UsefulnessRow row in sorted)
            {
                _rows.Add(row);
            }
        }

        private sealed class UsefulnessRow
        {
            public string ItemName { get; private set; }
            public int Usefulness { get; private set; }

            public UsefulnessRow(string itemName, int usefulness)
            {
                ItemName = itemName;
                Usefulness = usefulness;
            }
        }
    }
}
