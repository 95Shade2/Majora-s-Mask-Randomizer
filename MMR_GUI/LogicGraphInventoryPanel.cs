using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    [ToolboxItem(false)]
    public sealed class LogicGraphInventoryPanel : Panel
    {
        private readonly FlowLayoutPanel _flow;
        private readonly Label _headerLabel;
        private readonly HashSet<string> _collapsedCategories = new HashSet<string>();
        private readonly Dictionary<string, Label> _itemRows =
            new Dictionary<string, Label>(StringComparer.Ordinal);
        private readonly Dictionary<string, Label> _categoryHeaders =
            new Dictionary<string, Label>(StringComparer.Ordinal);
        private readonly Dictionary<string, List<string>> _categoryItemNames =
            new Dictionary<string, List<string>>(StringComparer.Ordinal);

        private Dictionary<string, object> _tabData;
        private HashSet<string> _lastAcquired;
        private Dictionary<string, int> _lastGrantCounts;
        private bool _isBuilt;

        public LogicGraphInventoryPanel()
        {
            DoubleBuffered = true;
            BackColor = IsDesignEnvironment()
                ? SystemColors.Control
                : UiTheme.Current.PanelBackColor;
            Padding = new Padding(8, 4, 8, 8);

            _headerLabel = new Label
            {
                Dock = DockStyle.Top,
                Height = 24,
                Text = "Tracked Inventory",
                TextAlign = ContentAlignment.MiddleLeft
            };

            _flow = new FlowLayoutPanel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                FlowDirection = FlowDirection.TopDown,
                WrapContents = false,
                Padding = new Padding(0, 4, 0, 0)
            };

            Controls.Add(_flow);
            Controls.Add(_headerLabel);
            Resize += InventoryPanel_Resize;

            if (IsDesignEnvironment())
            {
                _flow.Controls.Add(CreateHintLabel("(Inventory populates at runtime)"));
            }
            else
            {
                LoadTabData();
            }
        }

        private static bool IsDesignEnvironment()
        {
            return LicenseManager.UsageMode == LicenseUsageMode.Designtime;
        }

        public void ResetInventory()
        {
            _flow.Controls.Clear();
            _itemRows.Clear();
            _categoryHeaders.Clear();
            _categoryItemNames.Clear();
            _isBuilt = false;
            _lastAcquired = null;
            _lastGrantCounts = null;
        }

        public void SyncInventory(
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts)
        {
            _lastAcquired = acquiredItems ?? new HashSet<string>();
            _lastGrantCounts = grantCounts ?? new Dictionary<string, int>();

            if (!_isBuilt)
            {
                BuildInventoryList();
            }

            foreach (string itemName in _itemRows.Keys)
            {
                UpdateItemRow(itemName);
            }
        }

        public void UpdateForPlacement(
            string placement,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts)
        {
            if (placement == "")
            {
                SyncInventory(acquiredItems, grantCounts);
                return;
            }

            _lastAcquired = acquiredItems ?? new HashSet<string>();
            _lastGrantCounts = grantCounts ?? new Dictionary<string, int>();

            if (!_isBuilt)
            {
                SyncInventory(acquiredItems, grantCounts);
                return;
            }

            string baseName = ItemGrantCounts.GetBaseItemName(placement);
            foreach (string itemName in _itemRows.Keys)
            {
                if (ItemGrantCounts.ItemsMatch(itemName, placement)
                    || ItemGrantCounts.ItemsMatch(itemName, baseName))
                {
                    UpdateItemRow(itemName);
                }
            }
        }

        private void InventoryPanel_Resize(object sender, EventArgs e)
        {
            if (!_isBuilt)
            {
                return;
            }

            int contentWidth = Math.Max(160, _flow.ClientSize.Width - 4);
            foreach (Label header in _categoryHeaders.Values)
            {
                header.Width = contentWidth;
            }

            foreach (Label row in _itemRows.Values)
            {
                row.Width = contentWidth;
            }
        }

        private void BuildInventoryList()
        {
            _flow.SuspendLayout();
            _flow.Controls.Clear();
            _itemRows.Clear();
            _categoryHeaders.Clear();
            _categoryItemNames.Clear();

            if (_tabData == null)
            {
                _flow.Controls.Add(CreateHintLabel("item_tabs.json not found."));
                _flow.ResumeLayout(true);
                _isBuilt = false;
                return;
            }

            int contentWidth = Math.Max(160, _flow.ClientSize.Width - 4);

            foreach (KeyValuePair<string, object> category in _tabData.OrderBy(entry => entry.Key))
            {
                string categoryName = category.Key;
                bool collapsed = _collapsedCategories.Contains(categoryName);
                List<string> itemNames = EnumerateCategoryItems(category.Value).ToList();
                _categoryItemNames[categoryName] = itemNames;

                Label header = CreateCategoryHeader(categoryName, collapsed, contentWidth);
                _categoryHeaders[categoryName] = header;
                _flow.Controls.Add(header);

                foreach (string itemName in itemNames)
                {
                    Label row = CreateItemRow(itemName, contentWidth);
                    row.Visible = !collapsed;
                    _itemRows[itemName] = row;
                    _flow.Controls.Add(row);
                }
            }

            _flow.ResumeLayout(true);
            _isBuilt = true;
        }

        private void ToggleCategory(string categoryName)
        {
            if (_collapsedCategories.Contains(categoryName))
            {
                _collapsedCategories.Remove(categoryName);
            }
            else
            {
                _collapsedCategories.Add(categoryName);
            }

            bool collapsed = _collapsedCategories.Contains(categoryName);
            Label header;
            if (_categoryHeaders.TryGetValue(categoryName, out header))
            {
                header.Text = (collapsed ? "\u25B6 " : "\u25BC ") + categoryName;
            }

            List<string> itemNames;
            if (_categoryItemNames.TryGetValue(categoryName, out itemNames))
            {
                foreach (string itemName in itemNames)
                {
                    Label row;
                    if (_itemRows.TryGetValue(itemName, out row))
                    {
                        row.Visible = !collapsed;
                    }
                }
            }
        }

        private void UpdateItemRow(string itemName)
        {
            Label row;
            if (! _itemRows.TryGetValue(itemName, out row))
            {
                return;
            }

            int owned = LogicUsefulnessResult.GetOwnedCount(
                itemName,
                _lastAcquired,
                _lastGrantCounts);
            bool hasItem = owned > 0;
            string countSuffix = owned > 1 ? "  x" + owned : "";
            Color ownedColor = UiTheme.Current.TabSelectedAccentColor;

            row.Text = (hasItem ? "\u2713 " : "\u2013 ") + itemName + countSuffix;
            row.ForeColor = hasItem ? ownedColor : UiTheme.Current.HintForeColor;
        }

        private static IEnumerable<string> EnumerateCategoryItems(object categoryValue)
        {
            if (categoryValue == null)
            {
                yield break;
            }

            if (categoryValue is ArrayList flatList)
            {
                foreach (string name in ToStringArray(flatList))
                {
                    if (name != "")
                    {
                        yield return name;
                    }
                }

                yield break;
            }

            if (categoryValue is Dictionary<string, object> pages)
            {
                foreach (string pageKey in pages.Keys.OrderBy(key => key))
                {
                    foreach (string name in ToStringArray(pages[pageKey] as ArrayList))
                    {
                        if (name != "")
                        {
                            yield return name;
                        }
                    }
                }
            }
        }

        private static string[] ToStringArray(ArrayList list)
        {
            if (list == null || list.Count == 0)
            {
                return new string[0];
            }

            string[] items = new string[list.Count];
            for (int i = 0; i < list.Count; i++)
            {
                items[i] = list[i] != null ? list[i].ToString() : "";
            }

            return items;
        }

        private Label CreateCategoryHeader(string categoryName, bool collapsed, int contentWidth)
        {
            Label header = new Label
            {
                AutoSize = false,
                Width = contentWidth,
                Height = 22,
                Margin = new Padding(0, 8, 0, 2),
                Text = (collapsed ? "\u25B6 " : "\u25BC ") + categoryName,
                Font = new Font(UiTheme.Current.BaseFont, FontStyle.Bold),
                ForeColor = UiTheme.Current.ForeColor,
                Cursor = Cursors.Hand
            };

            header.Click += delegate
            {
                ToggleCategory(categoryName);
            };

            return header;
        }

        private Label CreateItemRow(string itemName, int contentWidth)
        {
            return new Label
            {
                AutoSize = false,
                Width = contentWidth,
                Height = 18,
                Margin = new Padding(4, 0, 0, 0),
                Text = "\u2013 " + itemName,
                ForeColor = UiTheme.Current.HintForeColor
            };
        }

        private static Label CreateHintLabel(string text)
        {
            return new Label
            {
                AutoSize = true,
                MaximumSize = new Size(200, 0),
                Text = text,
                ForeColor = UiTheme.Current.HintForeColor
            };
        }

        private void LoadTabData()
        {
            string path = FindTabDataFile();
            if (path == null)
            {
                _tabData = null;
                return;
            }

            try
            {
                JavaScriptSerializer serializer = new JavaScriptSerializer();
                _tabData = serializer.Deserialize<Dictionary<string, object>>(
                    File.ReadAllText(path, Encoding.UTF8));
            }
            catch
            {
                _tabData = null;
            }
        }

        private static string FindTabDataFile()
        {
            string[] candidates =
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "item_tabs.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "item_tabs.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "item_tabs.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "MMR_GUI", "item_tabs.json")
            };

            foreach (string candidate in candidates)
            {
                string full = Path.GetFullPath(candidate);
                if (File.Exists(full))
                {
                    return full;
                }
            }

            return null;
        }
    }
}
