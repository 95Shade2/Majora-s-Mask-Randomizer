using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class CutscenesSelector
    {
        private readonly List<CheckBox> _allCutsceneChecks = new List<CheckBox>();

        private static readonly string[] RegionOrder =
        {
            "Clock Town",
            "Termina",
            "Swamp",
            "Mountain",
            "Great Bay",
            "Ikana"
        };

        private void ApplyModernLayout()
        {
            SuspendLayout();
            _allCutsceneChecks.Clear();

            MinimumSize = new Size(1000, 560);
            ClientSize = new Size(1040, 720);
            StartPosition = FormStartPosition.CenterParent;
            AutoScaleMode = AutoScaleMode.Dpi;

            GroupBox[] groupBoxes = Controls.OfType<GroupBox>().ToArray();
            foreach (Control control in Controls.Cast<Control>().ToArray())
            {
                Controls.Remove(control);
            }

            TableLayoutPanel root = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 2,
                Padding = new Padding(12)
            };
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));

            FlowLayoutPanel globalBar = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 0, 0, 8)
            };
            Button globalCheckAll = CreateToggleButton("Check All");
            Button globalUncheckAll = CreateToggleButton("Uncheck All");
            globalCheckAll.Click += (sender, args) => SetChecks(_allCutsceneChecks, true);
            globalUncheckAll.Click += (sender, args) => SetChecks(_allCutsceneChecks, false);
            globalBar.Controls.Add(globalCheckAll);
            globalBar.Controls.Add(globalUncheckAll);

            Panel scrollPanel = new Panel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                Padding = new Padding(0, 0, 4, 0)
            };

            TableLayoutPanel regionGrid = new TableLayoutPanel
            {
                AutoSize = true,
                ColumnCount = 2,
                Dock = DockStyle.Top,
                Padding = new Padding(0)
            };
            regionGrid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            regionGrid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));

            List<GroupBox> orderedGroups = RegionOrder
                .Select(name => groupBoxes.FirstOrDefault(g => g.Text == name))
                .Where(g => g != null)
                .ToList();

            int gridRows = (orderedGroups.Count + 1) / 2;
            for (int i = 0; i < gridRows; i++)
            {
                regionGrid.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            for (int i = 0; i < orderedGroups.Count; i++)
            {
                GroupBox groupBox = orderedGroups[i];
                RebuildRegionGroup(groupBox);
                regionGrid.Controls.Add(groupBox, i % 2, i / 2);
            }

            scrollPanel.Controls.Add(regionGrid);
            root.Controls.Add(globalBar, 0, 0);
            root.Controls.Add(scrollPanel, 0, 1);
            Controls.Add(root);

            ResumeLayout(true);
            UiTheme.ApplyToForm(this);
        }

        private void RebuildRegionGroup(GroupBox groupBox)
        {
            CheckBox[] checks = groupBox.Controls.OfType<CheckBox>().OrderBy(c => c.Top).ToArray();
            groupBox.Controls.Clear();

            TableLayoutPanel container = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 2,
                AutoSize = true,
                Padding = new Padding(4)
            };
            container.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            container.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            FlowLayoutPanel groupBar = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 0, 0, 4)
            };
            List<CheckBox> groupChecks = new List<CheckBox>(checks);
            Button groupCheckAll = CreateToggleButton("Check All");
            Button groupUncheckAll = CreateToggleButton("Uncheck All");
            groupCheckAll.Click += (sender, args) => SetChecks(groupChecks, true);
            groupUncheckAll.Click += (sender, args) => SetChecks(groupChecks, false);
            groupBar.Controls.Add(groupCheckAll);
            groupBar.Controls.Add(groupUncheckAll);

            int columns = checks.Length > 8 ? 2 : 1;
            int rows = (checks.Length + columns - 1) / columns;
            TableLayoutPanel checkGrid = new TableLayoutPanel
            {
                AutoSize = true,
                ColumnCount = columns,
                RowCount = rows,
                Dock = DockStyle.Top
            };
            for (int c = 0; c < columns; c++)
            {
                checkGrid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F / columns));
            }

            for (int r = 0; r < rows; r++)
            {
                checkGrid.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            for (int i = 0; i < checks.Length; i++)
            {
                CheckBox check = checks[i];
                check.AutoSize = true;
                check.Margin = new Padding(2, 2, 8, 4);
                check.Anchor = AnchorStyles.Left | AnchorStyles.Top;
                _allCutsceneChecks.Add(check);
                checkGrid.Controls.Add(check, i % columns, i / columns);
            }

            container.Controls.Add(groupBar, 0, 0);
            container.Controls.Add(checkGrid, 0, 1);
            groupBox.Controls.Add(container);
            groupBox.AutoSize = true;
            groupBox.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            groupBox.Dock = DockStyle.Fill;
            groupBox.Margin = new Padding(4);
            groupBox.Padding = new Padding(8, 4, 8, 8);
            groupBox.MinimumSize = new Size(490, 0);
        }

        private static Button CreateToggleButton(string text)
        {
            return new Button
            {
                Text = text,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                Margin = new Padding(0, 0, 6, 0),
                MinimumSize = new Size(0, UiTheme.Current.ButtonMinHeight)
            };
        }

        private void SetChecks(IEnumerable<CheckBox> checks, bool value)
        {
            foreach (CheckBox check in checks)
            {
                check.Checked = value;
            }
        }
    }
}
