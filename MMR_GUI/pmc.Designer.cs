namespace Majora_s_Mask_Randomizer_GUI
{
    partial class pmc
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(pmc));
            this.scrollPanel = new System.Windows.Forms.Panel();
            this.rootTable = new System.Windows.Forms.TableLayoutPanel();
            this.label6 = new System.Windows.Forms.Label();
            this.toggleBar = new System.Windows.Forms.FlowLayoutPanel();
            this.checkAllButton = new System.Windows.Forms.Button();
            this.uncheckAllButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.pauseMenuTable = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.ItemScreen_Checkbox = new System.Windows.Forms.CheckBox();
            this.ItemSelect_Button = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.MapScreen_Checkbox = new System.Windows.Forms.CheckBox();
            this.MapScreen_Button = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.QuestScreen_Checkbox = new System.Windows.Forms.CheckBox();
            this.Quest_Button = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.MaskScreen_Checkbox = new System.Windows.Forms.CheckBox();
            this.MaskScreen_Button = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.NamePlate_Checkbox = new System.Windows.Forms.CheckBox();
            this.NamePlate_Button = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tunicsTable = new System.Windows.Forms.TableLayoutPanel();
            this.label8 = new System.Windows.Forms.Label();
            this.LinkColor_Checkbox = new System.Windows.Forms.CheckBox();
            this.LinkColor_Button = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.DekuColor_Checkbox = new System.Windows.Forms.CheckBox();
            this.DekuColor_Button = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.GoronColor_Checkbox = new System.Windows.Forms.CheckBox();
            this.GoronColor_Button = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.ZoraColor_Checkbox = new System.Windows.Forms.CheckBox();
            this.ZoraColor_Button = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.FDColor_Checkbox = new System.Windows.Forms.CheckBox();
            this.FDColor_Button = new System.Windows.Forms.Button();
            this.buttonBar = new System.Windows.Forms.FlowLayoutPanel();
            this.Confirm_Button = new System.Windows.Forms.Button();
            this.ItemSelect_ColorBox = new System.Windows.Forms.ColorDialog();
            this.MapScreen_ColorBox = new System.Windows.Forms.ColorDialog();
            this.Quest_ColorBox = new System.Windows.Forms.ColorDialog();
            this.MaskScreen_ColorBox = new System.Windows.Forms.ColorDialog();
            this.NamePlate_BolorBox = new System.Windows.Forms.ColorDialog();
            this.LinkColor_ColorBox = new System.Windows.Forms.ColorDialog();
            this.DekuColor_ColorBox = new System.Windows.Forms.ColorDialog();
            this.GoronColor_ColorBox = new System.Windows.Forms.ColorDialog();
            this.ZoraColor_ColorBox = new System.Windows.Forms.ColorDialog();
            this.FDColor_ColorBox = new System.Windows.Forms.ColorDialog();
            this.scrollPanel.SuspendLayout();
            this.rootTable.SuspendLayout();
            this.toggleBar.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.pauseMenuTable.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tunicsTable.SuspendLayout();
            this.buttonBar.SuspendLayout();
            this.SuspendLayout();
            // scrollPanel
            this.scrollPanel.AutoScroll = true;
            this.scrollPanel.Controls.Add(this.rootTable);
            this.scrollPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scrollPanel.Location = new System.Drawing.Point(0, 0);
            this.scrollPanel.Name = "scrollPanel";
            this.scrollPanel.Padding = new System.Windows.Forms.Padding(0, 0, 4, 0);
            this.scrollPanel.TabIndex = 0;
            // rootTable
            this.rootTable.AutoSize = true;
            this.rootTable.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.rootTable.ColumnCount = 1;
            this.rootTable.Controls.Add(this.label6, 0, 0);
            this.rootTable.Controls.Add(this.toggleBar, 0, 1);
            this.rootTable.Controls.Add(this.groupBox1, 0, 2);
            this.rootTable.Controls.Add(this.groupBox2, 0, 3);
            this.rootTable.Controls.Add(this.buttonBar, 0, 4);
            this.rootTable.Dock = System.Windows.Forms.DockStyle.Top;
            this.rootTable.Location = new System.Drawing.Point(0, 0);
            this.rootTable.Name = "rootTable";
            this.rootTable.Padding = new System.Windows.Forms.Padding(12);
            this.rootTable.RowCount = 5;
            this.rootTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.rootTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.rootTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.rootTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.rootTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.rootTable.TabIndex = 0;
            // label6
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 12);
            this.label6.Margin = new System.Windows.Forms.Padding(3, 0, 3, 8);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(318, 15);
            this.label6.TabIndex = 0;
            this.label6.Tag = "Hint";
            this.label6.Text = "Check a box to use a random color instead of the picker.";
            // toggleBar
            this.toggleBar.AutoSize = true;
            this.toggleBar.Controls.Add(this.checkAllButton);
            this.toggleBar.Controls.Add(this.uncheckAllButton);
            this.toggleBar.Location = new System.Drawing.Point(15, 35);
            this.toggleBar.Margin = new System.Windows.Forms.Padding(3, 0, 3, 8);
            this.toggleBar.Name = "toggleBar";
            this.toggleBar.Size = new System.Drawing.Size(406, 37);
            this.toggleBar.TabIndex = 1;
            this.toggleBar.WrapContents = false;
            // checkAllButton
            this.checkAllButton.AutoSize = true;
            this.checkAllButton.Location = new System.Drawing.Point(3, 3);
            this.checkAllButton.Margin = new System.Windows.Forms.Padding(0, 0, 6, 0);
            this.checkAllButton.MinimumSize = new System.Drawing.Size(0, 30);
            this.checkAllButton.Name = "checkAllButton";
            this.checkAllButton.Size = new System.Drawing.Size(68, 30);
            this.checkAllButton.TabIndex = 0;
            this.checkAllButton.Text = "Check All";
            this.checkAllButton.UseVisualStyleBackColor = true;
            this.checkAllButton.Click += new System.EventHandler(this.checkAllButton_Click);
            // uncheckAllButton
            this.uncheckAllButton.AutoSize = true;
            this.uncheckAllButton.Location = new System.Drawing.Point(77, 3);
            this.uncheckAllButton.MinimumSize = new System.Drawing.Size(0, 30);
            this.uncheckAllButton.Name = "uncheckAllButton";
            this.uncheckAllButton.Size = new System.Drawing.Size(82, 30);
            this.uncheckAllButton.TabIndex = 1;
            this.uncheckAllButton.Text = "Uncheck All";
            this.uncheckAllButton.UseVisualStyleBackColor = true;
            this.uncheckAllButton.Click += new System.EventHandler(this.uncheckAllButton_Click);
            // groupBox1
            this.groupBox1.AutoSize = false;
            this.groupBox1.Controls.Add(this.pauseMenuTable);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox1.Location = new System.Drawing.Point(15, 80);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(0, 0, 0, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(8, 4, 8, 8);
            this.groupBox1.Size = new System.Drawing.Size(396, 204);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Pause Menu";
            // pauseMenuTable
            this.pauseMenuTable.AutoSize = false;
            this.pauseMenuTable.ColumnCount = 3;
            this.pauseMenuTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.pauseMenuTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.pauseMenuTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 36F));
            this.pauseMenuTable.Controls.Add(this.label1, 0, 0);
            this.pauseMenuTable.Controls.Add(this.ItemScreen_Checkbox, 1, 0);
            this.pauseMenuTable.Controls.Add(this.ItemSelect_Button, 2, 0);
            this.pauseMenuTable.Controls.Add(this.label2, 0, 1);
            this.pauseMenuTable.Controls.Add(this.MapScreen_Checkbox, 1, 1);
            this.pauseMenuTable.Controls.Add(this.MapScreen_Button, 2, 1);
            this.pauseMenuTable.Controls.Add(this.label3, 0, 2);
            this.pauseMenuTable.Controls.Add(this.QuestScreen_Checkbox, 1, 2);
            this.pauseMenuTable.Controls.Add(this.Quest_Button, 2, 2);
            this.pauseMenuTable.Controls.Add(this.label4, 0, 3);
            this.pauseMenuTable.Controls.Add(this.MaskScreen_Checkbox, 1, 3);
            this.pauseMenuTable.Controls.Add(this.MaskScreen_Button, 2, 3);
            this.pauseMenuTable.Controls.Add(this.label5, 0, 4);
            this.pauseMenuTable.Controls.Add(this.NamePlate_Checkbox, 1, 4);
            this.pauseMenuTable.Controls.Add(this.NamePlate_Button, 2, 4);
            this.pauseMenuTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pauseMenuTable.Location = new System.Drawing.Point(8, 20);
            this.pauseMenuTable.Name = "pauseMenuTable";
            this.pauseMenuTable.Padding = new System.Windows.Forms.Padding(4);
            this.pauseMenuTable.RowCount = 5;
            this.pauseMenuTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.pauseMenuTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.pauseMenuTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.pauseMenuTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.pauseMenuTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.pauseMenuTable.Size = new System.Drawing.Size(372, 168);
            this.pauseMenuTable.TabIndex = 0;
            ConfigureColorRow(this.label1, "Item Screen Color", this.ItemScreen_Checkbox, this.ItemSelect_Button, this.ItemSelect_Button_Click);
            ConfigureColorRow(this.label2, "Map Screen Color", this.MapScreen_Checkbox, this.MapScreen_Button, this.MapScreen_Button_Click);
            ConfigureColorRow(this.label3, "Quest Screen Color", this.QuestScreen_Checkbox, this.Quest_Button, this.Quest_Button_Click);
            ConfigureColorRow(this.label4, "Mask Screen Color", this.MaskScreen_Checkbox, this.MaskScreen_Button, this.MaskScreen_Button_Click);
            ConfigureColorRow(this.label5, "Nameplate Color", this.NamePlate_Checkbox, this.NamePlate_Button, this.NamePlate_Button_Click);
            // groupBox2
            this.groupBox2.AutoSize = false;
            this.groupBox2.Controls.Add(this.tunicsTable);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox2.Location = new System.Drawing.Point(15, 292);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(8, 4, 8, 8);
            this.groupBox2.Size = new System.Drawing.Size(396, 204);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tunics";
            // tunicsTable
            this.tunicsTable.AutoSize = false;
            this.tunicsTable.ColumnCount = 3;
            this.tunicsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tunicsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tunicsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 36F));
            this.tunicsTable.Controls.Add(this.label8, 0, 0);
            this.tunicsTable.Controls.Add(this.LinkColor_Checkbox, 1, 0);
            this.tunicsTable.Controls.Add(this.LinkColor_Button, 2, 0);
            this.tunicsTable.Controls.Add(this.label9, 0, 1);
            this.tunicsTable.Controls.Add(this.DekuColor_Checkbox, 1, 1);
            this.tunicsTable.Controls.Add(this.DekuColor_Button, 2, 1);
            this.tunicsTable.Controls.Add(this.label10, 0, 2);
            this.tunicsTable.Controls.Add(this.GoronColor_Checkbox, 1, 2);
            this.tunicsTable.Controls.Add(this.GoronColor_Button, 2, 2);
            this.tunicsTable.Controls.Add(this.label11, 0, 3);
            this.tunicsTable.Controls.Add(this.ZoraColor_Checkbox, 1, 3);
            this.tunicsTable.Controls.Add(this.ZoraColor_Button, 2, 3);
            this.tunicsTable.Controls.Add(this.label12, 0, 4);
            this.tunicsTable.Controls.Add(this.FDColor_Checkbox, 1, 4);
            this.tunicsTable.Controls.Add(this.FDColor_Button, 2, 4);
            this.tunicsTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tunicsTable.Location = new System.Drawing.Point(8, 20);
            this.tunicsTable.Name = "tunicsTable";
            this.tunicsTable.Padding = new System.Windows.Forms.Padding(4);
            this.tunicsTable.RowCount = 5;
            this.tunicsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tunicsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tunicsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tunicsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tunicsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tunicsTable.Size = new System.Drawing.Size(372, 168);
            this.tunicsTable.TabIndex = 0;
            ConfigureColorRow(this.label8, "Link", this.LinkColor_Checkbox, this.LinkColor_Button, this.LinkColor_Button_Click);
            ConfigureColorRow(this.label9, "Deku", this.DekuColor_Checkbox, this.DekuColor_Button, this.DekuColor_Button_Click);
            ConfigureColorRow(this.label10, "Goron", this.GoronColor_Checkbox, this.GoronColor_Button, this.GoronColor_Button_Click);
            ConfigureColorRow(this.label11, "Zora", this.ZoraColor_Checkbox, this.ZoraColor_Button, this.ZoraColor_Button_Click);
            ConfigureColorRow(this.label12, "Fierce Deity", this.FDColor_Checkbox, this.FDColor_Button, this.FDColor_Button_Click);
            // buttonBar
            this.buttonBar.AutoSize = true;
            this.buttonBar.Controls.Add(this.Confirm_Button);
            this.buttonBar.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonBar.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.buttonBar.Location = new System.Drawing.Point(15, 496);
            this.buttonBar.Margin = new System.Windows.Forms.Padding(0);
            this.buttonBar.Name = "buttonBar";
            this.buttonBar.TabIndex = 4;
            this.buttonBar.WrapContents = false;
            // Confirm_Button
            this.Confirm_Button.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.Confirm_Button.AutoSize = true;
            this.Confirm_Button.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Confirm_Button.Location = new System.Drawing.Point(303, 3);
            this.Confirm_Button.Margin = new System.Windows.Forms.Padding(0, 8, 0, 0);
            this.Confirm_Button.MinimumSize = new System.Drawing.Size(100, 30);
            this.Confirm_Button.Name = "Confirm_Button";
            this.Confirm_Button.Size = new System.Drawing.Size(100, 30);
            this.Confirm_Button.TabIndex = 0;
            this.Confirm_Button.Text = "Confirm";
            this.Confirm_Button.UseVisualStyleBackColor = true;
            this.Confirm_Button.Click += new System.EventHandler(this.Confirm_Button_Click);
            // pmc
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(440, 580);
            this.Controls.Add(this.scrollPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(420, 560);
            this.Name = "pmc";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Colors";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.pmc_FormClosing);
            this.Load += new System.EventHandler(this.pmc_Load);
            this.scrollPanel.ResumeLayout(false);
            this.scrollPanel.PerformLayout();
            this.rootTable.ResumeLayout(false);
            this.rootTable.PerformLayout();
            this.toggleBar.ResumeLayout(false);
            this.toggleBar.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.pauseMenuTable.ResumeLayout(false);
            this.pauseMenuTable.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.tunicsTable.ResumeLayout(false);
            this.tunicsTable.PerformLayout();
            this.buttonBar.ResumeLayout(false);
            this.buttonBar.PerformLayout();
            this.ResumeLayout(false);
        }

        private void ConfigureColorRow(
            System.Windows.Forms.Label label,
            string text,
            System.Windows.Forms.CheckBox checkBox,
            System.Windows.Forms.Button colorButton,
            System.EventHandler clickHandler)
        {
            label.Anchor = System.Windows.Forms.AnchorStyles.Left;
            label.AutoSize = true;
            label.Margin = new System.Windows.Forms.Padding(0, 6, 4, 0);
            label.Text = text;
            checkBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            checkBox.AutoSize = false;
            checkBox.Margin = new System.Windows.Forms.Padding(0);
            checkBox.MinimumSize = new System.Drawing.Size(18, 18);
            checkBox.Size = new System.Drawing.Size(22, 22);
            checkBox.Text = string.Empty;
            checkBox.UseVisualStyleBackColor = true;
            checkBox.CheckedChanged += new System.EventHandler(this.LinkColor_Checkbox_CheckedChanged);
            colorButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            colorButton.Margin = new System.Windows.Forms.Padding(0);
            colorButton.Size = new System.Drawing.Size(28, 30);
            colorButton.Tag = "ColorSwatch";
            colorButton.UseVisualStyleBackColor = false;
            colorButton.Click += clickHandler;
        }

        #endregion

        private System.Windows.Forms.Panel scrollPanel;
        private System.Windows.Forms.TableLayoutPanel rootTable;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.FlowLayoutPanel toggleBar;
        private System.Windows.Forms.Button checkAllButton;
        private System.Windows.Forms.Button uncheckAllButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TableLayoutPanel pauseMenuTable;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TableLayoutPanel tunicsTable;
        private System.Windows.Forms.FlowLayoutPanel buttonBar;
        private System.Windows.Forms.ColorDialog ItemSelect_ColorBox;
        private System.Windows.Forms.Button ItemSelect_Button;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ColorDialog MapScreen_ColorBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button MapScreen_Button;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button Quest_Button;
        private System.Windows.Forms.ColorDialog Quest_ColorBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button MaskScreen_Button;
        private System.Windows.Forms.ColorDialog MaskScreen_ColorBox;
        private System.Windows.Forms.Button Confirm_Button;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ColorDialog NamePlate_BolorBox;
        private System.Windows.Forms.Button NamePlate_Button;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button LinkColor_Button;
        private System.Windows.Forms.Button DekuColor_Button;
        private System.Windows.Forms.Button GoronColor_Button;
        private System.Windows.Forms.Button ZoraColor_Button;
        private System.Windows.Forms.Button FDColor_Button;
        private System.Windows.Forms.ColorDialog LinkColor_ColorBox;
        private System.Windows.Forms.ColorDialog DekuColor_ColorBox;
        private System.Windows.Forms.ColorDialog GoronColor_ColorBox;
        private System.Windows.Forms.ColorDialog ZoraColor_ColorBox;
        private System.Windows.Forms.ColorDialog FDColor_ColorBox;
        private System.Windows.Forms.CheckBox LinkColor_Checkbox;
        private System.Windows.Forms.CheckBox DekuColor_Checkbox;
        private System.Windows.Forms.CheckBox GoronColor_Checkbox;
        private System.Windows.Forms.CheckBox ZoraColor_Checkbox;
        private System.Windows.Forms.CheckBox FDColor_Checkbox;
        private System.Windows.Forms.CheckBox ItemScreen_Checkbox;
        private System.Windows.Forms.CheckBox MapScreen_Checkbox;
        private System.Windows.Forms.CheckBox QuestScreen_Checkbox;
        private System.Windows.Forms.CheckBox MaskScreen_Checkbox;
        private System.Windows.Forms.CheckBox NamePlate_Checkbox;
    }
}
