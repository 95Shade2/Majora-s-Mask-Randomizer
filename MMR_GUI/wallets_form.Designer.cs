namespace Majora_s_Mask_Randomizer_GUI
{
    partial class wallets_form
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(wallets_form));
            this.layoutTable = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.SmallWallet_NumBox = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.MediumWallet_NumBox = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.LargeWallet_NumBox = new System.Windows.Forms.NumericUpDown();
            this.buttonBar = new System.Windows.Forms.FlowLayoutPanel();
            this.ConfirmWallet_Button = new System.Windows.Forms.Button();
            this.layoutTable.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SmallWallet_NumBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.MediumWallet_NumBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.LargeWallet_NumBox)).BeginInit();
            this.buttonBar.SuspendLayout();
            this.SuspendLayout();
            // 
            // layoutTable
            // 
            this.layoutTable.AutoSize = true;
            this.layoutTable.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.layoutTable.ColumnCount = 2;
            this.layoutTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.layoutTable.Controls.Add(this.label1, 0, 0);
            this.layoutTable.Controls.Add(this.SmallWallet_NumBox, 1, 0);
            this.layoutTable.Controls.Add(this.label2, 0, 1);
            this.layoutTable.Controls.Add(this.MediumWallet_NumBox, 1, 1);
            this.layoutTable.Controls.Add(this.label3, 0, 2);
            this.layoutTable.Controls.Add(this.LargeWallet_NumBox, 1, 2);
            this.layoutTable.Controls.Add(this.buttonBar, 0, 3);
            this.layoutTable.SetColumnSpan(this.buttonBar, 2);
            this.layoutTable.Location = new System.Drawing.Point(0, 0);
            this.layoutTable.Name = "layoutTable";
            this.layoutTable.Padding = new System.Windows.Forms.Padding(16, 12, 16, 12);
            this.layoutTable.RowCount = 4;
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 9);
            this.label1.Margin = new System.Windows.Forms.Padding(3, 6, 12, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(30, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "Child";
            // 
            // SmallWallet_NumBox
            // 
            this.SmallWallet_NumBox.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.SmallWallet_NumBox.Location = new System.Drawing.Point(64, 6);
            this.SmallWallet_NumBox.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.SmallWallet_NumBox.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SmallWallet_NumBox.Name = "SmallWallet_NumBox";
            this.SmallWallet_NumBox.Size = new System.Drawing.Size(120, 23);
            this.SmallWallet_NumBox.TabIndex = 1;
            this.SmallWallet_NumBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SmallWallet_NumBox.ValueChanged += new System.EventHandler(this.SmallWallet_NumBox_ValueChanged);
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 38);
            this.label2.Margin = new System.Windows.Forms.Padding(3, 6, 12, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "Adult";
            // 
            // MediumWallet_NumBox
            // 
            this.MediumWallet_NumBox.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.MediumWallet_NumBox.Location = new System.Drawing.Point(64, 35);
            this.MediumWallet_NumBox.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.MediumWallet_NumBox.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MediumWallet_NumBox.Name = "MediumWallet_NumBox";
            this.MediumWallet_NumBox.Size = new System.Drawing.Size(120, 23);
            this.MediumWallet_NumBox.TabIndex = 3;
            this.MediumWallet_NumBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MediumWallet_NumBox.ValueChanged += new System.EventHandler(this.MediumWallet_NumBox_ValueChanged);
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(19, 67);
            this.label3.Margin = new System.Windows.Forms.Padding(3, 6, 12, 6);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 15);
            this.label3.TabIndex = 4;
            this.label3.Text = "Giant";
            // 
            // LargeWallet_NumBox
            // 
            this.LargeWallet_NumBox.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.LargeWallet_NumBox.Location = new System.Drawing.Point(64, 64);
            this.LargeWallet_NumBox.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.LargeWallet_NumBox.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.LargeWallet_NumBox.Name = "LargeWallet_NumBox";
            this.LargeWallet_NumBox.Size = new System.Drawing.Size(120, 23);
            this.LargeWallet_NumBox.TabIndex = 5;
            this.LargeWallet_NumBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.LargeWallet_NumBox.ValueChanged += new System.EventHandler(this.LargeWallet_NumBox_ValueChanged);
            // 
            // buttonBar
            // 
            this.buttonBar.AutoSize = true;
            this.buttonBar.Controls.Add(this.ConfirmWallet_Button);
            this.buttonBar.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.buttonBar.Location = new System.Drawing.Point(19, 93);
            this.buttonBar.Margin = new System.Windows.Forms.Padding(0, 12, 0, 0);
            this.buttonBar.Name = "buttonBar";
            this.buttonBar.TabIndex = 6;
            this.buttonBar.WrapContents = false;
            // 
            // ConfirmWallet_Button
            // 
            this.ConfirmWallet_Button.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.ConfirmWallet_Button.AutoSize = true;
            this.ConfirmWallet_Button.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ConfirmWallet_Button.Location = new System.Drawing.Point(141, 3);
            this.ConfirmWallet_Button.Margin = new System.Windows.Forms.Padding(0, 12, 0, 0);
            this.ConfirmWallet_Button.MinimumSize = new System.Drawing.Size(100, 30);
            this.ConfirmWallet_Button.Name = "ConfirmWallet_Button";
            this.ConfirmWallet_Button.TabIndex = 0;
            this.ConfirmWallet_Button.Text = "Confirm";
            this.ConfirmWallet_Button.UseVisualStyleBackColor = true;
            this.ConfirmWallet_Button.Click += new System.EventHandler(this.ConfirmWallet_Button_Click);
            // 
            // wallets_form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(284, 148);
            this.Controls.Add(this.layoutTable);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "wallets_form";
            this.Padding = new System.Windows.Forms.Padding(0);
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Wallets";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.wallets_form_FormClosing);
            this.Load += new System.EventHandler(this.wallets_form_Load);
            this.layoutTable.ResumeLayout(false);
            this.layoutTable.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SmallWallet_NumBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.MediumWallet_NumBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.LargeWallet_NumBox)).EndInit();
            this.buttonBar.ResumeLayout(false);
            this.buttonBar.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel layoutTable;
        private System.Windows.Forms.FlowLayoutPanel buttonBar;
        private System.Windows.Forms.NumericUpDown SmallWallet_NumBox;
        private System.Windows.Forms.NumericUpDown MediumWallet_NumBox;
        private System.Windows.Forms.NumericUpDown LargeWallet_NumBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button ConfirmWallet_Button;
    }
}
