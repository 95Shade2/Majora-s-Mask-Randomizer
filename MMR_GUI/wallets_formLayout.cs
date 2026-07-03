using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class wallets_form
    {
        private void ApplyModernLayout()
        {
            SuspendLayout();

            StartPosition = FormStartPosition.CenterParent;
            AutoScaleMode = AutoScaleMode.Dpi;
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;

            TableLayoutPanel table = new TableLayoutPanel
            {
                AutoSize = true,
                ColumnCount = 2,
                RowCount = 4,
                Padding = new Padding(16, 12, 16, 12)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 120F));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            table.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            table.Controls.Add(new Label { Text = "Child", AutoSize = true, Anchor = AnchorStyles.Left, Margin = new Padding(0, 6, 12, 6) }, 0, 0);
            table.Controls.Add(SmallWallet_NumBox, 1, 0);
            table.Controls.Add(new Label { Text = "Adult", AutoSize = true, Anchor = AnchorStyles.Left, Margin = new Padding(0, 6, 12, 6) }, 0, 1);
            table.Controls.Add(MediumWallet_NumBox, 1, 1);
            table.Controls.Add(new Label { Text = "Giant", AutoSize = true, Anchor = AnchorStyles.Left, Margin = new Padding(0, 6, 12, 6) }, 0, 2);
            table.Controls.Add(LargeWallet_NumBox, 1, 2);

            ConfirmWallet_Button.AutoSize = true;
            ConfirmWallet_Button.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            ConfirmWallet_Button.Anchor = AnchorStyles.Right;
            ConfirmWallet_Button.Margin = new Padding(0, 12, 0, 0);
            ConfirmWallet_Button.MinimumSize = new Size(100, UiTheme.Current.ButtonMinHeight);

            FlowLayoutPanel buttonBar = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.RightToLeft,
                WrapContents = false,
                Dock = DockStyle.Fill,
                Margin = new Padding(0)
            };
            buttonBar.Controls.Add(ConfirmWallet_Button);
            table.Controls.Add(buttonBar, 0, 3);
            table.SetColumnSpan(buttonBar, 2);

            Controls.Clear();
            Controls.Add(table);

            AutoSize = true;
            AutoSizeMode = AutoSizeMode.GrowAndShrink;
            Padding = new Padding(0);

            ResumeLayout(true);
            UiTheme.ApplyToForm(this);
        }
    }
}
