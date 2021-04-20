namespace Majora_s_Mask_Randomizer_GUI
{
        partial class wallets_form
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
            this.SmallWallet_NumBox = new System.Windows.Forms.NumericUpDown();
            this.MediumWallet_NumBox = new System.Windows.Forms.NumericUpDown();
            this.LargeWallet_NumBox = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.ConfirmWallet_Button = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.SmallWallet_NumBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.MediumWallet_NumBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.LargeWallet_NumBox)).BeginInit();
            this.SuspendLayout();
            // 
            // SmallWallet_NumBox
            // 
            this.SmallWallet_NumBox.Location = new System.Drawing.Point(50, 12);
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
            this.SmallWallet_NumBox.Size = new System.Drawing.Size(120, 20);
            this.SmallWallet_NumBox.TabIndex = 0;
            this.SmallWallet_NumBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SmallWallet_NumBox.ValueChanged += new System.EventHandler(this.SmallWallet_NumBox_ValueChanged);
            // 
            // MediumWallet_NumBox
            // 
            this.MediumWallet_NumBox.Location = new System.Drawing.Point(50, 38);
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
            this.MediumWallet_NumBox.Size = new System.Drawing.Size(120, 20);
            this.MediumWallet_NumBox.TabIndex = 1;
            this.MediumWallet_NumBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MediumWallet_NumBox.ValueChanged += new System.EventHandler(this.MediumWallet_NumBox_ValueChanged);
            // 
            // LargeWallet_NumBox
            // 
            this.LargeWallet_NumBox.Location = new System.Drawing.Point(50, 66);
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
            this.LargeWallet_NumBox.Size = new System.Drawing.Size(120, 20);
            this.LargeWallet_NumBox.TabIndex = 2;
            this.LargeWallet_NumBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.LargeWallet_NumBox.ValueChanged += new System.EventHandler(this.LargeWallet_NumBox_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(30, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Child";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Adult";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Giant";
            // 
            // ConfirmWallet_Button
            // 
            this.ConfirmWallet_Button.Location = new System.Drawing.Point(58, 92);
            this.ConfirmWallet_Button.Name = "ConfirmWallet_Button";
            this.ConfirmWallet_Button.Size = new System.Drawing.Size(75, 23);
            this.ConfirmWallet_Button.TabIndex = 6;
            this.ConfirmWallet_Button.Text = "Confirm";
            this.ConfirmWallet_Button.UseVisualStyleBackColor = true;
            this.ConfirmWallet_Button.Click += new System.EventHandler(this.ConfirmWallet_Button_Click);
            // 
            // wallets_form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(185, 125);
            this.Controls.Add(this.ConfirmWallet_Button);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.LargeWallet_NumBox);
            this.Controls.Add(this.MediumWallet_NumBox);
            this.Controls.Add(this.SmallWallet_NumBox);
            this.Name = "wallets_form";
            this.Text = "Wallets";
            this.Load += new System.EventHandler(this.wallets_form_Load);
            ((System.ComponentModel.ISupportInitialize)(this.SmallWallet_NumBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.MediumWallet_NumBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.LargeWallet_NumBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

                }

                #endregion

                private System.Windows.Forms.NumericUpDown SmallWallet_NumBox;
                private System.Windows.Forms.NumericUpDown MediumWallet_NumBox;
                private System.Windows.Forms.NumericUpDown LargeWallet_NumBox;
                private System.Windows.Forms.Label label1;
                private System.Windows.Forms.Label label2;
                private System.Windows.Forms.Label label3;
                private System.Windows.Forms.Button ConfirmWallet_Button;
        }
}