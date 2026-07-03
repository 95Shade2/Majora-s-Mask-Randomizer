using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class ItemTabBuilder
    {
        public const int ExpectedItemCount = 106;
        private const int RowHeight = 30;
        private const int ItemTableWidth = 680;

        public static Dictionary<string, Item> Build(
            TabControl itemsTab,
            EventHandler checkChanged,
            EventHandler poolChanged,
            EventHandler givesChanged)
        {
            string jsonPath = ResolveJsonPath();
            string json = File.ReadAllText(jsonPath, Encoding.UTF8);
            JavaScriptSerializer serializer = new JavaScriptSerializer();
            Dictionary<string, object> root = serializer.Deserialize<Dictionary<string, object>>(json);

            int declaredCount = CountItems(root);
            if (declaredCount != ExpectedItemCount)
            {
                throw new InvalidOperationException(
                    $"item_tabs.json defines {declaredCount} items; expected {ExpectedItemCount}.");
            }

            itemsTab.SuspendLayout();
            itemsTab.TabPages.Clear();

            Dictionary<string, Item> items = new Dictionary<string, Item>();
            int controlCount = 0;

            try
            {
                foreach (KeyValuePair<string, object> category in root)
                {
                    TabPage categoryPage = new TabPage(category.Key);
                    UiTheme.EnableDoubleBuffer(categoryPage);
                    itemsTab.TabPages.Add(categoryPage);

                    if (category.Value is ArrayList flatList)
                    {
                        string[] flatItems = ToStringArray(flatList);
                        Control content = CreateItemContent(flatItems, checkChanged, poolChanged, givesChanged, items, ref controlCount);
                        content.Dock = DockStyle.Fill;
                        categoryPage.Controls.Add(content);
                    }
                    else if (category.Value is Dictionary<string, object> pages)
                    {
                        Control content = CreateGroupedItemContent(category.Key, pages, checkChanged, poolChanged, givesChanged, items, ref controlCount);
                        content.Dock = DockStyle.Fill;
                        categoryPage.Controls.Add(content);
                    }
                }
            }
            finally
            {
                itemsTab.ResumeLayout(false);
            }

            if (items.Count != ExpectedItemCount)
            {
                throw new InvalidOperationException(
                    $"ItemTabBuilder expected {ExpectedItemCount} items but built {items.Count}.");
            }

            if (controlCount != ExpectedItemCount * 3)
            {
                throw new InvalidOperationException(
                    $"ItemTabBuilder expected {ExpectedItemCount * 3} controls but built {controlCount}.");
            }

            TabCategoryIcons.ConfigureCategoryTabs(itemsTab);

            return items;
        }

        public static void ValidateItemNames(string[] itemNames, Dictionary<string, Item> items)
        {
            if (itemNames == null)
            {
                throw new ArgumentNullException(nameof(itemNames));
            }

            if (itemNames.Length != ExpectedItemCount)
            {
                throw new InvalidOperationException(
                    $"Create_Item_Names defines {itemNames.Length} items; expected {ExpectedItemCount}.");
            }

            HashSet<string> expected = new HashSet<string>(itemNames);
            HashSet<string> built = new HashSet<string>(items.Keys);

            foreach (string missing in expected.Except(built))
            {
                throw new InvalidOperationException(
                    $"ItemTabBuilder did not create UI for item '{missing}'.");
            }

            foreach (string extra in built.Except(expected))
            {
                throw new InvalidOperationException(
                    $"item_tabs.json contains unknown item '{extra}'.");
            }
        }

        public static void ScheduleHandleWarmup(TabControl itemsTab)
        {
            if (itemsTab == null || itemsTab.IsDisposed)
            {
                return;
            }

            itemsTab.BeginInvoke(new Action(() => WarmNextTabPage(itemsTab, 0)));
        }

        private static void WarmNextTabPage(TabControl itemsTab, int pageIndex)
        {
            if (itemsTab == null || itemsTab.IsDisposed || pageIndex >= itemsTab.TabPages.Count)
            {
                return;
            }

            WarmControlHandles(itemsTab.TabPages[pageIndex]);
            itemsTab.BeginInvoke(new Action(() => WarmNextTabPage(itemsTab, pageIndex + 1)));
        }

        private static void WarmControlHandles(Control root)
        {
            if (root == null || root.IsDisposed)
            {
                return;
            }

            IntPtr handle = root.Handle;
            foreach (Control child in root.Controls)
            {
                WarmControlHandles(child);
            }
        }

        private static string ResolveJsonPath()
        {
            string[] candidates = new[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "item_tabs.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "item_tabs.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "item_tabs.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "MMR_GUI", "item_tabs.json")
            };

            foreach (string candidate in candidates)
            {
                if (File.Exists(candidate))
                {
                    return candidate;
                }
            }

            throw new FileNotFoundException("item_tabs.json not found.");
        }

        private static int CountItems(Dictionary<string, object> root)
        {
            int count = 0;
            foreach (object value in root.Values)
            {
                if (value is ArrayList flat)
                {
                    count += flat.Count;
                }
                else if (value is Dictionary<string, object> pages)
                {
                    foreach (object page in pages.Values)
                    {
                        count += ((ArrayList)page).Count;
                    }
                }
            }

            return count;
        }

        private static string[] ToStringArray(ArrayList list)
        {
            return list.Cast<object>().Select(o => o.ToString()).ToArray();
        }

        private static Control CreateGroupedItemContent(
            string categoryName,
            Dictionary<string, object> pages,
            EventHandler checkChanged,
            EventHandler poolChanged,
            EventHandler givesChanged,
            Dictionary<string, Item> items,
            ref int controlCount)
        {
            List<KeyValuePair<string, string[]>> groups = new List<KeyValuePair<string, string[]>>();
            foreach (KeyValuePair<string, object> pageEntry in pages.OrderBy(p => p.Key))
            {
                groups.Add(new KeyValuePair<string, string[]>(
                    GetGroupTitle(categoryName, pageEntry.Key),
                    ToStringArray(pageEntry.Value as ArrayList)));
            }

            return CreateItemContent(groups, checkChanged, poolChanged, givesChanged, items, ref controlCount);
        }

        private static Control CreateItemContent(
            string[] itemNames,
            EventHandler checkChanged,
            EventHandler poolChanged,
            EventHandler givesChanged,
            Dictionary<string, Item> items,
            ref int controlCount)
        {
            List<KeyValuePair<string, string[]>> groups = new List<KeyValuePair<string, string[]>>
            {
                new KeyValuePair<string, string[]>(string.Empty, itemNames)
            };

            return CreateItemContent(groups, checkChanged, poolChanged, givesChanged, items, ref controlCount);
        }

        private static Control CreateItemContent(
            IList<KeyValuePair<string, string[]>> groups,
            EventHandler checkChanged,
            EventHandler poolChanged,
            EventHandler givesChanged,
            Dictionary<string, Item> items,
            ref int controlCount)
        {
            Panel scroll = new Panel { Dock = DockStyle.Fill, AutoScroll = true, Padding = new Padding(4) };
            UiTheme.EnableDoubleBuffer(scroll);

            TableLayoutPanel stack = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 1,
                RowCount = groups.Count,
                Padding = Padding.Empty
            };
            stack.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));

            int row = 0;
            foreach (KeyValuePair<string, string[]> group in groups)
            {
                Control section = CreateItemSection(
                    group.Key,
                    group.Value,
                    checkChanged,
                    poolChanged,
                    givesChanged,
                    items,
                    ref controlCount);
                section.Margin = new Padding(0, 0, 0, 10);
                stack.RowStyles.Add(new RowStyle(SizeType.AutoSize));
                stack.Controls.Add(section, 0, row);
                row++;
            }

            scroll.Controls.Add(stack);
            return scroll;
        }

        private static Control CreateItemSection(
            string title,
            string[] itemNames,
            EventHandler checkChanged,
            EventHandler poolChanged,
            EventHandler givesChanged,
            Dictionary<string, Item> items,
            ref int controlCount)
        {
            TableLayoutPanel table = CreateItemTable(itemNames, checkChanged, poolChanged, givesChanged, items, ref controlCount);
            if (string.IsNullOrEmpty(title))
            {
                return table;
            }

            GroupBox group = new GroupBox
            {
                Text = title,
                Width = ItemTableWidth + 22,
                Height = table.Height + 34,
                Padding = new Padding(10, 18, 10, 10),
                AutoSize = false
            };
            table.Location = new Point(10, 22);
            group.Controls.Add(table);
            return group;
        }

        private static TableLayoutPanel CreateItemTable(
            string[] itemNames,
            EventHandler checkChanged,
            EventHandler poolChanged,
            EventHandler givesChanged,
            Dictionary<string, Item> items,
            ref int controlCount)
        {
            TableLayoutPanel table = new TableLayoutPanel
            {
                Location = Point.Empty,
                AutoSize = false,
                ColumnCount = 3,
                RowCount = itemNames.Length + 1,
                Width = ItemTableWidth
            };

            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 35F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 40F));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute, RowHeight));

            AddHeader(table, "Manual", 0);
            AddHeader(table, "Pool", 1);
            AddHeader(table, "Item", 2);

            int row = 1;
            foreach (string displayName in itemNames)
            {
                string controlName = TextToCheckbox(displayName);

                CheckBox check = new CheckBox
                {
                    Name = controlName,
                    Text = displayName,
                    AutoSize = false,
                    Dock = DockStyle.Fill,
                    Margin = new Padding(0, 2, 0, 0)
                };
                check.CheckedChanged += checkChanged;

                ComboBox pool = new ComboBox
                {
                    Name = controlName + "_Pool",
                    Enabled = false,
                    Dock = DockStyle.Fill
                };
                pool.SelectedIndexChanged += poolChanged;

                ComboBox gives = new ComboBox
                {
                    Name = controlName + "_Gives",
                    Enabled = false,
                    Dock = DockStyle.Fill
                };
                gives.SelectedIndexChanged += givesChanged;

                table.Controls.Add(check, 0, row);
                table.Controls.Add(pool, 1, row);
                table.Controls.Add(gives, 2, row);
                table.RowStyles.Add(new RowStyle(SizeType.Absolute, RowHeight));

                items.Add(displayName, new Item(check, pool, gives));
                controlCount += 3;
                row++;
            }

            table.Height = (itemNames.Length + 1) * RowHeight + 4;
            return table;
        }

        private static void AddHeader(TableLayoutPanel table, string text, int column)
        {
            Label label = new Label
            {
                Text = text,
                AutoSize = true,
                Font = new System.Drawing.Font(UiTheme.Current.BaseFont, System.Drawing.FontStyle.Bold),
                Margin = new Padding(2, 2, 2, 6)
            };
            table.Controls.Add(label, column, 0);
        }

        private static string TextToCheckbox(string text)
        {
            text = text.Replace(" ", "_");
            text = text.Replace("-", "_");
            text = text.Replace("'", "");
            text = text.Replace("(", "");
            text = text.Replace(")", "");
            return text;
        }

        private static string GetGroupTitle(string categoryName, string pageName)
        {
            Dictionary<string, string> categoryTitles;
            if (GroupTitles.TryGetValue(categoryName, out categoryTitles))
            {
                string title;
                if (categoryTitles.TryGetValue(pageName, out title))
                {
                    return title;
                }
            }

            return pageName;
        }

        private static readonly Dictionary<string, Dictionary<string, string>> GroupTitles =
            new Dictionary<string, Dictionary<string, string>>
            {
                {
                    "Items",
                    new Dictionary<string, string>
                    {
                        { "Page 1", "Explosives, Wallets & Basics" },
                        { "Page 2", "Weapons & Upgrades" },
                        { "Page 3", "Quest Items & Deeds" },
                        { "Page 4", "Trade Items & Deeds" }
                    }
                },
                {
                    "Masks",
                    new Dictionary<string, string>
                    {
                        { "Page 1", "Transformation & Early Masks" },
                        { "Page 2", "Utility Masks" },
                        { "Page 3", "Remaining Masks" }
                    }
                },
                {
                    "Bottles",
                    new Dictionary<string, string>
                    {
                        { "Page 1", "Bottle Contents" },
                        { "Page 2", "More Bottle Contents" }
                    }
                }
            };
    }
}
