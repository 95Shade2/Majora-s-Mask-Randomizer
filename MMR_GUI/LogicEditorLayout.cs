using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class LogicEditor
    {
        private ToolTip _dayNightScheduleToolTip;

        private void ApplyModernLayout()
        {
            SuspendLayout();

            MinimumSize = new Size(900, 560);
            ClientSize = new Size(1000, 620);
            StartPosition = FormStartPosition.CenterParent;
            AutoScaleMode = AutoScaleMode.Dpi;

            Control[] controls = Controls.Cast<Control>().ToArray();
            foreach (Control control in controls)
            {
                Controls.Remove(control);
            }

            SplitContainer mainSplit = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Vertical
            };

            Panel leftPanel = new Panel { Dock = DockStyle.Fill, Padding = new Padding(8) };
            TableLayoutPanel leftTable = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                RowCount = 4,
                ColumnCount = 1
            };
            leftTable.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            leftTable.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            leftTable.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            leftTable.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            GroupBox filesGroup = new GroupBox { Text = "Logic Files", Dock = DockStyle.Fill };
            LogicFiles_ListBox.Dock = DockStyle.Fill;
            filesGroup.Controls.Add(LogicFiles_ListBox);
            leftTable.Controls.Add(filesGroup, 0, 0);

            FlowLayoutPanel newLogicFlow = new FlowLayoutPanel { Dock = DockStyle.Fill, AutoSize = true };
            newLogicFlow.Controls.Add(NewLogic_TextBox);
            newLogicFlow.Controls.Add(NewLogic_Button);
            leftTable.Controls.Add(newLogicFlow, 0, 1);

            FlowLayoutPanel saveFlow = new FlowLayoutPanel { Dock = DockStyle.Fill, AutoSize = true };
            saveFlow.Controls.Add(SaveLogic_Button);
            saveFlow.Controls.Add(CancelLogic_Button);
            leftTable.Controls.Add(saveFlow, 0, 2);

            leftPanel.Controls.Add(leftTable);
            mainSplit.Panel1.Controls.Add(leftPanel);

            SplitContainer rightSplit = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Horizontal
            };

            TableLayoutPanel topTable = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 3,
                RowCount = 2
            };
            topTable.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 32F));
            topTable.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 32F));
            topTable.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 36F));
            topTable.RowStyles.Add(new RowStyle(SizeType.Percent, 55F));
            topTable.RowStyles.Add(new RowStyle(SizeType.Percent, 45F));

            GroupBox itemsGroup = BuildListGroup("Items", Items_ListBox, label1);
            GroupBox groupsGroup = BuildItemGroupsPanel();
            GroupBox invalidGroup = BuildInvalidPanel();

            topTable.Controls.Add(itemsGroup, 0, 0);
            topTable.SetRowSpan(itemsGroup, 2);
            topTable.Controls.Add(groupsGroup, 1, 0);
            topTable.SetRowSpan(groupsGroup, 2);
            topTable.Controls.Add(invalidGroup, 2, 0);
            topTable.SetRowSpan(invalidGroup, 2);

            rightSplit.Panel1.Controls.Add(topTable);

            GroupBox schedulePanel = BuildDayNightSchedulePanel();
            schedulePanel.Dock = DockStyle.Fill;
            rightSplit.Panel2.Controls.Add(schedulePanel);

            mainSplit.Panel2.Controls.Add(rightSplit);
            Controls.Add(mainSplit);

            ResumeLayout(true);

            UiTheme.ScheduleSplitLayout(this, mainSplit, 220, 180, 500);
            UiTheme.ScheduleSplitLayout(this, rightSplit, 360, 220, 150);

            UiTheme.ApplyToForm(this);
        }

        private GroupBox BuildListGroup(string title, ListBox list, Label caption)
        {
            GroupBox group = new GroupBox { Text = title, Dock = DockStyle.Fill, Padding = new Padding(8) };
            TableLayoutPanel table = new TableLayoutPanel { Dock = DockStyle.Fill, RowCount = 2, ColumnCount = 1 };
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            caption.AutoSize = true;
            table.Controls.Add(caption, 0, 0);
            list.Dock = DockStyle.Fill;
            table.Controls.Add(list, 0, 1);
            group.Controls.Add(table);
            return group;
        }

        private GroupBox BuildItemGroupsPanel()
        {
            GroupBox group = new GroupBox { Text = "Item Groups / Needed Items", Dock = DockStyle.Fill, Padding = new Padding(8) };
            TableLayoutPanel table = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 1, RowCount = 4 };
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 35F));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 40F));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            ItemGroups_ListBox.Dock = DockStyle.Fill;
            table.Controls.Add(ItemGroups_ListBox, 0, 0);

            FlowLayoutPanel groupButtons = new FlowLayoutPanel { AutoSize = true, Dock = DockStyle.Fill };
            groupButtons.Controls.Add(NewItemGroup_Button);
            groupButtons.Controls.Add(DeleteItemGroup_Button);
            groupButtons.Controls.Add(Duplicate_ItemSet_Button);
            table.Controls.Add(groupButtons, 0, 1);

            ItemsNeeded_ListBox.Dock = DockStyle.Fill;
            table.Controls.Add(ItemsNeeded_ListBox, 0, 2);

            FlowLayoutPanel neededRow = new FlowLayoutPanel { AutoSize = true, Dock = DockStyle.Fill, WrapContents = true };
            neededRow.Controls.Add(EditItem_ComboBox);
            neededRow.Controls.Add(EditItem_Number);
            neededRow.Controls.Add(NewNeededItem_Button);
            neededRow.Controls.Add(SaveNeededItem_Button);
            neededRow.Controls.Add(RemoveNeededItem_Button);
            table.Controls.Add(neededRow, 0, 3);

            group.Controls.Add(table);
            return group;
        }

        private GroupBox BuildInvalidPanel()
        {
            GroupBox group = new GroupBox { Text = "Invalid Items", Dock = DockStyle.Fill, Padding = new Padding(8) };
            TableLayoutPanel table = new TableLayoutPanel { Dock = DockStyle.Fill, RowCount = 3, ColumnCount = 1 };
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 48F));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 52F));

            InvalidItems_ListBox.Dock = DockStyle.Fill;
            table.Controls.Add(InvalidItems_ListBox, 0, 0);

            FlowLayoutPanel invalidButtons = new FlowLayoutPanel
            {
                AutoSize = true,
                Dock = DockStyle.Fill,
                WrapContents = true,
                Margin = new Padding(0, 4, 0, 4)
            };
            invalidButtons.Controls.Add(InvalidItem_ComboBox);
            invalidButtons.Controls.Add(NewInvalidItem_Button);
            invalidButtons.Controls.Add(SaveInvalidItem_Button);
            invalidButtons.Controls.Add(RemoveInvalidItem_Button);
            table.Controls.Add(invalidButtons, 0, 1);

            label5.Visible = false;
            label9.Visible = false;
            DayNight_ItemNeeded_Label.Visible = false;
            DayNight_Item_Label.Visible = false;

            GroupBox commentGroup = new GroupBox
            {
                Text = "Comment",
                Dock = DockStyle.Fill,
                Padding = new Padding(6, 4, 6, 6)
            };
            Comment_TextBox.Dock = DockStyle.Fill;
            Comment_TextBox.Font = new Font("Consolas", 9.5F);
            commentGroup.Controls.Add(Comment_TextBox);
            table.Controls.Add(commentGroup, 0, 2);

            group.Controls.Add(table);
            return group;
        }

        private GroupBox BuildDayNightSchedulePanel()
        {
            _dayNightScheduleToolTip = new ToolTip
            {
                AutoPopDelay = 8000,
                InitialDelay = 400
            };

            GroupBox group = new GroupBox
            {
                Text = "Day-Night Availability",
                Dock = DockStyle.Fill,
                Padding = new Padding(6, 4, 6, 6)
            };

            TableLayoutPanel grid = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 8,
                RowCount = 3,
                CellBorderStyle = TableLayoutPanelCellBorderStyle.Single,
                Padding = new Padding(4, 8, 4, 4),
                Margin = Padding.Empty
            };

            grid.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 120F));
            for (int column = 0; column < 7; column++)
            {
                grid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F / 7F));
            }

            for (int row = 0; row < 3; row++)
            {
                grid.RowStyles.Add(new RowStyle(SizeType.Percent, 100F / 3F));
            }

            grid.Controls.Add(new Panel { Dock = DockStyle.Fill }, 0, 0);

            string[] shortHeaders = { "D1", "N1", "D2", "N2", "D3", "N3", "Moon" };
            string[] longHeaders = { "Day 1", "Night 1", "Day 2", "Night 2", "Day 3", "Night 3", "Moon" };
            for (int column = 0; column < shortHeaders.Length; column++)
            {
                grid.Controls.Add(CreateScheduleHeader(shortHeaders[column], longHeaders[column]), column + 1, 0);
            }

            AddScheduleRow(
                grid,
                1,
                "Usable in set",
                "When the item can be used in this set",
                new[]
                {
                    Day1_Needed_Checkbox, Night1_Needed_Checkbox, Day2_Needed_Checkbox,
                    Night2_Needed_Checkbox, Day3_Needed_Checkbox, Night3_Needed_Checkbox,
                    Moon_Needed_Checkbox
                });

            AddScheduleRow(
                grid,
                2,
                "Given at loc.",
                "When the item at this location is given",
                new[]
                {
                    Day1_ItemGiven_Checkbox, Night1_ItemGiven_Checkbox, Day2_ItemGiven_Checkbox,
                    Night2_ItemGiven_Checkbox, Day3_ItemGiven_Checkbox, Night3_ItemGiven_Checkbox,
                    Moon_ItemGiven_Checkbox
                });

            group.Controls.Add(grid);
            return group;
        }

        private Label CreateScheduleHeader(string text, string tooltip)
        {
            Label header = new Label
            {
                Text = text,
                Dock = DockStyle.Fill,
                TextAlign = ContentAlignment.MiddleCenter,
                Font = new Font(UiTheme.Current.BaseFont, FontStyle.Bold),
                Margin = Padding.Empty
            };
            _dayNightScheduleToolTip.SetToolTip(header, tooltip);
            return header;
        }

        private void AddScheduleRow(TableLayoutPanel grid, int row, string labelText, string tooltip, CheckBox[] boxes)
        {
            Label rowLabel = new Label
            {
                Text = labelText,
                Dock = DockStyle.Fill,
                TextAlign = ContentAlignment.MiddleLeft,
                Font = UiTheme.Current.BaseFont,
                Margin = new Padding(4, 0, 2, 0)
            };
            _dayNightScheduleToolTip.SetToolTip(rowLabel, tooltip);
            grid.Controls.Add(rowLabel, 0, row);

            for (int column = 0; column < boxes.Length; column++)
            {
                grid.Controls.Add(PlaceScheduleCheckBox(boxes[column]), column + 1, row);
            }
        }

        private static Panel PlaceScheduleCheckBox(CheckBox checkBox)
        {
            if (checkBox.Parent != null)
            {
                checkBox.Parent.Controls.Remove(checkBox);
            }

            checkBox.AutoSize = false;
            checkBox.Visible = true;
            checkBox.Text = string.Empty;
            checkBox.Size = new Size(22, 22);
            checkBox.MinimumSize = new Size(18, 18);
            checkBox.Margin = Padding.Empty;

            Panel cell = new Panel
            {
                Dock = DockStyle.Fill,
                Margin = Padding.Empty,
                Padding = Padding.Empty
            };
            cell.Controls.Add(checkBox);

            void CenterCheckBox()
            {
                checkBox.Left = Math.Max(0, (cell.ClientSize.Width - checkBox.Width) / 2);
                checkBox.Top = Math.Max(0, (cell.ClientSize.Height - checkBox.Height) / 2);
            }

            cell.Resize += (sender, args) => CenterCheckBox();
            cell.Layout += (sender, args) => CenterCheckBox();
            CenterCheckBox();
            return cell;
        }
    }
}
