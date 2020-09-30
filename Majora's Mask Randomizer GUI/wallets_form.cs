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
                Dictionary<string, int> Wallet_Sizes;

                public wallets_form()
                {
                        InitializeComponent();
                }

                private void wallets_form_Load(object sender, EventArgs e)
                {
                        Wallet_Sizes = new Dictionary<string, int>();

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
        }
}
