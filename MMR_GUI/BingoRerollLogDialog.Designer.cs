namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class BingoRerollLogDialog
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
            this._logBox = new System.Windows.Forms.TextBox();
            this.buttonBar = new System.Windows.Forms.FlowLayoutPanel();
            this.closeButton = new System.Windows.Forms.Button();
            this.copyButton = new System.Windows.Forms.Button();
            this.buttonBar.SuspendLayout();
            this.SuspendLayout();
            // 
            // _logBox
            // 
            this._logBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this._logBox.Font = new System.Drawing.Font("Consolas", 9F);
            this._logBox.Location = new System.Drawing.Point(0, 0);
            this._logBox.Multiline = true;
            this._logBox.Name = "_logBox";
            this._logBox.ReadOnly = true;
            this._logBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this._logBox.Size = new System.Drawing.Size(720, 432);
            this._logBox.TabIndex = 0;
            this._logBox.WordWrap = true;
            // 
            // buttonBar
            // 
            this.buttonBar.AutoSize = true;
            this.buttonBar.Controls.Add(this.closeButton);
            this.buttonBar.Controls.Add(this.copyButton);
            this.buttonBar.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonBar.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.buttonBar.Location = new System.Drawing.Point(0, 432);
            this.buttonBar.Name = "buttonBar";
            this.buttonBar.Padding = new System.Windows.Forms.Padding(0, 8, 0, 0);
            this.buttonBar.Size = new System.Drawing.Size(720, 48);
            this.buttonBar.TabIndex = 1;
            this.buttonBar.WrapContents = false;
            // 
            // closeButton
            // 
            this.closeButton.AutoSize = true;
            this.closeButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.closeButton.Location = new System.Drawing.Point(653, 11);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(64, 25);
            this.closeButton.TabIndex = 0;
            this.closeButton.Text = "Close";
            this.closeButton.UseVisualStyleBackColor = true;
            // 
            // copyButton
            // 
            this.copyButton.AutoSize = true;
            this.copyButton.Location = new System.Drawing.Point(571, 11);
            this.copyButton.Margin = new System.Windows.Forms.Padding(0, 0, 8, 0);
            this.copyButton.Name = "copyButton";
            this.copyButton.Size = new System.Drawing.Size(74, 25);
            this.copyButton.TabIndex = 1;
            this.copyButton.Text = "Copy log";
            this.copyButton.UseVisualStyleBackColor = true;
            this.copyButton.Click += new System.EventHandler(this.copyButton_Click);
            // 
            // BingoRerollLogDialog
            // 
            this.AcceptButton = this.closeButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(720, 480);
            this.Controls.Add(this._logBox);
            this.Controls.Add(this.buttonBar);
            this.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.MinimumSize = new System.Drawing.Size(640, 420);
            this.Name = "BingoRerollLogDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Bingo Reroll Log";
            this.buttonBar.ResumeLayout(false);
            this.buttonBar.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.TextBox _logBox;
        private System.Windows.Forms.FlowLayoutPanel buttonBar;
        private System.Windows.Forms.Button closeButton;
        private System.Windows.Forms.Button copyButton;
    }
}
