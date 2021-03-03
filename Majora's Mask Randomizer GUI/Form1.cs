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
        Dictionary<int,
        string> Item_Pools_Keys;
        Dictionary<string,
        Dictionary<int,
        string>> Item_Pools;
        Dictionary<string,
        RichTextBox> Pool_Tables;
        string[] Item_Names;
        Dictionary<string,
        Dictionary<string,
        Dictionary<string,
        string>>> Presets;
        Dictionary<int,
        string> Preset_Keys;
        Process Rando;
        Dictionary<string,
        Item> Item_Objects;

        public Dictionary<string,
        Color> Game_Colors;
        public Dictionary<string,
        int> Wallet_Sizes;

        public Main_Window()
        {
            InitializeComponent();
        }

        private void Main_Window_Load(object sender, EventArgs e)
        {
            Item_Pools_Keys = new Dictionary<int,
            string>();
            Item_Pools = new Dictionary<string,
            Dictionary<int,
            string>>();
            Pool_Tables = new Dictionary<string,
            RichTextBox>();
            Presets = new Dictionary<string,
            Dictionary<string,
            Dictionary<string,
            string>>>();
            Preset_Keys = new Dictionary<int,
            string>();
            Game_Colors = Default_Pause();
            Wallet_Sizes = Default_Wallets();
            BlastMaskFrames_Num.Value = 310;

            Create_Item_Names();

            Item_Objects = Get_Item_Objects();

            Create_Item_Gives();

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
            
            //update the presets combobox
            Load_Presets();

            //update the logic combobox
            Load_Logic();

            Tunic_Button.BackColor = Tunic_ColorDialog.Color;
        }

        private Dictionary<string,
        Item> Get_Item_Objects()
        {
            Dictionary<string,
            Item> The_Items = new Dictionary<string,
            Item>();

            Form gui = this;

            Dictionary<int,
            TabPage> pages = new Dictionary<int,
            TabPage>();

            TabControl Outer_Tab = (TabControl)gui.Controls["Items_Tab"];

            TabPage Item_Tab = (TabPage)(Outer_Tab.Controls["Items_Items_Tab"]);
            TabControl Item_Sub_Tab = (TabControl)Item_Tab.Controls["Items_Sub_Tab"];
            TabPage Tab_Page = (TabPage)Item_Sub_Tab.Controls["Items_Sub_Tab_1"];
            pages.Add(0, Tab_Page);
            Tab_Page = (TabPage)Item_Sub_Tab.Controls["Items_Sub_Tab_2"];
            pages.Add(1, Tab_Page);
            Tab_Page = (TabPage)Item_Sub_Tab.Controls["Items_Sub_Tab_3"];
            pages.Add(2, Tab_Page);
            Tab_Page = (TabPage)Item_Sub_Tab.Controls["Items_Sub_Tab_4"];
            pages.Add(3, Tab_Page);

            TabPage Mask_Tab = (TabPage)(Outer_Tab.Controls["Mask_Page"]);
            TabControl Mask_Sub_Tab = (TabControl)Mask_Tab.Controls["Mask_Sub_Tab"];
            Tab_Page = (TabPage)Mask_Sub_Tab.Controls["Mask_Page_1"];
            pages.Add(4, Tab_Page);
            Tab_Page = (TabPage)Mask_Sub_Tab.Controls["Mask_Page_2"];
            pages.Add(5, Tab_Page);
            Tab_Page = (TabPage)Mask_Sub_Tab.Controls["Mask_Page_3"];
            pages.Add(6, Tab_Page);

            TabPage Bottle_Page = (TabPage)(Outer_Tab.Controls["Bottle_Page"]);
            TabControl Bottle_Sub_Tab = (TabControl)Bottle_Page.Controls["Bottle_Tab"];
            Tab_Page = (TabPage)Bottle_Sub_Tab.Controls["Bottle_Page_1"];
            pages.Add(7, Tab_Page);
            Tab_Page = (TabPage)Bottle_Sub_Tab.Controls["Bottle_Page_2"];
            pages.Add(8, Tab_Page);

            TabPage Song_Page = (TabPage)(Outer_Tab.Controls["Song_Page"]);
            pages.Add(9, Song_Page);

            TabPage Rupee_Page = (TabPage)(Outer_Tab.Controls["Rupee_Page"]);
            pages.Add(10, Rupee_Page);

            TabPage Other_Page = (TabPage)(Outer_Tab.Controls["Other_Page"]);
            pages.Add(11, Other_Page);

            int i;

            for (i = 0; i < Item_Names.Length; i++)
            {
                string item = Item_Names[i];
                bool found = false;

                item = Text_To_Checkbox(item);

                for (int p = 0; p < pages.Count; p++)
                {
                    TabPage page = pages[p];

                    //this tab page as the checkbox item
                    if (page.Controls[item] != null)
                    {
                        found = true;
                        p = pages.Count;

                        CheckBox check = (CheckBox)page.Controls[item];
                        ComboBox pool = (ComboBox)page.Controls[item + "_Pool"];
                        ComboBox gives = (ComboBox)page.Controls[item + "_Gives"];

                        The_Items.Add(Item_Names[i], new Item(check, pool, gives));
                    }
                }

                if (!found)
                {
                    MessageBox.Show("Could not find " + item);
                    return The_Items;
                }
            }

            return The_Items;
        }

        private string Text_To_Checkbox(string text)
        {
            text = Replace_All(text, ' ', '_');
            text = Replace_All(text, '-', '_');
            text = Remove_All(text, '\'');
            text = Remove_All(text, '(');
            text = Remove_All(text, ')');
            return text;
        }

        private void Create_Item_Gives()
        {
            string Item_Name;
            ComboBox gives;

            //edit each item's gives combobox
            for (int i = 0; i < Item_Names.Length; i++)
            {
                Item_Name = Item_Names[i];
                gives = Item_Objects[Item_Name].Get_Gives();

                for (int s = 0; s < Item_Names.Length; s++)
                {
                    gives.Items.Add(Item_Names[s]);
                }
            }
        }

        private void Create_Item_Names()
        {
            Item_Names = new string[] {
                "Adult Wallet",
                "All-Night Mask",
                "Big Bomb Bag",
                "Big Poe",
                "Poe",
                "Biggest Bomb Bag",
                "Blast Mask",
                "Blue Potion",
                "Blue Rupee",
                "Bomb Bag",
                "Bomber's Notebook",
                "Bow",
                "Bremen Mask",
                "Bugs",
                "Bunny Hood",
                "Captain's Hat",
                "Chateau Romani",
                "Circus Leader's Mask",
                "Couple's Mask",
                "Deku Mask",
                "Deku Nuts",
                "Deku Nuts (10)",
                "Deku Princess",
                "Deku Stick",
                "Don Gero's Mask",
                "Express Letter to Mama",
                "Fairy",
                "Fierce Deity Mask",
                "Fire Arrow",
                "Fish",
                "Garo Mask",
                "Giant Wallet",
                "Giant's Mask",
                "Gibdo Mask",
                "Gilded Sword",
                "Gold Dust",
                "Gold Rupee",
                "Goron Mask",
                "Great Fairy's Mask",
                "Great Fairy's Sword",
                "Green Potion",
                "Green Rupee",
                "Hero's Shield",
                "Hookshot",
                "Hot Spring Water",
                "Ice Arrow",
                "Kafei's Mask",
                "Kamaro's Mask",
                "Keaton Mask",
                "Kokiri Sword",
                "Land Title Deed",
                "Large Quiver",
                "Largest Quiver",
                "Lens of Truth",
                "Letter to Kafei",
                "Light Arrow",
                "Magic Beans",
                "Mask of Scents",
                "Mask of Truth",
                "Milk",
                "Mirror Shield",
                "Moon's Tear",
                "Mountain Title Deed",
                "Mushroom",
                "Ocean Title Deed",
                "Pendant of Memories",
                "Pictograph Box",
                "Postman's Hat",
                "Powder Keg",
                "Purple Rupee",
                "Razor Sword",
                "Red Potion",
                "Red Rupee",
                "Romani's Mask",
                "Room Key",
                "Seahorse",
                "Silver Rupee",
                "Spring Water",
                "Stone Mask",
                "Swamp Title Deed",
                "Zora Egg",
                "Zora Mask",
                "Clocktown Map",
                "Woodfall Map",
                "Snowhead Map",
                "Romani Ranch Map",
                "Great Bay Map",
                "Stone Tower Map",
                "Song of Healing",
                "Song of Storms",
                "Song of Soaring",
                "Epona's Song",
                "Sonata of Awakening",
                "Goron Lullaby",
                "New Wave Bossa Nova",
                "Elegy of Emptiness",
                "Oath to Order",
                "Heart Piece",
                "Heart Container",
                "Bombchu",
                "Bombchus (5)",
                "Bombchus (10)",
                "Odolwa's Remains",
                "Goht's Remains",
                "Gyorg's Remains",
                "Twinmold's Remains"
            };

            //sort the item names
            Array.Sort(Item_Names);
        }

        private string Remove_All(string text, char chr)
        {
            while (text.Contains(chr))
            {
                text = text.Remove(text.IndexOf(chr), 1);
            }
            return text;
        }

        private string Replace_All(string text, char old_char, char new_char)
        {
            while (text.Contains(old_char))
            {
                text = text.Replace(old_char, new_char);
            }
            return text;
        }

        private Dictionary<string,
        int> Default_Wallets()
        {
            Dictionary<string,
            int> wall = new Dictionary<string,
            int>();

            wall.Add("small", 99);
            wall.Add("medium", 200);
            wall.Add("large", 500);

            return wall;
        }

        private Dictionary<string,
        Color> Default_Pause()
        {
            Dictionary<string,
            Color> default_colors = new Dictionary<string,
            Color>();
            Color default_color = Rgb_Color(180, 180, 120);
            Color name = Rgb_Color(150, 140, 90);
            Color def_green = Rgb_Color(30, 105, 27);

            default_colors.Add("Item", default_color);
            default_colors.Add("Map", default_color);
            default_colors.Add("Quest", default_color);
            default_colors.Add("Mask", default_color);
            default_colors.Add("Name", name);
            default_colors.Add("Link", def_green);
            default_colors.Add("Deku", def_green);
            default_colors.Add("Goron", def_green);
            default_colors.Add("Zora", def_green);
            default_colors.Add("FD", def_green);

            return default_colors;
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
                logic = logics[i].Substring(8); //remove the folder path
                logic = logic.Substring(0, logic.IndexOf('.')); //remove the extension
                Logic_Combobox.Items.Add(logic);
            }

            Logic_Combobox.SelectedIndex = 0; //default logic is none
        }

        private void Load_Presets()
        {
            //create the presets folder if it does not exist
            if (!System.IO.Directory.Exists("./presets"))
            {
                System.IO.Directory.CreateDirectory("./presets");
            }

            string[] presets = Directory.GetFiles("./presets", "*.ini");
            Dictionary<string,
            Dictionary<string,
            string>> File_Contents;
            string file = "";

            Presets = new Dictionary<string,
            Dictionary<string,
            Dictionary<string,
            string>>>();
            Presets_Combobox.Items.Clear(); //Clear the combobox

            for (int i = 0; i < presets.Length; i++)
            {
                file = presets[i].Substring(presets[i].LastIndexOf('\\') + 1);
                file = file.Remove(file.LastIndexOf('.'));
                Presets_Combobox.Items.Add(file);

                File_Contents = OpenAsIni(presets[i]);
                Presets.Add(file, File_Contents);
                Preset_Keys.Add(Preset_Keys.Count, file); //add key to list of keys (preset names)
            }
        }

        private Dictionary<string,
        Dictionary<string,
        string>> OpenAsIni(string filename)
        {
            Dictionary<string,
            Dictionary<string,
            string>> contents = new Dictionary<string,
            Dictionary<string,
            string>>();
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
                    section = line.Replace("[", "").Replace("]", ""); //remove the brackets to get the section name
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
            CheckBox check = (sender as CheckBox);
            string checkbox_text = check.Text;
            ComboBox pool = Item_Objects[checkbox_text].Get_Pool();
            ComboBox gives = Item_Objects[checkbox_text].Get_Gives();

            bool Is_Checked = (sender as CheckBox).Checked;
            
            pool.Enabled = Is_Checked;
            gives.Enabled = Is_Checked;

            //if the user just turned the checkbox on, then make the pool the first one in the list of pools
            if (Is_Checked)
            {
                //only change the from -1 if there is at least 1 pool
                if (Item_Pools_Keys.Count > 0)
                {
                    pool.SelectedIndex = 0;
                }
            }
            //if the user disables this item randomization, then make the pool and what the item gives both null values
            else
            {
                pool.SelectedIndex = -1;
                gives.SelectedIndex = -1;
            }

            Update_Pools();
        }

        private string Get_Item_Name(string pool_or_gives_name)
        {
            string item = pool_or_gives_name;
            string Item_Name;
            CheckBox check;

            //remove the pool or gives extension making it look like the checkbox name
            if (item.Contains("Pool"))
            {
                item = item.Substring(0, item.IndexOf("Pool")-1);
            }
            else
            {
                item = item.Substring(0, item.IndexOf("Gives") - 1);
            }

            for (int i = 0; i < Item_Names.Length; i++)
            {
                Item_Name = Item_Names[i];
                check = Item_Objects[Item_Name].Get_Checkbox();
                
                //this is the item
                if (check.Name == item)
                {
                    return Item_Name;
                }
            }

            //could not find the name of the item
            return "";
        }

        private void All_Night_Mask_Pool_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox pool = (ComboBox)sender;
            string Item_Name = Get_Item_Name(pool.Name);
            ComboBox gives = Item_Objects[Item_Name].Get_Gives();
            CheckBox checkbox = Item_Objects[Item_Name].Get_Checkbox();
            
            //If the user made this item in a pool, then make sure it doesn't manually give anything
            if (pool.SelectedIndex != -1)
            {
                gives.SelectedIndex = -1;
            }
            
            //remove this item from the previous pool
            Remove_Item_Pool(Item_Name);
            
            //Add this item to the pool if a pool was selected
            if (pool.Text != "")
            {
                Add_Item_Pool(Item_Pools[pool.Text], Item_Name);
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

        private Dictionary<int,
        string> Remove_Dictionary(Dictionary<int, string> Dict, int Key)
        {
            Dictionary<int,
            string> New_Dict = new Dictionary<int,
            string>();

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
            for (int i = 0; i < Item_Names.Length; i++)
            {
                string Item_Name = Item_Names[i];
                ComboBox PoolBox = Item_Objects[Item_Name].Get_Pool();
                PoolBox.Items.Add(Pool);
            }
            
            Change_Pool_Name_Combobox.Items.Add(Pool);
            Remove_Pool_Combobox.Items.Add(Pool);
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
            ComboBox gives = (ComboBox)sender;
            string Item_Name = Get_Item_Name(gives.Name);
            ComboBox pool = Item_Objects[Item_Name].Get_Pool();
            
            //if the item manually gives another item, then make sure that a pool isn't selected
            if (gives.SelectedIndex != -1)
            {
                pool.SelectedIndex = -1;
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
                this.Invoke(d, new object[] { });
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
            catch (Exception e)
            {

            }

            return file_contents;
        }

        private void RunRandomizer()
        {
            try
            {
                Rando = Process.Start("randomizer.exe");
                Rando.EnableRaisingEvents = true;
                Rando.Exited += new EventHandler(Rando_Process_Exited);
            }
            catch (Exception e)
            {
                Error("Cannot find \"randomizer.exe\" Make sure it's in the same directory");
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

        private void SaveSettingsAsIni(string location, bool items = true, bool settings = true, bool pools = true, bool colors = true, bool wallets = true)
        {
            string Text = "";
            string Item_Pool = "";
            string Item_Name = "";
            CheckBox check;
            ComboBox pool;
            ComboBox gives;

            if (items)
            {
                Text = "[items]\n";

                for (int i = 0; i < Item_Names.Length; i++)
                {
                    Item_Name = Item_Names[i];
                    check = Item_Objects[Item_Name].Get_Checkbox();
                    pool = Item_Objects[Item_Name].Get_Pool();
                    gives = Item_Objects[Item_Name].Get_Gives();
                
                    //if item is randomzied (or has a manually inserted item)
                    if (check.Checked)
                    {
                        //if randomized into a pool
                        if (pool.SelectedIndex != -1)
                        {
                            Item_Pool = pool.Items[pool.SelectedIndex].ToString();
                        }
                        //if manually inserted an item
                        else if (gives.SelectedIndex != -1)
                        {
                            Item_Pool = "#" + gives.Items[gives.SelectedIndex].ToString();
                        }

                        Text += Item_Name + "=" + Item_Pool + "\n";
                    }
                }
            }

            if (settings)
            {
                Text += "[settings]\n";

                Text += "Rom=" + Open_Base_Rom_Dialog.FileName + "\n"; //rom location
                Text += "Seed=" + Seed_Textbox.Text + "\n"; //seed
                Text += "Wad=" + createWadToolStripMenuItem.Checked + "\n"; //true or false to create wad as well

                if (Logic_Combobox.SelectedIndex != -1)
                {
                    Text += "Logic=" + Logic_Combobox.Items[Logic_Combobox.SelectedIndex] + "\n"; //which logic to use
                }
                else
                {
                    Text += "Logic=\n"; //no logic
                }

                Text += "Kafei=" + playAsKafeiToolStripMenuItem.Checked + "\n"; //whether or not to play as Kafei instead of Link

                Text += "ScrubBeans=" + swampScrubSalesBeansToolStripMenuItem.Checked + "\n"; //whether or not the scrub in swamp sells magic beans or what they're randomized to

                //Text += "Tunic=" + Get_Color_Tunic() + "\n";     //color of the tunics

                Text += "Remove_Cutscenes=" + removeCutscenesToolStripMenuItem.Checked + "\n"; //whether or not to remove the cutscenes

                Text += "GC_Hud=" + gCHudToolStripMenuItem.Checked + "\n"; //use or not use the GC Hud

                Text += "BlastMask_Cooldown=" + BlastMaskFrames_Num.Value + "\n"; //the blast mask cooldown

                Text += "RespawnHPs=" + respawnHPsToolStripMenuItem.Checked + "\n"; //whether or not to respawn the hps every cycle

                Text += "LikeLikeMirror=" + edibleMirrorShieldToolStripMenuItem.Checked + "\n"; //whether or not a likelike can eat the mirror shield
            }

            if (colors)
            {
                Text += "[colors]\n";

                //tunic colors
                Text += "Link=" + Color_To_String(Game_Colors["Link"]) + "\n";
                Text += "Deku=" + Color_To_String(Game_Colors["Deku"]) + "\n";
                Text += "Goron=" + Color_To_String(Game_Colors["Goron"]) + "\n";
                Text += "Zora=" + Color_To_String(Game_Colors["Zora"]) + "\n";
                Text += "FD=" + Color_To_String(Game_Colors["FD"]) + "\n";

                //pause menu colors
                Text += "Item=" + Color_To_String(Game_Colors["Item"]) + "\n";
                Text += "Map=" + Color_To_String(Game_Colors["Map"]) + "\n";
                Text += "Quest=" + Color_To_String(Game_Colors["Quest"]) + "\n";
                Text += "Mask=" + Color_To_String(Game_Colors["Mask"]) + "\n";
                Text += "Name=" + Color_To_String(Game_Colors["Name"]) + "\n";
            }

            if (wallets)
            {
                Text += "[wallets]\n";

                //wallet sizes
                Text += "Small=" + Wallet_Sizes["small"] + "\n";
                Text += "Medium=" + Wallet_Sizes["medium"] + "\n";
                Text += "Large=" + Wallet_Sizes["large"] + "\n";
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

        public string Color_To_String(Color clr)
        {
            string color = "";
            int red = clr.R;
            int blue = clr.B;
            int green = clr.G;

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
            Presets_Combobox.Text = ""; //clear text
            MessageBox.Show("Preset Saved", "Saved");
            Load_Presets(); //update the presets
        }

        private void Load_Preset(string Preset)
        {
            Dictionary<string,
            Dictionary<string,
            string>> Selected_Preset;

            Selected_Preset = Presets[Preset];

            Clear_Items(); //make all items not checked
            Clear_Pools(); //make no pools
            Clear_Pool_List(); //clear the pool list on the right side

            Create_Pools(Selected_Preset["pools"]);
            Update_Pool_List();
            Update_Items(Selected_Preset["items"]);
            Update_Settings(Selected_Preset["settings"]);

            if (Selected_Preset.ContainsKey("colors"))
            {
                Update_Colors(Selected_Preset["colors"]);
            }
            else
            {
                Game_Colors = Default_Pause();
            }

            if (Selected_Preset.ContainsKey("wallets"))
            {
                Update_Wallets(Selected_Preset["wallets"]);
            }
            else
            {
                Wallet_Sizes = Default_Wallets();
            }

            Presets_Combobox.Text = "";

            //make sure the change and remove combobox are blank now
            Change_Pool_Name_Combobox.Text = "";
            Remove_Pool_Combobox.Text = "";
        }

        private void Update_Wallets(Dictionary<string, string> wallet_sizes)
        {
            Wallet_Sizes = new Dictionary<string,
            int>();

            Wallet_Sizes.Add("small", String_To_Int(wallet_sizes["Small"]));
            Wallet_Sizes.Add("medium", String_To_Int(wallet_sizes["Medium"]));
            Wallet_Sizes.Add("large", String_To_Int(wallet_sizes["Large"]));
        }

        private void Update_Colors(Dictionary<string, string> colors)
        {
            if (colors == null || colors.Count == 0)
            {
                return;
            }

            //get the tunic colors
            if (colors.ContainsKey("Link"))
            {
                Game_Colors["Link"] = String_To_Color(colors["Link"]);
            }
            if (colors.ContainsKey("Deku"))
            {
                Game_Colors["Deku"] = String_To_Color(colors["Deku"]);
            }
            if (colors.ContainsKey("Goron"))
            {
                Game_Colors["Goron"] = String_To_Color(colors["Goron"]);
            }
            if (colors.ContainsKey("Zora"))
            {
                Game_Colors["Zora"] = String_To_Color(colors["Zora"]);
            }
            if (colors.ContainsKey("FD"))
            {
                Game_Colors["FD"] = String_To_Color(colors["FD"]);
            }

            //get the pause menu colors
            if (colors.ContainsKey("Item"))
            {
                Game_Colors["Item"] = String_To_Color(colors["Item"]);
            }
            if (colors.ContainsKey("Mask"))
            {
                Game_Colors["Mask"] = String_To_Color(colors["Mask"]);
            }
            if (colors.ContainsKey("Status"))
            {
                Game_Colors["Status"] = String_To_Color(colors["Status"]);
            }
            if (colors.ContainsKey("Map"))
            {
                Game_Colors["Map"] = String_To_Color(colors["Map"]);
            }
            if (colors.ContainsKey("Name"))
            {
                Game_Colors["Name"] = String_To_Color(colors["Name"]);
            }
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

            if (settings.ContainsKey("Remove_Cutscenes") && settings["Remove_Cutscenes"] == "True")
            {
                removeCutscenesToolStripMenuItem.Checked = true;
            }

            if (settings.ContainsKey("GC_Hud") && settings["GC_Hud"] == "True")
            {
                gCHudToolStripMenuItem.Checked = true;
            }

            if (settings.ContainsKey("BlastMask_Cooldown"))
            {
                BlastMaskFrames_Num.Value = String_To_Int(settings["BlastMask_Cooldown"]);
            }
            else
            {
                BlastMaskFrames_Num.Value = 310;
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
            color = color.Substring(start + 2); //color = "X G=Y B=Z]"
            end = color.IndexOf(',');
            color_text = color.Substring(0, end); //color_text = "X"
            r = String_To_Int(color_text); //r = X

            start = color.IndexOf('G');
            color = color.Substring(start + 2); //color = "Y B=Z]"
            end = color.IndexOf(',');
            color_text = color.Substring(0, end); //color_text = "Y"
            g = String_To_Int(color_text); //g = Y

            start = color.IndexOf('B');
            color = color.Substring(start + 2); //color = "Z]"
            end = color.IndexOf(']');
            color_text = color.Substring(0, end); //color_text = "Z"
            b = String_To_Int(color_text); //g = Y

            new_color = Rgb_Color(r, g, b);

            return new_color;
        }

        public Color Rgb_Color(int red, int green, int blue)
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
                
                Item = Text_To_Checkbox(Item);

                //check the checkbox
                CheckBox check = Item_Objects[key].Get_Checkbox();
                check.Checked = true;

                Pool = Preset_Items[key];
                if (Pool[0] == '#') //manual item
                {
                    ComboBox gives = Item_Objects[key].Get_Gives();
                    gives.SelectedIndex = gives.Items.IndexOf(Pool.Substring(1));
                }
                else
                {
                    ComboBox pool = Item_Objects[key].Get_Pool();
                    pool.SelectedIndex = pool.Items.IndexOf(Pool);
                }
            }
        }

        private void Clear_Pool_List()
        {
            Pool_Tables = new Dictionary<string,
            RichTextBox>();
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
            Item_Pools_Keys.Add(Item_Pools_Keys.Count, Pool_Name); //pool key list
            Item_Pools.Add(Pool_Name, new Dictionary<int, string>()); //pool list
        }

        private void Clear_Pools()
        {
            Item_Pools_Keys = new Dictionary<int,
            string>();
            Item_Pools = new Dictionary<string,
            Dictionary<int,
            string>>();

            //clear the list of pool for each item pool combobox
            for (int i = 0; i < Item_Names.Length; i++)
            {
                string Item_Name = Item_Names[i];
                ComboBox pool = Item_Objects[Item_Name].Get_Pool();
                pool.Items.Clear();
            }

            Change_Pool_Name_Combobox.Items.Clear();
            Remove_Pool_Combobox.Items.Clear();
        }

        private void Clear_Items()
        {
            string Item_Name = "";
            CheckBox checkbox;

            //make each checkbox unchecked
            for (int i = 0; i < Item_Names.Length; i++)
            {
                Item_Name = Item_Names[i];
                checkbox = Item_Objects[Item_Name].Get_Checkbox();
                checkbox.Checked = false;
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
                Item_Pools_Keys[index] = New; //replace the key for the pool name with the new name
                Item_Pools.Add(New, Item_Pools[Old]); //add the pool and get the items for the old pool
                Item_Pools.Remove(Old); //Remove the old pool items
                Update_Pools_Changed_Name(Old, New); //update each combobox pool (replace the old pool name with the new one)
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
            for (int i = 0; i < Item_Names.Length; i++)
            {
                string Item_Name = Item_Names[i];
                ComboBox pool = Item_Objects[Item_Name].Get_Pool();
                pool.Items[index] = New;
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
                    if (New_Name != "")
                    {
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
        private Dictionary<int,
        string> Fix_Keys(Dictionary<int, string> Object)
        {
            Dictionary<int,
            string> New_Obj = new Dictionary<int,
            string>();
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

            index = Index_Of_Value(Item_Pools_Keys, Pool);

            if (index != -1)
            {
                Item_Pools_Keys.Remove(index);
                Item_Pools_Keys = Fix_Keys(Item_Pools_Keys);

                Pool_Tabs.TabPages.Remove(Pool_Tabs.TabPages[index]);
                
                for (int i = 0; i < Item_Names.Length; i++)
                {
                    string Item_Name = Item_Names[i];
                    ComboBox pool = Item_Objects[Item_Name].Get_Pool();

                    //if this item is in the pool we're removing
                    if (pool.SelectedIndex == index)
                    {
                        pool.Items.RemoveAt(index);

                        //make the new pool the first one
                        if (pool.Items.Count > 0)
                        {
                            pool.SelectedIndex = 0;
                        }
                        //make the new pool nothing becuase there are no more pools
                        else
                        {
                            pool.SelectedIndex = -1;
                            pool.Text = ""; //make sure the text becomes blank
                        }
                    }
                    //not selected, so only remove the pool from the list
                    else
                    {
                        pool.Items.RemoveAt(index);
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

        private void pauseMenuColorsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            pmc pause_menu_form = new pmc();

            pause_menu_form.parent = this;
            pause_menu_form.Show();
        }

        private void walletToolStripMenuItem_Click(object sender, EventArgs e)
        {
            wallets_form WalletForm = new wallets_form();

            WalletForm.parent = this;
            WalletForm.Show();
        }

        private int Num_Digits(double number)
        {
            int num_int = (int)number;
            int num_digs = 0;

            while (num_int != 0)
            {
                num_int /= 10;
                num_digs++;
            }

            number *= 10;
            while (number != 0)
            {
                number -= (int)number;
                number *= 10;
                num_digs++;
            }

            return num_digs;
        }

        private int[] Dec_To_DigitArray(double num)
        {
            int num_int = (int)num;
            int[] num_ints;
            int digits;

            if (num == 0)
            {
                num_ints = new int[1];
                num_ints[0] = 0;
            }
            else
            {
                digits = Num_Digits(num);
                num_ints = new int[digits];

                for (int i = 0; i < digits; i++)
                {
                    num_int = (int)num;
                    num_ints[i] = num_int;
                    num = (num - num_int) * 10;
                }
            }

            return num_ints;
        }

        private double Round(double dec, int places)
        {
            int right_shifts = 0;
            int[] numbers;

            //12.34 => 0.1234
            while ((int)dec != 0)
            {
                dec /= 10;
                right_shifts++;
            }

            //0.1234 => 1.234
            if (right_shifts > 0)
            {
                dec *= 10;
                right_shifts--;
            }

            numbers = Dec_To_DigitArray(dec);

            //dec = Round(numbers, places, right_shifts);

            //12.34 => 0.1234
            while (right_shifts > 0)
            {
                dec *= 10;
                right_shifts--;
            }

            return dec;
        }

        private int Divide(int num, int den)
        {
            return (int)Divide(num, (double)den);
        }

        private double Divide(int num, double d_den)
        {
            double quo = 0;
            int den = (int)d_den;
            int rem;
            int left_shifts = 0;

            if (den == 0)
            {
                return 0;
            }

            quo += num / den;
            rem = num % den;

            while (rem != 0)
            {
                num = rem * 10;
                rem = num % den;

                quo *= 10;
                left_shifts++;
                quo += num / den;
            }

            while (left_shifts != 0)
            {
                quo /= 10.0;
                left_shifts--;
            }

            return quo;
        }

        private void BlastMask_Center(int left, int right)
        {
            int width = BlastMaskSeconds_Label.Width;
            int middle = (left + right) / 2;
            int x = middle - (width / 2);
            int y = BlastMaskSeconds_Label.Location.Y;
            BlastMaskSeconds_Label.Location = new Point(x, y);
        }

        private double Minus(double num1, int num2)
        {
            double num1_NoDec = num1;
            double num1_int = (int)num1_NoDec;
            int left_shifts = 0;

            //12.34 => 1234
            while (num1_NoDec - num1_int != 0)
            {
                num1_NoDec *= 10;
                num2 *= 10;
                num1_int = (int)num1_NoDec;
                left_shifts++;
            }

            num1_NoDec -= num2;

            for (int i = 0; i < left_shifts; i++)
            {
                num1_NoDec /= 10;
            }

            return num1_NoDec;
        }

        private double Mod(double num, int den)
        {
            int num_int = (int)num;
            num = Minus(num, num_int);
            num_int = num_int % den;
            num += num_int;
            return num;
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            int frames = (int)BlastMaskFrames_Num.Value;
            double seconds = Divide(frames, 20.0);
            int minutes = (int)(seconds / 60);
            string seconds_str = "";

            seconds = Mod(seconds, 60);

            if (seconds < 10 && minutes > 0)
            {
                seconds_str += "0";
            }
            seconds_str += seconds;

            if (minutes > 0)
            {
                BlastMaskSeconds_Label.Text = minutes + ":" + seconds_str;
            }
            else
            {
                BlastMaskSeconds_Label.Text = seconds_str;
            }

            BlastMask_Center(411, 532);
        }
    }
}