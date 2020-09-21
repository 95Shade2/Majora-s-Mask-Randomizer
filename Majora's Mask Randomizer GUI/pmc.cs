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
        public partial class pmc : Form
        {
                public Main_Window parent;

                public pmc()
                {
                        InitializeComponent();
                }

                private void pmc_Load(object sender, EventArgs e)
                {
                        ItemSelect_Button.BackColor = parent.Game_Colors["Item"];
                        MapScreen_Button.BackColor = parent.Game_Colors["Map"];
                        Quest_Button.BackColor = parent.Game_Colors["Quest"];
                        MaskScreen_Button.BackColor = parent.Game_Colors["Mask"];
                        NamePlate_Button.BackColor = parent.Game_Colors["Name"];
                        LinkColor_Button.BackColor = parent.Game_Colors["Link"];
                        DekuColor_Button.BackColor = parent.Game_Colors["Deku"];
                        GoronColor_Button.BackColor = parent.Game_Colors["Goron"];
                        ZoraColor_Button.BackColor = parent.Game_Colors["Zora"];
                        FDColor_Button.BackColor = parent.Game_Colors["FD"];
                }

                private void ItemSelect_Button_Click(object sender, EventArgs e)
                {
                        ItemSelect_ColorBox.ShowDialog();
                        ItemSelect_Button.BackColor = ItemSelect_ColorBox.Color;
                }

                private void MapScreen_Button_Click(object sender, EventArgs e)
                {
                        MapScreen_ColorBox.ShowDialog();
                        MapScreen_Button.BackColor = MapScreen_ColorBox.Color;
                }

                private void Quest_Button_Click(object sender, EventArgs e)
                {
                        Quest_ColorBox.ShowDialog();
                        Quest_Button.BackColor = Quest_ColorBox.Color;
                }

                private void MaskScreen_Button_Click(object sender, EventArgs e)
                {
                        MaskScreen_ColorBox.ShowDialog();
                        MaskScreen_Button.BackColor = MaskScreen_ColorBox.Color;
                }

                private void Confirm_Button_Click(object sender, EventArgs e)
                {
                        parent.Game_Colors["Item"] = ItemSelect_Button.BackColor;
                        parent.Game_Colors["Map"] = MapScreen_Button.BackColor;
                        parent.Game_Colors["Quest"] = Quest_Button.BackColor;
                        parent.Game_Colors["Mask"] = MaskScreen_Button.BackColor;
                        parent.Game_Colors["Name"] = NamePlate_Button.BackColor;
                        parent.Game_Colors["Link"] = LinkColor_Button.BackColor;
                        parent.Game_Colors["Deku"] = DekuColor_Button.BackColor;
                        parent.Game_Colors["Goron"] = GoronColor_Button.BackColor;
                        parent.Game_Colors["Zora"] = ZoraColor_Button.BackColor;
                        parent.Game_Colors["FD"] = FDColor_Button.BackColor;

                        this.Close();
                }

                private void NamePlate_Button_Click(object sender, EventArgs e)
                {
                        NamePlate_BolorBox.ShowDialog();
                        NamePlate_Button.BackColor = NamePlate_BolorBox.Color;
                }
                
                private void LinkColor_Button_Click(object sender, EventArgs e)
                {
                        LinkColor_ColorBox.ShowDialog();
                        LinkColor_Button.BackColor = LinkColor_ColorBox.Color;
                }

                private void DekuColor_Button_Click(object sender, EventArgs e)
                {
                        DekuColor_ColorBox.ShowDialog();
                        DekuColor_Button.BackColor = DekuColor_ColorBox.Color;
                }

                private void GoronColor_Button_Click(object sender, EventArgs e)
                {
                        GoronColor_ColorBox.ShowDialog();
                        GoronColor_Button.BackColor = GoronColor_ColorBox.Color;
                }

                private void ZoraColor_Button_Click(object sender, EventArgs e)
                {
                        ZoraColor_ColorBox.ShowDialog();
                        ZoraColor_Button.BackColor = ZoraColor_ColorBox.Color;
                }

                private void FDColor_Button_Click(object sender, EventArgs e)
                {
                        FDColor_ColorBox.ShowDialog();
                        FDColor_Button.BackColor = FDColor_ColorBox.Color;
                }
        }
}
