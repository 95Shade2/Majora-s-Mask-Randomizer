using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class Main_Window
    {
        private SplitContainer _mainSplit;
        private const int SettingsLabelWidth = 132;
        private const int SettingsFieldWidth = 220;
        private const int SettingsButtonWidth = 80;
        private const int SettingsRenameFieldWidth = 120;
        private const int SettingsTargetingWidth = 150;
        private const int SettingsPresetsWidth = 260;
        private const int SettingsMiddleGroupHeight = 108;

        private void ApplyShellLayout()
        {
            SuspendLayout();

            MinimumSize = new Size(1100, 640);
            ClientSize = new Size(1240, 760);

            _mainSplit = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Vertical,
                SplitterWidth = 6
            };

            Items_Tab.Dock = DockStyle.Fill;
            AddSettingsTab();

            Panel leftPanel = new Panel { Dock = DockStyle.Fill };
            leftPanel.Controls.Add(Items_Tab);

            TableLayoutPanel rightPanel = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 2,
                Padding = new Padding(4, 0, 4, 4)
            };
            rightPanel.RowStyles.Add(new RowStyle(SizeType.Absolute, 52F));
            rightPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));

            Randomize_Button.Dock = DockStyle.Fill;
            Randomize_Button.Margin = new Padding(0, 4, 0, 4);
            Randomize_Button.Height = 44;
            UiTheme.StylePrimaryButton(Randomize_Button);

            GroupBox poolsGroup = new GroupBox
            {
                Text = "Item Pools",
                Dock = DockStyle.Fill,
                Padding = new Padding(8, 4, 8, 8)
            };
            Pool_Tabs.Dock = DockStyle.Fill;
            poolsGroup.Controls.Add(Pool_Tabs);

            rightPanel.Controls.Add(Randomize_Button, 0, 0);
            rightPanel.Controls.Add(poolsGroup, 0, 1);

            _mainSplit.Panel1.Controls.Add(leftPanel);
            _mainSplit.Panel2.Controls.Add(rightPanel);

            Controls.Clear();
            MainMenuStrip = menuStrip1;
            Controls.Add(_mainSplit);
            Controls.Add(menuStrip1);
            _mainSplit.Dock = DockStyle.Fill;
            menuStrip1.Dock = DockStyle.Top;
            settingsToolStripMenuItem.Visible = false;

            TabCategoryIcons.ConfigureCategoryTabs(Items_Tab);
            UiTheme.EnableDoubleBuffer(Items_Tab);
            Items_Tab.SelectedIndex = 0;

            ResumeLayout(true);

            UiTheme.ScheduleSplitLayout(this, _mainSplit, 920, 680, 280);
        }

        private void AddSettingsTab()
        {
            TabPage settingsPage = new TabPage("Settings");
            Panel settingsScroll = new Panel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                Padding = new Padding(16, 12, 16, 12)
            };

            TableLayoutPanel content = BuildSettingsPanel();
            content.Dock = DockStyle.Top;
            content.AutoSize = true;
            content.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            settingsScroll.Controls.Add(content);

            settingsPage.Controls.Add(settingsScroll);
            Items_Tab.TabPages.Insert(0, settingsPage);
        }

        private TableLayoutPanel BuildSettingsPanel()
        {
            TableLayoutPanel root = new TableLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 1,
                RowCount = 5,
                Padding = Padding.Empty
            };
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            root.Controls.Add(BuildPoolManagementGroup(), 0, 0);
            root.Controls.Add(BuildTargetingPresetsRow(), 0, 1);
            root.Controls.Add(BuildPlandoGroup(), 0, 2);
            root.Controls.Add(BuildRandomizerOptionsGroup(), 0, 3);
            root.Controls.Add(BuildPatchOptionsGroup(), 0, 4);

            return root;
        }

        private GroupBox BuildPoolManagementGroup()
        {
            GroupBox group = CreateConfigGroupBox("Pool Management");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 3,
                RowCount = 3,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsLabelWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsFieldWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsButtonWidth));

            for (int i = 0; i < 3; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            AddSettingsFieldRow(table, 0, "Create pool", Create_Pool_Textbox, Create_Pool_Button);
            AddSettingsFieldRow(table, 1, "Remove pool", Remove_Pool_Combobox, Remove_Pool_Button);

            Label changeLabel = CreateFieldLabel("Rename pool");
            FlowLayoutPanel renameFields = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 2, 0, 2)
            };
            Change_Pool_Name_Combobox.Width = SettingsRenameFieldWidth;
            Change_Pool_Name_Textbox.Width = SettingsRenameFieldWidth;
            Change_Pool_Name_Combobox.Margin = new Padding(0, 0, 6, 0);
            Change_Pool_Name_Textbox.Margin = new Padding(0, 0, 6, 0);
            PrepareReparentedControl(Change_Pool_Name_Combobox);
            PrepareReparentedControl(Change_Pool_Name_Textbox);
            PrepareReparentedControl(Change_Pool_Name_Button);
            StyleSettingsInput(Change_Pool_Name_Combobox);
            StyleSettingsInput(Change_Pool_Name_Textbox);
            Change_Pool_Name_Combobox.Width = SettingsRenameFieldWidth;
            Change_Pool_Name_Textbox.Width = SettingsRenameFieldWidth;
            StyleSettingsAction(Change_Pool_Name_Button);
            renameFields.Controls.Add(Change_Pool_Name_Combobox);
            renameFields.Controls.Add(Change_Pool_Name_Textbox);
            renameFields.Controls.Add(Change_Pool_Name_Button);
            table.Controls.Add(changeLabel, 0, 2);
            table.Controls.Add(renameFields, 1, 2);
            table.SetColumnSpan(renameFields, 2);

            group.Controls.Add(table);
            return group;
        }

        private GroupBox BuildPlandoGroup()
        {
            Plando_Group = CreateConfigGroupBox("Plando");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 3,
                RowCount = 7,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsLabelWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsFieldWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsButtonWidth));

            for (int i = 0; i < 7; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            AddSettingsFieldRow(table, 0, "Location", Plando_Location_Combobox, Plando_Add_Button);
            AddSettingsFieldRow(table, 1, "Item", Plando_Item_Combobox, Plando_Remove_Button);

            int plandoListWidth = SettingsLabelWidth + SettingsFieldWidth + SettingsButtonWidth;
            PrepareReparentedControl(Plando_List);
            Plando_List.Width = plandoListWidth;
            Plando_List.Margin = new Padding(0, 6, 0, 0);
            table.Controls.Add(Plando_List, 0, 2);
            table.SetColumnSpan(Plando_List, 3);

            WarnPoolBalance_Checkbox = new CheckBox
            {
                Text = "Warn before randomize when pool issues remain",
                AutoSize = true,
                Margin = new Padding(0, 8, 0, 4)
            };
            table.Controls.Add(WarnPoolBalance_Checkbox, 0, 3);
            table.SetColumnSpan(WarnPoolBalance_Checkbox, 3);

            Label issuesLabel = CreateFieldLabel("Pool issues");
            issuesLabel.Margin = new Padding(0, 4, 8, 4);
            table.Controls.Add(issuesLabel, 0, 4);

            Pool_Issues_List = new ListView
            {
                View = View.Details,
                FullRowSelect = true,
                MultiSelect = false,
                HideSelection = false,
                Height = 96,
                Width = plandoListWidth,
                HeaderStyle = ColumnHeaderStyle.Nonclickable,
                OwnerDraw = true
            };
            Pool_Issues_List.Columns.Add("Issue", plandoListWidth - 8);
            Pool_Issues_List.DrawColumnHeader += Pool_Issues_DrawColumnHeader;
            Pool_Issues_List.DrawItem += Pool_Issues_DrawItem;
            Pool_Issues_List.DrawSubItem += Pool_Issues_DrawSubItem;
            PrepareReparentedControl(Pool_Issues_List);
            Pool_Issues_List.Margin = new Padding(0, 0, 0, 4);
            table.Controls.Add(Pool_Issues_List, 0, 5);
            table.SetColumnSpan(Pool_Issues_List, 3);

            FlowLayoutPanel issueActions = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 0, 0, 0)
            };
            Pool_Issue_Fix_Button = new Button { Text = "Resolve" };
            Pool_Issue_Ignore_Button = new Button { Text = "Ignore" };
            StyleSettingsAction(Pool_Issue_Fix_Button);
            StyleSettingsAction(Pool_Issue_Ignore_Button);
            Pool_Issue_Fix_Button.Margin = new Padding(0, 0, 8, 0);
            issueActions.Controls.Add(Pool_Issue_Fix_Button);
            issueActions.Controls.Add(Pool_Issue_Ignore_Button);
            table.Controls.Add(issueActions, 0, 6);
            table.SetColumnSpan(issueActions, 3);

            Plando_Group.Controls.Add(table);
            return Plando_Group;
        }

        private GroupBox BuildPatchOptionsGroup()
        {
            GroupBox group = CreateConfigGroupBox("Patch Options");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 2,
                RowCount = 5,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 245F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 245F));

            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            AddPatchOption(table, 0, 0, "Create WAD", createWadToolStripMenuItem);
            AddPatchOption(table, 1, 0, "Swamp Scrub Sales Beans", swampScrubSalesBeansToolStripMenuItem);
            AddPatchOption(table, 0, 1, "Play as Kafei", playAsKafeiToolStripMenuItem);
            AddPatchOption(table, 1, 1, "GC HUD", gCHudToolStripMenuItem);
            AddPatchOption(table, 0, 2, "Respawn HPs", respawnHPsToolStripMenuItem);
            AddPatchOption(table, 1, 2, "Edible Mirror Shield", edibleMirrorShieldToolStripMenuItem);
            AddPatchOption(table, 0, 3, "Keep Razor Sword on SoT", keepRazorSwordOnSoTToolStripMenuItem);
            AddPatchOption(table, 1, 3, "Ocean Spider House Any Day", oceanSpiderHouseAnyDayToolStripMenuItem);
            AddPatchOption(table, 0, 4, "Respawn HCs", respawnHCsToolStripMenuItem);
            AddPatchOption(table, 1, 4, "Remove Scrub Salesmen after trading", removeScrubSalesmanAfterTradingToolStripMenuItem);

            group.Controls.Add(table);
            return group;
        }

        private CheckBox AddPatchOption(TableLayoutPanel table, int column, int row, string text, ToolStripMenuItem menuItem)
        {
            CheckBox checkBox = new CheckBox
            {
                Text = text,
                AutoSize = true,
                Checked = menuItem.Checked,
                Anchor = AnchorStyles.Left,
                Margin = new Padding(4, 4, 12, 4)
            };

            checkBox.CheckedChanged += (sender, args) =>
            {
                if (menuItem.Checked != checkBox.Checked)
                {
                    menuItem.Checked = checkBox.Checked;
                }
            };
            menuItem.CheckedChanged += (sender, args) =>
            {
                if (checkBox.Checked != menuItem.Checked)
                {
                    checkBox.Checked = menuItem.Checked;
                }
            };

            table.Controls.Add(checkBox, column, row);
            return checkBox;
        }

        private static void PrepareReparentedControl(Control control)
        {
            control.Dock = DockStyle.None;
            control.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            control.Location = Point.Empty;
        }

        private static void AddSettingsFieldRow(
            TableLayoutPanel table,
            int row,
            string labelText,
            Control field,
            Control action)
        {
            table.Controls.Add(CreateFieldLabel(labelText), 0, row);
            PrepareReparentedControl(field);
            StyleSettingsInput(field);
            field.Margin = new Padding(0, 2, 8, 2);
            table.Controls.Add(field, 1, row);
            PrepareReparentedControl(action);
            StyleSettingsAction(action);
            table.Controls.Add(action, 2, row);
        }

        private static void StyleSettingsAction(Control action)
        {
            action.Margin = new Padding(0, 2, 0, 2);
            action.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            if (action is Button button)
            {
                button.AutoSize = false;
                button.Width = SettingsButtonWidth;
            }
        }

        private static void StyleSettingsInput(Control control)
        {
            control.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            if (control is TextBox || control is ComboBox)
            {
                control.Width = SettingsFieldWidth;
            }
        }

        private Control BuildTargetingPresetsRow()
        {
            TableLayoutPanel row = new TableLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 2,
                RowCount = 1,
                Margin = new Padding(0, 8, 0, 0),
                Padding = Padding.Empty,
                Dock = DockStyle.Top
            };
            row.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsTargetingWidth + 24));
            row.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsPresetsWidth));
            row.RowStyles.Add(new RowStyle(SizeType.Absolute, SettingsMiddleGroupHeight));

            GroupBox targeting = CreateConfigGroupBox("Targeting", dockTop: false);
            ConfigureFixedSettingsGroup(targeting, SettingsTargetingWidth, SettingsMiddleGroupHeight);
            targeting.Margin = new Padding(0, 0, 24, 8);
            PrepareReparentedControl(Targeting_Switch);
            PrepareReparentedControl(Targeting_Hold);
            Targeting_Switch.AutoSize = true;
            Targeting_Hold.AutoSize = true;
            Targeting_Switch.Location = new Point(10, 24);
            Targeting_Hold.Location = new Point(10, 52);
            targeting.Controls.Add(Targeting_Switch);
            targeting.Controls.Add(Targeting_Hold);

            GroupBox presets = CreateConfigGroupBox("Presets", dockTop: false);
            ConfigureFixedSettingsGroup(presets, SettingsPresetsWidth, SettingsMiddleGroupHeight);
            presets.Margin = new Padding(0, 0, 0, 8);
            PrepareReparentedControl(Presets_Combobox);
            PrepareReparentedControl(Load_Preset_Button);
            PrepareReparentedControl(Save_Preset_Button);
            Presets_Combobox.Width = SettingsFieldWidth;
            Presets_Combobox.Location = new Point(10, 22);
            StyleSettingsAction(Load_Preset_Button);
            StyleSettingsAction(Save_Preset_Button);
            Load_Preset_Button.Text = "Load";
            Save_Preset_Button.Text = "Save";
            Load_Preset_Button.Location = new Point(10, 52);
            Save_Preset_Button.Location = new Point(100, 52);
            presets.Controls.Add(Presets_Combobox);
            presets.Controls.Add(Load_Preset_Button);
            presets.Controls.Add(Save_Preset_Button);

            row.Controls.Add(targeting, 0, 0);
            row.Controls.Add(presets, 1, 0);
            return row;
        }

        private GroupBox BuildRandomizerOptionsGroup()
        {
            GroupBox group = CreateConfigGroupBox("Randomizer Options");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 2,
                RowCount = 5,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsLabelWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            table.Controls.Add(CreateFieldLabel("Logic"), 0, 0);
            PrepareReparentedControl(Logic_Combobox);
            StyleSettingsInput(Logic_Combobox);
            Logic_Combobox.Margin = new Padding(0, 2, 0, 6);
            table.Controls.Add(Logic_Combobox, 1, 0);

            table.Controls.Add(CreateFieldLabel("Base ROM"), 0, 1);
            FlowLayoutPanel baseRomPanel = new FlowLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                FlowDirection = FlowDirection.TopDown,
                WrapContents = false,
                Margin = new Padding(0, 2, 0, 6)
            };
            PrepareReparentedControl(Open_Base_Rom_Button);
            PrepareReparentedControl(Base_Rom_Label);
            Open_Base_Rom_Button.AutoSize = true;
            Open_Base_Rom_Button.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            Open_Base_Rom_Button.Margin = new Padding(0, 0, 0, 4);
            Base_Rom_Label.AutoSize = true;
            Base_Rom_Label.MaximumSize = new Size(SettingsFieldWidth, 0);
            Base_Rom_Label.ForeColor = SystemColors.GrayText;
            Base_Rom_Label.Font = UiTheme.Current.HintFont;
            Base_Rom_Label.Margin = Padding.Empty;
            Base_Rom_Label.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            baseRomPanel.Controls.Add(Open_Base_Rom_Button);
            baseRomPanel.Controls.Add(Base_Rom_Label);
            table.Controls.Add(baseRomPanel, 1, 1);

            table.Controls.Add(CreateFieldLabel("Blast mask cooldown"), 0, 2);
            FlowLayoutPanel blastPanel = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 2, 0, 6)
            };
            PrepareReparentedControl(BlastMaskFrames_Num);
            PrepareReparentedControl(label6);
            PrepareReparentedControl(BlastMaskSeconds_Label);
            BlastMaskFrames_Num.Margin = new Padding(0, 0, 8, 0);
            label6.AutoSize = true;
            label6.Text = "frames";
            label6.Margin = new Padding(0, 4, 4, 0);
            BlastMaskSeconds_Label.AutoSize = true;
            BlastMaskSeconds_Label.Margin = new Padding(0, 4, 0, 0);
            blastPanel.Controls.Add(BlastMaskFrames_Num);
            blastPanel.Controls.Add(label6);
            blastPanel.Controls.Add(BlastMaskSeconds_Label);
            table.Controls.Add(blastPanel, 1, 2);

            table.Controls.Add(CreateFieldLabel("Seed"), 0, 3);
            PrepareReparentedControl(Seed_Textbox);
            StyleSettingsInput(Seed_Textbox);
            Seed_Textbox.Margin = new Padding(0, 2, 0, 2);
            table.Controls.Add(Seed_Textbox, 1, 3);

            PrepareReparentedControl(label20);
            label20.AutoSize = true;
            label20.Font = UiTheme.Current.HintFont;
            label20.ForeColor = SystemColors.GrayText;
            label20.MaximumSize = new Size(SettingsFieldWidth, 0);
            label20.Margin = new Padding(0, 0, 0, 2);
            table.Controls.Add(label20, 1, 4);

            group.Controls.Add(table);
            return group;
        }

        private static Label CreateFieldLabel(string text)
        {
            return new Label
            {
                Text = text,
                AutoSize = true,
                Anchor = AnchorStyles.Left,
                TextAlign = ContentAlignment.MiddleLeft,
                Font = UiTheme.Current.BaseFont,
                Margin = new Padding(0, 6, 8, 6)
            };
        }

        private static void ConfigureFixedSettingsGroup(GroupBox group, int width, int height)
        {
            group.AutoSize = false;
            group.AutoSizeMode = AutoSizeMode.GrowOnly;
            group.Dock = DockStyle.None;
            group.Size = new Size(width, height);
            group.Padding = Padding.Empty;
        }

        private static GroupBox CreateConfigGroupBox(string title, bool dockTop = true)
        {
            return new GroupBox
            {
                Text = title,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                Dock = dockTop ? DockStyle.Top : DockStyle.None,
                Padding = new Padding(10, 6, 12, 10),
                Margin = new Padding(0, 0, 0, 8)
            };
        }

        private static void Pool_Issues_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            e.DrawDefault = true;
        }

        private static void Pool_Issues_DrawItem(object sender, DrawListViewItemEventArgs e)
        {
            e.DrawDefault = false;
        }

        private static void Pool_Issues_DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
        {
            ListView list = e.Item.ListView;
            Color back = e.Item.Selected
                ? SystemColors.Highlight
                : (e.ItemIndex % 2 == 0 ? Color.White : Color.FromArgb(242, 245, 250));
            Color fore = e.Item.Selected
                ? SystemColors.HighlightText
                : Color.FromArgb(180, 40, 40);

            using (SolidBrush brush = new SolidBrush(back))
            {
                e.Graphics.FillRectangle(brush, e.Bounds);
            }

            TextRenderer.DrawText(
                e.Graphics,
                e.SubItem.Text,
                list.Font,
                e.Bounds,
                fore,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);
        }
    }
}
