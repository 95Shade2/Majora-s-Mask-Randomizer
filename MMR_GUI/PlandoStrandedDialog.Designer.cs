namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class PlandoStrandedDialog
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
            this.layoutTable = new System.Windows.Forms.TableLayoutPanel();
            this.promptLabel = new System.Windows.Forms.Label();
            this._locationCombo = new System.Windows.Forms.ComboBox();
            this.buttonBar = new System.Windows.Forms.FlowLayoutPanel();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.layoutTable.SuspendLayout();
            this.buttonBar.SuspendLayout();
            this.SuspendLayout();
            // 
            // layoutTable
            // 
            this.layoutTable.AutoSize = true;
            this.layoutTable.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.layoutTable.ColumnCount = 1;
            this.layoutTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.layoutTable.Controls.Add(this.promptLabel, 0, 0);
            this.layoutTable.Controls.Add(this._locationCombo, 0, 1);
            this.layoutTable.Controls.Add(this.buttonBar, 0, 2);
            this.layoutTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.layoutTable.Location = new System.Drawing.Point(12, 12);
            this.layoutTable.Name = "layoutTable";
            this.layoutTable.RowCount = 3;
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.AutoSize));
            this.layoutTable.Size = new System.Drawing.Size(360, 120);
            this.layoutTable.TabIndex = 0;
            // 
            // promptLabel
            // 
            this.promptLabel.AutoSize = true;
            this.promptLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.promptLabel.Location = new System.Drawing.Point(3, 0);
            this.promptLabel.Margin = new System.Windows.Forms.Padding(3, 0, 3, 8);
            this.promptLabel.MaximumSize = new System.Drawing.Size(360, 0);
            this.promptLabel.Name = "promptLabel";
            this.promptLabel.Size = new System.Drawing.Size(354, 15);
            this.promptLabel.TabIndex = 0;
            this.promptLabel.Text = "Choose a location:";
            // 
            // _locationCombo
            // 
            this._locationCombo.Dock = System.Windows.Forms.DockStyle.Fill;
            this._locationCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._locationCombo.Location = new System.Drawing.Point(3, 26);
            this._locationCombo.Margin = new System.Windows.Forms.Padding(3, 3, 3, 12);
            this._locationCombo.Name = "_locationCombo";
            this._locationCombo.Size = new System.Drawing.Size(354, 23);
            this._locationCombo.TabIndex = 1;
            // 
            // buttonBar
            // 
            this.buttonBar.AutoSize = true;
            this.buttonBar.Controls.Add(this.okButton);
            this.buttonBar.Controls.Add(this.cancelButton);
            this.buttonBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonBar.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.buttonBar.Location = new System.Drawing.Point(3, 64);
            this.buttonBar.Name = "buttonBar";
            this.buttonBar.Size = new System.Drawing.Size(354, 53);
            this.buttonBar.TabIndex = 2;
            this.buttonBar.WrapContents = false;
            // 
            // okButton
            // 
            this.okButton.AutoSize = true;
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(252, 3);
            this.okButton.Margin = new System.Windows.Forms.Padding(8, 3, 3, 3);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(99, 25);
            this.okButton.TabIndex = 0;
            this.okButton.Text = "Add Plando";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.AutoSize = true;
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(171, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 25);
            this.cancelButton.TabIndex = 1;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // PlandoStrandedDialog
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(384, 144);
            this.Controls.Add(this.layoutTable);
            this.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "PlandoStrandedDialog";
            this.Padding = new System.Windows.Forms.Padding(12);
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Place Stranded Item";
            this.layoutTable.ResumeLayout(false);
            this.layoutTable.PerformLayout();
            this.buttonBar.ResumeLayout(false);
            this.buttonBar.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel layoutTable;
        private System.Windows.Forms.Label promptLabel;
        private System.Windows.Forms.ComboBox _locationCombo;
        private System.Windows.Forms.FlowLayoutPanel buttonBar;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
    }
}
