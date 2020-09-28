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

                private Dictionary<CheckBox, Button> Check_To_Button;
                private Dictionary<CheckBox, ColorDialog> Check_To_Color;
                private Dictionary<CheckBox, string> Check_To_ID;
                Random rnd;

                public pmc()
                {
                        InitializeComponent();
                }

                private void pmc_Load(object sender, EventArgs e)
                {
                        Check_To_Button = new Dictionary<CheckBox, Button>();
                        Check_To_Color = new Dictionary<CheckBox, ColorDialog>();
                        Check_To_ID = new Dictionary<CheckBox, string>();
                        rnd = new Random();

                        //checkbox to buttons
                        Check_To_Button.Add(LinkColor_Checkbox, LinkColor_Button);
                        Check_To_Button.Add(DekuColor_Checkbox, DekuColor_Button);
                        Check_To_Button.Add(GoronColor_Checkbox, GoronColor_Button);
                        Check_To_Button.Add(ZoraColor_Checkbox, ZoraColor_Button);
                        Check_To_Button.Add(FDColor_Checkbox, FDColor_Button);

                        Check_To_Button.Add(ItemScreen_Checkbox, ItemSelect_Button);
                        Check_To_Button.Add(MapScreen_Checkbox, MapScreen_Button);
                        Check_To_Button.Add(QuestScreen_Checkbox, Quest_Button);
                        Check_To_Button.Add(MaskScreen_Checkbox, MaskScreen_Button);
                        Check_To_Button.Add(NamePlate_Checkbox, NamePlate_Button);

                        //checkbox to color dialog boxes
                        Check_To_Color.Add(LinkColor_Checkbox, LinkColor_ColorBox);
                        Check_To_Color.Add(DekuColor_Checkbox, DekuColor_ColorBox);
                        Check_To_Color.Add(GoronColor_Checkbox, GoronColor_ColorBox);
                        Check_To_Color.Add(ZoraColor_Checkbox, ZoraColor_ColorBox);
                        Check_To_Color.Add(FDColor_Checkbox, FDColor_ColorBox);

                        Check_To_Color.Add(ItemScreen_Checkbox, ItemSelect_ColorBox);
                        Check_To_Color.Add(MapScreen_Checkbox, MapScreen_ColorBox);
                        Check_To_Color.Add(QuestScreen_Checkbox, Quest_ColorBox);
                        Check_To_Color.Add(MaskScreen_Checkbox, MaskScreen_ColorBox);
                        Check_To_Color.Add(NamePlate_Checkbox, NamePlate_BolorBox);

                        //checkbox to color text ids
                        Check_To_ID.Add(LinkColor_Checkbox, "Link");
                        Check_To_ID.Add(DekuColor_Checkbox, "Deku");
                        Check_To_ID.Add(GoronColor_Checkbox, "Goron");
                        Check_To_ID.Add(ZoraColor_Checkbox, "Zora");
                        Check_To_ID.Add(FDColor_Checkbox, "FD");

                        Check_To_ID.Add(ItemScreen_Checkbox, "Item");
                        Check_To_ID.Add(MapScreen_Checkbox, "Map");
                        Check_To_ID.Add(QuestScreen_Checkbox, "Quest");
                        Check_To_ID.Add(MaskScreen_Checkbox, "Mask");
                        Check_To_ID.Add(NamePlate_Checkbox, "Name");


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

                        ItemSelect_ColorBox.Color = parent.Game_Colors["Item"];
                        MapScreen_ColorBox.Color = parent.Game_Colors["Map"];
                        Quest_ColorBox.Color = parent.Game_Colors["Quest"];
                        MaskScreen_ColorBox.Color = parent.Game_Colors["Mask"];
                        NamePlate_BolorBox.Color = parent.Game_Colors["Name"];
                        LinkColor_ColorBox.Color = parent.Game_Colors["Link"];
                        DekuColor_ColorBox.Color = parent.Game_Colors["Deku"];
                        GoronColor_ColorBox.Color = parent.Game_Colors["Goron"];
                        ZoraColor_ColorBox.Color = parent.Game_Colors["Zora"];
                        FDColor_ColorBox.Color = parent.Game_Colors["FD"];
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

                private Color Random_Color()
                {
                        int red = rnd.Next(1, 256);  // creates a number between 1 and 255
                        int blue = rnd.Next(1, 256);
                        int green = rnd.Next(1, 256);

                        return parent.Rgb_Color(red, green, blue);
                }

                private void Confirm_Button_Click(object sender, EventArgs e)
                {
                        Dictionary<CheckBox, Button>.KeyCollection keys = Check_To_Button.Keys;

                        for (int k = 0; k < keys.Count; k++)
                        {
                                CheckBox key = keys.ElementAt(k);
                                string id = Check_To_ID[key];
                                Color clr;

                                //random color
                                if (key.Checked)
                                {
                                        clr = Random_Color();
                                }
                                //user specified color
                                else
                                {
                                        clr = Check_To_Button[key].BackColor;
                                }

                                parent.Game_Colors[id] = clr;
                        }

                        //parent.Game_Colors["Item"] = ItemSelect_Button.BackColor;
                        //parent.Game_Colors["Map"] = MapScreen_Button.BackColor;
                        //parent.Game_Colors["Quest"] = Quest_Button.BackColor;
                        //parent.Game_Colors["Mask"] = MaskScreen_Button.BackColor;
                        //parent.Game_Colors["Name"] = NamePlate_Button.BackColor;
                        //parent.Game_Colors["Link"] = LinkColor_Button.BackColor;
                        //parent.Game_Colors["Deku"] = DekuColor_Button.BackColor;
                        //parent.Game_Colors["Goron"] = GoronColor_Button.BackColor;
                        //parent.Game_Colors["Zora"] = ZoraColor_Button.BackColor;
                        //parent.Game_Colors["FD"] = FDColor_Button.BackColor;

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

                private void LinkColor_Checkbox_CheckedChanged(object sender, EventArgs e)
                {
                        CheckBox CheckboxColor = (CheckBox)sender;
                        Button ButtonColor = Check_To_Button[CheckboxColor];
                        ColorDialog ColorColor = Check_To_Color[CheckboxColor];

                        ButtonColor.Enabled = !CheckboxColor.Checked;

                        if (ButtonColor.Enabled)
                        {
                                ButtonColor.BackColor = ColorColor.Color;
                        }
                        else
                        {
                                ButtonColor.BackColor = DefaultBackColor;
                        }
                }
        }
}
