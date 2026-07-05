using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class LogicGraphDialog : Form
    {
        public bool showing;
        public Main_Window form;

        private GraphCanvas _canvas;
        private LogicGraphInventoryPanel _inventoryPanel;

        private LogicUsefulnessResult _logicData;
        private Dictionary<string, string> _placements;
        private bool _suppressSearchSelectionClear;
        private string _logicName = "";
        private string _defaultDetailText = "";
        private GraphNode _detailNode;

        public LogicGraphDialog()
        {
            InitializeComponent();
            SetupCustomControls();
            WireEvents();
        }

        private void SetupCustomControls()
        {
            _canvas = new GraphCanvas
            {
                Dock = DockStyle.Fill
            };

            _inventoryPanel = new LogicGraphInventoryPanel
            {
                Dock = DockStyle.Fill,
                MinimumSize = new Size(200, 0)
            };

            _mainSplit.Panel1.Controls.Add(_canvas);
            _mainSplit.Panel2.Controls.Add(_inventoryPanel);

            if (IsDesignEnvironment())
            {
                _statusLabel.Text =
                    "Logic graph — graph and inventory populate at runtime when opened from the main window.";
                _detailLabel.Text =
                    "Mark checks after collecting them to track inventory and unlock paths.";
            }
        }

        private void WireEvents()
        {
            _searchTextBox.TextChanged += SearchTextBox_TextChanged;
            _searchTextBox.KeyDown += SearchTextBox_KeyDown;
            _searchResultsList.KeyDown += SearchResultsList_KeyDown;
            _searchResultsList.MouseClick += SearchResultsList_MouseClick;
            _searchResultsList.MouseDoubleClick += SearchResultsList_MouseDoubleClick;
            _searchPanel.Resize += SearchPanel_Resize;

            _canvas.NodeSelected += Canvas_NodeSelected;
            _canvas.NodeHovered += Canvas_NodeHovered;
            _canvas.ProgressChanged += Canvas_ProgressChanged;
            _canvas.InventoryChanged += Canvas_InventoryChanged;

            Load += LogicGraphDialog_Load;
            FormClosing += LogicGraphDialog_FormClosing;
        }

        private void SearchPanel_Resize(object sender, EventArgs e)
        {
            _searchTextBox.Width = _searchPanel.ClientSize.Width - _searchTextBox.Left - 8;
        }

        internal void LoadGraphData(
            LogicUsefulnessResult logicData,
            Dictionary<string, string> placements,
            string logicName)
        {
            _logicData = logicData;
            _placements = placements ?? new Dictionary<string, string>();
            _logicName = logicName;
            _defaultDetailText =
                "Mark a check after collecting it — the placed item is added to tracked inventory. "
                + "Lines show which collected checks contributed to a node becoming reachable. "
                + "Search finds check names; item names only appear for checks you've already collected. "
                + "Hold Shift while hovering to highlight unlock chains.";
            _detailNode = null;
            _detailLabel.Text = _defaultDetailText;
            _searchTextBox.Text = "";
            HideSearchResults();
            _canvas.ResetGraph(_logicData, _placements, form?.Item_Names);
            _canvas.ClearSearchSelection();
            RefreshStatusBar();
        }

        private void SyncInventoryPanelFull()
        {
            if (_inventoryPanel == null)
            {
                return;
            }

            _inventoryPanel.ResetInventory();
            HashSet<string> acquired;
            Dictionary<string, int> grantCounts;
            _canvas.GetTrackedInventory(out acquired, out grantCounts);
            _inventoryPanel.SyncInventory(acquired, grantCounts);
        }

        private void UpdateInventoryForPlacement(string placement)
        {
            if (_inventoryPanel == null)
            {
                return;
            }

            HashSet<string> acquired;
            Dictionary<string, int> grantCounts;
            _canvas.GetTrackedInventory(out acquired, out grantCounts);
            _inventoryPanel.UpdateForPlacement(placement, acquired, grantCounts);
        }

        private void Canvas_InventoryChanged(object sender, GraphInventoryEventArgs e)
        {
            if (e == null || e.FullRefresh)
            {
                SyncInventoryPanelFull();
                return;
            }

            UpdateInventoryForPlacement(e.Placement);
        }

        private void RefreshInventoryPanel()
        {
            SyncInventoryPanelFull();
        }

        private void Canvas_ProgressChanged(object sender, EventArgs e)
        {
            RefreshStatusBar();
        }

        private void RefreshStatusBar()
        {
            int revealed;
            int total;
            _canvas.GetProgressCounts(out revealed, out total);

            string logicText = string.IsNullOrWhiteSpace(_logicName) ? "None" : _logicName;
            _statusLabel.Text = string.Format(
                "Logic: {0} — Collected: {1} / {2} visible — Mark after collecting a check; the placed item is added to tracked inventory. Click to mark; right-click to unmark; drag to pan.",
                logicText,
                revealed,
                total);
        }

        private void LogicGraphDialog_Load(object sender, EventArgs e)
        {
            if (IsDesignEnvironment())
            {
                return;
            }

            showing = true;
            UiTheme.ApplyToForm(this);
            _searchResultsList.ItemHeight = Math.Max(18, UiTheme.Current.BaseFont.Height + 2);
            ApplySplitLayout();
        }

        private void ApplySplitLayout()
        {
            if (_mainSplit == null || _mainSplit.Width <= 0)
            {
                return;
            }

            const int inventoryWidth = 240;
            _mainSplit.Panel1MinSize = 400;
            _mainSplit.Panel2MinSize = 200;

            int maxDistance = _mainSplit.Width - _mainSplit.Panel2MinSize;
            int minDistance = _mainSplit.Panel1MinSize;
            if (maxDistance < minDistance)
            {
                return;
            }

            int distance = _mainSplit.Width - inventoryWidth - _mainSplit.SplitterWidth;
            distance = Math.Max(minDistance, Math.Min(distance, maxDistance));
            _mainSplit.SplitterDistance = distance;
        }

        private void LogicGraphDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (IsDesignEnvironment())
            {
                return;
            }

            if (form != null)
            {
                form.logic_graph_dialog = new LogicGraphDialog();
            }

            showing = false;
        }

        private void Canvas_NodeSelected(object sender, GraphNodeEventArgs e)
        {
            _detailNode = e.Node;
            if (e.Node == null)
            {
                _detailLabel.Text = _defaultDetailText;
                return;
            }

            _detailLabel.Text = FormatNodeDetail(e.Node);
        }

        private void Canvas_NodeHovered(object sender, GraphNodeEventArgs e)
        {
            if (e.Node != null)
            {
                _detailLabel.Text = FormatNodeHoverDetail(e.Node);
                return;
            }

            if (_detailNode != null)
            {
                _detailLabel.Text = FormatNodeDetail(_detailNode);
            }
            else
            {
                _detailLabel.Text = _defaultDetailText;
            }
        }

        private string FormatNodeHoverDetail(GraphNode node)
        {
            string hoverText = _canvas.GetHoverDetailText(node);
            StringBuilder detail = new StringBuilder();
            detail.Append(node.DisplayName);

            if (hoverText != "")
            {
                detail.AppendLine();
                detail.AppendLine();
                detail.Append(hoverText);
            }

            return detail.ToString();
        }

        private void SearchTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!_suppressSearchSelectionClear)
            {
                _canvas.ClearSearchSelection();
            }

            UpdateSearchResults();
        }

        private void SearchTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Down && _searchResultsList.Visible && _searchResultsList.Items.Count > 0)
            {
                _searchResultsList.Focus();
                _searchResultsList.SelectedIndex = 0;
                e.Handled = true;
                e.SuppressKeyPress = true;
                return;
            }

            if (e.KeyCode == Keys.Enter)
            {
                NavigateToSearchSelection(_searchTextBox.Text.Trim());
                e.Handled = true;
                e.SuppressKeyPress = true;
                return;
            }

            if (e.KeyCode == Keys.Escape)
            {
                _searchTextBox.Text = "";
                HideSearchResults();
                e.Handled = true;
                e.SuppressKeyPress = true;
            }
        }

        private void SearchResultsList_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                NavigateToSelectedSearchResult();
                e.Handled = true;
                e.SuppressKeyPress = true;
                return;
            }

            if (e.KeyCode == Keys.Escape)
            {
                HideSearchResults();
                _searchTextBox.Focus();
                e.Handled = true;
                e.SuppressKeyPress = true;
            }
        }

        private void SearchResultsList_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            NavigateToSelectedSearchResult();
        }

        private void SearchResultsList_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                NavigateToSelectedSearchResult();
            }
        }

        private void UpdateSearchResults()
        {
            string query = _searchTextBox.Text.Trim();
            List<string> matches = _canvas.FindMatchingNodes(query);

            _searchResultsList.BeginUpdate();
            _searchResultsList.Items.Clear();
            foreach (string match in matches)
            {
                _searchResultsList.Items.Add(match);
            }
            _searchResultsList.EndUpdate();

            if (matches.Count == 0 || query == "")
            {
                HideSearchResults();
                return;
            }

            const int maxRows = 8;
            int visibleRows = Math.Min(matches.Count, maxRows);
            int rowHeight = _searchResultsList.ItemHeight;
            _searchResultsList.Height = visibleRows * rowHeight + 4;
            _searchResultsList.Visible = true;
            _searchPanel.Height = 36 + _searchResultsList.Height;
        }

        private void HideSearchResults()
        {
            _searchResultsList.Visible = false;
            _searchPanel.Height = 36;
        }

        private void NavigateToSelectedSearchResult()
        {
            if (_searchResultsList.SelectedItem == null)
            {
                return;
            }

            NavigateToSearchSelection(_searchResultsList.SelectedItem.ToString());
        }

        private void NavigateToSearchSelection(string selection)
        {
            if (selection == "")
            {
                return;
            }

            List<string> matches = _canvas.FindMatchingNodes(selection);
            if (matches.Count == 0)
            {
                return;
            }

            string target = matches.FirstOrDefault(
                match => string.Equals(match, selection, StringComparison.OrdinalIgnoreCase))
                ?? matches[0];

            if (_canvas.FocusNode(target))
            {
                _suppressSearchSelectionClear = true;
                _searchTextBox.Text = target;
                _searchTextBox.SelectAll();
                _suppressSearchSelectionClear = false;

                HideSearchResults();
                _searchTextBox.Focus();
            }
        }

        private string FormatNodeDetail(GraphNode node)
        {
            if (!node.Revealed)
            {
                return node.DisplayName + " — click to reveal.";
            }

            if (node.Kind == GraphNodeKind.StartItem)
            {
                string placed;
                if (_placements != null && _placements.TryGetValue(node.DisplayName, out placed)
                    && placed != "")
                {
                    return node.DisplayName + " — starting check gives => " + placed;
                }

                return node.DisplayName + " — starting check (placement unknown).";
            }

            string slotPlaced;
            if (_placements != null && _placements.TryGetValue(node.DisplayName, out slotPlaced))
            {
                return node.DisplayName + " => " + slotPlaced;
            }

            return node.DisplayName + " — placement unknown.";
        }

        private static bool IsDesignEnvironment()
        {
            return LicenseManager.UsageMode == LicenseUsageMode.Designtime;
        }
    }
}
