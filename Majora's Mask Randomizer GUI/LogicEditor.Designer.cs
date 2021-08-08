namespace Majora_s_Mask_Randomizer_GUI
{
    partial class LogicEditor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.LogicFiles_ListBox = new System.Windows.Forms.ListBox();
            this.NewLogic_Button = new System.Windows.Forms.Button();
            this.NewLogic_TextBox = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.Items_ListBox = new System.Windows.Forms.ListBox();
            this.ItemsNeeded_ListBox = new System.Windows.Forms.ListBox();
            this.label8 = new System.Windows.Forms.Label();
            this.SaveNeededItem_Button = new System.Windows.Forms.Button();
            this.NewItemGroup_Button = new System.Windows.Forms.Button();
            this.ItemGroups_ListBox = new System.Windows.Forms.ListBox();
            this.EditItem_Number = new System.Windows.Forms.NumericUpDown();
            this.Comment_TextBox = new System.Windows.Forms.RichTextBox();
            this.EditItem_ComboBox = new System.Windows.Forms.ComboBox();
            this.RemoveNeededItem_Button = new System.Windows.Forms.Button();
            this.NewNeededItem_Button = new System.Windows.Forms.Button();
            this.DeleteItemGroup_Button = new System.Windows.Forms.Button();
            this.InvalidItems_ListBox = new System.Windows.Forms.ListBox();
            this.label5 = new System.Windows.Forms.Label();
            this.InvalidItem_ComboBox = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.SaveInvalidItem_Button = new System.Windows.Forms.Button();
            this.RemoveInvalidItem_Button = new System.Windows.Forms.Button();
            this.NewInvalidItem_Button = new System.Windows.Forms.Button();
            this.SaveLogic_Button = new System.Windows.Forms.Button();
            this.CancelLogic_Button = new System.Windows.Forms.Button();
            this.Duplicate_ItemSet_Button = new System.Windows.Forms.Button();
            this.DayNight_ItemNeeded_Label = new System.Windows.Forms.Label();
            this.DayNight_Item_Label = new System.Windows.Forms.Label();
            this.Day1_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Day2_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Day3_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Night1_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Night2_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Night3_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Night3_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            this.Night2_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            this.Night1_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            this.Day3_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            this.Day2_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            this.Day1_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            this.Moon_Needed_Checkbox = new System.Windows.Forms.CheckBox();
            this.Moon_ItemGiven_Checkbox = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.EditItem_Number)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(44, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Logic";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(176, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Items";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(320, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Item Sets";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(468, 9);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(62, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Items in Set";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(623, 95);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(93, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Item Set Comment";
            // 
            // LogicFiles_ListBox
            // 
            this.LogicFiles_ListBox.FormattingEnabled = true;
            this.LogicFiles_ListBox.Location = new System.Drawing.Point(12, 25);
            this.LogicFiles_ListBox.Name = "LogicFiles_ListBox";
            this.LogicFiles_ListBox.Size = new System.Drawing.Size(100, 160);
            this.LogicFiles_ListBox.TabIndex = 6;
            this.LogicFiles_ListBox.SelectedIndexChanged += new System.EventHandler(this.LogicFiles_ListBox_SelectedIndexChanged);
            // 
            // NewLogic_Button
            // 
            this.NewLogic_Button.Location = new System.Drawing.Point(16, 233);
            this.NewLogic_Button.Name = "NewLogic_Button";
            this.NewLogic_Button.Size = new System.Drawing.Size(75, 23);
            this.NewLogic_Button.TabIndex = 7;
            this.NewLogic_Button.Text = "Create";
            this.NewLogic_Button.UseVisualStyleBackColor = true;
            this.NewLogic_Button.Click += new System.EventHandler(this.NewLogic_Button_Click);
            // 
            // NewLogic_TextBox
            // 
            this.NewLogic_TextBox.Location = new System.Drawing.Point(7, 207);
            this.NewLogic_TextBox.Name = "NewLogic_TextBox";
            this.NewLogic_TextBox.Size = new System.Drawing.Size(100, 20);
            this.NewLogic_TextBox.TabIndex = 8;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(13, 191);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 13);
            this.label7.TabIndex = 9;
            this.label7.Text = "New Logic Name";
            // 
            // Items_ListBox
            // 
            this.Items_ListBox.FormattingEnabled = true;
            this.Items_ListBox.Location = new System.Drawing.Point(118, 25);
            this.Items_ListBox.Name = "Items_ListBox";
            this.Items_ListBox.Size = new System.Drawing.Size(148, 342);
            this.Items_ListBox.TabIndex = 10;
            this.Items_ListBox.SelectedIndexChanged += new System.EventHandler(this.Items_ListBox_SelectedIndexChanged);
            // 
            // ItemsNeeded_ListBox
            // 
            this.ItemsNeeded_ListBox.FormattingEnabled = true;
            this.ItemsNeeded_ListBox.Location = new System.Drawing.Point(434, 25);
            this.ItemsNeeded_ListBox.Name = "ItemsNeeded_ListBox";
            this.ItemsNeeded_ListBox.Size = new System.Drawing.Size(137, 134);
            this.ItemsNeeded_ListBox.TabIndex = 11;
            this.ItemsNeeded_ListBox.SelectedIndexChanged += new System.EventHandler(this.ItemsNeeded_ListBox_SelectedIndexChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(635, 9);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(48, 13);
            this.label8.TabIndex = 14;
            this.label8.Text = "Edit Item";
            // 
            // SaveNeededItem_Button
            // 
            this.SaveNeededItem_Button.Location = new System.Drawing.Point(584, 52);
            this.SaveNeededItem_Button.Name = "SaveNeededItem_Button";
            this.SaveNeededItem_Button.Size = new System.Drawing.Size(75, 23);
            this.SaveNeededItem_Button.TabIndex = 12;
            this.SaveNeededItem_Button.Text = "Save";
            this.SaveNeededItem_Button.UseVisualStyleBackColor = true;
            this.SaveNeededItem_Button.Click += new System.EventHandler(this.SaveNeededItem_Button_Click);
            // 
            // NewItemGroup_Button
            // 
            this.NewItemGroup_Button.Location = new System.Drawing.Point(272, 204);
            this.NewItemGroup_Button.Name = "NewItemGroup_Button";
            this.NewItemGroup_Button.Size = new System.Drawing.Size(75, 23);
            this.NewItemGroup_Button.TabIndex = 15;
            this.NewItemGroup_Button.Text = "New List";
            this.NewItemGroup_Button.UseVisualStyleBackColor = true;
            this.NewItemGroup_Button.Click += new System.EventHandler(this.NewItemGroup_Button_Click);
            // 
            // ItemGroups_ListBox
            // 
            this.ItemGroups_ListBox.FormattingEnabled = true;
            this.ItemGroups_ListBox.Location = new System.Drawing.Point(272, 25);
            this.ItemGroups_ListBox.Name = "ItemGroups_ListBox";
            this.ItemGroups_ListBox.Size = new System.Drawing.Size(156, 173);
            this.ItemGroups_ListBox.TabIndex = 16;
            this.ItemGroups_ListBox.SelectedIndexChanged += new System.EventHandler(this.ItemGroups_ListBox_SelectedIndexChanged);
            // 
            // EditItem_Number
            // 
            this.EditItem_Number.Location = new System.Drawing.Point(704, 25);
            this.EditItem_Number.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.EditItem_Number.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.EditItem_Number.Name = "EditItem_Number";
            this.EditItem_Number.Size = new System.Drawing.Size(52, 20);
            this.EditItem_Number.TabIndex = 17;
            this.EditItem_Number.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // Comment_TextBox
            // 
            this.Comment_TextBox.Location = new System.Drawing.Point(580, 111);
            this.Comment_TextBox.Name = "Comment_TextBox";
            this.Comment_TextBox.Size = new System.Drawing.Size(176, 74);
            this.Comment_TextBox.TabIndex = 18;
            this.Comment_TextBox.Text = "";
            this.Comment_TextBox.TextChanged += new System.EventHandler(this.Comment_TextBox_TextChanged);
            // 
            // EditItem_ComboBox
            // 
            this.EditItem_ComboBox.FormattingEnabled = true;
            this.EditItem_ComboBox.Location = new System.Drawing.Point(577, 25);
            this.EditItem_ComboBox.Name = "EditItem_ComboBox";
            this.EditItem_ComboBox.Size = new System.Drawing.Size(121, 21);
            this.EditItem_ComboBox.TabIndex = 19;
            // 
            // RemoveNeededItem_Button
            // 
            this.RemoveNeededItem_Button.Location = new System.Drawing.Point(679, 52);
            this.RemoveNeededItem_Button.Name = "RemoveNeededItem_Button";
            this.RemoveNeededItem_Button.Size = new System.Drawing.Size(75, 23);
            this.RemoveNeededItem_Button.TabIndex = 20;
            this.RemoveNeededItem_Button.Text = "Remove";
            this.RemoveNeededItem_Button.UseVisualStyleBackColor = true;
            this.RemoveNeededItem_Button.Click += new System.EventHandler(this.RemoveNeededItem_Button_Click);
            // 
            // NewNeededItem_Button
            // 
            this.NewNeededItem_Button.Location = new System.Drawing.Point(465, 165);
            this.NewNeededItem_Button.Name = "NewNeededItem_Button";
            this.NewNeededItem_Button.Size = new System.Drawing.Size(75, 23);
            this.NewNeededItem_Button.TabIndex = 21;
            this.NewNeededItem_Button.Text = "New Item";
            this.NewNeededItem_Button.UseVisualStyleBackColor = true;
            this.NewNeededItem_Button.Click += new System.EventHandler(this.NewNeededItem_Button_Click);
            // 
            // DeleteItemGroup_Button
            // 
            this.DeleteItemGroup_Button.Location = new System.Drawing.Point(353, 204);
            this.DeleteItemGroup_Button.Name = "DeleteItemGroup_Button";
            this.DeleteItemGroup_Button.Size = new System.Drawing.Size(75, 23);
            this.DeleteItemGroup_Button.TabIndex = 22;
            this.DeleteItemGroup_Button.Text = "Delete List";
            this.DeleteItemGroup_Button.UseVisualStyleBackColor = true;
            this.DeleteItemGroup_Button.Click += new System.EventHandler(this.DeleteItemGroup_Button_Click);
            // 
            // InvalidItems_ListBox
            // 
            this.InvalidItems_ListBox.FormattingEnabled = true;
            this.InvalidItems_ListBox.Location = new System.Drawing.Point(434, 217);
            this.InvalidItems_ListBox.Name = "InvalidItems_ListBox";
            this.InvalidItems_ListBox.Size = new System.Drawing.Size(140, 121);
            this.InvalidItems_ListBox.TabIndex = 23;
            this.InvalidItems_ListBox.SelectedIndexChanged += new System.EventHandler(this.InvalidItems_ListBox_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(468, 201);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(66, 13);
            this.label5.TabIndex = 24;
            this.label5.Text = "Invalid Items";
            // 
            // InvalidItem_ComboBox
            // 
            this.InvalidItem_ComboBox.FormattingEnabled = true;
            this.InvalidItem_ComboBox.Location = new System.Drawing.Point(585, 251);
            this.InvalidItem_ComboBox.Name = "InvalidItem_ComboBox";
            this.InvalidItem_ComboBox.Size = new System.Drawing.Size(156, 21);
            this.InvalidItem_ComboBox.TabIndex = 25;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(620, 235);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(82, 13);
            this.label9.TabIndex = 26;
            this.label9.Text = "Edit Invalid Item";
            // 
            // SaveInvalidItem_Button
            // 
            this.SaveInvalidItem_Button.Location = new System.Drawing.Point(585, 278);
            this.SaveInvalidItem_Button.Name = "SaveInvalidItem_Button";
            this.SaveInvalidItem_Button.Size = new System.Drawing.Size(75, 23);
            this.SaveInvalidItem_Button.TabIndex = 27;
            this.SaveInvalidItem_Button.Text = "Save";
            this.SaveInvalidItem_Button.UseVisualStyleBackColor = true;
            this.SaveInvalidItem_Button.Click += new System.EventHandler(this.SaveInvalidItem_Button_Click);
            // 
            // RemoveInvalidItem_Button
            // 
            this.RemoveInvalidItem_Button.Location = new System.Drawing.Point(666, 278);
            this.RemoveInvalidItem_Button.Name = "RemoveInvalidItem_Button";
            this.RemoveInvalidItem_Button.Size = new System.Drawing.Size(75, 23);
            this.RemoveInvalidItem_Button.TabIndex = 28;
            this.RemoveInvalidItem_Button.Text = "Remove";
            this.RemoveInvalidItem_Button.UseVisualStyleBackColor = true;
            this.RemoveInvalidItem_Button.Click += new System.EventHandler(this.RemoveInvalidItem_Button_Click);
            // 
            // NewInvalidItem_Button
            // 
            this.NewInvalidItem_Button.Location = new System.Drawing.Point(465, 344);
            this.NewInvalidItem_Button.Name = "NewInvalidItem_Button";
            this.NewInvalidItem_Button.Size = new System.Drawing.Size(75, 23);
            this.NewInvalidItem_Button.TabIndex = 29;
            this.NewInvalidItem_Button.Text = "New Item";
            this.NewInvalidItem_Button.UseVisualStyleBackColor = true;
            this.NewInvalidItem_Button.Click += new System.EventHandler(this.NewInvalidItem_Button_Click);
            // 
            // SaveLogic_Button
            // 
            this.SaveLogic_Button.Location = new System.Drawing.Point(16, 344);
            this.SaveLogic_Button.Name = "SaveLogic_Button";
            this.SaveLogic_Button.Size = new System.Drawing.Size(75, 23);
            this.SaveLogic_Button.TabIndex = 30;
            this.SaveLogic_Button.Text = "Save";
            this.SaveLogic_Button.UseVisualStyleBackColor = true;
            this.SaveLogic_Button.Click += new System.EventHandler(this.SaveLogic_Button_Click);
            // 
            // CancelLogic_Button
            // 
            this.CancelLogic_Button.Location = new System.Drawing.Point(681, 344);
            this.CancelLogic_Button.Name = "CancelLogic_Button";
            this.CancelLogic_Button.Size = new System.Drawing.Size(75, 23);
            this.CancelLogic_Button.TabIndex = 31;
            this.CancelLogic_Button.Text = "Cancel";
            this.CancelLogic_Button.UseVisualStyleBackColor = true;
            this.CancelLogic_Button.Click += new System.EventHandler(this.CancelLogic_Button_Click);
            // 
            // Duplicate_ItemSet_Button
            // 
            this.Duplicate_ItemSet_Button.Location = new System.Drawing.Point(312, 233);
            this.Duplicate_ItemSet_Button.Name = "Duplicate_ItemSet_Button";
            this.Duplicate_ItemSet_Button.Size = new System.Drawing.Size(75, 23);
            this.Duplicate_ItemSet_Button.TabIndex = 32;
            this.Duplicate_ItemSet_Button.Text = "Duplicate";
            this.Duplicate_ItemSet_Button.UseVisualStyleBackColor = true;
            this.Duplicate_ItemSet_Button.Click += new System.EventHandler(this.Duplicate_ItemSet_Button_Click);
            // 
            // DayNight_ItemNeeded_Label
            // 
            this.DayNight_ItemNeeded_Label.AutoSize = true;
            this.DayNight_ItemNeeded_Label.Location = new System.Drawing.Point(762, 27);
            this.DayNight_ItemNeeded_Label.Name = "DayNight_ItemNeeded_Label";
            this.DayNight_ItemNeeded_Label.Size = new System.Drawing.Size(185, 13);
            this.DayNight_ItemNeeded_Label.TabIndex = 33;
            this.DayNight_ItemNeeded_Label.Text = "When the item can be used in this set";
            // 
            // DayNight_Item_Label
            // 
            this.DayNight_Item_Label.AutoSize = true;
            this.DayNight_Item_Label.Location = new System.Drawing.Point(762, 111);
            this.DayNight_Item_Label.Name = "DayNight_Item_Label";
            this.DayNight_Item_Label.Size = new System.Drawing.Size(186, 13);
            this.DayNight_Item_Label.TabIndex = 34;
            this.DayNight_Item_Label.Text = "When the item at this location is given";
            // 
            // Day1_Needed_Checkbox
            // 
            this.Day1_Needed_Checkbox.AutoSize = true;
            this.Day1_Needed_Checkbox.Location = new System.Drawing.Point(765, 43);
            this.Day1_Needed_Checkbox.Name = "Day1_Needed_Checkbox";
            this.Day1_Needed_Checkbox.Size = new System.Drawing.Size(54, 17);
            this.Day1_Needed_Checkbox.TabIndex = 35;
            this.Day1_Needed_Checkbox.Text = "Day 1";
            this.Day1_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Day1_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Day2_Needed_Checkbox
            // 
            this.Day2_Needed_Checkbox.AutoSize = true;
            this.Day2_Needed_Checkbox.Location = new System.Drawing.Point(831, 43);
            this.Day2_Needed_Checkbox.Name = "Day2_Needed_Checkbox";
            this.Day2_Needed_Checkbox.Size = new System.Drawing.Size(54, 17);
            this.Day2_Needed_Checkbox.TabIndex = 36;
            this.Day2_Needed_Checkbox.Text = "Day 2";
            this.Day2_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Day2_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Day3_Needed_Checkbox
            // 
            this.Day3_Needed_Checkbox.AutoSize = true;
            this.Day3_Needed_Checkbox.Location = new System.Drawing.Point(897, 43);
            this.Day3_Needed_Checkbox.Name = "Day3_Needed_Checkbox";
            this.Day3_Needed_Checkbox.Size = new System.Drawing.Size(54, 17);
            this.Day3_Needed_Checkbox.TabIndex = 37;
            this.Day3_Needed_Checkbox.Text = "Day 3";
            this.Day3_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Day3_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Night1_Needed_Checkbox
            // 
            this.Night1_Needed_Checkbox.AutoSize = true;
            this.Night1_Needed_Checkbox.Location = new System.Drawing.Point(765, 66);
            this.Night1_Needed_Checkbox.Name = "Night1_Needed_Checkbox";
            this.Night1_Needed_Checkbox.Size = new System.Drawing.Size(60, 17);
            this.Night1_Needed_Checkbox.TabIndex = 38;
            this.Night1_Needed_Checkbox.Text = "Night 1";
            this.Night1_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Night1_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Night2_Needed_Checkbox
            // 
            this.Night2_Needed_Checkbox.AutoSize = true;
            this.Night2_Needed_Checkbox.Location = new System.Drawing.Point(831, 66);
            this.Night2_Needed_Checkbox.Name = "Night2_Needed_Checkbox";
            this.Night2_Needed_Checkbox.Size = new System.Drawing.Size(60, 17);
            this.Night2_Needed_Checkbox.TabIndex = 39;
            this.Night2_Needed_Checkbox.Text = "Night 2";
            this.Night2_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Night2_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Night3_Needed_Checkbox
            // 
            this.Night3_Needed_Checkbox.AutoSize = true;
            this.Night3_Needed_Checkbox.Location = new System.Drawing.Point(897, 66);
            this.Night3_Needed_Checkbox.Name = "Night3_Needed_Checkbox";
            this.Night3_Needed_Checkbox.Size = new System.Drawing.Size(60, 17);
            this.Night3_Needed_Checkbox.TabIndex = 40;
            this.Night3_Needed_Checkbox.Text = "Night 3";
            this.Night3_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Night3_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Night3_ItemGiven_Checkbox
            // 
            this.Night3_ItemGiven_Checkbox.AutoSize = true;
            this.Night3_ItemGiven_Checkbox.Location = new System.Drawing.Point(894, 150);
            this.Night3_ItemGiven_Checkbox.Name = "Night3_ItemGiven_Checkbox";
            this.Night3_ItemGiven_Checkbox.Size = new System.Drawing.Size(60, 17);
            this.Night3_ItemGiven_Checkbox.TabIndex = 46;
            this.Night3_ItemGiven_Checkbox.Text = "Night 3";
            this.Night3_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Night3_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Night2_ItemGiven_Checkbox
            // 
            this.Night2_ItemGiven_Checkbox.AutoSize = true;
            this.Night2_ItemGiven_Checkbox.Location = new System.Drawing.Point(828, 150);
            this.Night2_ItemGiven_Checkbox.Name = "Night2_ItemGiven_Checkbox";
            this.Night2_ItemGiven_Checkbox.Size = new System.Drawing.Size(60, 17);
            this.Night2_ItemGiven_Checkbox.TabIndex = 45;
            this.Night2_ItemGiven_Checkbox.Text = "Night 2";
            this.Night2_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Night2_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Night1_ItemGiven_Checkbox
            // 
            this.Night1_ItemGiven_Checkbox.AutoSize = true;
            this.Night1_ItemGiven_Checkbox.Location = new System.Drawing.Point(762, 150);
            this.Night1_ItemGiven_Checkbox.Name = "Night1_ItemGiven_Checkbox";
            this.Night1_ItemGiven_Checkbox.Size = new System.Drawing.Size(60, 17);
            this.Night1_ItemGiven_Checkbox.TabIndex = 44;
            this.Night1_ItemGiven_Checkbox.Text = "Night 1";
            this.Night1_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Night1_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Day3_ItemGiven_Checkbox
            // 
            this.Day3_ItemGiven_Checkbox.AutoSize = true;
            this.Day3_ItemGiven_Checkbox.Location = new System.Drawing.Point(894, 127);
            this.Day3_ItemGiven_Checkbox.Name = "Day3_ItemGiven_Checkbox";
            this.Day3_ItemGiven_Checkbox.Size = new System.Drawing.Size(54, 17);
            this.Day3_ItemGiven_Checkbox.TabIndex = 43;
            this.Day3_ItemGiven_Checkbox.Text = "Day 3";
            this.Day3_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Day3_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Day2_ItemGiven_Checkbox
            // 
            this.Day2_ItemGiven_Checkbox.AutoSize = true;
            this.Day2_ItemGiven_Checkbox.Location = new System.Drawing.Point(828, 127);
            this.Day2_ItemGiven_Checkbox.Name = "Day2_ItemGiven_Checkbox";
            this.Day2_ItemGiven_Checkbox.Size = new System.Drawing.Size(54, 17);
            this.Day2_ItemGiven_Checkbox.TabIndex = 42;
            this.Day2_ItemGiven_Checkbox.Text = "Day 2";
            this.Day2_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Day2_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Day1_ItemGiven_Checkbox
            // 
            this.Day1_ItemGiven_Checkbox.AutoSize = true;
            this.Day1_ItemGiven_Checkbox.Location = new System.Drawing.Point(762, 127);
            this.Day1_ItemGiven_Checkbox.Name = "Day1_ItemGiven_Checkbox";
            this.Day1_ItemGiven_Checkbox.Size = new System.Drawing.Size(54, 17);
            this.Day1_ItemGiven_Checkbox.TabIndex = 41;
            this.Day1_ItemGiven_Checkbox.Text = "Day 1";
            this.Day1_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Day1_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Moon_Needed_Checkbox
            // 
            this.Moon_Needed_Checkbox.AutoSize = true;
            this.Moon_Needed_Checkbox.Location = new System.Drawing.Point(831, 91);
            this.Moon_Needed_Checkbox.Name = "Moon_Needed_Checkbox";
            this.Moon_Needed_Checkbox.Size = new System.Drawing.Size(53, 17);
            this.Moon_Needed_Checkbox.TabIndex = 47;
            this.Moon_Needed_Checkbox.Text = "Moon";
            this.Moon_Needed_Checkbox.UseVisualStyleBackColor = true;
            this.Moon_Needed_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // Moon_ItemGiven_Checkbox
            // 
            this.Moon_ItemGiven_Checkbox.AutoSize = true;
            this.Moon_ItemGiven_Checkbox.Location = new System.Drawing.Point(828, 173);
            this.Moon_ItemGiven_Checkbox.Name = "Moon_ItemGiven_Checkbox";
            this.Moon_ItemGiven_Checkbox.Size = new System.Drawing.Size(53, 17);
            this.Moon_ItemGiven_Checkbox.TabIndex = 48;
            this.Moon_ItemGiven_Checkbox.Text = "Moon";
            this.Moon_ItemGiven_Checkbox.UseVisualStyleBackColor = true;
            this.Moon_ItemGiven_Checkbox.CheckStateChanged += new System.EventHandler(this.Day1_Needed_Checkbox_CheckedChanged);
            // 
            // LogicEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(961, 376);
            this.Controls.Add(this.Moon_ItemGiven_Checkbox);
            this.Controls.Add(this.Moon_Needed_Checkbox);
            this.Controls.Add(this.Night3_ItemGiven_Checkbox);
            this.Controls.Add(this.Night2_ItemGiven_Checkbox);
            this.Controls.Add(this.Night1_ItemGiven_Checkbox);
            this.Controls.Add(this.Day3_ItemGiven_Checkbox);
            this.Controls.Add(this.Day2_ItemGiven_Checkbox);
            this.Controls.Add(this.Day1_ItemGiven_Checkbox);
            this.Controls.Add(this.Night3_Needed_Checkbox);
            this.Controls.Add(this.Night2_Needed_Checkbox);
            this.Controls.Add(this.Night1_Needed_Checkbox);
            this.Controls.Add(this.Day3_Needed_Checkbox);
            this.Controls.Add(this.Day2_Needed_Checkbox);
            this.Controls.Add(this.Day1_Needed_Checkbox);
            this.Controls.Add(this.DayNight_Item_Label);
            this.Controls.Add(this.DayNight_ItemNeeded_Label);
            this.Controls.Add(this.Duplicate_ItemSet_Button);
            this.Controls.Add(this.CancelLogic_Button);
            this.Controls.Add(this.SaveLogic_Button);
            this.Controls.Add(this.NewInvalidItem_Button);
            this.Controls.Add(this.RemoveInvalidItem_Button);
            this.Controls.Add(this.SaveInvalidItem_Button);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.InvalidItem_ComboBox);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.InvalidItems_ListBox);
            this.Controls.Add(this.DeleteItemGroup_Button);
            this.Controls.Add(this.NewNeededItem_Button);
            this.Controls.Add(this.RemoveNeededItem_Button);
            this.Controls.Add(this.EditItem_ComboBox);
            this.Controls.Add(this.Comment_TextBox);
            this.Controls.Add(this.EditItem_Number);
            this.Controls.Add(this.ItemGroups_ListBox);
            this.Controls.Add(this.NewItemGroup_Button);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.SaveNeededItem_Button);
            this.Controls.Add(this.ItemsNeeded_ListBox);
            this.Controls.Add(this.Items_ListBox);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.NewLogic_TextBox);
            this.Controls.Add(this.NewLogic_Button);
            this.Controls.Add(this.LogicFiles_ListBox);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "LogicEditor";
            this.Text = "Logic Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.LogicEditor_FormClosing);
            this.Load += new System.EventHandler(this.LogicEditor_Load);
            this.Shown += new System.EventHandler(this.LogicEditor_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.EditItem_Number)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ListBox LogicFiles_ListBox;
        private System.Windows.Forms.Button NewLogic_Button;
        private System.Windows.Forms.TextBox NewLogic_TextBox;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ListBox Items_ListBox;
        private System.Windows.Forms.ListBox ItemsNeeded_ListBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button SaveNeededItem_Button;
        private System.Windows.Forms.Button NewItemGroup_Button;
        private System.Windows.Forms.ListBox ItemGroups_ListBox;
        private System.Windows.Forms.NumericUpDown EditItem_Number;
        private System.Windows.Forms.RichTextBox Comment_TextBox;
        private System.Windows.Forms.ComboBox EditItem_ComboBox;
        private System.Windows.Forms.Button RemoveNeededItem_Button;
        private System.Windows.Forms.Button NewNeededItem_Button;
        private System.Windows.Forms.Button DeleteItemGroup_Button;
        private System.Windows.Forms.ListBox InvalidItems_ListBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox InvalidItem_ComboBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button SaveInvalidItem_Button;
        private System.Windows.Forms.Button RemoveInvalidItem_Button;
        private System.Windows.Forms.Button NewInvalidItem_Button;
        private System.Windows.Forms.Button SaveLogic_Button;
        private System.Windows.Forms.Button CancelLogic_Button;
        private System.Windows.Forms.Button Duplicate_ItemSet_Button;
        private System.Windows.Forms.Label DayNight_ItemNeeded_Label;
        private System.Windows.Forms.Label DayNight_Item_Label;
        private System.Windows.Forms.CheckBox Day1_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Day2_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Day3_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Night1_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Night2_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Night3_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Night3_ItemGiven_Checkbox;
        private System.Windows.Forms.CheckBox Night2_ItemGiven_Checkbox;
        private System.Windows.Forms.CheckBox Night1_ItemGiven_Checkbox;
        private System.Windows.Forms.CheckBox Day3_ItemGiven_Checkbox;
        private System.Windows.Forms.CheckBox Day2_ItemGiven_Checkbox;
        private System.Windows.Forms.CheckBox Day1_ItemGiven_Checkbox;
        private System.Windows.Forms.CheckBox Moon_Needed_Checkbox;
        private System.Windows.Forms.CheckBox Moon_ItemGiven_Checkbox;
    }
}