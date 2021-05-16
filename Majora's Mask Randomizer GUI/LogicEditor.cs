using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class LogicEditor : Form
    {
        public bool showing;
        public Main_Window form;

        string[] items;
        Dictionary<int, string> logics;
        Dictionary<string, //logic
            Dictionary<string,  //item name
                Dictionary<int, //item groups
                    Dictionary<int, string>>>>  //items in group
                   Logic_Data;
        Dictionary<string, //logic
            Dictionary<string,  //item name
                Dictionary<int, //item groups
                    Dictionary<int, int>>>>  //items in group
                   Count_Data;  //count of the corresponding item gotten by the item name from Logic_Data's entry
        Dictionary<string, //logic
            Dictionary<string,  //item name
                Dictionary<int, string>>>  //each group
                   Comment_Data;  //Comment for this logic group
        Dictionary<string,  //logic
            Dictionary<string,  //item
                Dictionary<int, string>>>   //each invalid item for this item in logic
                    Logic_Invalid;

        public LogicEditor()
        {
            InitializeComponent();
        }

        private void LogicEditor_Load(object sender, EventArgs e)
        {
            showing = true;
            items = form.Item_Names;    //get a list of every item that can be randomized
            logics = new Dictionary<int, string>(); //create an empty dictionary to store logic names
            Logic_Data = new Dictionary<string, Dictionary<string, Dictionary<int, Dictionary<int, string>>>>();
            Count_Data = new Dictionary<string, Dictionary<string, Dictionary<int, Dictionary<int, int>>>>();
            Comment_Data = new Dictionary<string, Dictionary<string, Dictionary<int, string>>>();
            Logic_Invalid = new Dictionary<string, Dictionary<string, Dictionary<int, string>>>();

            //disable the edit item section things to prevent editing of nothing
            Enable_Edit_Items(false);
            Enable_Invalid_Items(false);

            Comment_TextBox.Enabled = false;
            NewItemGroup_Button.Enabled = false;
            DeleteItemGroup_Button.Enabled = false;

            Setup_Item_Combobox();

            //load the logic from the logic files
            Load_Logic();
        }

        private void Enable_Invalid_Items(bool enable)
        {
            InvalidItem_ComboBox.Enabled = enable;
            SaveInvalidItem_Button.Enabled = enable;
            RemoveInvalidItem_Button.Enabled = enable;
            NewInvalidItem_Button.Enabled = enable;
        }

        //creates empty logic for every item for the logic given
        private void Setup_Logic_Items(string logic)
        {
            string item;

            for (int i = 0; i < items.Length; i++)
            {
                item = items[i];
                Create_New_Logic_Set(logic, item);
            }
        }
        
        private void Add_Logic(string logic_path, string logic_name)
        {
            StreamReader file;
            string line = "";
            string location = "";
            string comment = "";
            int Item_Set_Index = 0;
            Dictionary<int, string> Item_Set;
            Dictionary<int, int> Count_Set;
            Dictionary<int, string> Invalid_Set;
            string item = "";
            Dictionary<int, string> Line_Array;
            Dictionary<int, string> Item_Array;
            string Item_Text = "";
            int amount = 0;
            bool Is_Grouped = false;
            string[] Grouped_Items =
            {
                "Big Bomb Bag",
                "Biggest Bomb Bag",
                "Large Quiver",
                "Largest Quiver"
            };
            Dictionary<string, string> Replace_Items = new Dictionary<string, string>()
            {
                {"Bomb Bag", "Bombs" },
                {"Bombchu", "Bombchus" }
            };
            Dictionary<int, string> Ignore_Items = new Dictionary<int, string>()
            {
                {0, "Deku Nuts (10)" }
            };

            //make logic have every item even if it's not in the logic file
            Setup_Logic_Items(logic_name);

            file = new StreamReader(logic_path);
            
            //for each line in the file
            while (!file.EndOfStream)
            {
                line = file.ReadLine();

                //move the comment from the line and put it in a variable
                if (line.Contains("//"))
                {
                    //multiple line comment
                    if (comment != "")
                    {
                        comment += "\n";
                    }

                    comment += line.Substring(line.IndexOf("//") + 2);
                    line = line.Substring(0, line.IndexOf("//"));
                }
                //default comment is blank
                else
                {
                    comment = "";
                }

                //remove tab indents
                while (line.Contains("\t"))
                {
                    line = line.Remove(line.IndexOf("\t"), 1);
                }
                
                //if this line isn't empty
                if (line != "")
                {
                    //this is an item's location
                    if (location == "")
                    {
                        //get the item location name, and remove "{"
                        location = line.Substring(0, line.IndexOf("{") - 1);
                        Create_New_Logic_Set(logic_name, location);
                    }
                    //end of item list for this location
                    else if (line == "}")
                    {
                        location = "";
                        Item_Set_Index = 0;
                    }
                    //this is a list of items that can never be placed here
                    else if (line[0] == '#')
                    {
                        Invalid_Set = new Dictionary<int, string>();
                        line = line.Substring(1);   //remove #
                        Item_Array = Split(line, ", "); //split the needed items in this set into an array

                        //for each item in the line
                        for (int i = 0; i < Item_Array.Count; i++)
                        {
                            item = Item_Array[i];

                            //Bomb Bag => Bombs, ect.
                            if (Replace_Items.ContainsKey(item))
                            {
                                item = Replace_Items[item];
                            }

                            //if this is not a groued item, then add it to the list (large quiver, ect.)
                            if (!Grouped_Items.Contains(item))
                            {
                                //add item to set
                                Invalid_Set.Add(Invalid_Set.Count, item);
                            }
                        }

                        Logic_Invalid[logic_name][location] = Invalid_Set;
                    }
                    //this is an item set
                    else
                    {
                        Item_Set = new Dictionary<int, string>();
                        Count_Set = new Dictionary<int, int>();
                        Item_Array = Split(line, ", "); //split the needed items in this set into an array
                        Is_Grouped = false;

                        //for each item in the line
                        for (int i = 0; i < Item_Array.Count; i++)
                        {
                            Item_Text = Item_Array[i];
                            Line_Array = Split(Item_Text, " ");  //split the item into an array separated by spaces

                            //do we need a specific amount of this item
                            if (int.TryParse(Line_Array[0], out _))
                            {
                                amount = int.Parse(Line_Array[0]);  //get the amount needed of this item
                                item = Item_Text.Substring(Item_Text.IndexOf(" ") + 1); //get the item
                            }
                            else
                            {
                                amount = 1;
                                item = Item_Text;
                            }

                            //this item set is a grouped set and doesnt need to be added
                            if (Grouped_Items.Contains(item))
                            {
                                i = Item_Array.Count;
                                Is_Grouped = true;
                            }
                            //this item is on the ignore list
                            else if (Ignore_Items.ContainsValue(item))
                            {

                            }
                            else
                            {
                                //Bomb Bag => Bombs, ect.
                                if (Replace_Items.ContainsKey(item))
                                {
                                    item = Replace_Items[item];
                                }

                                //add item to set
                                Item_Set.Add(Item_Set.Count, item);
                                Count_Set.Add(Count_Set.Count, amount);
                            }
                        }
                        
                        //add the set if it's not a grouped set (large quiver, ect)
                        if (!Is_Grouped)
                        {
                            Logic_Data[logic_name][location][Item_Set_Index] = Item_Set;
                            Count_Data[logic_name][location][Item_Set_Index] = Count_Set;

                            //create an empty comment for this item set
                            if (Comment_Data[logic_name][location].Count == Item_Set_Index)
                            {
                                Comment_Data[logic_name][location][Item_Set_Index] = "";
                            }

                            Item_Set_Index++;
                        }
                    }
                }
                //if the line only had a comment
                else if (comment != "")
                {
                    //we are in the item needed lists, so this is an item set comment
                    if (location != "")
                    {
                        //add the comment to the current item set
                        Comment_Data[logic_name][location][Item_Set_Index] = comment;
                    }
                }
            }

            file.Close();
        }

        private Dictionary<int, string> Split(string data, string Split_On)
        {
            Dictionary<int, string> Split_Data = new Dictionary<int, string>();
            string word = "";

            if (data.Contains(Split_On))
            {
                while (data.Contains(Split_On))
                {
                    word = data.Substring(0, data.IndexOf(Split_On));
                    data = data.Substring(data.IndexOf(Split_On) + Split_On.Length);

                    Split_Data.Add(Split_Data.Count, word);
                }
            }

            //add te last item to the array
            Split_Data.Add(Split_Data.Count, data);

            return Split_Data;
        }

        private void Load_Logic()
        {
            string[] Logic_Files;
            string logic = "";
            
            if (System.IO.Directory.Exists("./logic"))
            {
                Logic_Files = Directory.GetFiles("./logic", "*.txt");

                for (int i = 0; i < Logic_Files.Length; i++)
                {
                    logic = Logic_Files[i].Substring(8); //remove the folder path
                    logic = logic.Substring(0, logic.IndexOf('.')); //remove the extension
                    LogicFiles_ListBox.Items.Add(logic);    //add the logic name to the list of logic files
                    Add_Logic(Logic_Files[i], logic);
                }
                
            }
        }

        private void Enable_Edit_Items(bool enabled)
        {
            SaveNeededItem_Button.Enabled = enabled;
            NewNeededItem_Button.Enabled = enabled;
            EditItem_ComboBox.Enabled = enabled;
            EditItem_Number.Enabled = enabled;
            RemoveNeededItem_Button.Enabled = enabled;
        }

        private void Setup_Item_Combobox()
        {
            string item = "";
            string[] Skip_Items =
            {
                "Large Quiver",
                "Largest Quiver",
                "Big Bomb Bag",
                "Biggest Bomb Bag",
                "Deku Nuts (10)",
                "Bombchus (5)",
                "Bombchus (10)",
                "Blue Rupee",
                "Green Rupee",
                "Silver Rupee",
                "Purple Rupee",
                "Gold Rupee"
            };
            Dictionary<string, string> Replace_Items = new Dictionary<string, string>()
            {
                {"Bomb Bag", "Bombs" },
                {"Red Rupee", "Rupees" },
                {"Bombchu", "Bombchus" }
            };

            for (int i = 0; i < items.Length; i++)
            {
                item = items[i];

                //skip items that don't need to be here for logic, like 10 nuts because nuts is there
                if (!Skip_Items.Contains(item))
                {
                    //replace some items for clearer reading
                    if (Replace_Items.ContainsKey(item))
                    {
                        item = Replace_Items[item];
                    }

                    EditItem_ComboBox.Items.Add(item);
                    InvalidItem_ComboBox.Items.Add(item);
                }
            }
        }

        private void LogicEditor_Shown(object sender, EventArgs e)
        {

        }

        private void LogicEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            form.logic_editor = new LogicEditor();
            showing = false;
        }

        private bool Valid_Logic_Name(string name)
        {
            string Invalid_Characters = "\\/:*?\"<>|";

            if (name != "")
            {
                if (!logics.ContainsValue(name))
                {
                    for (int c = 0; c < Invalid_Characters.Length; c++)
                    {
                        //fount an invalid character for files
                        if (name.Contains(Invalid_Characters[c]))
                        {
                            return false;
                        }
                    }

                    //passed all checks, should be a valid logic name
                    return true;
                }
            }

            return false;
        }

        private void NewLogic_Button_Click(object sender, EventArgs e)
        {
            string logic_name = NewLogic_TextBox.Text;
            string item = "";

            //checks if the logic name is valid
            if (Valid_Logic_Name(logic_name))
            {
                //clear the new logic textbox
                NewLogic_TextBox.Clear();
                
                //Add each item to the logic
                for (int i = 0; i < items.Length; i++)
                {
                    item = items[i];
                    Create_New_Logic_Set(logic_name, item);
                }

                //add logic name to the list of logic names
                logics.Add(logics.Count, logic_name);   

                //Add new logic to the logic listbox and select it
                LogicFiles_ListBox.Items.Add(logic_name);
                LogicFiles_ListBox.SelectedIndex = LogicFiles_ListBox.Items.Count - 1;
            }
            else
            {
                MessageBox.Show("Invalid logic name");
            }
        }

        private void Create_New_Logic_Set(string logic, string item)
        {
            Dictionary<int, string> item_set = new Dictionary<int, string>();
            Dictionary<int, int> count_set = new Dictionary<int, int>();

            item_set.Add(0, "");
            count_set.Add(0, 0);

            //add new logic if there is no logic with this name
            if (!Logic_Data.ContainsKey(logic))
            {
                Logic_Data.Add(logic, new Dictionary<string, Dictionary<int, Dictionary<int, string>>>());
                Count_Data.Add(logic, new Dictionary<string, Dictionary<int, Dictionary<int, int>>>());
                Comment_Data.Add(logic, new Dictionary<string, Dictionary<int, string>>());
                Logic_Invalid.Add(logic, new Dictionary<string, Dictionary<int, string>>());
            }

            //create objects for this new item in the logic
            if (!Logic_Data[logic].ContainsKey(item))
            {
                Logic_Data[logic].Add(item, new Dictionary<int, Dictionary<int, string>>());
                Count_Data[logic].Add(item, new Dictionary<int, Dictionary<int, int>>());
                Comment_Data[logic].Add(item, new Dictionary<int, string>());
                Logic_Invalid[logic].Add(item, new Dictionary<int, string>());
            }
            
            Logic_Data[logic][item].Add(Logic_Data[logic][item].Count, item_set);
            Count_Data[logic][item].Add(Count_Data[logic][item].Count, count_set);
            Comment_Data[logic][item].Add(Comment_Data[logic][item].Count, "");
            Logic_Invalid[logic][item].Add(Logic_Invalid[logic][item].Count, "");
        }

        //a logic set is selected
        private void LogicFiles_ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            string logic;
            string item = "";

            if (LogicFiles_ListBox.SelectedIndex != -1)
            {
                logic = LogicFiles_ListBox.SelectedItem.ToString();

                //add all the items to the list if there are none
                if (Items_ListBox.Items.Count == 0)
                {
                    for (int i = 0; i < items.Length; i++)
                    {
                        item = items[i];
                        Items_ListBox.Items.Add(item);
                    }
                }

                //do not select any item by default
                Items_ListBox.SelectedIndex = -1;
            }
        }
        
        private string Items_Needed_String(Dictionary<int, string> items, Dictionary<int, int> counts, string sep = "")
        {
            string new_str = "";

            for (int s = 0; s < items.Count; s++)
            {
                //only add the item if it's not null
                if (items[s] != "")
                {
                    //add the separator if it's nto the first item
                    if (s > 0)
                    {
                        new_str += sep;
                    }

                    //add the number of this item if it's more than one
                    if (counts[s] > 1)
                    {
                        new_str += counts[s] + " ";
                    }
                    new_str += items[s];
                }
            }

            return new_str;
        }

        //an item is selected
        private void Items_ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            string item;
            string logic;
            Dictionary<int, Dictionary<int, string>> Item_Groups;
            Dictionary<int, string> Items_Needed;
            Dictionary<int, string> Invalid_Items;
            string Items_In_Group;
            
            //if an item is selected
            if (Items_ListBox.SelectedIndex != -1)
            {
                item = Items_ListBox.SelectedItem.ToString();
                logic = LogicFiles_ListBox.SelectedItem.ToString();
                Items_Needed = new Dictionary<int, string>();
                
                Item_Groups = Logic_Data[logic][item];

                //add the item groups to the listbox of groups
                ItemGroups_ListBox.Items.Clear();
                for (int i = 0; i < Item_Groups.Count; i++)
                {
                    Items_In_Group = Items_Needed_String(Item_Groups[i], Count_Data[logic][item][i], ", ");
                    ItemGroups_ListBox.Items.Add(Items_In_Group);
                }
                //select the first item group
                ItemGroups_ListBox.SelectedIndex = 0;

                //add the invalid items to the invalid list
                InvalidItems_ListBox.Items.Clear();
                Invalid_Items = Logic_Invalid[logic][item];
                for (int i = 0; i < Invalid_Items.Count; i++)
                {
                    InvalidItems_ListBox.Items.Add(Invalid_Items[i]);
                }
                //select the first invalid item
                InvalidItems_ListBox.SelectedIndex = 0;
                
                //enable the new and delete item set buttons
                NewItemGroup_Button.Enabled = true;
                DeleteItemGroup_Button.Enabled = true;

                //enable the new invalid item button
                NewInvalidItem_Button.Enabled = true;
            }
            //a different logic was selected
            else
            {
                Items_ListBox.SelectedIndex = -1;
                Enable_Edit_Items(false);
                Enable_Invalid_Items(false);

                //dsiable the new and delete item set buttons
                NewItemGroup_Button.Enabled = false;
                DeleteItemGroup_Button.Enabled = false;

                //dsiable the new invalid item button
                NewInvalidItem_Button.Enabled = false;

                //clear item sets, items, and invalid item listboxes
                ItemGroups_ListBox.Items.Clear();
                ItemsNeeded_ListBox.Items.Clear();
                InvalidItems_ListBox.Items.Clear();

                //clear the item comboboxes
                EditItem_ComboBox.SelectedIndex = -1;
                EditItem_Number.Value = 1;
                InvalidItem_ComboBox.SelectedIndex = -1;
            }
        }

        //an item set is selected
        private void ItemGroups_ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            Dictionary<int, string> Items_Needed = new Dictionary<int, string>();
            int Item_Set_Index = ItemGroups_ListBox.SelectedIndex;
            string logic = "";
            string item = "";
            string Item_Needed = "";

            //if an item set is selected
            if (Item_Set_Index != -1)
            {
                logic = LogicFiles_ListBox.SelectedItem.ToString();
                item = Items_ListBox.SelectedItem.ToString();
                Items_Needed = Logic_Data[logic][item][Item_Set_Index];

                //clear all the items in the set before putting this set there
                ItemsNeeded_ListBox.Items.Clear();
                for (int i = 0; i < Items_Needed.Count; i++)
                {
                    Item_Needed = Items_Needed[i];
                    Item_Needed = Combine_Count_Item(Item_Needed, Count_Data[logic][item][Item_Set_Index][i]);
                    ItemsNeeded_ListBox.Items.Add(Item_Needed);
                }

                //select the first item in the list
                ItemsNeeded_ListBox.SelectedIndex = 0;

                //enable the commmet
                Comment_TextBox.Enabled = true;

                //get the comment data for the item set
                Comment_TextBox.Text = Comment_Data[logic][item][Item_Set_Index];
            }
            else
            {
                //disable the commmet
                Comment_TextBox.Enabled = false;
            }
        }

        //when a needed item is selected
        private void ItemsNeeded_ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            string item = "";
            int index = -1;
            int count = 1;

            //if an item is selected
            if (ItemsNeeded_ListBox.SelectedIndex != -1)
            {
                //enable editting the items
                Enable_Edit_Items(true);
                item = ItemsNeeded_ListBox.SelectedItem.ToString();

                //if the item has a number in front of it
                if (!EditItem_ComboBox.Items.Contains(item) && item != "")
                {
                    try
                    {
                        count = int.Parse(item.Substring(0, item.IndexOf(' ')));    //get the count of the item
                        item = item.Substring(item.IndexOf(' ') + 1);   //remove the count and get just the item
                    }
                    catch
                    {
                        //not a number
                    }
                }

                index = EditItem_ComboBox.Items.IndexOf(item);

                EditItem_ComboBox.SelectedIndex = index;    //update the combobox
                EditItem_Number.Value = count;  //update the count
            }
        }

        private string Combine_Count_Item(string item, int count)
        {
            string combined = "";

            if (count > 1)
            {
                combined += count.ToString() + " ";
            }
            combined += item;

            return combined;
        }

        private void SaveNeededItem_Button_Click(object sender, EventArgs e)
        {
            //return if no item is selected
            if (EditItem_ComboBox.SelectedIndex == -1)
            {
                return;
            }

            string Item_Needed = EditItem_ComboBox.SelectedItem.ToString();
            int count = (int)EditItem_Number.Value;
            int Item_Needed_Index = ItemsNeeded_ListBox.SelectedIndex;
            int Item_Set_Index = ItemGroups_ListBox.SelectedIndex;
            string logic = LogicFiles_ListBox.SelectedItem.ToString();
            string item = Items_ListBox.SelectedItem.ToString();
            string Count_And_Item = "";
            
            //update the data
            Logic_Data[logic][item][Item_Set_Index][Item_Needed_Index] = Item_Needed;
            Count_Data[logic][item][Item_Set_Index][Item_Needed_Index] = count;

            //update the gui
            Count_And_Item = Combine_Count_Item(Item_Needed, count);
            ItemsNeeded_ListBox.Items[Item_Needed_Index] = Count_And_Item;
            ItemGroups_ListBox.Items[Item_Set_Index] = Items_Needed_String(Logic_Data[logic][item][Item_Set_Index], Count_Data[logic][item][Item_Set_Index], ", ");

            //re-select the same item in the set that was selected
            ItemsNeeded_ListBox.SelectedIndex = Item_Needed_Index;
        }

        private void NewNeededItem_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            int Item_Set;
            
            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();
            Item_Set = ItemGroups_ListBox.SelectedIndex;

            //add a new item needed to the data
            Logic_Data[logic][item][Item_Set].Add(Logic_Data[logic][item][Item_Set].Count, "");
            Count_Data[logic][item][Item_Set].Add(Count_Data[logic][item][Item_Set].Count, 1);

            //update the gui and select it
            ItemsNeeded_ListBox.Items.Add("");
            ItemsNeeded_ListBox.SelectedIndex = ItemsNeeded_ListBox.Items.Count - 1;
        }

        private void NewItemGroup_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            Dictionary<int, string> Item_Set = new Dictionary<int, string>();
            Dictionary<int, int> Count_Set = new Dictionary<int, int>();

            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();
            Item_Set.Add(0, "");
            Count_Set.Add(0, 0);

            //add new item set to the data
            Logic_Data[logic][item].Add(Logic_Data[logic][item].Count, Item_Set);
            Count_Data[logic][item].Add(Count_Data[logic][item].Count, Count_Set);
            Comment_Data[logic][item].Add(Comment_Data[logic][item].Count, "");

            //update the gui
            ItemGroups_ListBox.Items.Add(Items_Needed_String(Item_Set, Count_Set, ", "));
            ItemGroups_ListBox.SelectedIndex = ItemGroups_ListBox.Items.Count - 1;
        }

        private void InvalidItems_ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            string logic;
            string item;
            string Invalid_Item;

            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();

            //if an invalid item is selected
            if (InvalidItems_ListBox.SelectedIndex != -1)
            {
                Invalid_Item = InvalidItems_ListBox.SelectedItem.ToString();

                Enable_Invalid_Items(true);
                InvalidItem_ComboBox.SelectedIndex = InvalidItem_ComboBox.Items.IndexOf(Invalid_Item);
            }
        }

        private void SaveInvalidItem_Button_Click(object sender, EventArgs e)
        {
            int index;
            string Invalid_Item;
            string logic;
            string item;

            if (InvalidItem_ComboBox.SelectedIndex != -1)
            {
                index = InvalidItems_ListBox.SelectedIndex;
                Invalid_Item = InvalidItem_ComboBox.SelectedItem.ToString();
                logic = LogicFiles_ListBox.SelectedItem.ToString();
                item = Items_ListBox.SelectedItem.ToString();

                //update the data
                Logic_Invalid[logic][item][index] = Invalid_Item;

                //update the gui
                InvalidItems_ListBox.Items[index] = Invalid_Item;
            }
        }

        private Dictionary<int, string> Remove(Dictionary<int, string> old_dict, int index)
        {
            Dictionary<int, string> new_dict = new Dictionary<int, string>();
            int one = 0;

            for (int i = 0; i < old_dict.Count; i++)
            {
                if (i != index)
                {
                    new_dict.Add(one, old_dict[i]);
                    one += 1;
                }
                
            }

            return new_dict;
        }

        private Dictionary<int, Dictionary<int, string>> Remove(Dictionary<int, Dictionary<int, string>> old_dict, int index)
        {
            Dictionary<int, Dictionary<int, string>> new_dict = new Dictionary<int, Dictionary<int, string>>();
            int one = 0;

            for (int i = 0; i < old_dict.Count; i++)
            {
                if (i != index)
                {
                    new_dict.Add(one, old_dict[i]);
                    one += 1;
                }

            }

            return new_dict;
        }

        private Dictionary<int, Dictionary<int, int>> Remove(Dictionary<int, Dictionary<int, int>> old_dict, int index)
        {
            Dictionary<int, Dictionary<int, int>> new_dict = new Dictionary<int, Dictionary<int, int>>();
            int one = 0;

            for (int i = 0; i < old_dict.Count; i++)
            {
                if (i != index)
                {
                    new_dict.Add(one, old_dict[i]);
                    one += 1;
                }

            }

            return new_dict;
        }

        private void RemoveNeededItem_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            int index;
            int List_Index;
            int Item_Index;

            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();
            index = ItemsNeeded_ListBox.SelectedIndex;
            List_Index = ItemGroups_ListBox.SelectedIndex;
            Item_Index = Items_ListBox.SelectedIndex;

            //remove the item from the list
            if (ItemsNeeded_ListBox.Items.Count > 1)
            {
                //update the data
                //Logic_Data[logic][item][List_Index].Remove(index);
                Logic_Data[logic][item][List_Index] = Remove(Logic_Data[logic][item][List_Index], index);

                //update the gui
                ItemsNeeded_ListBox.Items.RemoveAt(index);
            }
            //just make the item blank because it's the only one
            else
            {
                //update the data
                Logic_Data[logic][item][List_Index][index] = "";

                //update the gui
                ItemsNeeded_ListBox.Items[index] = "";
            }

            //update the item set in the gui
            ItemGroups_ListBox.Items[List_Index] = Items_Needed_String(Logic_Data[logic][item][List_Index], Count_Data[logic][item][List_Index], ", ");
            
            //select where the last item was
            if (ItemsNeeded_ListBox.Items.Count > index)
            {
                ItemsNeeded_ListBox.SelectedIndex = index;
            }
            else
            {
                ItemsNeeded_ListBox.SelectedIndex = index - 1;
            }
        }

        private void NewInvalidItem_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            int index;

            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();
            index = InvalidItems_ListBox.SelectedIndex + 1;

            //update the data
            Logic_Invalid[logic][item].Add(index, "");

            //update the gui
            InvalidItems_ListBox.Items.Add("");
            InvalidItems_ListBox.SelectedIndex = index;
        }

        private void RemoveInvalidItem_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            int index;

            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();
            index = InvalidItems_ListBox.SelectedIndex;

            //this deletes the wrong entry in data - seems to delete index + 1
            if (Logic_Invalid[logic][item].Count > 1)
            {
                //update the data
                Logic_Invalid[logic][item] = Remove(Logic_Invalid[logic][item], index);

                //update the gui
                InvalidItems_ListBox.Items.RemoveAt(index);
            }
            else
            {
                //update the data
                Logic_Invalid[logic][item][0] = "";

                //update the gui
                InvalidItems_ListBox.Items[0] = "";
            }
            
            //select where the last item was
            if (InvalidItems_ListBox.Items.Count > index)
            {
                InvalidItems_ListBox.SelectedIndex = index;
            }
            else
            {
                InvalidItems_ListBox.SelectedIndex = index - 1;
            }
        }

        private void Comment_TextBox_TextChanged(object sender, EventArgs e)
        {
            string logic;
            string item;
            int index;

            logic = LogicFiles_ListBox.SelectedItem.ToString();
            item = Items_ListBox.SelectedItem.ToString();
            index = ItemGroups_ListBox.SelectedIndex;

            //update the comment data
            Comment_Data[logic][item][index] = Comment_TextBox.Text;
        }

        private void CancelLogic_Button_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private Dictionary<int, Dictionary<int, string>> Insert(Dictionary<int, Dictionary<int, string>> dictionary, Dictionary<int, string> new_array, int index)
        {
            Dictionary<int, Dictionary<int, string>> Updated_Dictionary = new Dictionary<int, Dictionary<int, string>>();
            int it = 0;

            for (int e = 0; e < dictionary.Count + 1; e++)
            {
                //get a copy of each entry from the dictionary
                if (e != index)
                {
                    Updated_Dictionary[Updated_Dictionary.Count] = dictionary[it];
                    it++;
                }
                //adds the new entry when reaching index and then goes back to copying
                else
                {
                    Updated_Dictionary[Updated_Dictionary.Count] = new_array;
                }

            }

            return Updated_Dictionary;
        }

        private Dictionary<int, Dictionary<int, int>> Insert(Dictionary<int, Dictionary<int, int>> dictionary, Dictionary<int, int> new_array, int index)
        {
            Dictionary<int, Dictionary<int, int>> Updated_Dictionary = new Dictionary<int, Dictionary<int, int>>();
            int it = 0;

            for (int e = 0; e < dictionary.Count + 1; e++)
            {
                //get a copy of each entry from the dictionary
                if (e != index)
                {
                    Updated_Dictionary[Updated_Dictionary.Count] = dictionary[it];
                    it++;
                }
                //adds the new entry when reaching index and then goes back to copying
                else
                {
                    Updated_Dictionary[Updated_Dictionary.Count] = new_array;
                }

            }

            return Updated_Dictionary;
        }

        private Dictionary<int, string> Insert(Dictionary<int, string> dictionary, string new_text, int index)
        {
            Dictionary<int, string> Updated_Dictionary = new Dictionary<int, string>();
            int it = 0;

            for (int e = 0; e < dictionary.Count + 1; e++)
            {
                //get a copy of each entry from the dictionary
                if (e != index)
                {
                    Updated_Dictionary[Updated_Dictionary.Count] = dictionary[it];
                    it++;
                }
                //adds the new entry when reaching index and then goes back to copying
                else
                {
                    Updated_Dictionary[Updated_Dictionary.Count] = new_text;
                }

            }

            return Updated_Dictionary;
        }

        private Dictionary<int, string> Copy(Dictionary<int, string> original)
        {
            Dictionary<int, string> copy = new Dictionary<int, string>();

            for (int i = 0; i < original.Count; i++)
            {
                copy[i] = original[i];
            }

            return copy;
        }

        private Dictionary<int, int> Copy(Dictionary<int, int> original)
        {
            Dictionary<int, int> copy = new Dictionary<int, int>();

            for (int i = 0; i < original.Count; i++)
            {
                copy[i] = original[i];
            }

            return copy;
        }

        private void Save_Logic(string logic)
        {
            string File_Path = "./logic\\" + logic + ".txt";
            StreamWriter file;
            string item;
            Dictionary<int, 
                Dictionary<int, string>> Item_Sets;
            Dictionary<int, string> Item_Set;
            Dictionary<int, string> Item_Set_Edited;
            Dictionary<int, Dictionary<int, int>> Count_Sets;
            Dictionary<int, int> Count_Set;
            Dictionary<int, string> comments;
            string comment;
            string Item_String;
            string line;
            Dictionary<int, string> Invalid_Items;
            string Item_Name;
            int Item_Count;
            Dictionary<int, string> Comment_Lines;

            file = new StreamWriter(File_Path);

            //write each item
            for (int i = 0; i < items.Length; i++)
            {
                item = items[i];
                comments = Comment_Data[logic][item];
                Invalid_Items = Logic_Invalid[logic][item];

                //write the item
                file.WriteLine(item + " {");

                //write each item set
                Item_Sets = Logic_Data[logic][item];
                Count_Sets = Count_Data[logic][item];

                for (int s = 0; s < Item_Sets.Count; s++)
                {
                    Item_Set = Item_Sets[s];
                    Count_Set = Count_Sets[s];
                    comment = comments[s];
                    line = "\t";

                    //write the comment for this set if any
                    if (comment != "")
                    {
                        //write each line of the comment
                        Comment_Lines = Split(comment, "\n");

                        for (int c = 0; c < Comment_Lines.Count; c++)
                        {
                            file.WriteLine("\t//" + Comment_Lines[c]);
                        }
                    }

                    //setup the item in the set to write to file
                    for (int n = 0; n < Item_Set.Count; n++)
                    {
                        //bombs creates more bows after this which creates extra large and largest quivers
                        if (Item_Set[n] == "Bow" && !(Item_Set.ContainsValue("Big Bomb Bag") || Item_Set.ContainsValue("Biggest Bomb Bag")))
                        {
                            Item_Set_Edited = Copy(Item_Set);
                            Item_Set_Edited[n] = "Largest Quiver";
                            Item_Sets = Insert(Item_Sets, Item_Set_Edited, s + 1);
                            Count_Sets = Insert(Count_Sets, Count_Set, s + 1);
                            comments = Insert(comments, "", s + 1);

                            Item_Set_Edited = Copy(Item_Set);
                            Item_Set_Edited[n] = "Large Quiver";
                            Item_Sets = Insert(Item_Sets, Item_Set_Edited, s + 1);
                            Count_Sets = Insert(Count_Sets, Count_Set, s + 1);
                            comments = Insert(comments, "", s + 1);
                        }
                        else if (Item_Set[n] == "Bombs")
                        {
                            Item_Set[n] = "Bomb Bag";

                            Item_Set_Edited = Copy(Item_Set);
                            Item_Set_Edited[n] = "Biggest Bomb Bag";
                            Item_Sets = Insert(Item_Sets, Item_Set_Edited, s + 1);
                            Count_Sets = Insert(Count_Sets, Count_Set, s + 1);
                            comments = Insert(comments, "", s + 1);

                            Item_Set_Edited = Copy(Item_Set);
                            Item_Set_Edited[n] = "Big Bomb Bag";
                            Item_Sets = Insert(Item_Sets, Item_Set_Edited, s + 1);
                            Count_Sets = Insert(Count_Sets, Count_Set, s + 1);
                            comments = Insert(comments, "", s + 1);
                        }

                        Item_Name = Item_Set[n];
                        Item_Count = Count_Set[n];

                        //if all the player need is 1 rupee, then output "1 Rupees" in the logic file
                        if (Item_Name == "Rupees" && Item_Count == 1)
                        {
                            Item_String = "1 Rupees";
                        }
                        else
                        {
                            //get the item and count into a string
                            Item_String = Combine_Count_Item(Item_Name, Item_Count);
                        }

                        //add the item separator if not the first item
                        if (n > 0)
                        {
                            line += ", ";
                        }

                        line += Item_String;
                    }

                    //write the item set to the file if there were items. If there were none for this item, then write an empty line
                    if (line != "\t" || s == 0)
                    {
                        file.WriteLine(line);
                    }
                }

                //write the invalid list if any
                if (Invalid_Items[0] != "")
                {
                    line = "";
                    for (int inv = 0; inv < Invalid_Items.Count; inv++)
                    {
                        if (inv == 0)
                        {
                            line += "\t#";   //tab and pound at the start
                        }
                        else
                        {
                            line += ", ";   //separator
                        }

                        line += Invalid_Items[inv];
                    }
                    file.WriteLine(line);
                }

                //write the closing bracket
                file.WriteLine("}");

                //clear the buffer after each item logic to make sure there is room
                file.Flush();

                //write an empty line to make it look nicer between the items
                if (i < items.Length - 1)
                {
                    file.WriteLine("");
                }
            }

            file.Close();
        }

        private void Save_Logics()
        {
            for (int l = 0; l < Logic_Data.Count; l++)
            {
                Save_Logic(Logic_Data.Keys.ElementAt(l));
            }
        }

        private void SaveLogic_Button_Click(object sender, EventArgs e)
        {
            //Save each logic
            Save_Logics();

            //update the logic combobox in the main form
            form.Load_Logic();

            this.Close();
        }

        private void Clear_Set(string logic, string item, int index)
        {
            Dictionary<int, string> Item_Set;
            Dictionary<int, int> Count_Set;
            
            //item
            Item_Set = new Dictionary<int, string>();
            Item_Set.Add(0, "");
            Logic_Data[logic][item][index] = Item_Set;
            
            //count
            Count_Set = new Dictionary<int, int>();
            Count_Set.Add(0, 0);
            Count_Data[logic][item][index] = Count_Set;

            //comment
            Comment_Data[logic][item][index] = "";
        }

        private void DeleteItemGroup_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            int Set_Index;

            if (ItemGroups_ListBox.SelectedIndex != -1)
            {
                logic = LogicFiles_ListBox.SelectedItem.ToString();
                item = Items_ListBox.SelectedItem.ToString();
                Set_Index = ItemGroups_ListBox.SelectedIndex;

                //remove the item set
                if (Logic_Data[logic][item].Count > 1)
                {
                    Logic_Data[logic][item] = Remove(Logic_Data[logic][item], Set_Index);
                    Count_Data[logic][item] = Remove(Count_Data[logic][item], Set_Index);
                    Comment_Data[logic][item] = Remove(Comment_Data[logic][item], Set_Index);

                    //remove the data from the gui
                    ItemGroups_ListBox.Items.RemoveAt(Set_Index);
                }
                //clear this item set because it's the only one
                else
                {
                    Clear_Set(logic, item, Set_Index);
                    ItemGroups_ListBox.Items[0] = "";
                }
                
                //select where the last item set was
                if (ItemGroups_ListBox.Items.Count > Set_Index)
                {
                    ItemGroups_ListBox.SelectedIndex = Set_Index;
                }
                else
                {
                    ItemGroups_ListBox.SelectedIndex = Set_Index - 1;
                }
            }
        }

        private void Duplicate_ItemSet_Button_Click(object sender, EventArgs e)
        {
            string logic;
            string item;
            int Item_Index;
            int Item_Set_Index;
            Dictionary<int, Dictionary<int, string>> Item_Sets;
            Dictionary<int, Dictionary<int, int>> Count_Sets;
            Dictionary<int, string> comments;
            Dictionary<int, string> Item_Set;
            Dictionary<int, int> Count_Set;
            string comment;

            //if a logic is selected
            if (LogicFiles_ListBox.SelectedIndex != -1) {
                logic = LogicFiles_ListBox.SelectedItem.ToString();
                
                //if an item is selected
                if (Items_ListBox.SelectedIndex != -1)
                {
                    item = Items_ListBox.SelectedItem.ToString();
                    Item_Index = Items_ListBox.SelectedIndex;

                    //if an item set is selected
                    if (ItemGroups_ListBox.SelectedIndex != -1)
                    {
                        Item_Set_Index = ItemGroups_ListBox.SelectedIndex;

                        //copy the item set
                        Item_Sets = Logic_Data[logic][item];
                        Count_Sets = Count_Data[logic][item];
                        comments = Comment_Data[logic][item];

                        Item_Set = Copy(Item_Sets[Item_Set_Index]);
                        Count_Set = Copy(Count_Sets[Item_Set_Index]);
                        comment = comments[Item_Set_Index];

                        //insert a duplicate of the item set right after
                        Item_Sets = Insert(Item_Sets, Item_Set, Item_Set_Index + 1);
                        Count_Sets = Insert(Count_Sets, Count_Set, Item_Set_Index + 1);
                        comments = Insert(comments, comment, Item_Set_Index + 1);

                        //update the data
                        Logic_Data[logic][item] = Item_Sets;
                        Count_Data[logic][item] = Count_Sets;
                        Comment_Data[logic][item] = comments;

                        //update the gui, and select the duplicate item set
                        Items_ListBox_SelectedIndexChanged(Duplicate_ItemSet_Button, new EventArgs());
                        ItemGroups_ListBox.SelectedIndex = Item_Set_Index + 1;
                    }
                }
            }
        }
    }
}
