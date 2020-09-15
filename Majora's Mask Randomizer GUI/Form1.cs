using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace Majora_s_Mask_Randomizer_GUI
{
        public partial class Main_Window : Form
        {
                Dictionary<int, string> Item_Pools_Keys;
                Dictionary<string, Dictionary<int, string>> Item_Pools;
                Dictionary<string, RichTextBox> Pool_Tables;
                Dictionary<string, string> Combo_To_Item;
                Dictionary<string, ComboBox> Checkbox_To_Combobox;
                Dictionary<string, ComboBox> Checkbox_To_Combobox2;
                Dictionary<int, string> Checkboxes_Names;
                Dictionary<string, CheckBox> Checkbox_List;
                Dictionary<int, string> Item_Names;
                Dictionary<string, Dictionary<string, Dictionary<string, string>>> Presets;
                Dictionary<int, string> Preset_Keys;
                Process Rando;

                public Main_Window()
                {
                        InitializeComponent();
                }

                private void Main_Window_Load(object sender, EventArgs e)
                {
                        Item_Pools_Keys = new Dictionary<int, string>();
                        Item_Pools = new Dictionary<string, Dictionary<int, string>>();
                        Pool_Tables = new Dictionary<string, RichTextBox>();
                        Combo_To_Item = new Dictionary<string, string>();
                        Checkbox_To_Combobox = new Dictionary<string, ComboBox>();
                        Checkbox_To_Combobox2 = new Dictionary<string, ComboBox>();
                        Checkboxes_Names = new Dictionary<int, string>();
                        Item_Names = new Dictionary<int, string>();
                        Checkbox_List = new Dictionary<string, CheckBox>();
                        Presets = new Dictionary<string, Dictionary<string, Dictionary<string, string>>>();
                        Preset_Keys = new Dictionary<int, string>();

                        Item_Pools_Keys.Add(0, "Items");

                        //create the empty pool list on the right
                        for (int i = 0; i < Item_Pools_Keys.Count; i++)
                        {
                                Item_Pools.Add(Item_Pools_Keys[i], new Dictionary<int, string>());

                                Add_Pools(Item_Pools_Keys[i]);

                                TabPage New_Tab = new TabPage(Item_Pools_Keys[i]);
                                RichTextBox Pool_Textbox = new RichTextBox();

                                Pool_Textbox.Width = Pool_Tabs.Width - 8;
                                Pool_Textbox.ReadOnly = true;
                                Pool_Textbox.Height = Pool_Tabs.Height - 8;
                                Pool_Tables.Add(Item_Pools_Keys[i], Pool_Textbox);

                                New_Tab.Controls.Add(Pool_Textbox);

                                Pool_Tabs.TabPages.Add(New_Tab);
                        }

                        //create the iten names
                        Update_Item_Names();
                        //create the checkbox names
                        Update_Check_Names();
                        //create the combobox to item name relationship
                        Update_Combo();
                        //create the checkbox name to comboboxes relationship
                        Update_Check_To_Combo();
                        //create a list of the textboxes
                        Update_Checkbox_List();

                        //update the presets combobox
                        Load_Presets();

                        //update the logic combobox
                        Load_Logic();

                        //tooltip for scrub that sells magic beans
                        //Scrub_Sells_Beans_Tooltip.SetToolTip(Scrub_Sells_Beans_Checkbox, "If checked, the deku scrub salesman in Swamp will sell you\nMagic Beans instead if what they are randomized to (He still\nchecks to see if you have Magic Beans in inventory before selling)");

                        Tunic_Button.BackColor = Tunic_ColorDialog.Color;
                }

                private void Load_Logic()
                {
                        //create the logic folder if it does not exist
                        if (!System.IO.Directory.Exists("./logic"))
                        {
                                System.IO.Directory.CreateDirectory("./logic");
                        }

                        string[] logics = Directory.GetFiles("./logic", "*.txt");
                        string logic = "";

                        Logic_Combobox.Items.Add("None");

                        for (int i = 0; i < logics.Length; i++)
                        {
                                logic = logics[i].Substring(8);  //remove the folder path
                                logic = logic.Substring(0, logic.IndexOf('.')); //remove the extension
                                Logic_Combobox.Items.Add(logic);
                        }

                        Logic_Combobox.SelectedIndex = 0;       //default logic is none
                }

                private void Load_Presets()
                {
                        //create the presets folder if it does not exist
                        if (!System.IO.Directory.Exists("./presets"))
                        {
                                System.IO.Directory.CreateDirectory("./presets");
                        }

                        string[] presets = Directory.GetFiles("./presets", "*.ini");
                        Dictionary<string, Dictionary<string, string>> File_Contents;
                        string file = "";

                        Presets = new Dictionary<string, Dictionary<string, Dictionary<string, string>>>();
                        Presets_Combobox.Items.Clear(); //Clear the combobox

                        for (int i = 0; i < presets.Length; i++)
                        {
                                file = presets[i].Substring(presets[i].LastIndexOf('\\') + 1);
                                file = file.Remove(file.LastIndexOf('.'));
                                Presets_Combobox.Items.Add(file);

                                File_Contents = OpenAsIni(presets[i]);
                                Presets.Add(file, File_Contents);
                                Preset_Keys.Add(Preset_Keys.Count, file);       //add key to list of keys (preset names)
                        }
                }

                private Dictionary<string, Dictionary<string, string>> OpenAsIni(string filename)
                {
                        Dictionary<string, Dictionary<string, string>> contents = new Dictionary<string, Dictionary<string, string>>();
                        StreamReader file;
                        string line = "";
                        string section = "";
                        string key = "";
                        string value = "";

                        file = new StreamReader(filename);

                        while (!file.EndOfStream)
                        {
                                line = file.ReadLine();

                                //if the name of a section
                                if (line[0] == '[')
                                {
                                        section = line.Replace("[", "").Replace("]", "");        //remove the brackets to get the section name
                                        contents.Add(section, new Dictionary<string, string>()); //create an empty dictionary with the section as the key
                                }
                                else
                                {
                                        key = line.Remove(line.IndexOf('='));
                                        value = line.Substring(line.IndexOf('=') + 1);

                                        contents[section].Add(key, value);
                                }
                        }

                        return contents;
                }

                private void checkBox1_CheckedChanged(object sender, EventArgs e)
                {
                        bool check = (sender as CheckBox).Checked;

                        Checkbox_To_Combobox[(sender as CheckBox).Name].Enabled = check;
                        Checkbox_To_Combobox2[(sender as CheckBox).Name].Enabled = check;

                        if (check)
                        {
                                Checkbox_To_Combobox[(sender as CheckBox).Name].SelectedIndex = 0;
                        }
                        else
                        {
                                Checkbox_To_Combobox[(sender as CheckBox).Name].SelectedIndex = -1;
                                Checkbox_To_Combobox2[(sender as CheckBox).Name].SelectedIndex = -1;
                        }

                        Update_Pools();
                }

                private void All_Night_Mask_Pool_SelectedIndexChanged(object sender, EventArgs e)
                {
                        if (Checkbox_To_Combobox[(sender as ComboBox).Name.Remove((sender as ComboBox).Name.IndexOf("_Pool"))].SelectedIndex != -1)
                        {
                                Checkbox_To_Combobox2[(sender as ComboBox).Name.Remove((sender as ComboBox).Name.IndexOf("_Pool"))].SelectedIndex = -1;
                        }

                        Remove_Item_Pool(Combo_To_Item[(sender as ComboBox).Name]);
                        if ((sender as ComboBox).Text != "") {
                                Add_Item_Pool(Item_Pools[(sender as ComboBox).Text], Combo_To_Item[(sender as ComboBox).Name]);
                        }

                        Update_Pools();
                }

                private void Add_Item_Pool(Dictionary<int, string> Pool, string item)
                {
                        Pool.Add(Pool.Count, item);
                }

                private void Remove_Item_Pool(string item)
                {
                        for (int p = 0; p < Item_Pools_Keys.Count; p++)
                        {
                                string Pool = Item_Pools_Keys[p];
                                int index = -1;

                                for (int i = 0; i < Item_Pools[Pool].Count; i++)
                                {
                                        if (Item_Pools[Pool][i] == item)
                                        {
                                                index = i;
                                        }
                                }

                                if (index != -1)
                                {
                                        Item_Pools[Pool] = Remove_Dictionary(Item_Pools[Pool], index);
                                }
                        }
                }

                private Dictionary<int, string> Remove_Dictionary(Dictionary<int, string> Dict, int Key)
                {
                        Dictionary<int, string> New_Dict = new Dictionary<int, string>();

                        for (int i = 0; i < Dict.Count; i++)
                        {
                                if (i < Key)
                                {
                                        New_Dict.Add(i, Dict[i]);
                                }
                                else if (i > Key)
                                {
                                        New_Dict.Add(i - 1, Dict[i]);
                                }
                        }

                        return New_Dict;
                }

                ///<summery>
                ///Add the pool name to items comboboxes
                ///Also adds the pool name to the change and remove comboboxes
                ///</summery>
                private void Add_Pools(string Pool)
                {
                        Adult_Wallet_Pool.Items.Add(Pool);
                        All_Night_Mask_Pool.Items.Add(Pool);
                        Big_Bomb_Bag_Pool.Items.Add(Pool);
                        Big_Poe_Pool.Items.Add(Pool);
                        Biggest_Bomb_Bag_Pool.Items.Add(Pool);
                        Blast_Mask_Pool.Items.Add(Pool);
                        Blue_Potion_Pool.Items.Add(Pool);
                        Bomb_Bag_Pool.Items.Add(Pool);
                        Bombers_Notebook_Pool.Items.Add(Pool);
                        Bow_Pool.Items.Add(Pool);
                        Bremen_Mask_Pool.Items.Add(Pool);
                        Bugs_Pool.Items.Add(Pool);
                        Bunny_Hood_Pool.Items.Add(Pool);
                        Captains_Hat_Pool.Items.Add(Pool);
                        Chateau_Romani_Pool.Items.Add(Pool);
                        Circus_Leaders_Mask_Pool.Items.Add(Pool);
                        Couples_Mask_Pool.Items.Add(Pool);
                        Deku_Mask_Pool.Items.Add(Pool);
                        Deku_Nuts_Pool.Items.Add(Pool);
                        Deku_Nuts_10_Pool.Items.Add(Pool);
                        Deku_Princess_Pool.Items.Add(Pool);
                        Deku_Stick_Pool.Items.Add(Pool);
                        Don_Geros_Mask_Pool.Items.Add(Pool);
                        Elegy_Of_Emptiness_Pool.Items.Add(Pool);
                        Eponas_Song_Pool.Items.Add(Pool);
                        Express_Letter_To_Mama_Pool.Items.Add(Pool);
                        Fairy_Pool.Items.Add(Pool);
                        Fierce_Deity_Mask_Pool.Items.Add(Pool);
                        Fire_Arrow_Pool.Items.Add(Pool);
                        Fish_Pool.Items.Add(Pool);
                        Garo_Mask_Pool.Items.Add(Pool);
                        Giant_Wallet_Pool.Items.Add(Pool);
                        Giants_Mask_Pool.Items.Add(Pool);
                        Gibdo_Mask_Pool.Items.Add(Pool);
                        Gilded_Sword_Pool.Items.Add(Pool);
                        Gold_Dust_Pool.Items.Add(Pool);
                        Goron_Lullaby_Pool.Items.Add(Pool);
                        Goron_Mask_Pool.Items.Add(Pool);
                        Great_Fairys_Mask_Pool.Items.Add(Pool);
                        Great_Fairys_Sword_Pool.Items.Add(Pool);
                        Green_Potion_Pool.Items.Add(Pool);
                        Heart_Container_Pool.Items.Add(Pool);
                        Heart_Piece_Pool.Items.Add(Pool);
                        Heros_Shield_Pool.Items.Add(Pool);
                        Hookshot_Pool.Items.Add(Pool);
                        Hot_Spring_Water_Pool.Items.Add(Pool);
                        Ice_Arrow_Pool.Items.Add(Pool);
                        Kafeis_Mask_Pool.Items.Add(Pool);
                        Kamaros_Mask_Pool.Items.Add(Pool);
                        Keaton_Mask_Pool.Items.Add(Pool);
                        Kokiri_Sword_Pool.Items.Add(Pool);
                        Land_Title_Deed_Pool.Items.Add(Pool);
                        Large_Quiver_Pool.Items.Add(Pool);
                        Largest_Quiver_Pool.Items.Add(Pool);
                        Lens_Of_Truth_Pool.Items.Add(Pool);
                        Letter_To_Kafei_Pool.Items.Add(Pool);
                        Light_Arrow_Pool.Items.Add(Pool);
                        Magic_Beans_Pool.Items.Add(Pool);
                        Mask_Of_Scents_Pool.Items.Add(Pool);
                        Mask_Of_Truth_Pool.Items.Add(Pool);
                        Milk_Pool.Items.Add(Pool);
                        Mirror_Shield_Pool.Items.Add(Pool);
                        Moons_Tear_Pool.Items.Add(Pool);
                        Mountain_Title_Deed_Pool.Items.Add(Pool);
                        Mushroom_Pool.Items.Add(Pool);
                        New_Wave_Bossa_Nova_Pool.Items.Add(Pool);
                        Oath_To_Order_Pool.Items.Add(Pool);
                        Ocean_Title_Deed_Pool.Items.Add(Pool);
                        Pendant_Of_Memories_Pool.Items.Add(Pool);
                        Pictograph_Box_Pool.Items.Add(Pool);
                        Poe_Pool.Items.Add(Pool);
                        Postmans_Hat_Pool.Items.Add(Pool);
                        Powder_Keg_Pool.Items.Add(Pool);
                        Razor_Sword_Pool.Items.Add(Pool);
                        Red_Potion_Pool.Items.Add(Pool);
                        Romanis_Mask_Pool.Items.Add(Pool);
                        Room_Key_Pool.Items.Add(Pool);
                        Seahorse_Pool.Items.Add(Pool);
                        Sonata_Of_Awakening_Pool.Items.Add(Pool);
                        Song_Of_Healing_Pool.Items.Add(Pool);
                        Song_Of_Soaring_Pool.Items.Add(Pool);
                        Song_Of_Storms_Pool.Items.Add(Pool);
                        Spring_Water_Pool.Items.Add(Pool);
                        Stone_Mask_Pool.Items.Add(Pool);
                        Swamp_Title_Deed_Pool.Items.Add(Pool);
                        Zora_Egg_Pool.Items.Add(Pool);
                        Zora_Mask_Pool.Items.Add(Pool);
                        Clocktown_Map_Pool.Items.Add(Pool);
                        Woodfall_Map_Pool.Items.Add(Pool);
                        Snowhead_Map_Pool.Items.Add(Pool);
                        Romani_Ranch_Map_Pool.Items.Add(Pool);
                        Great_Bay_Map_Pool.Items.Add(Pool);
                        Stone_Tower_Map_Pool.Items.Add(Pool);

                        Change_Pool_Name_Combobox.Items.Add(Pool);
                        Remove_Pool_Combobox.Items.Add(Pool);
                }

                private void Update_Item_Names()
                {
                        int index = 0;

                        Item_Names.Add(index++, "Adult Wallet");
                        Item_Names.Add(index++, "All-Night Mask");
                        Item_Names.Add(index++, "Big Bomb Bag");
                        Item_Names.Add(index++, "Big Poe");
                        Item_Names.Add(index++, "Biggest Bomb Bag");
                        Item_Names.Add(index++, "Blast Mask");
                        Item_Names.Add(index++, "Blue Potion");
                        Item_Names.Add(index++, "Bomb Bag");
                        Item_Names.Add(index++, "Bomber's Notebook");
                        Item_Names.Add(index++, "Bow");
                        Item_Names.Add(index++, "Bremen Mask");
                        Item_Names.Add(index++, "Bugs");
                        Item_Names.Add(index++, "Bunny Hood");
                        Item_Names.Add(index++, "Captain's Hat");
                        Item_Names.Add(index++, "Chateau Romani");
                        Item_Names.Add(index++, "Circus Leader's Mask");
                        Item_Names.Add(index++, "Clocktown Map");
                        Item_Names.Add(index++, "Couple's Mask");
                        Item_Names.Add(index++, "Deku Mask");
                        Item_Names.Add(index++, "Deku Nuts");
                        Item_Names.Add(index++, "Deku Nuts (10)");
                        Item_Names.Add(index++, "Deku Princess");
                        Item_Names.Add(index++, "Deku Stick");
                        Item_Names.Add(index++, "Don Gero's Mask");
                        Item_Names.Add(index++, "Elegy of Emptiness");
                        Item_Names.Add(index++, "Epona's Song");
                        Item_Names.Add(index++, "Express Letter to Mama");
                        Item_Names.Add(index++, "Fairy");
                        Item_Names.Add(index++, "Fierce Deity Mask");
                        Item_Names.Add(index++, "Fire Arrow");
                        Item_Names.Add(index++, "Fish");
                        Item_Names.Add(index++, "Garo Mask");
                        Item_Names.Add(index++, "Giant Wallet");
                        Item_Names.Add(index++, "Giant's Mask");
                        Item_Names.Add(index++, "Gibdo Mask");
                        Item_Names.Add(index++, "Gilded Sword");
                        Item_Names.Add(index++, "Gold Dust");
                        Item_Names.Add(index++, "Goron Lullaby");
                        Item_Names.Add(index++, "Goron Mask");
                        Item_Names.Add(index++, "Great Bay Map");
                        Item_Names.Add(index++, "Great Fairy's Mask");
                        Item_Names.Add(index++, "Great Fairy's Sword");
                        Item_Names.Add(index++, "Green Potion");
                        Item_Names.Add(index++, "Heart Container");
                        Item_Names.Add(index++, "Heart Piece");
                        Item_Names.Add(index++, "Hero's Shield");
                        Item_Names.Add(index++, "Hookshot");
                        Item_Names.Add(index++, "Hot Spring Water");
                        Item_Names.Add(index++, "Ice Arrow");
                        Item_Names.Add(index++, "Kafei's Mask");
                        Item_Names.Add(index++, "Kamaro's Mask");
                        Item_Names.Add(index++, "Keaton Mask");
                        Item_Names.Add(index++, "Kokiri Sword");
                        Item_Names.Add(index++, "Land Title Deed");
                        Item_Names.Add(index++, "Large Quiver");
                        Item_Names.Add(index++, "Largest Quiver");
                        Item_Names.Add(index++, "Lens of Truth");
                        Item_Names.Add(index++, "Letter to Kafei");
                        Item_Names.Add(index++, "Light Arrow");
                        Item_Names.Add(index++, "Magic Beans");
                        Item_Names.Add(index++, "Mask of Scents");
                        Item_Names.Add(index++, "Mask of Truth");
                        Item_Names.Add(index++, "Milk");
                        Item_Names.Add(index++, "Mirror Shield");
                        Item_Names.Add(index++, "Moon's Tear");
                        Item_Names.Add(index++, "Mountain Title Deed");
                        Item_Names.Add(index++, "Mushroom");
                        Item_Names.Add(index++, "New Wave Bossa Nova");
                        Item_Names.Add(index++, "Oath to Order");
                        Item_Names.Add(index++, "Ocean Title Deed");
                        Item_Names.Add(index++, "Pendant of Memories");
                        Item_Names.Add(index++, "Pictograph Box");
                        Item_Names.Add(index++, "Poe");
                        Item_Names.Add(index++, "Postman's Hat");
                        Item_Names.Add(index++, "Powder Keg");
                        Item_Names.Add(index++, "Razor Sword");
                        Item_Names.Add(index++, "Red Potion");
                        Item_Names.Add(index++, "Romani Ranch Map");
                        Item_Names.Add(index++, "Romani's Mask");
                        Item_Names.Add(index++, "Room Key");
                        Item_Names.Add(index++, "Seahorse");
                        Item_Names.Add(index++, "Sonata of Awakening");
                        Item_Names.Add(index++, "Song of Healing");
                        Item_Names.Add(index++, "Song of Soaring");
                        Item_Names.Add(index++, "Song of Storms");
                        Item_Names.Add(index++, "Snowhead Map");
                        Item_Names.Add(index++, "Spring Water");
                        Item_Names.Add(index++, "Stone Mask");
                        Item_Names.Add(index++, "Stone Tower Map");
                        Item_Names.Add(index++, "Swamp Title Deed");
                        Item_Names.Add(index++, "Woodfall Map");
                        Item_Names.Add(index++, "Zora Egg");
                        Item_Names.Add(index++, "Zora Mask");
                }

                private void Update_Check_Names()
                {
                        int index = 0;

                        Checkboxes_Names.Add(index++, "Adult_Wallet");
                        Checkboxes_Names.Add(index++, "All_Night_Mask");
                        Checkboxes_Names.Add(index++, "Big_Bomb_Bag");
                        Checkboxes_Names.Add(index++, "Big_Poe");
                        Checkboxes_Names.Add(index++, "Biggest_Bomb_Bag");
                        Checkboxes_Names.Add(index++, "Blast_Mask");
                        Checkboxes_Names.Add(index++, "Blue_Potion");
                        Checkboxes_Names.Add(index++, "Bomb_Bag");
                        Checkboxes_Names.Add(index++, "Bombers_Notebook");
                        Checkboxes_Names.Add(index++, "Bow");
                        Checkboxes_Names.Add(index++, "Bremen_Mask");
                        Checkboxes_Names.Add(index++, "Bugs");
                        Checkboxes_Names.Add(index++, "Bunny_Hood");
                        Checkboxes_Names.Add(index++, "Captains_Hat");
                        Checkboxes_Names.Add(index++, "Chateau_Romani");
                        Checkboxes_Names.Add(index++, "Circus_Leaders_Mask");
                        Checkboxes_Names.Add(index++, "Clocktown_Map");
                        Checkboxes_Names.Add(index++, "Couples_Mask");
                        Checkboxes_Names.Add(index++, "Deku_Mask");
                        Checkboxes_Names.Add(index++, "Deku_Nuts");
                        Checkboxes_Names.Add(index++, "Deku_Nuts_10");
                        Checkboxes_Names.Add(index++, "Deku_Princess");
                        Checkboxes_Names.Add(index++, "Deku_Stick");
                        Checkboxes_Names.Add(index++, "Don_Geros_Mask");
                        Checkboxes_Names.Add(index++, "Elegy_Of_Emptiness");
                        Checkboxes_Names.Add(index++, "Eponas_Song");
                        Checkboxes_Names.Add(index++, "Express_Letter_To_Mama");
                        Checkboxes_Names.Add(index++, "Fairy");
                        Checkboxes_Names.Add(index++, "Fierce_Deity_Mask");
                        Checkboxes_Names.Add(index++, "Fire_Arrow");
                        Checkboxes_Names.Add(index++, "Fish");
                        Checkboxes_Names.Add(index++, "Garo_Mask");
                        Checkboxes_Names.Add(index++, "Giant_Wallet");
                        Checkboxes_Names.Add(index++, "Giants_Mask");
                        Checkboxes_Names.Add(index++, "Gibdo_Mask");
                        Checkboxes_Names.Add(index++, "Gilded_Sword");
                        Checkboxes_Names.Add(index++, "Gold_Dust");
                        Checkboxes_Names.Add(index++, "Goron_Lullaby");
                        Checkboxes_Names.Add(index++, "Goron_Mask");
                        Checkboxes_Names.Add(index++, "Great_Bay_Map");
                        Checkboxes_Names.Add(index++, "Great_Fairys_Mask");
                        Checkboxes_Names.Add(index++, "Great_Fairys_Sword");
                        Checkboxes_Names.Add(index++, "Green_Potion");
                        Checkboxes_Names.Add(index++, "Heart_Container");
                        Checkboxes_Names.Add(index++, "Heart_Piece");
                        Checkboxes_Names.Add(index++, "Heros_Shield");
                        Checkboxes_Names.Add(index++, "Hookshot");
                        Checkboxes_Names.Add(index++, "Hot_Spring_Water");
                        Checkboxes_Names.Add(index++, "Ice_Arrow");
                        Checkboxes_Names.Add(index++, "Kafeis_Mask");
                        Checkboxes_Names.Add(index++, "Kamaros_Mask");
                        Checkboxes_Names.Add(index++, "Keaton_Mask");
                        Checkboxes_Names.Add(index++, "Kokiri_Sword");
                        Checkboxes_Names.Add(index++, "Land_Title_Deed");
                        Checkboxes_Names.Add(index++, "Large_Quiver");
                        Checkboxes_Names.Add(index++, "Largest_Quiver");
                        Checkboxes_Names.Add(index++, "Lens_Of_Truth");
                        Checkboxes_Names.Add(index++, "Letter_To_Kafei");
                        Checkboxes_Names.Add(index++, "Light_Arrow");
                        Checkboxes_Names.Add(index++, "Magic_Beans");
                        Checkboxes_Names.Add(index++, "Mask_Of_Scents");
                        Checkboxes_Names.Add(index++, "Mask_Of_Truth");
                        Checkboxes_Names.Add(index++, "Milk");
                        Checkboxes_Names.Add(index++, "Mirror_Shield");
                        Checkboxes_Names.Add(index++, "Moons_Tear");
                        Checkboxes_Names.Add(index++, "Mountain_Title_Deed");
                        Checkboxes_Names.Add(index++, "Mushroom");
                        Checkboxes_Names.Add(index++, "New_Wave_Bossa_Nova");
                        Checkboxes_Names.Add(index++, "Oath_To_Order");
                        Checkboxes_Names.Add(index++, "Ocean_Title_Deed");
                        Checkboxes_Names.Add(index++, "Pendant_Of_Memories");
                        Checkboxes_Names.Add(index++, "Pictograph_Box");
                        Checkboxes_Names.Add(index++, "Poe");
                        Checkboxes_Names.Add(index++, "Postmans_Hat");
                        Checkboxes_Names.Add(index++, "Powder_Keg");
                        Checkboxes_Names.Add(index++, "Razor_Sword");
                        Checkboxes_Names.Add(index++, "Red_Potion");
                        Checkboxes_Names.Add(index++, "Romani_Ranch_Map");
                        Checkboxes_Names.Add(index++, "Romanis_Mask");
                        Checkboxes_Names.Add(index++, "Room_Key");
                        Checkboxes_Names.Add(index++, "Seahorse");
                        Checkboxes_Names.Add(index++, "Sonata_Of_Awakening");
                        Checkboxes_Names.Add(index++, "Song_Of_Healing");
                        Checkboxes_Names.Add(index++, "Song_Of_Soaring");
                        Checkboxes_Names.Add(index++, "Song_Of_Storms");
                        Checkboxes_Names.Add(index++, "Snowhead_Map");
                        Checkboxes_Names.Add(index++, "Spring_Water");
                        Checkboxes_Names.Add(index++, "Stone_Mask");
                        Checkboxes_Names.Add(index++, "Stone_Tower_Map");
                        Checkboxes_Names.Add(index++, "Swamp_Title_Deed");
                        Checkboxes_Names.Add(index++, "Woodfall_Map");
                        Checkboxes_Names.Add(index++, "Zora_Egg");
                        Checkboxes_Names.Add(index++, "Zora_Mask");
                }

                private void Update_Check_To_Combo()
                {
                        Checkbox_To_Combobox.Add("Adult_Wallet", Adult_Wallet_Pool);
                        Checkbox_To_Combobox.Add("All_Night_Mask", All_Night_Mask_Pool);
                        Checkbox_To_Combobox.Add("Big_Bomb_Bag", Big_Bomb_Bag_Pool);
                        Checkbox_To_Combobox.Add("Big_Poe", Big_Poe_Pool);
                        Checkbox_To_Combobox.Add("Biggest_Bomb_Bag", Biggest_Bomb_Bag_Pool);
                        Checkbox_To_Combobox.Add("Blast_Mask", Blast_Mask_Pool);
                        Checkbox_To_Combobox.Add("Blue_Potion", Blue_Potion_Pool);
                        Checkbox_To_Combobox.Add("Bomb_Bag", Bomb_Bag_Pool);
                        Checkbox_To_Combobox.Add("Bombers_Notebook", Bombers_Notebook_Pool);
                        Checkbox_To_Combobox.Add("Bow", Bow_Pool);
                        Checkbox_To_Combobox.Add("Bremen_Mask", Bremen_Mask_Pool);
                        Checkbox_To_Combobox.Add("Bugs", Bugs_Pool);
                        Checkbox_To_Combobox.Add("Bunny_Hood", Bunny_Hood_Pool);
                        Checkbox_To_Combobox.Add("Captains_Hat", Captains_Hat_Pool);
                        Checkbox_To_Combobox.Add("Chateau_Romani", Chateau_Romani_Pool);
                        Checkbox_To_Combobox.Add("Circus_Leaders_Mask", Circus_Leaders_Mask_Pool);
                        Checkbox_To_Combobox.Add("Clocktown_Map", Clocktown_Map_Pool);
                        Checkbox_To_Combobox.Add("Couples_Mask", Couples_Mask_Pool);
                        Checkbox_To_Combobox.Add("Deku_Mask", Deku_Mask_Pool);
                        Checkbox_To_Combobox.Add("Deku_Nuts", Deku_Nuts_Pool);
                        Checkbox_To_Combobox.Add("Deku_Nuts_10", Deku_Nuts_10_Pool);
                        Checkbox_To_Combobox.Add("Deku_Princess", Deku_Princess_Pool);
                        Checkbox_To_Combobox.Add("Deku_Stick", Deku_Stick_Pool);
                        Checkbox_To_Combobox.Add("Don_Geros_Mask", Don_Geros_Mask_Pool);
                        Checkbox_To_Combobox.Add("Elegy_Of_Emptiness", Elegy_Of_Emptiness_Pool);
                        Checkbox_To_Combobox.Add("Eponas_Song", Eponas_Song_Pool);
                        Checkbox_To_Combobox.Add("Express_Letter_To_Mama", Express_Letter_To_Mama_Pool);
                        Checkbox_To_Combobox.Add("Fairy", Fairy_Pool);
                        Checkbox_To_Combobox.Add("Fierce_Deity_Mask", Fierce_Deity_Mask_Pool);
                        Checkbox_To_Combobox.Add("Fire_Arrow", Fire_Arrow_Pool);
                        Checkbox_To_Combobox.Add("Fish", Fish_Pool);
                        Checkbox_To_Combobox.Add("Garo_Mask", Garo_Mask_Pool);
                        Checkbox_To_Combobox.Add("Giant_Wallet", Giant_Wallet_Pool);
                        Checkbox_To_Combobox.Add("Giants_Mask", Giants_Mask_Pool);
                        Checkbox_To_Combobox.Add("Gibdo_Mask", Gibdo_Mask_Pool);
                        Checkbox_To_Combobox.Add("Gilded_Sword", Gilded_Sword_Pool);
                        Checkbox_To_Combobox.Add("Gold_Dust", Gold_Dust_Pool);
                        Checkbox_To_Combobox.Add("Goron_Lullaby", Goron_Lullaby_Pool);
                        Checkbox_To_Combobox.Add("Goron_Mask", Goron_Mask_Pool);
                        Checkbox_To_Combobox.Add("Great_Bay_Map", Great_Bay_Map_Pool);
                        Checkbox_To_Combobox.Add("Great_Fairys_Mask", Great_Fairys_Mask_Pool);
                        Checkbox_To_Combobox.Add("Great_Fairys_Sword", Great_Fairys_Sword_Pool);
                        Checkbox_To_Combobox.Add("Green_Potion", Green_Potion_Pool);
                        Checkbox_To_Combobox.Add("Heart_Container", Heart_Container_Pool);
                        Checkbox_To_Combobox.Add("Heart_Piece", Heart_Piece_Pool);
                        Checkbox_To_Combobox.Add("Heros_Shield", Heros_Shield_Pool);
                        Checkbox_To_Combobox.Add("Hookshot", Hookshot_Pool);
                        Checkbox_To_Combobox.Add("Hot_Spring_Water", Hot_Spring_Water_Pool);
                        Checkbox_To_Combobox.Add("Ice_Arrow", Ice_Arrow_Pool);
                        Checkbox_To_Combobox.Add("Kafeis_Mask", Kafeis_Mask_Pool);
                        Checkbox_To_Combobox.Add("Kamaros_Mask", Kamaros_Mask_Pool);
                        Checkbox_To_Combobox.Add("Keaton_Mask", Keaton_Mask_Pool);
                        Checkbox_To_Combobox.Add("Kokiri_Sword", Kokiri_Sword_Pool);
                        Checkbox_To_Combobox.Add("Land_Title_Deed", Land_Title_Deed_Pool);
                        Checkbox_To_Combobox.Add("Large_Quiver", Large_Quiver_Pool);
                        Checkbox_To_Combobox.Add("Largest_Quiver", Largest_Quiver_Pool);
                        Checkbox_To_Combobox.Add("Lens_Of_Truth", Lens_Of_Truth_Pool);
                        Checkbox_To_Combobox.Add("Letter_To_Kafei", Letter_To_Kafei_Pool);
                        Checkbox_To_Combobox.Add("Light_Arrow", Light_Arrow_Pool);
                        Checkbox_To_Combobox.Add("Magic_Beans", Magic_Beans_Pool);
                        Checkbox_To_Combobox.Add("Mask_Of_Scents", Mask_Of_Scents_Pool);
                        Checkbox_To_Combobox.Add("Mask_Of_Truth", Mask_Of_Truth_Pool);
                        Checkbox_To_Combobox.Add("Milk", Milk_Pool);
                        Checkbox_To_Combobox.Add("Mirror_Shield", Mirror_Shield_Pool);
                        Checkbox_To_Combobox.Add("Moons_Tear", Moons_Tear_Pool);
                        Checkbox_To_Combobox.Add("Mountain_Title_Deed", Mountain_Title_Deed_Pool);
                        Checkbox_To_Combobox.Add("Mushroom", Mushroom_Pool);
                        Checkbox_To_Combobox.Add("New_Wave_Bossa_Nova", New_Wave_Bossa_Nova_Pool);
                        Checkbox_To_Combobox.Add("Oath_To_Order", Oath_To_Order_Pool);
                        Checkbox_To_Combobox.Add("Ocean_Title_Deed", Ocean_Title_Deed_Pool);
                        Checkbox_To_Combobox.Add("Pendant_Of_Memories", Pendant_Of_Memories_Pool);
                        Checkbox_To_Combobox.Add("Pictograph_Box", Pictograph_Box_Pool);
                        Checkbox_To_Combobox.Add("Poe", Poe_Pool);
                        Checkbox_To_Combobox.Add("Postmans_Hat", Postmans_Hat_Pool);
                        Checkbox_To_Combobox.Add("Powder_Keg", Powder_Keg_Pool);
                        Checkbox_To_Combobox.Add("Razor_Sword", Razor_Sword_Pool);
                        Checkbox_To_Combobox.Add("Red_Potion", Red_Potion_Pool);
                        Checkbox_To_Combobox.Add("Romani_Ranch_Map", Romani_Ranch_Map_Pool);
                        Checkbox_To_Combobox.Add("Romanis_Mask", Romanis_Mask_Pool);
                        Checkbox_To_Combobox.Add("Room_Key", Room_Key_Pool);
                        Checkbox_To_Combobox.Add("Seahorse", Seahorse_Pool);
                        Checkbox_To_Combobox.Add("Sonata_Of_Awakening", Sonata_Of_Awakening_Pool);
                        Checkbox_To_Combobox.Add("Song_Of_Healing", Song_Of_Healing_Pool);
                        Checkbox_To_Combobox.Add("Song_Of_Soaring", Song_Of_Soaring_Pool);
                        Checkbox_To_Combobox.Add("Song_Of_Storms", Song_Of_Storms_Pool);
                        Checkbox_To_Combobox.Add("Snowhead_Map", Snowhead_Map_Pool);
                        Checkbox_To_Combobox.Add("Spring_Water", Spring_Water_Pool);
                        Checkbox_To_Combobox.Add("Stone_Mask", Stone_Mask_Pool);
                        Checkbox_To_Combobox.Add("Stone_Tower_Map", Stone_Tower_Map_Pool);
                        Checkbox_To_Combobox.Add("Swamp_Title_Deed", Swamp_Title_Deed_Pool);
                        Checkbox_To_Combobox.Add("Woodfall_Map", Woodfall_Map_Pool);
                        Checkbox_To_Combobox.Add("Zora_Egg", Zora_Egg_Pool);
                        Checkbox_To_Combobox.Add("Zora_Mask", Zora_Mask_Pool);
                        
                        Checkbox_To_Combobox2.Add("Adult_Wallet", Adult_Wallet_Gives);
                        Checkbox_To_Combobox2.Add("All_Night_Mask", All_Night_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Big_Bomb_Bag", Big_Bomb_Bag_Gives);
                        Checkbox_To_Combobox2.Add("Big_Poe", Big_Poe_Gives);
                        Checkbox_To_Combobox2.Add("Biggest_Bomb_Bag", Biggest_Bomb_Bag_Gives);
                        Checkbox_To_Combobox2.Add("Blast_Mask", Blast_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Blue_Potion", Blue_Potion_Gives);
                        Checkbox_To_Combobox2.Add("Bomb_Bag", Bomb_Bag_Gives);
                        Checkbox_To_Combobox2.Add("Bombers_Notebook", Bombers_Notebook_Gives);
                        Checkbox_To_Combobox2.Add("Bow", Bow_Gives);
                        Checkbox_To_Combobox2.Add("Bremen_Mask", Bremen_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Bugs", Bugs_Gives);
                        Checkbox_To_Combobox2.Add("Bunny_Hood", Bunny_Hood_Gives);
                        Checkbox_To_Combobox2.Add("Captains_Hat", Captains_Hat_Gives);
                        Checkbox_To_Combobox2.Add("Chateau_Romani", Chateau_Romani_Gives);
                        Checkbox_To_Combobox2.Add("Circus_Leaders_Mask", Circus_Leaders_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Clocktown_Map", Clocktown_Map_Gives);
                        Checkbox_To_Combobox2.Add("Couples_Mask", Couples_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Deku_Mask", Deku_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Deku_Nuts", Deku_Nuts_Gives);
                        Checkbox_To_Combobox2.Add("Deku_Nuts_10", Deku_Nuts_10_Gives);
                        Checkbox_To_Combobox2.Add("Deku_Princess", Deku_Princess_Gives);
                        Checkbox_To_Combobox2.Add("Deku_Stick", Deku_Stick_Gives);
                        Checkbox_To_Combobox2.Add("Don_Geros_Mask", Don_Geros_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Elegy_Of_Emptiness", Elegy_Of_Emptiness_Gives);
                        Checkbox_To_Combobox2.Add("Eponas_Song", Eponas_Song_Gives);
                        Checkbox_To_Combobox2.Add("Express_Letter_To_Mama", Express_Letter_To_Mama_Gives);
                        Checkbox_To_Combobox2.Add("Fairy", Fairy_Gives);
                        Checkbox_To_Combobox2.Add("Fierce_Deity_Mask", Fierce_Deity_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Fire_Arrow", Fire_Arrow_Gives);
                        Checkbox_To_Combobox2.Add("Fish", Fish_Gives);
                        Checkbox_To_Combobox2.Add("Garo_Mask", Garo_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Giant_Wallet", Giant_Wallet_Gives);
                        Checkbox_To_Combobox2.Add("Giants_Mask", Giants_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Gibdo_Mask", Gibdo_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Gilded_Sword", Gilded_Sword_Gives);
                        Checkbox_To_Combobox2.Add("Gold_Dust", Gold_Dust_Gives);
                        Checkbox_To_Combobox2.Add("Goron_Lullaby", Goron_Lullaby_Gives);
                        Checkbox_To_Combobox2.Add("Goron_Mask", Goron_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Great_Bay_Map", Great_Bay_Map_Gives);
                        Checkbox_To_Combobox2.Add("Great_Fairys_Mask", Great_Fairys_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Great_Fairys_Sword", Great_Fairys_Sword_Gives);
                        Checkbox_To_Combobox2.Add("Green_Potion", Green_Potion_Gives);
                        Checkbox_To_Combobox2.Add("Heart_Container", Heart_Container_Gives);
                        Checkbox_To_Combobox2.Add("Heart_Piece", Heart_Piece_Gives);
                        Checkbox_To_Combobox2.Add("Heros_Shield", Heros_Shield_Gives);
                        Checkbox_To_Combobox2.Add("Hookshot", Hookshot_Gives);
                        Checkbox_To_Combobox2.Add("Hot_Spring_Water", Hot_Spring_Water_Gives);
                        Checkbox_To_Combobox2.Add("Ice_Arrow", Ice_Arrow_Gives);
                        Checkbox_To_Combobox2.Add("Kafeis_Mask", Kafeis_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Kamaros_Mask", Kamaros_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Keaton_Mask", Keaton_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Kokiri_Sword", Kokiri_Sword_Gives);
                        Checkbox_To_Combobox2.Add("Land_Title_Deed", Land_Title_Deed_Gives);
                        Checkbox_To_Combobox2.Add("Large_Quiver", Large_Quiver_Gives);
                        Checkbox_To_Combobox2.Add("Largest_Quiver", Largest_Quiver_Gives);
                        Checkbox_To_Combobox2.Add("Lens_Of_Truth", Lens_Of_Truth_Gives);
                        Checkbox_To_Combobox2.Add("Letter_To_Kafei", Letter_To_Kafei_Gives);
                        Checkbox_To_Combobox2.Add("Light_Arrow", Light_Arrow_Gives);
                        Checkbox_To_Combobox2.Add("Magic_Beans", Magic_Beans_Gives);
                        Checkbox_To_Combobox2.Add("Mask_Of_Scents", Mask_Of_Scents_Gives);
                        Checkbox_To_Combobox2.Add("Mask_Of_Truth", Mask_Of_Truth_Gives);
                        Checkbox_To_Combobox2.Add("Milk", Milk_Gives);
                        Checkbox_To_Combobox2.Add("Mirror_Shield", Mirror_Shield_Gives);
                        Checkbox_To_Combobox2.Add("Moons_Tear", Moons_Tear_Gives);
                        Checkbox_To_Combobox2.Add("Mountain_Title_Deed", Mountain_Title_Deed_Gives);
                        Checkbox_To_Combobox2.Add("Mushroom", Mushroom_Gives);
                        Checkbox_To_Combobox2.Add("New_Wave_Bossa_Nova", New_Wave_Bossa_Nova_Gives);
                        Checkbox_To_Combobox2.Add("Oath_To_Order", Oath_To_Order_Gives);
                        Checkbox_To_Combobox2.Add("Ocean_Title_Deed", Ocean_Title_Deed_Gives);
                        Checkbox_To_Combobox2.Add("Pendant_Of_Memories", Pendant_Of_Memories_Gives);
                        Checkbox_To_Combobox2.Add("Pictograph_Box", Pictograph_Box_Gives);
                        Checkbox_To_Combobox2.Add("Poe", Poe_Gives);
                        Checkbox_To_Combobox2.Add("Postmans_Hat", Postmans_Hat_Gives);
                        Checkbox_To_Combobox2.Add("Powder_Keg", Powder_Keg_Gives);
                        Checkbox_To_Combobox2.Add("Razor_Sword", Razor_Sword_Gives);
                        Checkbox_To_Combobox2.Add("Red_Potion", Red_Potion_Gives);
                        Checkbox_To_Combobox2.Add("Romani_Ranch_Map", Romani_Ranch_Map_Gives);
                        Checkbox_To_Combobox2.Add("Romanis_Mask", Romanis_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Room_Key", Room_Key_Gives);
                        Checkbox_To_Combobox2.Add("Seahorse", Seahorse_Gives);
                        Checkbox_To_Combobox2.Add("Sonata_Of_Awakening", Sonata_Of_Awakening_Gives);
                        Checkbox_To_Combobox2.Add("Song_Of_Healing", Song_Of_Healing_Gives);
                        Checkbox_To_Combobox2.Add("Song_Of_Soaring", Song_Of_Soaring_Gives);
                        Checkbox_To_Combobox2.Add("Song_Of_Storms", Song_Of_Storms_Gives);
                        Checkbox_To_Combobox2.Add("Snowhead_Map", Snowhead_Map_Gives);
                        Checkbox_To_Combobox2.Add("Spring_Water", Spring_Water_Gives);
                        Checkbox_To_Combobox2.Add("Stone_Mask", Stone_Mask_Gives);
                        Checkbox_To_Combobox2.Add("Stone_Tower_Map", Stone_Tower_Map_Gives);
                        Checkbox_To_Combobox2.Add("Swamp_Title_Deed", Swamp_Title_Deed_Gives);
                        Checkbox_To_Combobox2.Add("Woodfall_Map", Woodfall_Map_Gives);
                        Checkbox_To_Combobox2.Add("Zora_Egg", Zora_Egg_Gives);
                        Checkbox_To_Combobox2.Add("Zora_Mask", Zora_Mask_Gives);
                        
                        for (int i = 0; i < Checkboxes_Names.Count; i++)
                        {
                                for (int ii = 0; ii < Item_Names.Count; ii++)
                                {
                                        Checkbox_To_Combobox2[Checkboxes_Names[i]].Items.Add(Item_Names[ii]);
                                }
                        }
                }

                private void Update_Combo()
                {
                        Combo_To_Item.Add("Adult_Wallet_Pool", "Adult Wallet");
                        Combo_To_Item.Add("All_Night_Mask_Pool", "All-Night Mask");
                        Combo_To_Item.Add("Big_Bomb_Bag_Pool", "Big Bomb Bag");
                        Combo_To_Item.Add("Big_Poe_Pool", "Big Poe");
                        Combo_To_Item.Add("Biggest_Bomb_Bag_Pool", "Biggest Bomb Bag");
                        Combo_To_Item.Add("Blast_Mask_Pool", "Blast Mask");
                        Combo_To_Item.Add("Blue_Potion_Pool", "Blue Potion");
                        Combo_To_Item.Add("Bomb_Bag_Pool", "Bomb Bag");
                        Combo_To_Item.Add("Bombers_Notebook_Pool", "Bomber's Notebook");
                        Combo_To_Item.Add("Bow_Pool", "Bow");
                        Combo_To_Item.Add("Bremen_Mask_Pool", "Bremen Mask");
                        Combo_To_Item.Add("Bugs_Pool", "Bugs");
                        Combo_To_Item.Add("Bunny_Hood_Pool", "Bunny Hood");
                        Combo_To_Item.Add("Captains_Hat_Pool", "Captain's Hat");
                        Combo_To_Item.Add("Chateau_Romani_Pool", "Chateau Romani");
                        Combo_To_Item.Add("Clocktown_Map_Pool", "Clocktown Map");
                        Combo_To_Item.Add("Circus_Leaders_Mask_Pool", "Circus Leader's Mask");
                        Combo_To_Item.Add("Couples_Mask_Pool", "Couple's Mask");
                        Combo_To_Item.Add("Deku_Mask_Pool", "Deku Mask");
                        Combo_To_Item.Add("Deku_Nuts_Pool", "Deku Nuts");
                        Combo_To_Item.Add("Deku_Nuts_10_Pool", "Deku Nuts (10)");
                        Combo_To_Item.Add("Deku_Princess_Pool", "Deku Princess");
                        Combo_To_Item.Add("Deku_Stick_Pool", "Deku Stick");
                        Combo_To_Item.Add("Don_Geros_Mask_Pool", "Don Gero's Mask");
                        Combo_To_Item.Add("Elegy_Of_Emptiness_Pool", "Elegy of Emptiness");
                        Combo_To_Item.Add("Eponas_Song_Pool", "Epona's Song");
                        Combo_To_Item.Add("Express_Letter_To_Mama_Pool", "Express Letter to Mama");
                        Combo_To_Item.Add("Fairy_Pool", "Fairy");
                        Combo_To_Item.Add("Fierce_Deity_Mask_Pool", "Fierce Deity Mask");
                        Combo_To_Item.Add("Fire_Arrow_Pool", "Fire Arrow");
                        Combo_To_Item.Add("Fish_Pool", "Fish");
                        Combo_To_Item.Add("Garo_Mask_Pool", "Garo Mask");
                        Combo_To_Item.Add("Giant_Wallet_Pool", "Giant Wallet");
                        Combo_To_Item.Add("Giants_Mask_Pool", "Giant's Mask");
                        Combo_To_Item.Add("Gibdo_Mask_Pool", "Gibdo Mask");
                        Combo_To_Item.Add("Gilded_Sword_Pool", "Gilded Sword");
                        Combo_To_Item.Add("Gold_Dust_Pool", "Gold Dust");
                        Combo_To_Item.Add("Goron_Lullaby_Pool", "Goron Lullaby");
                        Combo_To_Item.Add("Goron_Mask_Pool", "Goron Mask");
                        Combo_To_Item.Add("Great_Bay_Map_Pool", "Great Bay Map");
                        Combo_To_Item.Add("Great_Fairys_Mask_Pool", "Great Fairy's Mask");
                        Combo_To_Item.Add("Great_Fairys_Sword_Pool", "Great Fairy's Sword");
                        Combo_To_Item.Add("Green_Potion_Pool", "Green Potion");
                        Combo_To_Item.Add("Heart_Container_Pool", "Heart Container");
                        Combo_To_Item.Add("Heart_Piece_Pool", "Heart Piece");
                        Combo_To_Item.Add("Heros_Shield_Pool", "Hero's Shield");
                        Combo_To_Item.Add("Hookshot_Pool", "Hookshot");
                        Combo_To_Item.Add("Hot_Spring_Water_Pool", "Hot Spring Water");
                        Combo_To_Item.Add("Ice_Arrow_Pool", "Ice Arrow");
                        Combo_To_Item.Add("Kafeis_Mask_Pool", "Kafei's Mask");
                        Combo_To_Item.Add("Kamaros_Mask_Pool", "Kamaro's Mask");
                        Combo_To_Item.Add("Keaton_Mask_Pool", "Keaton Mask");
                        Combo_To_Item.Add("Kokiri_Sword_Pool", "Kokiri Sword");
                        Combo_To_Item.Add("Land_Title_Deed_Pool", "Land Title Deed");
                        Combo_To_Item.Add("Large_Quiver_Pool", "Large Quiver");
                        Combo_To_Item.Add("Largest_Quiver_Pool", "Largest Quiver");
                        Combo_To_Item.Add("Lens_Of_Truth_Pool", "Lens of Truth");
                        Combo_To_Item.Add("Letter_To_Kafei_Pool", "Letter to Kafei");
                        Combo_To_Item.Add("Light_Arrow_Pool", "Light Arrow");
                        Combo_To_Item.Add("Magic_Beans_Pool", "Magic Beans");
                        Combo_To_Item.Add("Mask_Of_Scents_Pool", "Mask of Scents");
                        Combo_To_Item.Add("Mask_Of_Truth_Pool", "Mask of Truth");
                        Combo_To_Item.Add("Milk_Pool", "Milk");
                        Combo_To_Item.Add("Mirror_Shield_Pool", "Mirror Shield");
                        Combo_To_Item.Add("Moons_Tear_Pool", "Moon's Tear");
                        Combo_To_Item.Add("Mountain_Title_Deed_Pool", "Mountain Title Deed");
                        Combo_To_Item.Add("Mushroom_Pool", "Mushroom");
                        Combo_To_Item.Add("New_Wave_Bossa_Nova_Pool", "New Wave Bossa Nova");
                        Combo_To_Item.Add("Oath_To_Order_Pool", "Oath to Order");
                        Combo_To_Item.Add("Ocean_Title_Deed_Pool", "Ocean Title Deed");
                        Combo_To_Item.Add("Pendant_Of_Memories_Pool", "Pendant of Memories");
                        Combo_To_Item.Add("Pictograph_Box_Pool", "Pictograph Box");
                        Combo_To_Item.Add("Poe_Pool", "Poe");
                        Combo_To_Item.Add("Postmans_Hat_Pool", "Postman's Hat");
                        Combo_To_Item.Add("Powder_Keg_Pool", "Powder Keg");
                        Combo_To_Item.Add("Razor_Sword_Pool", "Razor Sword");
                        Combo_To_Item.Add("Red_Potion_Pool", "Red Potion");
                        Combo_To_Item.Add("Romani_Ranch_Map_Pool", "Romani Ranch Map");
                        Combo_To_Item.Add("Romanis_Mask_Pool", "Romani's Mask");
                        Combo_To_Item.Add("Room_Key_Pool", "Room Key");
                        Combo_To_Item.Add("Seahorse_Pool", "Seahorse");
                        Combo_To_Item.Add("Sonata_Of_Awakening_Pool", "Sonata of Awakening");
                        Combo_To_Item.Add("Song_Of_Healing_Pool", "Song of Healing");
                        Combo_To_Item.Add("Song_Of_Soaring_Pool", "Song of Soaring");
                        Combo_To_Item.Add("Song_Of_Storms_Pool", "Song of Storms");
                        Combo_To_Item.Add("Snowhead_Map_Pool", "Snowhead Map");
                        Combo_To_Item.Add("Spring_Water_Pool", "Spring Water");
                        Combo_To_Item.Add("Stone_Mask_Pool", "Stone Mask");
                        Combo_To_Item.Add("Stone_Tower_Map_Pool", "Stone Tower Map");
                        Combo_To_Item.Add("Swamp_Title_Deed_Pool", "Swamp Title Deed");
                        Combo_To_Item.Add("Woodfall_Map_Pool", "Woodfall Map");
                        Combo_To_Item.Add("Zora_Egg_Pool", "Zora Egg");
                        Combo_To_Item.Add("Zora_Mask_Pool", "Zora Mask");
                }

                private void Update_Checkbox_List()
                {
                        Checkbox_List.Add("Adult_Wallet", Adult_Wallet);
                        Checkbox_List.Add("All_Night_Mask", All_Night_Mask);
                        Checkbox_List.Add("Big_Bomb_Bag", Big_Bomb_Bag);
                        Checkbox_List.Add("Big_Poe", Big_Poe);
                        Checkbox_List.Add("Biggest_Bomb_Bag", Biggest_Bomb_Bag);
                        Checkbox_List.Add("Blast_Mask", Blast_Mask);
                        Checkbox_List.Add("Blue_Potion", Blue_Potion);
                        Checkbox_List.Add("Bomb_Bag", Bomb_Bag);
                        Checkbox_List.Add("Bombers_Notebook", Bombers_Notebook);
                        Checkbox_List.Add("Bow", Bow);
                        Checkbox_List.Add("Bremen_Mask", Bremen_Mask);
                        Checkbox_List.Add("Bugs", Bugs);
                        Checkbox_List.Add("Bunny_Hood", Bunny_Hood);
                        Checkbox_List.Add("Captains_Hat", Captains_Hat);
                        Checkbox_List.Add("Chateau_Romani", Chateau_Romani);
                        Checkbox_List.Add("Circus_Leaders_Mask", Circus_Leaders_Mask);
                        Checkbox_List.Add("Clocktown_Map", Clocktown_Map);
                        Checkbox_List.Add("Couples_Mask", Couples_Mask);
                        Checkbox_List.Add("Deku_Mask", Deku_Mask);
                        Checkbox_List.Add("Deku_Nuts", Deku_Nuts);
                        Checkbox_List.Add("Deku_Nuts_10", Deku_Nuts_10);
                        Checkbox_List.Add("Deku_Princess", Deku_Princess);
                        Checkbox_List.Add("Deku_Stick", Deku_Stick);
                        Checkbox_List.Add("Don_Geros_Mask", Don_Geros_Mask);
                        Checkbox_List.Add("Elegy_Of_Emptiness", Elegy_Of_Emptiness);
                        Checkbox_List.Add("Eponas_Song", Eponas_Song);
                        Checkbox_List.Add("Express_Letter_To_Mama", Express_Letter_To_Mama);
                        Checkbox_List.Add("Fairy", Fairy);
                        Checkbox_List.Add("Fierce_Deity_Mask", Fierce_Deity_Mask);
                        Checkbox_List.Add("Fire_Arrow", Fire_Arrow);
                        Checkbox_List.Add("Fish", Fish);
                        Checkbox_List.Add("Garo_Mask", Garo_Mask);
                        Checkbox_List.Add("Giant_Wallet", Giant_Wallet);
                        Checkbox_List.Add("Giants_Mask", Giants_Mask);
                        Checkbox_List.Add("Gibdo_Mask", Gibdo_Mask);
                        Checkbox_List.Add("Gilded_Sword", Gilded_Sword);
                        Checkbox_List.Add("Gold_Dust", Gold_Dust);
                        Checkbox_List.Add("Goron_Lullaby", Goron_Lullaby);
                        Checkbox_List.Add("Goron_Mask", Goron_Mask);
                        Checkbox_List.Add("Great_Bay_Map", Great_Bay_Map);
                        Checkbox_List.Add("Great_Fairys_Mask", Great_Fairys_Mask);
                        Checkbox_List.Add("Great_Fairys_Sword", Great_Fairys_Sword);
                        Checkbox_List.Add("Green_Potion", Green_Potion);
                        Checkbox_List.Add("Heart_Container", Heart_Container);
                        Checkbox_List.Add("Heart_Piece", Heart_Piece);
                        Checkbox_List.Add("Heros_Shield", Heros_Shield);
                        Checkbox_List.Add("Hookshot", Hookshot);
                        Checkbox_List.Add("Hot_Spring_Water", Hot_Spring_Water);
                        Checkbox_List.Add("Ice_Arrow", Ice_Arrow);
                        Checkbox_List.Add("Kafeis_Mask", Kafeis_Mask);
                        Checkbox_List.Add("Kamaros_Mask", Kamaros_Mask);
                        Checkbox_List.Add("Keaton_Mask", Keaton_Mask);
                        Checkbox_List.Add("Kokiri_Sword", Kokiri_Sword);
                        Checkbox_List.Add("Land_Title_Deed", Land_Title_Deed);
                        Checkbox_List.Add("Large_Quiver", Large_Quiver);
                        Checkbox_List.Add("Largest_Quiver", Largest_Quiver);
                        Checkbox_List.Add("Lens_Of_Truth", Lens_Of_Truth);
                        Checkbox_List.Add("Letter_To_Kafei", Letter_To_Kafei);
                        Checkbox_List.Add("Light_Arrow", Light_Arrow);
                        Checkbox_List.Add("Magic_Beans", Magic_Beans);
                        Checkbox_List.Add("Mask_Of_Scents", Mask_Of_Scents);
                        Checkbox_List.Add("Mask_Of_Truth", Mask_Of_Truth);
                        Checkbox_List.Add("Milk", Milk);
                        Checkbox_List.Add("Mirror_Shield", Mirror_Shield);
                        Checkbox_List.Add("Moons_Tear", Moons_Tear);
                        Checkbox_List.Add("Mountain_Title_Deed", Mountain_Title_Deed);
                        Checkbox_List.Add("Mushroom", Mushroom);
                        Checkbox_List.Add("New_Wave_Bossa_Nova", New_Wave_Bossa_Nova);
                        Checkbox_List.Add("Oath_To_Order", Oath_To_Order);
                        Checkbox_List.Add("Ocean_Title_Deed", Ocean_Title_Deed);
                        Checkbox_List.Add("Pendant_Of_Memories", Pendant_Of_Memories);
                        Checkbox_List.Add("Pictograph_Box", Pictograph_Box);
                        Checkbox_List.Add("Poe", Poe);
                        Checkbox_List.Add("Postmans_Hat", Postmans_Hat);
                        Checkbox_List.Add("Powder_Keg", Powder_Keg);
                        Checkbox_List.Add("Razor_Sword", Razor_Sword);
                        Checkbox_List.Add("Red_Potion", Red_Potion);
                        Checkbox_List.Add("Romani_Ranch_Map", Romani_Ranch_Map);
                        Checkbox_List.Add("Romanis_Mask", Romanis_Mask);
                        Checkbox_List.Add("Room_Key", Room_Key);
                        Checkbox_List.Add("Seahorse", Seahorse);
                        Checkbox_List.Add("Sonata_Of_Awakening", Sonata_Of_Awakening);
                        Checkbox_List.Add("Song_Of_Healing", Song_Of_Healing);
                        Checkbox_List.Add("Song_Of_Soaring", Song_Of_Soaring);
                        Checkbox_List.Add("Song_Of_Storms", Song_Of_Storms);
                        Checkbox_List.Add("Snowhead_Map", Snowhead_Map);
                        Checkbox_List.Add("Spring_Water", Spring_Water);
                        Checkbox_List.Add("Stone_Mask", Stone_Mask);
                        Checkbox_List.Add("Stone_Tower_Map", Stone_Tower_Map);
                        Checkbox_List.Add("Swamp_Title_Deed", Swamp_Title_Deed);
                        Checkbox_List.Add("Woodfall_Map", Woodfall_Map);
                        Checkbox_List.Add("Zora_Egg", Zora_Egg);
                        Checkbox_List.Add("Zora_Mask", Zora_Mask);
                }

                private void Update_Pools()
                {
                        for (int p = 0; p < Item_Pools_Keys.Count; p++)
                        {
                                string Pool = Item_Pools_Keys[p];
                                RichTextBox Pool_Box = Pool_Tables[Pool];
                                string New_List = "";

                                for (int i = 0; i < Item_Pools[Pool].Count; i++)
                                {
                                        New_List += Item_Pools[Pool][i] + Environment.NewLine;
                                }

                                Pool_Box.Text = New_List;
                        }
                }

                private void Adult_Wallet_Gives_SelectedIndexChanged(object sender, EventArgs e)
                {
                        if ((Checkbox_To_Combobox2[(sender as ComboBox).Name.Remove((sender as ComboBox).Name.IndexOf("_Gives"))].SelectedIndex != -1)) {
                                Checkbox_To_Combobox[(sender as ComboBox).Name.Remove((sender as ComboBox).Name.IndexOf("_Gives"))].SelectedIndex = -1;
                        }

                        Update_Pools();
                }

                private void tabPage3_Click(object sender, EventArgs e)
                {

                }

                delegate void EnableRandoButtonCallback();
                
                private void Enable_Rando_Button()
                {
                        // InvokeRequired required compares the thread ID of the
                        // calling thread to the thread ID of the creating thread.
                        // If these threads are different, it returns true.
                        if (this.Randomize_Button.InvokeRequired)
                        {
                                EnableRandoButtonCallback d = new EnableRandoButtonCallback(Enable_Rando_Button);
                                this.Invoke(d, new object[] {});
                        }
                        else
                        {
                                this.Randomize_Button.Enabled = true;
                        }
                }

                private void Rando_Process_Exited(object sender, EventArgs e)
                {
                        string err_file = Open(".\\Error.txt");

                        if (err_file == "")
                        {
                                MessageBox.Show("All Done");
                                Enable_Rando_Button();
                        }
                        else
                        {
                                Error(err_file);
                                Enable_Rando_Button();
                        }
                }

                private string Open(string filename)
                {
                        string file_contents = "";
                        StreamReader file;

                        try
                        {
                                file = new StreamReader(filename);

                                file_contents = file.ReadToEnd();

                                file.Close();
                        }
                        catch(Exception e)
                        {

                        }


                        return file_contents;
                }

                private void RunRandomizer()
                {
                        try
                        {
                                Rando = Process.Start(".\\Majora's Mask Randomizer.exe");
                                Rando.EnableRaisingEvents = true;
                                Rando.Exited += new EventHandler(Rando_Process_Exited);
                        }
                        catch(Exception e)
                        {
                                Error("Cannot find \"Majora's Mask Randomizer.exe\" Make sure it's in the same directory");
                        }
                }
                
                private void Randomize_Button_Click(object sender, EventArgs e)
                {
                        if (Open_Base_Rom_Dialog.FileName != "openFileDialog1")
                        {
                                Randomize_Button.Enabled = false;
                                SaveSettingsAsIni("./settings.ini");
                                RunRandomizer();
                        }
                        else
                        {
                                MessageBox.Show("No base rom selected", "Error");
                        }
                }

                private void SaveSettingsAsIni(string location, bool items = true, bool settings = true, bool pools = true)
                {
                        string Text = "";
                        if (items) {
                                Text = "[items]\n";

                                for (int i = 0; i < Checkboxes_Names.Count; i++)
                                {
                                        string Item = Item_Names[i];
                                        string Check_Name = Checkboxes_Names[i];
                                        string Item_Pool = "";
                                        System.Windows.Forms.CheckBox Check = Checkbox_List[Check_Name];
                                        System.Windows.Forms.ComboBox Pool = Checkbox_To_Combobox[Check_Name];
                                        System.Windows.Forms.ComboBox Gives = Checkbox_To_Combobox2[Check_Name];

                                        //if item is randomzied (or has a manually inserted item)
                                        if (Check.Checked)
                                        {
                                                //if randomized into a pool
                                                if (Pool.SelectedIndex != -1)
                                                {
                                                        Item_Pool = Pool.Items[Pool.SelectedIndex].ToString();
                                                }
                                                //if manually inserted an item
                                                else if (Gives.SelectedIndex != -1)
                                                {
                                                        Item_Pool = "#" + Gives.Items[Gives.SelectedIndex].ToString();
                                                }

                                                Text += Item + "=" + Item_Pool + "\n";
                                        }
                                }
                        }

                        if (settings)
                        {
                                Text += "[settings]\n";

                                Text += "Rom=" + Open_Base_Rom_Dialog.FileName + "\n";  //rom location
                                Text += "Seed=" + Seed_Textbox.Text + "\n";     //seed
                                Text += "Wad=" + createWadToolStripMenuItem.Checked + "\n";    //true or false to create wad as well

                                if (Logic_Combobox.SelectedIndex != -1)
                                {
                                        Text += "Logic=" + Logic_Combobox.Items[Logic_Combobox.SelectedIndex] + "\n";   //which logic to use
                                }
                                else
                                {
                                        Text += "Logic=\n";     //no logic
                                }

                                Text += "Kafei=" + playAsKafeiToolStripMenuItem.Checked + "\n";       //whether or not to play as Kafei instead of Link

                                Text += "ScrubBeans=" + swampScrubSalesBeansToolStripMenuItem.Checked + "\n";       //whether or not the scrub in swamp sells magic beans or what they're randomized to

                                Text += "Tunic=" + Get_Color_Tunic() + "\n";     //color of the tunics

                                Text += "Remove_Cutscenes=" + removeCutscenesToolStripMenuItem.Checked + "\n";  //whether or not to remove the cutscenes

                                Text += "GC_Hud=" + gCHudToolStripMenuItem.Checked + "\n";      //use or not use the GC Hud
                        }

                        if (pools)
                        {
                                Text += "[pools]\n";

                                for (int i = 0; i < Item_Pools_Keys.Count; i++)
                                {
                                        Text += i + "=" + Item_Pools_Keys[i] + "\n";
                                }
                        }

                        WriteFile(location, Text);
                }

                private string Get_Color_Tunic()
                {
                        string color = "";
                        Color tunic = Tunic_ColorDialog.Color;
                        int red = tunic.R;
                        int blue = tunic.B;
                        int green = tunic.G;

                        color = "[R=" + red.ToString() + ", G=" + green.ToString() + ", B=" + blue.ToString() + "]";

                        return color;
                }

                private void WriteFile(string FileLocation, string Data)
                {
                        try
                        {
                                StreamWriter sw = new StreamWriter(FileLocation);

                                sw.Write(Data);

                                sw.Close();
                        }
                        catch (Exception e)
                        {

                        }
                }

                private void button1_Click(object sender, EventArgs e)
                {
                        Open_Base_Rom_Dialog.ShowDialog();
                }

                private int Count(string text, char chr)
                {
                        int c = 0;

                        for (int i = 0; i < text.Length; i++)
                        {
                                if (text[i] == chr)
                                {
                                        c++;
                                }
                        }

                        return c;
                }

                private void Open_Base_Rom_Dialog_FileOk(object sender, CancelEventArgs e)
                {
                        Update_Rom_Text();
                }

                private void Update_Rom_Text()
                {
                        string Rom_Location;

                        Rom_Location = Open_Base_Rom_Dialog.FileName;

                        if (Count(Rom_Location, '\\') > 2)
                        {
                                Rom_Location = Rom_Location.Remove(Rom_Location.IndexOf('\\') + 1) + "..." + Rom_Location.Substring(Rom_Location.LastIndexOf('\\'));
                        }

                        if (Rom_Location.Length > 21)
                        {
                                Rom_Location = "..." + Rom_Location.Substring(Rom_Location.Length - 21);
                        }

                        Base_Rom_Label.Text = Rom_Location;
                }

                private void Save_Preset_Button_Click(object sender, EventArgs e)
                {
                        string New_Preset;
                        DialogResult Overwrite;

                        New_Preset = Presets_Combobox.Text;

                        if (New_Preset != "")
                        {
                                if (Presets_Combobox.Items.Contains(New_Preset))
                                {
                                        Overwrite = MessageBox.Show("The preset \'" + New_Preset + "\' already exists, do you wish to overwrite it?", "Overwrite", MessageBoxButtons.YesNo);

                                        if (Overwrite == DialogResult.Yes)
                                        {
                                                Save_Preset(New_Preset);
                                        }
                                }
                                else
                                {
                                        Save_Preset(New_Preset);
                                }
                        }
                }

                private void Save_Preset(string New_Preset)
                {
                        //create the presets folder if it does not exist
                        if (!System.IO.Directory.Exists("./presets"))
                        {
                                System.IO.Directory.CreateDirectory("./presets");
                        }

                        SaveSettingsAsIni("./presets/" + New_Preset + ".ini");
                        Presets_Combobox.Text = "";     //clear text
                        MessageBox.Show("Preset Saved", "Saved");
                        Load_Presets(); //update the presets
                }

                private void Load_Preset(string Preset)
                {
                        Dictionary<string, Dictionary<string, string>> Selected_Preset;

                        Selected_Preset = Presets[Preset];

                        Clear_Items();     //make all items not checked
                        Clear_Pools();     //make no pools
                        Clear_Pool_List(); //clear the pool list on the right side

                        Create_Pools(Selected_Preset["pools"]);
                        Update_Pool_List();
                        Update_Items(Selected_Preset["items"]);
                        Update_Settings(Selected_Preset["settings"]);

                        Presets_Combobox.Text = "";
                        
                        //make sure the change and remove combobox are blank now
                        Change_Pool_Name_Combobox.Text = "";
                        Remove_Pool_Combobox.Text = "";
                }

                private void Update_Settings(Dictionary<string, string> settings)
                {
                        if (settings.ContainsKey("Rom") && settings["Rom"] != "" && settings["Rom"] != "openFileDialog1")
                        {
                                Open_Base_Rom_Dialog.FileName = settings["Rom"];
                                Update_Rom_Text();
                        }

                        if (settings.ContainsKey("Seed") && settings["Seed"] != "")
                        {
                                Seed_Textbox.Text = settings["Seed"];
                        }

                        if (settings.ContainsKey("Wad") && settings["Wad"] == "True")
                        {
                                createWadToolStripMenuItem.Checked = true;
                        }

                        if (settings.ContainsKey("Logic") && settings["Logic"] != "")
                        {
                                int log_index;
                                log_index = Logic_Combobox.Items.IndexOf(settings["Logic"]);
                                Logic_Combobox.SelectedIndex = log_index;
                        }

                        if (settings.ContainsKey("Kafei") && settings["Kafei"] == "True")
                        {
                                playAsKafeiToolStripMenuItem.Checked = true;
                        }

                        if (settings.ContainsKey("ScrubBeans") && settings["ScrubBeans"] == "True")
                        {
                                swampScrubSalesBeansToolStripMenuItem.Checked = true;
                        }

                        if (settings.ContainsKey("Tunic"))
                        {
                                Color tunic_color = String_To_Color(settings["Tunic"]);
                                Tunic_ColorDialog.Color = tunic_color;
                                Tunic_Button.BackColor = tunic_color;
                        }

                        if (settings.ContainsKey("Remove_Cutscenes") && settings["Remove_Cutscenes"] == "True")
                        {
                                removeCutscenesToolStripMenuItem.Checked = true;
                        }

                        if (settings.ContainsKey("GC_Hud") && settings["GC_Hud"] == "True")
                        {
                                gCHudToolStripMenuItem.Checked = true;
                        }
                }

                private Color String_To_Color(string color)
                {
                        //color = "[R=X, G=Y, B=Z]" 
                        // OR
                        //color = "[A=B, R=X, G=Y, B=Z]" 
                        Color new_color;
                        string color_text;
                        int r;
                        int g;
                        int b;
                        int start;
                        int end;

                        start = color.IndexOf('R');
                        color = color.Substring(start + 2);     //color = "X G=Y B=Z]"
                        end = color.IndexOf(',');
                        color_text = color.Substring(0, end);  //color_text = "X"
                        r = String_To_Int(color_text); //r = X

                        start = color.IndexOf('G');
                        color = color.Substring(start + 2);     //color = "Y B=Z]"
                        end = color.IndexOf(',');
                        color_text = color.Substring(0, end);  //color_text = "Y"
                        g = String_To_Int(color_text); //g = Y

                        start = color.IndexOf('B');
                        color = color.Substring(start + 2);     //color = "Z]"
                        end = color.IndexOf(']');
                        color_text = color.Substring(0, end);  //color_text = "Z"
                        b = String_To_Int(color_text); //g = Y

                        new_color = Rgb_Color(r, g, b);

                        return new_color;
                }

                private Color Rgb_Color(int red, int green, int blue)
                {
                        int color;

                        //      alpha
                        color = (255 << 24) | (red << 16) | (green << 8) | blue;
                        
                        return Color.FromArgb(color);
                }

                private int String_To_Int(string str)
                {
                        int num = 0;
                        int pos = 1;

                        if (str.Length > 0)
                        {
                                //negative number
                                if (str[0] == '-' && str.Length > 1)
                                {
                                        pos = -1;
                                        str = str.Substring(1);
                                }
                                
                                if (isPosNum(str))
                                {
                                        for (int c = 0; c < str.Length; c++)
                                        {
                                                int cur = str[c] - '0';
                                                num *= 10;
                                                num += cur;
                                        }
                                }
                        }

                        return num * pos;
                }

                private bool isPosNum(string str)
                {
                        //empty string is not a number
                        if (str.Length == 0)
                        {
                                return false;
                        }

                        //not a positive number
                        if (str[0] == '-')
                        {
                                return false;
                        }

                        for (int c = 0; c < str.Length; c++)
                        {
                                char cur = str[c];

                                //if the current char in the string is not a number, then the string is not a number
                                if ((cur - '0') < 0 || (cur - '0') > 9)
                                {
                                        return false;
                                }
                        }

                        return true;
                }

                //converts first letter for each word in a string of text to uppercase
                private string Uppercase(string text)
                {
                        string New_Text = "";
                        string[] words;

                        words = text.Split(' ');
                        
                        for (int i = 0; i < words.Length; i++)
                        {
                                if (i != 0)
                                {
                                        New_Text += " ";
                                }

                                New_Text += words[i][0].ToString().ToUpper() + words[i].Substring(1);
                        }
                        
                        return New_Text;
                }

                private void Update_Items(Dictionary<string, string> Preset_Items)
                {
                        string Item = "";
                        string key = "";
                        string Pool = "";
                        string[] Keys;

                        Keys = Preset_Items.Keys.ToArray();

                        for (int i = 0; i < Keys.Length; i++)
                        {
                                key = Keys[i];

                                //Captilize first letter of each word
                                Item = Uppercase(key);

                                //Keaton Mask => Keaton_Mask (replace spaces)
                                while (Item.Contains(' '))
                                {
                                        Item = Item.Replace(' ', '_');
                                }

                                //Kafei's_Mask => Kafeis_Mask (remove ')
                                while (Item.Contains('\''))
                                {
                                        Item = Item.Remove(Item.IndexOf('\''), 1);
                                }

                                //All-Night_Mask => All_Night_Mask (replace '-' with '_')
                                while (Item.Contains('-'))
                                {
                                        Item = Item.Replace('-', '_');
                                }

                                //Deku_Nuts_(10) => Deku_Nuts_10 (Remove paranthasis)
                                while (Item.Contains('('))
                                {
                                        Item = Item.Remove(Item.IndexOf('('), 1);
                                }
                                while (Item.Contains(')'))
                                {
                                        Item = Item.Remove(Item.IndexOf(')'), 1);
                                }
                                
                                //check the checkbox
                                Checkbox_List[Item].Checked = true;

                                Pool = Preset_Items[key];
                                if (Pool[0] == '#')     //manual item
                                {
                                        //Checkbox_To_Combobox2[Item].SelectedValue = Checkbox_To_Combobox2[Item].Items.IndexOf(Pool.Substring(1));
                                        Checkbox_To_Combobox2[Item].SelectedIndex = Checkbox_To_Combobox2[Item].Items.IndexOf(Pool.Substring(1));
                                }
                                else
                                {
                                        Checkbox_To_Combobox[Item].SelectedIndex = Checkbox_To_Combobox[Item].Items.IndexOf(Pool);
                                }
                        }
                }

                private void Clear_Pool_List() {
                        Pool_Tables = new Dictionary<string, RichTextBox>();
                        Pool_Tabs.TabPages.Clear();
                }

                private void Update_Pool_List()
                {
                        //create the empty pool list on the right
                        for (int i = 0; i < Item_Pools_Keys.Count; i++)
                        {
                                Add_Pool_List(Item_Pools_Keys[i]);
                        }
                }

                private void Create_Pools(Dictionary<string, string> Pool_List)
                {
                        for (int i = 0; i < Pool_List.Count; i++)
                        {
                                Create_Pool(Pool_List[i.ToString()]);
                        }
                }

                private void Create_Pool(string Pool_Name)
                {
                        Item_Pools_Keys.Add(Item_Pools_Keys.Count, Pool_Name);    //pool key list
                        Item_Pools.Add(Pool_Name, new Dictionary<int, string>()); //pool list
                }

                private void Clear_Pools()
                {
                        Item_Pools_Keys = new Dictionary<int, string>();
                        Item_Pools = new Dictionary<string, Dictionary<int, string>>();

                        //clear the list of pool for each item pool combobox
                        for (int i = 0; i < Checkboxes_Names.Count; i++)
                        {
                                Checkbox_To_Combobox[Checkboxes_Names[i]].Items.Clear();
                        }

                        Change_Pool_Name_Combobox.Items.Clear();
                        Remove_Pool_Combobox.Items.Clear();
                }

                private void Clear_Items()
                {
                        string Name = "";

                        for (int i = 0; i < Checkboxes_Names.Count; i++)
                        {
                                Name = Checkboxes_Names[i];
                                
                                if (Checkbox_List[Name].Checked)
                                {
                                        Checkbox_List[Name].Checked = false;
                                }
                        }
                }

                private void Save_Presets_Dialog_FileOk(object sender, CancelEventArgs e)
                {
                        SaveSettingsAsIni(Save_Presets_Dialog.FileName, true, false);
                }

                private void Load_Preset_Button_Click(object sender, EventArgs e)
                {
                        string Preset;

                        Preset = Presets_Combobox.Text;

                        //if the combobox is not empty
                        if (Preset != "")
                        {
                                //if trying to load a valid preset
                                if (Preset_Keys.ContainsValue(Preset))
                                {
                                        Load_Preset(Preset);
                                }
                                else
                                {
                                        MessageBox.Show("Invalid Preset", "Error");
                                }
                        }
                        else
                        {
                                MessageBox.Show("Preset cannot be nothing", "Error");
                        }
                }

                private void Add_Pool_List(string New_Pool)
                {
                        Add_Pools(New_Pool);

                        TabPage New_Tab = new TabPage(New_Pool);
                        RichTextBox Pool_Textbox = new RichTextBox();

                        Pool_Textbox.Width = Pool_Tabs.Width - 8;
                        Pool_Textbox.ReadOnly = true;
                        Pool_Textbox.Height = Pool_Tabs.Height - 8;
                        Pool_Tables.Add(New_Pool, Pool_Textbox);

                        New_Tab.Controls.Add(Pool_Textbox);

                        Pool_Tabs.TabPages.Add(New_Tab);
                }

                private void Create_Pool_Button_Click(object sender, EventArgs e)
                {
                        string New_Pool = Create_Pool_Textbox.Text;

                        if (New_Pool != "")
                        {
                                if (!Item_Pools_Keys.ContainsValue(New_Pool))
                                {
                                        Create_Pool(New_Pool);

                                        Add_Pool_List(New_Pool); //add the new pool to the list on the right

                                        Create_Pool_Textbox.Text = "";
                                }
                                else
                                {
                                        Error("There is already a pool with that name");
                                }
                        }
                        else
                        {
                                Error("You cannot create a pool with no name");
                        }
                }

                private void Error(string Error_Message)
                {
                        MessageBox.Show(Error_Message, "Error");
                }

                private void Change_Pool_Name(string Old, string New)
                {
                        int index = -1;

                        index = Index_Of_Value(Item_Pools_Keys, Old);

                        if (index != -1)
                        {
                                Item_Pools_Keys[index] = New;   //replace the key for the pool name with the new name
                                Item_Pools.Add(New, Item_Pools[Old]);   //add the pool and get the items for the old pool
                                Item_Pools.Remove(Old);         //Remove the old pool items
                                Update_Pools_Changed_Name(Old, New);    //update each combobox pool (replace the old pool name with the new one)
                        }
                        else
                        {       
                                //should never run this, but just in case this happens
                                Error("Could not find the pool with the selected name");
                        }
                }

                private void Update_Pools_Changed_Name(string Old, string New)
                {
                        int index = -1;
                        string Name = "";

                        //get the index of the item for the comboboxes
                        index = Change_Pool_Name_Combobox.Items.IndexOf(Old);

                        //change remove and change comboxoes on top
                        Change_Pool_Name_Combobox.Items[index] = New;
                        Remove_Pool_Combobox.Items[index] = New;

                        //edit the key of the rich textbox inside the tab on the right
                        Pool_Tables.Add(New, Pool_Tables[Old]);
                        Pool_Tables.Remove(Old);

                        //change each item's pool combobox
                        for (int i = 0; i < Checkboxes_Names.Count; i++)
                        {
                                Name = Checkboxes_Names[i];
                                
                                Checkbox_To_Combobox[Name].Items[index] = New;
                        }

                        //change the tab name to the new pool name
                        Pool_Tabs.TabPages[index].Text = New;

                        Change_Pool_Name_Textbox.Text = "";
                }

                private int Index_Of_Value(Dictionary<int, string> data, string value)
                {
                        for (int i = 0; i < data.Count; i++)
                        {
                                if (data[i] == value)
                                {
                                        return i;
                                }
                        }

                        return -1;
                }

                private void Change_Pool_Name_Button_Click(object sender, EventArgs e)
                {
                        string Old_Name = "";
                        string New_Name = "";

                        Old_Name = Change_Pool_Name_Combobox.Text;
                        New_Name = Change_Pool_Name_Textbox.Text;

                        if (Old_Name != "")
                        {
                                if (Item_Pools_Keys.ContainsValue(Old_Name))
                                {
                                        if (New_Name != "") {
                                                if (!Item_Pools_Keys.ContainsValue(New_Name))
                                                {
                                                        Change_Pool_Name(Old_Name, New_Name);
                                                }
                                                else
                                                {
                                                        Error("New pool name cannot be the same as an already existing pool");
                                                }
                                        }
                                        else
                                        {
                                                Error("New pool name cannot be blank");
                                        }
                                }
                                else
                                {
                                        Error("There is no pool with the name \"" + Old_Name + "\"");
                                }
                        }
                        else
                        {
                                Error("No pool name selected");
                        }
                }

                private void Remove_Pool_Combobox_SelectedIndexChanged(object sender, EventArgs e)
                {

                }

                //fix a array-like dictionary if a pair was removed
                private Dictionary<int, string> Fix_Keys(Dictionary<int, string> Object)
                {
                        Dictionary<int, string> New_Obj = new Dictionary<int, string>();
                        int[] Keys = Object.Keys.ToArray();

                        for (int i = 0; i < Keys.Length; i++)
                        {
                                New_Obj[i] = Object[Keys[i]];
                        }

                        return New_Obj;
                }

                private void Remove_Pool(string Pool)
                {
                        int index = -1;
                        string Item = "";

                        index = Index_Of_Value(Item_Pools_Keys, Pool);

                        if (index != -1)
                        {
                                Item_Pools_Keys.Remove(index);
                                Item_Pools_Keys = Fix_Keys(Item_Pools_Keys);

                                Pool_Tabs.TabPages.Remove(Pool_Tabs.TabPages[index]);

                                for (int i = 0; i < Checkboxes_Names.Count; i++)
                                {
                                        Item = Checkboxes_Names[i];

                                        //if this item is in the pool we're removing
                                        if (Checkbox_To_Combobox[Item].SelectedIndex == index)
                                        {
                                                Checkbox_To_Combobox[Item].Items.RemoveAt(index);
                                                
                                                if (Checkbox_To_Combobox[Item].Items.Count != 0)
                                                {
                                                        Checkbox_To_Combobox[Item].SelectedIndex = 0;
                                                }
                                                else
                                                {
                                                        Checkbox_To_Combobox[Item].SelectedIndex = -1;
                                                }
                                        }
                                        //else only delete the pool, the selected index stays the same (it gets updataed automatically if a pool in the middle is removed)
                                        else
                                        {
                                                Checkbox_To_Combobox[Item].Items.RemoveAt(index);
                                        }
                                }

                                Change_Pool_Name_Combobox.Items.RemoveAt(index);
                                Remove_Pool_Combobox.Items.RemoveAt(index);
                        }
                        else
                        {
                                Error("Could not find pool to remove");
                        }
                }

                private void Remove_Pool_Button_Click(object sender, EventArgs e)
                {
                        string Pool = "";

                        Pool = Remove_Pool_Combobox.Text;

                        if (Item_Pools_Keys.ContainsValue(Pool))
                        {
                                Remove_Pool(Pool);
                        }
                        else
                        {
                                Error("There is no pool with that name");
                        }
                }

                private void Scrub_Sells_Beans_Checkbox_MouseHover(object sender, EventArgs e)
                {
                        
                }

                private void Play_As_Kafei_Checkbox_MouseHover(object sender, EventArgs e)
                {

                }

                private void Play_As_Kafei_Checkbox_CheckedChanged(object sender, EventArgs e)
                {

                }

                private void Scrub_Sells_Beans_Checkbox_MouseEnter(object sender, EventArgs e)
                {
                        
                }

                private void Randomize_Button_MouseHover(object sender, EventArgs e)
                {

                }

                private void Tunic_Button_Click(object sender, EventArgs e)
                {
                        Tunic_ColorDialog.ShowDialog();
                        Tunic_Button.BackColor = Tunic_ColorDialog.Color;
                }

                private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
                {

                }

                private void createWadToolStripMenuItem_Click(object sender, EventArgs e)
                {
                        ToolStripMenuItem item = (sender as ToolStripMenuItem);
                        item.Checked = !item.Checked;
                }
        }
}
