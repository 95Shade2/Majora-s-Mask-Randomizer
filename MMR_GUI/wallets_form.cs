using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class wallets_form : Form
    {
        public Main_Window parent;
        public bool showing;

        Dictionary<string, int> Wallet_Sizes;

        public wallets_form()
        {
            InitializeComponent();
            FitCompactLayout();
        }

        public void FitCompactLayout()
        {
            AutoScaleMode = AutoScaleMode.None;
            AutoSize = false;
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;

            layoutTable.AutoSize = true;
            layoutTable.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            layoutTable.Dock = DockStyle.None;
            layoutTable.Location = Point.Empty;

            foreach (NumericUpDown numBox in new[] { SmallWallet_NumBox, MediumWallet_NumBox, LargeWallet_NumBox })
            {
                numBox.Width = 120;
                numBox.Anchor = AnchorStyles.Left;
                numBox.Dock = DockStyle.None;
            }

            buttonBar.Dock = DockStyle.None;
            buttonBar.AutoSize = true;
            ConfirmWallet_Button.AutoSize = true;
            ConfirmWallet_Button.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            ConfirmWallet_Button.Anchor = AnchorStyles.Right;
            ConfirmWallet_Button.Margin = new Padding(0, 12, 0, 0);
            ConfirmWallet_Button.MinimumSize = new Size(100, 30);

            layoutTable.PerformLayout();
            int width = Math.Max(220, layoutTable.PreferredSize.Width);
            int height = Math.Max(140, layoutTable.PreferredSize.Height);
            ClientSize = new Size(width, height);
        }

        private void wallets_form_Load(object sender, EventArgs e)
        {
            UiTheme.ApplyToForm(this);
            FitCompactLayout();

            Wallet_Sizes = new Dictionary<string, int>();
            showing = true;

            Wallet_Sizes.Add("small", parent.Wallet_Sizes["small"]);
            Wallet_Sizes.Add("medium", parent.Wallet_Sizes["medium"]);
            Wallet_Sizes.Add("large", parent.Wallet_Sizes["large"]);

            Update_GUI();
        }

        private void Update_GUI()
        {
            SmallWallet_NumBox.Value = Wallet_Sizes["small"];
            MediumWallet_NumBox.Value = Wallet_Sizes["medium"];
            LargeWallet_NumBox.Value = Wallet_Sizes["large"];
        }

        private void ConfirmWallet_Button_Click(object sender, EventArgs e)
        {
            parent.Wallet_Sizes["small"] = Wallet_Sizes["small"];
            parent.Wallet_Sizes["medium"] = Wallet_Sizes["medium"];
            parent.Wallet_Sizes["large"] = Wallet_Sizes["large"];

            this.Close();
        }

        private void SmallWallet_NumBox_ValueChanged(object sender, EventArgs e)
        {
            Wallet_Sizes["small"] = (int)SmallWallet_NumBox.Value;
        }

        private void MediumWallet_NumBox_ValueChanged(object sender, EventArgs e)
        {
            Wallet_Sizes["medium"] = (int)MediumWallet_NumBox.Value;
        }

        private void LargeWallet_NumBox_ValueChanged(object sender, EventArgs e)
        {
            Wallet_Sizes["large"] = (int)LargeWallet_NumBox.Value;
        }

        private void wallets_form_FormClosing(object sender, FormClosingEventArgs e)
        {
            parent.Wallet_Form = new wallets_form();
            showing = false;
        }
    }
}
