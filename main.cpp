#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <ctime>
#include <cstdlib>

using namespace std;

ofstream err_file;

template<typename value>
bool Vector_Has(vector<value>, value);

void Error(string Error_Message) {
    cout << Error_Message;
    err_file << Error_Message;
    err_file.close();
    exit(0);
}

class Time {
    public:
        int Day;
        bool isNight;
        int timeStart;
        int DayEnd;
        int isNightEnd;
        int timeEnd;
        vector<string> Items_Needed;
        map<string, bool> Flags;
        vector<string> Flag_Setters;

        void Default_Flags() {
            Flags["KillSakon"] = false;
            Flags["NoKillSakon"] = false;
        }

        void Set_Flag(string flag) {
            if (flag != "") {
                Flags[flag] = true;
            }
        }

        void Flag_Setter(string flag) {
            if (flag != "") {
                Flag_Setters.push_back(flag);
            }
        }

        Time() {
            Day = -1;
            isNight = false;
            timeStart = -1;
            timeEnd = -1;
            DayEnd = -1;
            isNightEnd = false;

            Default_Flags();
        }

        Time(int day_number, bool is_night, int start_time, int day_number_end, int is_night_end, int end_time) {
            Day = day_number;
            isNight = is_night;
            timeStart = start_time;
            DayEnd = day_number_end;
            isNightEnd = is_night_end;
            timeEnd = end_time;

            Default_Flags();
        }

        Time(int day_number, bool is_night, int start_time, int day_number_end, int is_night_end, int end_time, string flag) {
            Day = day_number;
            isNight = is_night;
            timeStart = start_time;
            DayEnd = day_number_end;
            isNightEnd = is_night_end;
            timeEnd = end_time;

            Default_Flags();

            Set_Flag(flag);
        }

        Time(int day_number, bool is_night, int start_time, int day_number_end, int is_night_end, int end_time, string flag, string set_flag) {
            Day = day_number;
            isNight = is_night;
            timeStart = start_time;
            DayEnd = day_number_end;
            isNightEnd = is_night_end;
            timeEnd = end_time;

            Default_Flags();

            Flag_Setter(set_flag);

            Set_Flag(flag);
        }

        Time(int day_number, bool is_night, int start_time, int day_number_end, int is_night_end, int end_time, string flag, string set_flag, vector<string> items) {
            Day = day_number;
            isNight = is_night;
            timeStart = start_time;
            DayEnd = day_number_end;
            isNightEnd = is_night_end;
            timeEnd = end_time;

            Default_Flags();

            Set_Flag(flag);

            Items_Needed = items;
        }

        operator=(const Time other) {
            Day = other.Day;
            isNight = other.isNight;
            timeStart = other.timeStart;
            DayEnd = other.DayEnd;
            isNightEnd = other.isNightEnd;
            timeEnd = other.timeEnd;

            Flags = other.Flags;
        }
};

//Item class
class Item {
    public:

        string Name;
        vector<string> Address_Get;
        vector<string> Address_Item_ID;
        vector<string> Address_Get_Item_ID;
        vector<string> Address_Text_ID;
        vector<string> Other_Locations;
        vector<string> Other_Locations2;
        vector<string> ID_Minus_61_Locations;
        vector<string> Song1_Locatinos;
        vector<string> Song2_Locatinos;
        vector<string> Show_Item_ID_Address;
        vector<string> Items_Needed;    //the items that the logic used to determine the item is obtainable
        vector<string> Item_Count_Locations;
        vector<string> Item_Count_Locations2;

        string Pool;

        string Item_ID;
        string Item_ID2;
        string Get_Item_ID;
        string Text_ID;
        string Flag;
        string Obj;
        string Get_Item_Model;
        string Other_locations_Data;
        string Other_locations_Data2;
        string Song1_ID;
        string Song2_ID;
        string Show_Item_ID;
        string Item_Count;
        string Item_Count2;

        vector<Time> Time_Setup;
        vector<Time> Time_Get;

        bool gives_item;    //whether or not the item gives something yet (if an item has been placed here or not)
        bool can_get;       //whether or not another item (or itself) gives this item
        int value;      //how valuable the item is, the more other items that can be obtained from using this item, the higher the number

        Item() {

        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = "00";
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, string item_count, vector<string> item_count_locs) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = "00";

            Item_Count = item_count;
            Item_Count_Locations = item_count_locs;
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, string inv_id, vector<string> add_get_table, vector<string> add_item_id, string item_count, vector<string> item_count_locations) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = inv_id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = item_count;
            Item_Count_Locations = item_count_locations;
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, string inv_id, vector<string> add_get_table, vector<string> add_item_id, string item_count, vector<string> item_count_locations, string item_count2, vector<string> item_count2_locations) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = inv_id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = item_count;
            Item_Count_Locations = item_count_locations;
            Item_Count2 = item_count2;
            Item_Count_Locations2 = item_count2_locations;
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Text_ID = add_text_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = "00";
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id, string item_count, vector<string> item_count_locations, int just_overlading) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Text_ID = add_text_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = item_count;
            Item_Count_Locations = item_count_locations;
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id, vector<string> add_get_item_id) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Get_Item_ID = add_get_item_id;
            Address_Text_ID = add_text_id;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = "00";
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id, vector<string> add_get_item_id, vector<string> Inverted_Song_Locations) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Get_Item_ID = add_get_item_id;
            Address_Text_ID = add_text_id;

            ID_Minus_61_Locations = Inverted_Song_Locations;

            Time_Get = time_available;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = "00";
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id, vector<string> add_get_item_id, vector<string> Inverted_Song_Locations, string songid1, string songid2, vector<string> song1_loc, vector<string> song2_loc) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Get_Item_ID = add_get_item_id;
            Address_Text_ID = add_text_id;

            ID_Minus_61_Locations = Inverted_Song_Locations;

            Time_Get = time_available;

            Song1_ID = songid1;
            Song2_ID = songid2;
            Song1_Locatinos = song1_loc;
            Song2_Locatinos = song2_loc;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = "00";
        }

        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id, vector<string> add_get_item_id, vector<string> Inverted_Song_Locations, string songid1, string songid2, vector<string> song1_loc, vector<string> song2_loc, string item_count, vector<string> item_count_locations) {
            Name = nam;

            Item_ID = id;
            Item_ID2 = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Get_Item_ID = add_get_item_id;
            Address_Text_ID = add_text_id;

            ID_Minus_61_Locations = Inverted_Song_Locations;

            Time_Get = time_available;

            Song1_ID = songid1;
            Song2_ID = songid2;
            Song1_Locatinos = song1_loc;
            Song2_Locatinos = song2_loc;

            gives_item = false;
            can_get = false;

            value = 0;

            Item_Count = item_count;
            Item_Count_Locations = item_count_locations;
        }

        /*
        Item(vector<Time> time_available, string nam, string id, string get_id, string text, string flg, string ob, string get_item_model, string pol, vector<string> add_get_table, vector<string> add_item_id, vector<string> add_text_id, vector<string> add_get_item_id, vector<string> Locations, string Locations_Data, vector<string> Locations2, string Locations_Data2) {
            Name = nam;

            Item_ID = id;
            Get_Item_ID = get_id;
            Text_ID = text;
            Flag = flg;
            Obj = ob;
            Get_Item_Model = get_item_model;

            Pool = pol;

            Address_Get = add_get_table;
            Address_Item_ID = add_item_id;
            Address_Get_Item_ID = add_get_item_id;
            Address_Text_ID = add_text_id;
            Other_Locations = Locations;
            Other_locations_Data = Locations_Data;
            Other_Locations2 = Locations2;
            Other_locations_Data2 = Locations_Data2;

            Time_Get = time_available;
        }
        */
};

class Inventory {
    public:
        map<string, bool> Items;

        void Default_Inventory() {
            Items.clear();

            Items["Postman's Hat"] = false;
            Items["All-Night Mask"] = false;
            Items["Blast Mask"] = false;
            Items["Stone Mask"] = false;
            Items["Great Fairy's Mask"] = false;
            Items["Deku Mask"] = false;
            Items["Keaton Mask"] = false;
            Items["Bremen Mask"] = false;
            Items["Bunny Hood"] = false;
            Items["Don Gero's Mask"] = false;
            Items["Mask of Scents"] = false;
            Items["Goron Mask"] = false;
            Items["Romani's Mask"] = false;
            Items["Circus Leader's Mask"] = false;
            Items["Kafei's Mask"] = false;
            Items["Couple's Mask"] = false;
            Items["Mask of Truth"] = false;
            Items["Zora Mask"] = false;
            Items["Kamaro's Mask"] = false;
            Items["Gibdo Mask"] = false;
            Items["Garo Mask"] = false;
            Items["Captain's Hat"] = false;
            Items["Giant's Mask"] = false;
            Items["Fierce Deity Mask"] = false;

            Items["Ocarina"] = true;
            Items["Quiver"] = false;
            Items["Bow"] = false;
            Items["Large Quiver"] = false;
            Items["Largest Quiver"] = false;
            Items["Fire Arrow"] = false;
            Items["Ice Arrow"] = false;
            Items["Light Arrow"] = false;
            Items["Moon's Tear"] = false;
            Items["Land Title Deed"] = false;
            Items["Swamp Title Deed"] = false;
            Items["Mountain Title Deed"] = false;
            Items["Ocean Title Deed"] = false;
            Items["Bombs"] = false;
            Items["Bomb Bag"] = false;
            Items["Big Bomb Bag"] = false;
            Items["Biggest Bomb Bag"] = false;
            Items["Deku Stick"] = false;
            Items["Nuts"] = false;
            Items["Deku Nuts"] = false;
            Items["Deku Nuts (10)"] = false;
            Items["Magic Beans"] = false;
            Items["Room Key"] = false;
            Items["Letter to Kafei"] = false;
            Items["Pendant of Memories"] = false;
            Items["Express Letter to Mama"] = false;
            Items["Powder Keg"] = false;
            Items["Pictograph Box"] = false;
            Items["Lens of Truth"] = false;
            Items["Hookshot"] = false;
            Items["Great Fairy's Sword"] = false;

            /*
            Items["Green Rupee"] = false;
            Items["Red Rupee"] = false;
            Items["Purple Rupee"] = false;
            Items["Silver Rupee"] = false;
            Items["Gold Rupee"] = false;
            */

            Items["Clocktown Map"] = false;
            Items["Woodfall Map"] = false;
            Items["Snowhead Map"] = false;
            Items["Romani Ranch Map"] = false;
            Items["Great Bay Map"] = false;
            Items["Stone Tower Map"] = false;

            Items["Song of Healing"] = false;
            Items["Song of Soaring"] = false;
            Items["Epona's Song"] = false;
            Items["Song of Storms"] = false;
            Items["Sonata of Awakening"] = false;
            Items["Goron Lullaby"] = false;
            Items["New Wave Bossa Nova"] = false;
            Items["Elegy of Emptiness"] = false;
            Items["Oath to Order"] = false;
        }

        Inventory() {
            Default_Inventory();
        }

        void Add_Item(string Item) {
            if (Item == "Bow" || Item == "Large Quiver" || Item == "Largest Quiver") {
                Items["Quiver"] = true;
            }
            else if (Item == "Bomb Bag" || Item == "Big Bomb Bag" || Item == "Biggest Bomb Bag") {
                Items["Bombs"] = true;
            }
            else if (Item == "Deku Nuts" || Item == "Deku Nuts (10)") {
                Items["Nuts"] = true;
            }

            Items[Item] = true;
        }

        bool All(map<string, Item> List) {
            vector<string> keys;

            //get the keys for each item
            for (const auto& kv : Items) {
                keys.push_back(kv.first);
            }

            //check to see if each item was obtained
            for (int i = 0; i < keys.size(); i++) {
                if (!Items[keys[i]]) {
                    //cout << Get_Source(keys[i], List) << " => " << keys[i] << endl;
                    return false;
                }
            }

            return true;
        }

        bool Print_Missing(map<string, Item> List) {
            vector<string> Missing_Items;
            vector<string> Missing_Items_Source;

            Missing_Items = Get_Missing_Items();
            Missing_Items_Source = Get_Missing_Items_Source(Missing_Items, List);

            for (int i = 0; i < Missing_Items_Source.size(); i++) {
                //cout << Missing_Items_Source[i] << " => " << Missing_Items[i] << endl;
            }
        }

        string Get_Source(string item, map<string, Item> List) {
            vector<string> keys;

            //get the keys for each item
            for (const auto& kv : List) {
                keys.push_back(kv.first);
            }

            for (int i = 0; i < keys.size(); i++) {
                if (List[keys[i]].Name == item) {
                    return List[keys[i]].Name;
                }
            }

            Error("No items give " + item);
            //cout << "Could not find source for " << item << endl;
            //exit(0);
        }

        vector<string> Get_Missing_Items_Source(vector<string> Missing_Items, map<string, Item> List) {
            vector<string> keys;
            vector<string> Missing_Items_Source;


            //get the keys for each item
            for (const auto& kv : Items) {
                keys.push_back(kv.first);
            }

            //get what gives current item
            for (int i = 0; i < Missing_Items.size(); i++) {
                for (int j = 0; j < keys.size(); j++) {
                    if (List[keys[j]].Name == Missing_Items[i]) {
                        //cout << keys[j] << "\t" << List[keys[j]].Name << "\t" << Missing_Items[i] << endl;
                        Missing_Items_Source.push_back(keys[j]);
                        j = keys.size();
                    }
                }
            }


            return Missing_Items_Source;
        }

        vector<string> Get_Missing_Items() {
            vector<string> keys;
            vector<string> missing;

            //get the keys for each item
            for (const auto& kv : Items) {
                keys.push_back(kv.first);
            }

            //check to see if each item was obtained
            for (int i = 0; i < keys.size(); i++) {
                if (!Items[keys[i]]) {
                    missing.push_back(keys[i]);
                }
            }

            return missing;
        }
};

fstream inFile;
ofstream outFile;
map<string, Item> Items = {};
vector<string> Item_Names;
vector<string> Item_Keys;
vector<string> Bottle_Names;
vector<string> Bottle_Keys;
vector<string> TMask_Names;
vector<string> TMask_Keys;
string Rom_Location = "mm2.z64";    //location of the decompressed rom
map<string, map<string, string> > Settings;
vector<string> Tried_Items; //for when swapping items when checking logic

string Single_Hex(int number) {
    if (number == 0) {
        return "0";
    }
    else if (number == 1) {
        return "1";
    }
    else if (number == 2) {
        return "2";
    }
    else if (number == 3) {
        return "3";
    }
    else if (number == 4) {
        return "4";
    }
    else if (number == 5) {
        return "5";
    }
    else if (number == 6) {
        return "6";
    }
    else if (number == 7) {
        return "7";
    }
    else if (number == 8) {
        return "8";
    }
    else if (number == 9) {
        return "9";
    }
    else if (number == 10) {
        return "A";
    }
    else if (number == 11) {
        return "B";
    }
    else if (number == 12) {
        return "C";
    }
    else if (number == 13) {
        return "D";
    }
    else if (number == 14) {
        return "E";
    }
    else if (number == 15) {
        return "F";
    }
    else {
        return "";
    }
}

string hex_to_binary(string hex) {
    string binary = "";
    string solo_hex;

    for (int i = 0; i < hex.size(); i++) {
        solo_hex = hex[i];

        if (solo_hex == "0") {
            binary += "0000";
        }
        else if (solo_hex == "1") {
            binary += "0001";
        }
        else if (solo_hex == "2") {
            binary += "0010";
        }
        else if (solo_hex == "3") {
            binary += "0011";
        }
        else if (solo_hex == "4") {
            binary += "0100";
        }
        else if (solo_hex == "5") {
            binary += "0101";
        }
        else if (solo_hex == "6") {
            binary += "0110";
        }
        else if (solo_hex == "7") {
            binary += "0111";
        }
        else if (solo_hex == "8") {
            binary += "1000";
        }
        else if (solo_hex == "9") {
            binary += "1001";
        }
        else if (solo_hex == "A") {
            binary += "1010";
        }
        else if (solo_hex == "B") {
            binary += "1011";
        }
        else if (solo_hex == "C") {
            binary += "1100";
        }
        else if (solo_hex == "D") {
            binary += "1101";
        }
        else if (solo_hex == "E") {
            binary += "1110";
        }
        else if (solo_hex == "F") {
            binary += "1111";
        }
    }

    return binary;
}

string dec_to_hex(int number) {
    string hex = "";
    string binary = "";
    int power;
    char bin;
    int value = 0;

    number %= 256;

    //get the binary form of the numbers
    for (int i = 8; i > 0; i--) {
        power = pow(2,i)/2;

        if (number >= power) {
            number -= power;
            binary += "1";
        }
        else {
            binary += "0";
        }
    }

    //convert the binary form to the hex form
    for (int i = 0; i < binary.size()/2; i++) {
        bin = binary[i];
        power = pow(2,4-i)/2;

        if (bin == '1') {
            value += power;
        }
    }

    hex = Single_Hex(value);
    value = 0;

    //convert the last four binary
    for (int i = 0; i < binary.size()/2; i++) {
        bin = binary[i+4];
        power = pow(2,4-i)/2;

        if (bin == '1') {
            value += power;
        }
    }

    hex += Single_Hex(value);

    return hex;
}

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string hex_to_string(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

int Random(int min, int max) {
    int number = rand();

    if (min == max) {
        return max;
    }

    number = number % max + min;

    return number;
}

/*
template <typename value>
void shuffle(vector<value> &vec) {
    vector<value> Shuffled;
    int index = 0;
    int stop = vec.size()-1;

    for (index = 0; index < stop; index++) {
        int number = Random(0, vec.size());

        Shuffled.push_back(vec[number]);
        vec.erase(vec.begin() + number);
    }

    Shuffled.push_back(vec[0]);

    vec = Shuffled;
}
*/

double Double_Mod(double number, int mod) {
    int C = 0;

    while (number > mod) {
        number -= mod;
        C++;
    }

    return number;
}

template<typename big, typename small>
int IndexOf(big Data, small Text) {
    for (int i = 0; i < Data.size(); i++) {
        if (Data[i] == Text) {
            return i;
        }
    }

    return -1;
}

int IndexOf_S(string Data, string Text) {
    for (int i = 0; i < Data.size(); i++) {
        int a = i;
        bool same = true;

        for (int b = 0; b < Text.size(); b++) {
            if (Data[a] != Text[b]) {
                same = false;
            }

            a++;
        }

        if (same) {
            return i;
        }
    }

    return -1;
}

bool Contains(string text, char chr) {
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == chr) {
            return true;
        }
    }

    return false;
}

void log(vector<string> stuff) {
    for (int i = 0; i < stuff.size(); i++) {
        cout << stuff[i] << endl;
    }

    cout << endl;
}

//converts a hexadecimal string to a decimal integer
int hex_to_decimal(string hex) {
    int number = 0;

    for (int i = 0; i < hex.size(); i++) {
        number *= 16;
        char h = hex[i];

        if (h == 'A') {
            number += 10;
        }
        else if (h == 'B') {
            number += 11;
        }
        else if (h == 'C') {
            number += 12;
        }
        else if (h == 'D') {
            number += 13;
        }
        else if (h == 'E') {
            number += 14;
        }
        else if (h == 'F') {
            number += 15;
        }
        else {
            number += (h - '0');
        }
    }

    return number;
}

int string_to_dec(string text) {
    int number = 0;

    for (int i = 0; i < text.size(); i++) {
        number *= 10;
        number += (text[i] - '0');
    }

    return number;
}

string dec_to_string(int dec, int depth = 0) {
    char digit;

    if (dec == 0) {
        if (depth == 0) {
            return "0";
        }
        else {
            return "";
        }
    }
    else {
        digit = (dec%10) + '0';
        return dec_to_string(dec/10, ++depth) + digit;
    }
}

bool isNumber(string text) {
    for (int i = 0; i < text.size(); i++) {
        if ((text[i] - '0') > 9) {
            return false;
        }
    }

    return true;
}

void shuffle(map<string, Item> &Items, string &Seed, int Seed_Increase = 0) {
    vector<string> Item_Names;
    vector<string> Item_Keys;
    vector<string> Same_Pool;
    vector<string> Same_Pool_2;
    vector<string> Shuffled;
    vector<string> manual;
    string Pool_Name = "";
    int index = -1;
    int stop = 0;

    //create random seed if no seed is specified
    if (Seed == "") {
        Seed = dec_to_string(time(0));
    }

    //save the seed in the spoiler log (only if this is the first time through randomizing items)
    if (Seed_Increase == 0) {
        outFile << "Seed: " << Seed << endl << endl;
    }

    //if Seed has characters
    if (!isNumber(Seed)) {
        srand(hex_to_decimal(string_to_hex(Seed)) + Seed_Increase);
    }
    //if string is only numbers
    else {
        srand(string_to_dec(Seed) + Seed_Increase);
    }

    //Get keys
    for (const auto& kv : Items) {
        //only add item to be randomized if in a pool (not equal to "")
        if (Items[kv.first].Pool != "") {
            Item_Names.push_back(kv.first);
            Item_Keys.push_back(kv.first);
        }
    }

    //Shuffle every pool
    while(Item_Names.size() > 0) {
        //Get items in same pool
        for (int i = 0; i < Item_Names.size(); i++) {
            //item is manually inserted into a different spot
            if (Contains(Items[Item_Names[i]].Pool, '#')) {
                manual.push_back(Item_Names[i]);
            }
            //if this item is randomized
            else if (Items[Item_Names[i]].Pool != "") {
                //get the pool name
                if (Pool_Name == "") {
                    Pool_Name = Items[Item_Names[i]].Pool;
                }

                //if this item is in the same pool, then add it to the pool list
                if (Items[Item_Names[i]].Pool == Pool_Name) {
                    Same_Pool.push_back(Item_Names[i]);
                }
            }
        }

        //if there are manually inserted items
        if (manual.size() > 0) {
            for (int i = 0; i < manual.size(); i++) {
                //update item
                Items[manual[i]].Name = Items[manual[i]].Pool.substr(1);    //removes '#' and makes name = the text that is left (what this item gives instead)

                //get index of the item
                index = IndexOf(Item_Names, manual[i]);

                //remove item
                if (index != -1) {
                    Item_Names.erase(Item_Names.begin() + index);
                }
                else {
                    cout << "Error\nCouldn't find " << manual[i] << " in " << "Item_Names" << endl;
                }
            }
        }

        //Remove items in the current pool from the names list
        for (int i = 0; i < Same_Pool.size(); i++) {
            //get index of the item
            index = IndexOf(Item_Names, Same_Pool[i]);
            //remove item
            if (index != -1) {
                Item_Names.erase(Item_Names.begin() + index);
            }
            else {
                cout << "Error\nCouldn't find " << Same_Pool[i] << " in " << "Item_Names" << endl;
            }
        }

        //shuffle the items in the current pool
        stop = Same_Pool.size()-1;
        Same_Pool_2 = Same_Pool;

        //shuffle the items in the same pool
        for (int i = 0; i < stop; i++) {
            int number = Random(0, Same_Pool_2.size());

            Shuffled.push_back(Same_Pool_2[number]);
            Same_Pool_2.erase(Same_Pool_2.begin() + number);
        }
        //add the last item in the pool to the shuffled list
        if (Same_Pool_2.size() > 0) {
            Shuffled.push_back(Same_Pool_2[0]);
        }

        //Replace Name in Items with item to be put there instead
        for (int i = 0; i <= stop; i++) {
            Items[Same_Pool[i]].Name = Shuffled[i];
        }

        //clear the pool for the next one
        Same_Pool.clear();
        Same_Pool_2.clear();
        Shuffled.clear();
        manual.clear();
        Pool_Name = "";
    };
}

string binary_to_hex(string binary) {
    string hex = "";
    string single;

    for (int i = 0; i < binary.size(); i+=4) {
        single = binary.substr(i, 4);

        if (single == "0000") {
            hex += "0";
        }
        else if (single == "0001") {
            hex += "1";
        }
        else if (single == "0010") {
            hex += "2";
        }
        else if (single == "0011") {
            hex += "3";
        }
        else if (single == "0100") {
            hex += "4";
        }
        else if (single == "0101") {
            hex += "5";
        }
        else if (single == "0110") {
            hex += "6";
        }
        else if (single == "0111") {
            hex += "7";
        }
        else if (single == "1000") {
            hex += "8";
        }
        else if (single == "1001") {
            hex += "9";
        }
        else if (single == "1010") {
            hex += "A";
        }
        else if (single == "1011") {
            hex += "B";
        }
        else if (single == "1100") {
            hex += "C";
        }
        else if (single == "1101") {
            hex += "D";
        }
        else if (single == "1110") {
            hex += "E";
        }
        else if (single == "1111") {
            hex += "F";
        }
        else {
            hex += "";
        }
    }

    return hex;
}

string decimal_to_hex(int dec) {
    string hex = "";
    int d = 0;

    if (dec == 0) {
        return "";
    }
    else {
        d = (dec % 16);

        if (d >= 10) {
            hex = (d/10) + '0';
            hex += (d%10) + '0';
        }
        else {
            hex = d + '0';
        }

        if (hex == "10") {
            hex = "A";
        }
        else if (hex == "11") {
            hex = "B";
        }
        else if (hex == "12") {
            hex = "C";
        }
        else if (hex == "13") {
            hex = "D";
        }
        else if (hex == "14") {
            hex = "E";
        }
        else if (hex == "15") {
            hex = "F";
        }

        return decimal_to_hex(dec / 16) + hex;
    }
}

void Open_File(string filename, fstream &file) {
    file.open(filename.c_str(), fstream::binary | fstream::out | fstream::in);

    if (!file) {
        cout << "\nError - opening\t" + filename;
        exit(0);
    }
}

string RemoveAll(string text, char chr) {
    string newText = "";

    for (int i = 0; i < text.length(); i++) {
        if (text[i] != chr) {
            newText += text[i];
        }
    }

    return newText;
}

void Write_To_Rom(int address, string hex) {
    try {
        if (Contains(hex, ' ')) {
            hex = RemoveAll(hex, ' ');
        }

        inFile.open(Rom_Location, fstream::binary | fstream::out | fstream::in);
        inFile.seekg(address);
        inFile.write(hex_to_string(hex).c_str(), hex.length()/2);
        inFile.close();
    }
    catch(exception e) {
        err_file << "Error writing to the decompressed rom";
        err_file.close();
        exit(0);
    }
}

string Item_Get(Item it) {
    string data = "";

    data = it.Item_ID + it.Flag + it.Get_Item_Model + it.Text_ID + it.Obj;

    return data;
}

//replace each 1 with a 0, and each 0 with a 1
string invert_binary(string binary) {
    string inverted = "";

    for (int i = 0; i < binary.size(); i++) {
        if (binary[i] == '0') {
            inverted += "1";
        }
        else {
            inverted += "0";
        }
    }

    return inverted;
}

string Hex_Minus(string hex, string hex_2) {
    string New_Hex;
    int Hex_Dec = hex_to_decimal(hex);
    int Hex_2_Dec = hex_to_decimal(hex_2);
    int New_Hex_Dec = Hex_Dec - Hex_2_Dec;

    //if it's negative then get FFFF - the value
    if (New_Hex_Dec < 0) {
        New_Hex_Dec *= -1;  //make it positive
        New_Hex_Dec -= 1;   //if it goes over 100, then the game ignores the 1 and only looks at the 00, but it's 1 too high, so doing this fixes it to give the correct item
        New_Hex = dec_to_hex(New_Hex_Dec);  //convert back to hex
        New_Hex = hex_to_binary(New_Hex);    //convert the new hex to binary
        New_Hex = invert_binary(New_Hex);   //invert the binary
        New_Hex = binary_to_hex(New_Hex);   //convert the inverted binary to hex
    }
    else {
        New_Hex = dec_to_hex(New_Hex_Dec);  //convert from positive number back to hex
    }

    return New_Hex;
}

template<typename value>
vector<string> Get_Keys(map<string, value> data) {
    vector<string> keys;

    for (const auto& kv : data) {
        keys.push_back(kv.first);
    }

    return keys;
}

string Get_Source(string item) {
    vector<string> keys;

    keys = Get_Keys(Items);

    for (int i = 0; i < keys.size(); i++) {
        if (Items[keys[i]].Name == item) {
            return keys[i];
        }
    }

    return "";
}

void Place_Items(map<string, Item> &Items, bool Songs_Same_Pool) {
    string Old_Item = "";
    string New_Item = "";

    for (const auto& kv : Items) {
        Old_Item = kv.first;
        New_Item = Items[Old_Item].Name;

        //replace each item id entry - starts at 1 because 0 is where the item is in the inventory screen (used for replacing deku mask)
        if (Items[Old_Item].Address_Item_ID.size() > 1) {
            for (int i = 1; i < Items[Old_Item].Address_Item_ID.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Address_Item_ID[i]), Items[New_Item].Item_ID);
            }
        }

        //replace each get item entry
        if (Items[Old_Item].Address_Get.size() > 0) {
            //cout << Items[Old_Item].Address_Get[0] << "\t" << Item_Get(Items[New_Item]) << endl;

            for (int i = 0; i < Items[Old_Item].Address_Get.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Address_Get[i]), Item_Get(Items[New_Item]));
            }
        }

        //replace each text id location
        if (Items[Old_Item].Address_Text_ID.size() > 0) {
            for (int i = 0; i < Items[Old_Item].Address_Text_ID.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Address_Text_ID[i]), Items[New_Item].Text_ID);
            }
        }

        //place the other locations (for rupees, it's the rupees amount) - this makes rupees only randomizable with other rupees
        if (Items[Old_Item].Other_Locations.size() > 0) {
            for (int i = 0; i < Items[Old_Item].Other_Locations.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Other_Locations[i]), Items[New_Item].Other_locations_Data);
            }
        }

        //making the get item give the rupee value of the correct rupee
        if (Items[Old_Item].Other_Locations2.size() > 0) {
            for (int i = 0; i < Items[Old_Item].Other_Locations2.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Other_Locations2[i]), Items[Old_Item].Other_locations_Data2);
            }
        }

        //replace the song staffs with the new song if a song is replacing a song AND if all songs are songs to prevent getting locked out of an item
        if (Items[Old_Item].Song2_Locatinos.size() > 0 && Items[New_Item].Song2_Locatinos.size() > 0 && Songs_Same_Pool) {
            for (int i = 0; i < Items[Old_Item].Song1_Locatinos.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Song1_Locatinos[i]), Items[New_Item].Song1_ID);
            }
            for (int i = 0; i < Items[Old_Item].Song2_Locatinos.size(); i++) {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Song2_Locatinos[i]), Items[New_Item].Song2_ID);
            }
        }
        //place the song ids minus 61 where they go
        else if (Items[Old_Item].ID_Minus_61_Locations.size() > 0) {
            for (int i = 0; i < Items[Old_Item].ID_Minus_61_Locations.size(); i++) {
                //hex minus will make it wrap back around when 61 is added to it
                Write_To_Rom(hex_to_decimal(Items[Old_Item].ID_Minus_61_Locations[i]), Hex_Minus(Items[New_Item].Item_ID, "61"));
            }
        }
    }
}

void Update_Wallet(int location, string wallet_amount) {
    int wallet_int = string_to_dec(wallet_amount);
    string wallet_hexL = dec_to_hex(wallet_int >> 8);   //left byte
    string wallet_hexR = dec_to_hex(wallet_int);        //right byte
    string wallet_size = wallet_hexL + wallet_hexR;

    Write_To_Rom(location, wallet_size);
}

void Change_Rupees(map<string, string> wallet_amounts) {
    int small_digits = wallet_amounts["Small"].size();
    int med_digits = wallet_amounts["Medium"].size();
    int large_digits = wallet_amounts["Large"].size();

    if (small_digits > 3) {
        small_digits = 3;
    }
    if (med_digits > 3) {
        med_digits = 3;
    }
    if (large_digits > 3) {
        large_digits = 3;
    }


    //small
    //write padding for small wallet
    Write_To_Rom(12935772, "000" + dec_to_string(3 - small_digits));

    //write number of digits for small wallet
    Write_To_Rom(12935780, "000" + dec_to_string(small_digits));

    //write the small wallet amount
    Update_Wallet(12944236, wallet_amounts["Small"]);


    //adult
    //write padding for adult wallet
    Write_To_Rom(12935774, "000" + dec_to_string(3 - med_digits));

    //write number of digits for adult wallet
    Write_To_Rom(12935782, "000" + dec_to_string(med_digits));

    //write the adult wallet amount
    Update_Wallet(12944238, wallet_amounts["Medium"]);

    //update the text for adult wallet
    string plurarl = "s";
    if (string_to_dec(wallet_amounts["Medium"]) == 1) {
        plurarl = "";
    }
    Write_To_Rom(11342319, string_to_hex("You can now carry ") + "06" + string_to_hex(wallet_amounts["Medium"]) + "00" + string_to_hex(" Rupee" + plurarl) + "002EBF");


    //giant
    //write padding for giant wallet
    Write_To_Rom(12935776, "000" + dec_to_string(3 - large_digits));

    //write number of digits for giant wallet
    Write_To_Rom(12935784, "000" + dec_to_string(large_digits));

    //write the giant wallet amount
    Update_Wallet(12944240, wallet_amounts["Large"]);

    //update the text for giant wallet
    plurarl = "s";
    if (string_to_dec(wallet_amounts["Large"]) == 1) {
        plurarl = "";
    }
    Write_To_Rom(11342482, "01" + string_to_hex(wallet_amounts["Large"] + " Rupee" + plurarl) + "002EBF");
}

string Even_Hex(string hex) {
    if (hex.size()/2 == 0) {
        return "0" + hex;
    }
    else {
        return hex;
    }
}

string Bits_Or(string bits_1, string bits_2) {
    if (bits_1.length() == 0 && bits_2.length() == 0) {
        return "";
    }
    else if (bits_1.length() == 0) {
        return bits_2;
    }
    else if (bits_2.length() == 0) {
        return bits_1;
    }
    else {
        if (bits_1.length() > bits_2.length()) {
            return bits_1[0] + Bits_Or(bits_1.substr(1), bits_2);
        }
        else if (bits_1.length() < bits_2.length()) {
            return bits_2[0] + Bits_Or(bits_1, bits_2.substr(1));
        }
        else {
            if (bits_1[0] == '1' || bits_2[0] == '1') {
                return "1" + Bits_Or(bits_1.substr(1), bits_2.substr(1));
            }
            else {
                return "0" + Bits_Or(bits_1.substr(1), bits_2.substr(1));
            }
        }
    }
}

//clears the bit at index bit_index of a byte
string Bit_Clear(string byte, int bit_index) {
    string new_byte = "";

    for (int b = 0; b < byte.size(); b++) {
        if (b != bit_index) {
            new_byte += byte[b];
        }
        else {
            new_byte += "0";
        }
    }

    return new_byte;
}

void Give_Starting_Items() {
    vector<string> Start_Sources = {"Deku Mask", "Song of Healing"};
    //vector<string> Start_Sources = {"Deku Mask"};
    string hex;
    string location;
    string Count_Location;
    string Start_Item;
    vector<string> Song_Names = {"Song of Time", "Song of Healing", "Song of Soaring", "Epona's Song", "Song of Storms", "Sonata of Awakening", "Goron Lullaby", "New Wave Bossa Nova", "Elegy of Emptiness", "Oath to Order"};
    vector<string> Song_Bit_Values = {"00010000", "00100000", "10000000", "01000000", "00000001", "01000000", "10000000", "00000001", "00000010", "00000100"};
    //string Songs_Bit_1 = "00000000";   //C5CE71 12963441
    //string Songs_Bit_2 = "00010000";   //C5CE72 12963442    always start out with Song of Time
    //string Songs_Bit_3 = "00000000";   //C5CE73 12963443
    map<string, int> Item_Counts;
    vector<string> Item_C_Locations;
    map<string, string> Item_Flags;
    vector<string> Item_F_Locations;
    string songs_1 = "C5CE71";
    string songs_2 = "C5CE72";
    string songs_3 = "C5CE73";
    string HC = "C5CDE9";
    string Health = "C5CDEB";
    string wallet = "C5CE6E";
    string sword_shield = "C5CE21";

    //give ocarina
    Write_To_Rom(12963364, "00");

    Item_Flags[songs_1] = "00000000";
    Item_Flags[songs_2] = "00010000";    //start out with Song of Time
    Item_Flags[songs_3] = "00000000";
    Item_Flags[wallet] = "00000000";    //start out with small wallet
    Item_Flags[sword_shield] = "00010001";    //start out with sword and shield
    Item_F_Locations.push_back(songs_1);
    Item_F_Locations.push_back(songs_2);
    Item_F_Locations.push_back(songs_3);
    Item_F_Locations.push_back(wallet);
    Item_F_Locations.push_back(sword_shield);

    //start out with 3 hearts
    Item_Counts[HC] = hex_to_decimal("30");
    Item_Counts[Health] = hex_to_decimal("30");
    Item_C_Locations.push_back(HC);
    Item_C_Locations.push_back(Health);

    //give song of time and song of healing
    //Write_To_Rom(12963442, "30");

    //replace each starting item with the new item
    for (int i = 0; i < Start_Sources.size(); i++) {
        Start_Item = Start_Sources[i];

        hex = Items[Items[Start_Item].Name].Item_ID2;
        hex = Even_Hex(hex);

        //if there is any locations for the item ids
        if (Items[Items[Start_Item].Name].Address_Item_ID.size() > 0) {
            location = Items[Items[Start_Item].Name].Address_Item_ID[0];

            //if there is a location in the inventory to place the item id
            if (location.size() > 0) {
                Write_To_Rom(hex_to_decimal(location), hex);
            }
        }

        //if there is a location in the inventory to place the item count, then add to it the count of the current item
        if (Items[Items[Start_Item].Name].Item_Count_Locations.size() > 0) {
            for (int l = 0; l < Items[Items[Start_Item].Name].Item_Count_Locations.size(); l++) {
                Count_Location = Items[Items[Start_Item].Name].Item_Count_Locations[l];

                string key = Count_Location;
                string count_str = Items[Items[Start_Item].Name].Item_Count;
                int Count;

                if (count_str[1] == '_') {
                    //this is a flag, not an item count. Or the item is a tier 3 item
                    if (count_str[0] == 'F' || count_str[0] == '3') {
                        count_str = count_str.substr(2);

                        //or the bits in the same byte
                        if (Vector_Has(Item_F_Locations, key)) {
                            Item_Flags[key] = Bits_Or(Item_Flags[key], count_str);
                        }
                        else {
                            Item_Flags[key] = count_str;
                            Item_F_Locations.push_back(Count_Location);
                        }
                    }
                    //this is a set value, the byte can be from one item or another, is not ored together or added. one or the other
                    else if (count_str[0] == 'S') {
                        count_str = count_str.substr(2);

                        if (!Vector_Has(Item_F_Locations, key)) {
                            Item_F_Locations.push_back(Count_Location);
                        }
                        Item_Flags[key] = count_str;
                    }
                    else if (count_str[0] == '1') {
                        count_str = count_str.substr(2);

                        if (Vector_Has(Item_F_Locations, key)) {
                            int ignore_bit = IndexOf_S(count_str, "1") - 1; //gets the bit that is for tier 2
                            count_str = Bits_Or(count_str, Item_Flags[key]);
                            count_str = Bit_Clear(count_str, ignore_bit);   //clears the tier 2 bit
                        }
                        else {
                            Item_F_Locations.push_back(Count_Location);
                        }

                        Item_Flags[key] = count_str;
                    }
                    //tier two upgrade item
                    else if (count_str[0] == '2') {
                        count_str = count_str.substr(2);

                        if (Vector_Has(Item_F_Locations, key)) {
                            int ignore_bit = IndexOf_S(count_str, "1") + 1; //gets the bit that is for tier 1 and 3 to make sure it's not set
                            count_str = Bits_Or(count_str, Item_Flags[key]);
                            count_str = Bit_Clear(count_str, ignore_bit);   //clears the lower/upper tier bit
                        }
                        else {
                            Item_F_Locations.push_back(Count_Location);
                        }

                        Item_Flags[key] = count_str;
                    }
                    //map
                    else if (count_str[0] == 'M') {
                        count_str = count_str.substr(2);
                        Count_Location = Items[Items[Start_Item].Name].Item_Count_Locations[i];
                        key = Count_Location;

                        Item_Flags[key] = count_str;
                        Item_F_Locations.push_back(Count_Location);

                        l++;
                    }
                    else {
                        count_str = count_str.substr(2);

                        Count = hex_to_decimal(count_str);

                        if (Vector_Has(Item_C_Locations, key)) {
                            Item_Counts[key] += Count;
                        }
                        else {
                            Item_Counts[key] = Count;
                            Item_C_Locations.push_back(Count_Location);
                        }
                    }
                }
                else {
                    Count = hex_to_decimal(count_str);

                    if (Vector_Has(Item_C_Locations, key)) {
                        Item_Counts[key] += Count;
                    }
                    else {
                        Item_Counts[key] = Count;
                        Item_C_Locations.push_back(Count_Location);
                    }
                }
            }
        }

        //these counts dont get added together
        if (Items[Items[Start_Item].Name].Item_Count_Locations2.size() > 0) {
            location = Items[Items[Start_Item].Name].Item_Count_Locations2[0];
            string key = location;

            if (location.size() > 0) {
                int Count = hex_to_decimal(Items[Items[Start_Item].Name].Item_Count2);

                if (!Vector_Has(Item_C_Locations, key)) {
                    Item_C_Locations.push_back(location);
                }
                Item_Counts[key] = Count;
            }
        }
    }

    //write the counts from each item to the locations
    for (int c = 0; c < Item_C_Locations.size(); c++) {
        string key = Item_C_Locations[c];
        string location = Item_C_Locations[c];
        int Count = Item_Counts[key];

        Write_To_Rom(hex_to_decimal(location), dec_to_hex(Count));
    }

    //write the flag bits from each item to the locations
    for (int c = 0; c < Item_F_Locations.size(); c++) {
        string key = Item_F_Locations[c];
        string location = Item_F_Locations[c];
        string Flag = Item_Flags[key];

        Write_To_Rom(hex_to_decimal(location), binary_to_hex(Flag));
    }

    /*
    if (Items[Start_Item].Name == "Adult Wallet") {
        Write_To_Rom(hex_to_decimal(Items[Items[Start_Item].Name].Address_Item_ID[0]), "10");
    }
    else if (Items[Start_Item].Name == "Giant Wallet") {
        Write_To_Rom(hex_to_decimal(Items[Items[Start_Item].Name].Address_Item_ID[0]), "20");
    }
    //bomber's notebook is in the same byte as the first songs list
    else if (Items[Start_Item].Name == "Bomber's Notebook") {
        Songs_Bit_1 = Bits_Or(Songs_Bit_1, "00000100");
    }
    else if (Items[Start_Item].Name == "Big Bomb Bag") {
        Write_To_Rom(hex_to_decimal("C5CE6F"), "10");   //bomb bag slot
        Write_To_Rom(hex_to_decimal(Items[Items[Start_Item].Name].Address_Item_ID[0]), "06");  //bomb slot
        Write_To_Rom(hex_to_decimal("C5CE5A"), "1E");   //bomb count
    }
    else if (Items[Start_Item].Name == "Biggest Bomb Bag") {
        Write_To_Rom(hex_to_decimal("C5CE6F"), "18");
        Write_To_Rom(hex_to_decimal(Items[Items[Start_Item].Name].Address_Item_ID[0]), "06");
        Write_To_Rom(hex_to_decimal("C5CE5A"), "28");
    }
    else if (Items[Start_Item].Name == "Bomb Bag") {
        Write_To_Rom(hex_to_decimal("C5CE6F"), "08");
        Write_To_Rom(hex_to_decimal(Items[Items[Start_Item].Name].Address_Item_ID[0]), "06");
        Write_To_Rom(hex_to_decimal("C5CE5A"), "14");
    }
    else if (Items[Start_Item].Name == "Large Quiver") {
        Write_To_Rom(hex_to_decimal("C5CE25"), "01");   //bow slot
        Write_To_Rom(hex_to_decimal("C5CE6F"), "02");   //quiver slot
        Write_To_Rom(hex_to_decimal("C5CE55"), "28");   //arrow count
    }
    else if (Items[Start_Item].Name == "Largest Quiver") {
        Write_To_Rom(hex_to_decimal("C5CE25"), "01");
        Write_To_Rom(hex_to_decimal("C5CE6F"), "03");
        Write_To_Rom(hex_to_decimal("C5CE55"), "32");
    }
    else if (Items[Start_Item].Name == "Hero's Shield") {
        Write_To_Rom(hex_to_decimal("C5CE21"), "11");
    }
    else if (Items[Start_Item].Name == "Mirror Shield") {
        Write_To_Rom(hex_to_decimal("C5CE21"), "21");
    }
    else if (Items[Start_Item].Name == "Heart Piece") {
        Write_To_Rom(12963440, "10");
    }
    else if (Items[Start_Item].Name == "Heart Container") {
        Write_To_Rom(12963305, "40");   //Max Hearts
        Write_To_Rom(12963307, "40");   //Current Hearts
    }
    //if cannot start with this item then dont start with anything
    else if (Items[Items[Start_Item].Name].Address_Item_ID[0] == "") {
        cout << Start_Item << " gives " << Items[Start_Item].Name << endl;
    }
    //if this starting item is a song
    else if (IndexOf(Song_Names, Items[Start_Item].Name) != -1) {
        int Song_Index = IndexOf(Song_Names, Items[Start_Item].Name);

        //song of storms
        if (Song_Index == 4) {
            Songs_Bit_1 = Bits_Or(Songs_Bit_1, Song_Bit_Values[Song_Index]);
        }
        //every other song except sonata and lullaby
        else if (Song_Index < 4 || Song_Index > 6) {
            Songs_Bit_2 = Bits_Or(Songs_Bit_2, Song_Bit_Values[Song_Index]);
        }
        //sonata or lullaby
        else {
            Songs_Bit_3 = Bits_Or(Songs_Bit_3, Song_Bit_Values[Song_Index]);
        }
    }
    else {
        //Get the items in the inventory
        if (Items[Start_Item].Name == "Bow") {
            Write_To_Rom(hex_to_decimal("C5CE6F"), "01");   //quiver slot
            Write_To_Rom(hex_to_decimal("C5CE55"), "1E");   //arrow count
        }
        else if (Items[Start_Item].Name == "Razor Sword") {
            Write_To_Rom(hex_to_decimal("C5CE21"), "12");   //inventory slot
            Write_To_Rom(hex_to_decimal("C5CDF1"), "64");   //durability
        }
        else if (Items[Start_Item].Name == "Gilded Sword") {
            Write_To_Rom(hex_to_decimal("C5CE21"), "13");
        }
        else if (Items[Start_Item].Name == "Deku Nuts") {
            Write_To_Rom(hex_to_decimal("C5CE5D"), "01");
        }
        else if (Items[Start_Item].Name == "Deku Nuts (10)") {
            Write_To_Rom(hex_to_decimal("C5CE5D"), "0A");
        }
        else if (Items[Start_Item].Name == "Deku Stick") {
            Write_To_Rom(hex_to_decimal("C5CE5C"), "01");
        }
        else if (Items[Start_Item].Name == "Magic Beans") {
            Write_To_Rom(hex_to_decimal("C5CE5E"), "01");
        }
        else if (Items[Start_Item].Name == "Powder Keg") {
            Write_To_Rom(hex_to_decimal("C5CE60"), "01");
        }

        Write_To_Rom(hex_to_decimal(Items[Items[Start_Item].Name].Address_Item_ID[0]), hex);
    }
    }
    */

    //write the songs to the file
    //Write_To_Rom(12963441, binary_to_hex(Songs_Bit_1));
    //Write_To_Rom(12963442, binary_to_hex(Songs_Bit_2));
    //Write_To_Rom(12963443, binary_to_hex(Songs_Bit_3));

}

void Apply_Mod(string filename) {
    fstream mod;
    filename = "mods/" + filename;
    Open_File(filename, mod);
    string file;
    string File_Hex = "";
    string End = "FF";
    string C = "";

    cout << endl << filename << endl;

    while (mod >> hex >> file) {
        File_Hex += string_to_hex(file);
        File_Hex += "0C";   //0C is removed when using this method, so have to add it back, OC might be the thing that it checks for the end of a line or something
    }

    int base = 0;
    do {
        C = "00";
        string address = "";
        for (int i = base; i < base+8; i++) {
            address += File_Hex[i];
        }

        string length;
        int len;
        for (int i = base+8; i < base+16; i++) {
            length += File_Hex[i];
        }
        len = hex_to_decimal(length)*2;

        string data;
        for (int i = base+16; i < base + 16 + len; i++) {
            data += File_Hex[i];
        }
        data = Even_Hex(data);

        cout << data << endl;

        Write_To_Rom(hex_to_decimal(address), data);

        base = base + 16 + len;
        C[0] = File_Hex[base];
        C[1] = File_Hex[base+1];
    }
    while (C != End);


    mod.close();
}

void Remove_Item_Checks() {
    Write_To_Rom(14439692, "9318360301CF182300031C003301004014010004");
    Write_To_Rom(13334652, "00000000");
    Write_To_Rom(13334692, "10000016");
    Write_To_Rom(15512024, "1000000F");
    Write_To_Rom(15511648, "00000000");   //this makes it where tingle doesnt write the map data
    //Write_To_Rom(17100472, "904E3F6831CF0008");   //this also caused the softlock to happen at couple's mask
    Write_To_Rom(15562996, "90783F85");
    Write_To_Rom(15563008, "33190040");
    Write_To_Rom(17175348, "10000005");
    Write_To_Rom(16304652, "91CE35EF");
    Write_To_Rom(16304664, "31CF0080");
    Write_To_Rom(17229352, "90593F6E");
    Write_To_Rom(17229364, "33280001");
    Write_To_Rom(17168980, "10000007");
    Write_To_Rom(12962932, "00000020");
    Write_To_Rom(12963012, "00000000");
    Write_To_Rom(12962936, "00003000");
    Write_To_Rom(12962908, "0C000000");
    Write_To_Rom(14893212, "90683F82");
    Write_To_Rom(14893224, "31090002");
    Write_To_Rom(12962940, "00000000");
    Write_To_Rom(16968828, "10000009");
    Write_To_Rom(14442228, "10000009");
    Write_To_Rom(17054436, "00000000");
    Write_To_Rom(16958052, "10000006");
    Write_To_Rom(14902472, "10000068");
    Write_To_Rom(14900732, "00000000");
    Write_To_Rom(16876332, "1000000E");
    Write_To_Rom(16365624, "10000005");
    Write_To_Rom(16019572, "10000010");
    Write_To_Rom(15445332, "000D0000");
    Write_To_Rom(16124449, "003E0000");
    Write_To_Rom(17059293, "39080000");
    Write_To_Rom(15445618, "1A400000");
    Write_To_Rom(15521640, "00000000");
    Write_To_Rom(15521792, "00000000");
    Write_To_Rom(16833748, "10000007");
    Write_To_Rom(16449840, "1000000C");
    Write_To_Rom(14085672, "10000006");
    Write_To_Rom(16726828, "1000000E");
    Write_To_Rom(16732804, "1000000A");
    Write_To_Rom(17113652, "10000003");
    Write_To_Rom(17113676, "10000004");
    Write_To_Rom(15204407, "000C0000");
    Write_To_Rom(15204429, "000C0000");
    Write_To_Rom(16479968, "906E3F79");
    Write_To_Rom(16479980, "31CF0004");
    Write_To_Rom(12962878, "000C");
    Write_To_Rom(12962882, "0000");
    Write_To_Rom(16566324, "10000004");
    Write_To_Rom(17274152, "10000008");
    Write_To_Rom(15573992, "10000004");
    Write_To_Rom(16567248, "10000006");
    Write_To_Rom(13757064, "10000005");
    Write_To_Rom(16040735, "57040000");
    Write_To_Rom(16040792, "57040000");
    Write_To_Rom(16041129, "57040000");
    Write_To_Rom(14508924, "00000000");
    Write_To_Rom(16058812, "00000000");
    Write_To_Rom(14914316, "00000000");
    Write_To_Rom(14914340, "00000000");
    Write_To_Rom(14914416, "0000000000000000");
    Write_To_Rom(14913628, "00000000");
    Write_To_Rom(14913744, "0000000000000000");
    Write_To_Rom(13334300, "10220006");
    Write_To_Rom(16805032, "904A3F7B314B0004");
    Write_To_Rom(16805048, "100B0006");
    Write_To_Rom(16806492, "904A3F7B314B0004100B0005");
    Write_To_Rom(16746480, "912A35EB314B0004500B0011");
    Write_To_Rom(15395316, "916A35EB314B0004540B0004");
    Write_To_Rom(16993428, "10000004");
    Write_To_Rom(14210760, "91033F7A306F0002");
    Write_To_Rom(15878344, "10000007");
    Write_To_Rom(15349324, "904F3F7C31F800400200202510180005");
    Write_To_Rom(15348816, "904F3F6F31F8000850180006");
    Write_To_Rom(13334668, "00000000");
    //Write_To_Rom(15723008, "240100AD");   //this made the game softlock after beaver hp
    Write_To_Rom(12228768, "0000000000000000");
    Write_To_Rom(12961392, "00000000");

    //Make witch always gives you bottled red potion, so I removed it so she gives you bottle and then whatever is randomized to red potion - in mystery woods
    //Write_To_Rom(15964788, "90693F733C014396316C0002");
    //Write_To_Rom(15964788, "90693F73");
    //Write_To_Rom(15964796, "312A0002");
    //Write_To_Rom(15964816, "24060059");

    //Make witch always gives you bottled red potion, so I removed it so she gives you bottle and then whatever is randomized to red potion
    //Write_To_Rom(15678556, "90CB3F733C014396316C0002");

    Write_To_Rom(16479984, "10000003");
    Write_To_Rom(12961148, "00000C00");
    Write_To_Rom(11835848, "8EE6038884C700002408000E1107000A00000000240900331127001500000000240A00661147001200000000240200FF03E000080000000084CC001C318D00FF2408000C110D000A000000002409000D112D000700000000240A0017114D000400000000240200FF03E00008000000003C05801F240800081104000D00000000240900091124000D00000000240A008D1144000A00000000240B008E1164000700000000240200FF03E000080000000090A2F6E803E000080000000090A2F6E903E0000800000000");
    Write_To_Rom(13857020, "00000000");
    Write_To_Rom(15678584, "2406005B");
    Write_To_Rom(12962948, "00000CF0");
    Write_To_Rom(16134584, "91AD36023C01422031AE0004");
    Write_To_Rom(16134504, "9108360231090004");
    Write_To_Rom(16134260, "91CE36023C0B80B03C0880B031CF0004");
    Write_To_Rom(13298264, "00000000");
    Write_To_Rom(15953724, "00000000");     //Make HMS always give you "deku mask"

    Write_To_Rom(14536572, "10000002");     //make kid ignore if you completed the trials
    Write_To_Rom(14537748, "240F00FF");     //make kid ignore if you have FD mask

    //Remove buying bomb bag checks
    Write_To_Rom(13492708, "15000003");
    Write_To_Rom(13492732, "15000003");

    Write_To_Rom(14914328, "24060023"); //make clock town archery give large quiver
    Write_To_Rom(14913640, "24060024"); //make swamp archery give largest quiver
    Write_To_Rom(13513644, "10000004"); //make giant's mask chest not check for giant's mask so that it always gives you the item
    Write_To_Rom(15521648, "24180000"); //make banker never check which wallet you have
    Write_To_Rom(16565908, "24060009"); //make giant wallet ignore what wallet you have

    //there are 2 places of the same 4 commands close by, maybe one for each day/night or something? Gonna replace each one
    Write_To_Rom(13492684, "24020000"); //make bomb shop guy let you always buy big bomb bag
    Write_To_Rom(13492812, "24020000");

    Write_To_Rom(13492580, "24020000"); //make curiosity shop guy not check for bomb bag size when buying big bomb bag

    Write_To_Rom(12234412, "240200FF"); //make town shop guy give you what 10 deku nuts give instead of nuts if nuts in inventory
    Write_To_Rom(13492116, "240C0000"); //make town shop guy not check if you have max nuts

    Write_To_Rom(12233552, "00000000"); //make bow do the get item animation if had a larger quiver
    Write_To_Rom(12233656, "00000000"); //make bomb bag play the get item animation when already having a bag

    Write_To_Rom(12274656, "24060020"); //make hot spring water give hot spring water when cools
    Write_To_Rom(12274668, "00000000"); //skip jumping to the textbox function for Hot Spring Water cooling

    Write_To_Rom(13513592, "240F0000"); //make captain's hat chest ignore if you have it

    Write_To_Rom(16000100, "10000003"); //make clock town scrub ignore if you've traded in swamp
    Write_To_Rom(48611460, "0007");     //remove woodfall scrub from clocktown
    Write_To_Rom(42168432, "0007");     //remove snowhead scrub from swamp
    Write_To_Rom(32628840, "0007");     //remove snowhead scrub from cleared swamp
    Write_To_Rom(42725596, "0007");     //remove ocean scrub from spring snowhead
    Write_To_Rom(44384392, "0007");     //remove ocean scrub from snowhead
    Write_To_Rom(44212312, "0007");     //remove ikana scrub from ocean

    Write_To_Rom(12233316, "240400FF"); //Make the game always give GFS from a randomized item
    Write_To_Rom(15923616, "24020000"); //always give song of soaring
    //Write_To_Rom(12100280, "24020000"); //always give Epona's Song - haha, makes Epona not spawn :)))))
    Write_To_Rom(12509920, "24020000"); //always give Epona's Song

    //elder goron ignores if you have lullaby and intro
    Write_To_Rom(16622184, "24030000");
    Write_To_Rom(16619604, "24030000");

    //Day 3 Gormon Brothers always give you Garo Mask
    Write_To_Rom(14282268, "2419FFFF");
    Write_To_Rom(14087392, "240DFFFF");

    Write_To_Rom(14536768, "00000000"); //fix dimming transformation masks during majora fight when receiving what FD gives
    Write_To_Rom(17167004, "240F0043"); //make healing kamaro give the notebook ribbon
    Write_To_Rom(13845660, "10000003"); //always play oath cs when picking up remains
    Write_To_Rom(13838032, "24040000"); //always spawn odolwa's remains
    Write_To_Rom(13838084, "24040000"); //always spawn goht's remains
    Write_To_Rom(13838124, "24040000"); //always spawn gyorh's remains
    Write_To_Rom(13838164, "24040000"); //always spawn twinmold's remains
    Write_To_Rom(15349096, "240F00FF"); //make gf always give gfm item after already getting magic (and have deku mask of course)

}

map<string, map<string, string> > OpenAsIni(string filename) {
    ifstream file;
    string line = "";
    string key = "";
    string value = "";
    string section = "";
    map<string, map<string, string> > File_Contents;

    file.open(filename.c_str());

    if (!file) {
        err_file << "Error opening " << filename.substr(2);
        err_file.close();
        exit(0);
    }

    //get each item
    while (getline(file, line) && line != "") {
        if (section == "" || line[0] == '[') {
            section = line.substr(1, IndexOf(line, ']')-1);
        }
        else {
            key = line.substr(0, IndexOf(line, '='));
            value = line.substr(IndexOf(line, '=')+1);

            File_Contents[section][key] = value;
        }
    }

    file.close();

    return File_Contents;
}

void Print_Map(ostream& out, map<string, map<string, string> > data) {
    vector<string> keys;
    vector<string> keys2;

    for (const auto& kv : data) {
        keys.push_back(kv.first);
    }

    for (int i = 0; i < keys.size(); i++) {
        cout << keys[i] << endl;

        for (const auto& kv : data[keys[i]]) {
            keys2.push_back(kv.first);
        }

        for (int ii = 0; ii < keys2.size(); ii++) {
            cout << "\t" << keys2[ii] << endl;
            cout << "\t\t" << data[keys[i]][keys2[ii]] << endl;
        }
    }
}

void Update_Pools(map<string, Item> &Items) {
    vector<string> keys;
    vector<string> values;

    //Print_Map(cout, Settings);

    for (const auto& kv : Settings["items"]) {
        keys.push_back(kv.first);
        values.push_back(kv.second);
    }

    for (int i = 0; i < keys.size(); i++) {
        Items[keys[i]].Pool = values[i];
    }
}

string Replace(string bigger_text, string smaller_text, string new_smaller_text) {
    string new_text = "";
    for (int a = 0; a < bigger_text.size(); a++) {
        int c = a;
        int b = 0;

        for (b = 0; b < smaller_text.size(); b++) {
            //not equal
            if (bigger_text[c] != smaller_text[b]) {
                b = smaller_text.size();
            }
            c++;
        }
        //found a match
        if (b == smaller_text.size()) {
            new_text += new_smaller_text;   //get the replacement
            a += smaller_text.size()-1;     //skip passed the old text (-1 cause for loop will increase it by 1)
        }
        else {
            new_text += bigger_text[a];
        }
    }

    return new_text;
}

vector<string> Split(string line, string Splitter) {
    vector<string> List;

    //while the line contains the splitter
    while (IndexOf_S(line, Splitter) != -1) {
        List.push_back(line.substr(0, IndexOf_S(line, Splitter)));
        line = line.substr(IndexOf_S(line, Splitter) + Splitter.size());
    }
    List.push_back(line);

    return List;
}

string Remove_Whitespace(string text) {
    text = RemoveAll(text, ' ');
    text = RemoveAll(text, '\t');

    return text;
}

vector<vector<string>> Get_Items_Needed(ifstream &Logic_File) {
    string line = "";
    vector<vector<string>> Items_Needed;
    vector<string> List;

    while (!Contains(line, '}')) {
        getline(Logic_File, line);

        //clear the list
        List.clear();

        //remove comments on line if any
        if (IndexOf_S(line, "//") != -1) {
            line = line.substr(0, IndexOf_S(line, "//"));
        }

        line = RemoveAll(line, '\t');

        if (Contains(line, ',')) {
            List = Split(line, ", ");
        }
        else if (line != "") {
            List.push_back(line);
        }

        if (List.size() > 0) {
            if (!Contains(List[0], '}')) {
                Items_Needed.push_back(List);
            }
        }
    }

    return Items_Needed;
}

void Print_Logic(map<string, vector<vector<string>>> Logic) {
    vector<string> keys;

    for (const auto& kv : Logic) {
        keys.push_back(kv.first);
    }

    for (int i = 0; i < keys.size(); i++) {
        cout << keys[i] << endl;
        for (int a = 0; a < Logic[keys[i]].size(); a++) {
            for (int b = 0; b < Logic[keys[i]][a].size(); b++) {
                if (b == 0) {
                    cout << "\t";
                }
                else {
                    cout << ", ";
                }

                cout << Logic[keys[i]][a][b];
            }
            cout << endl;
        }
        cout << endl;
    }
}

map<string, vector<vector<string>>> Get_Logic(string Logic_Location) {
    ifstream Logic_File;
    string line = "";
    map<string, vector<vector<string>>> Logic;
    string Item = "";

    Logic_Location = "./logic/" + Logic_Location + ".txt";

    //Logic_File.open("./logic/Glitched.txt");
    Logic_File.open(Logic_Location.c_str());

    if (!Logic_File) {
        Error("Could not find logic file: \"" + Logic_Location + "\"");
    }

    getline(Logic_File, line);

    while (getline(Logic_File, line)) {
        //remove comments on line if any
        if (IndexOf_S(line, "//") != -1) {
            line = line.substr(0, IndexOf_S(line, "//"));
        }

        if (line != "") {
            if (Contains(line, '{')) {
                Item = line.substr(0, IndexOf(line, '{')-1);
                Logic[Item] = Get_Items_Needed(Logic_File);
            }
        }
    }

    Logic_File.close();

    return Logic;
}

bool Has_Items(Inventory Inv, vector<string> Items, int depth = 0) {
    if (depth == Items.size()) {
        return true;
    }
    else {
        return (Inv.Items[Items[depth]] && Has_Items(Inv, Items, ++depth));
    }
}

template<typename value>
bool Vector_Has(vector<value> vect, value val) {
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] == val) {
            return true;
        }
    }

    return false;
}

bool Update_Inventory(Inventory &Inv, map<string, Item> Items, map<string, vector<vector<string>>> Logic, vector<string> &Items_Checked) {
    vector<string> keys;
    bool Can_Get = false;
    bool Possible = true;
    bool Got_Item = false;  //whether or not got an item in this run through
    bool free = true;    //whether or not you dont need items for the current item

    for (const auto& kv : Logic) {
        keys.push_back(kv.first);
    }

    string bbomb = "Bombs";
    //not actual items, so no need to check them
    if (!Vector_Has(Items_Checked, bbomb)) {
        Items_Checked.push_back("Bombs");
        Items_Checked.push_back("Quiver");
        Items_Checked.push_back("Nuts");
    }

    for (int i = 0; i < keys.size(); i++) {
        Can_Get = false;
        Possible = true;
        free = true;

        //if haven't checked item
        if (!Vector_Has(Items_Checked, keys[i])) {
            for (int a = 0; a < Logic[keys[i]].size(); a++) {
                //if it makes it into this loop, then that means you need items to get this item
                if (free) {
                    free = false;
                }

                for (int b = 0; b < Logic[keys[i]][a].size(); b++) {
                    Possible = Has_Items(Inv, Logic[keys[i]][a]);

                    if (Possible) {
                        Can_Get = true;
                    }
                }
            }

            if (Can_Get || free) {
                Inv.Add_Item(Items[keys[i]].Name);
                Got_Item = true;
                Items_Checked.push_back(keys[i]);
                //cout << "Checked " << keys[i] << " and got " << Items[keys[i]].Name << endl;
            }
        }
    }

    return Got_Item;
}

void Print_Vector(vector<string> Items, string separator = "", string preString = "") {
    for (int i = 0; i < Items.size(); i++) {
        if (i > 0) {
            cout << separator;
        }

        cout << preString << Items[i];
    }
}

bool Locked_Trade_Item(map<string, Item> Items) {
    vector<string> Deeds = {"Moon's Tear", "Land Title Deed", "Swamp Title Deed", "Mountain Title Deed", "Ocean Title Deed"};
    vector<string> Key = {"Room Key", "Letter to Kafei"};
    vector<string> Pendant = {"Pendant of Memories", "Express Letter to Mama"};

    if (IndexOf(Deeds, Items["Keaton Mask"].Name) != -1 && IndexOf(Deeds, Items["Express Letter to Mama"].Name) != -1) {
        return true;
    }
    else if (IndexOf(Key, Items["Keaton Mask"].Name) != -1 && IndexOf(Key, Items["Express Letter to Mama"].Name) != -1) {
        return true;
    }
    else if (IndexOf(Pendant, Items["Keaton Mask"].Name) != -1 && IndexOf(Pendant, Items["Express Letter to Mama"].Name) != -1) {
        return true;
    }
    else {
        return false;
    }
}

//get the key for the first highest value in a map (positive numbers only)
string Get_Key_Max(map<string, int> data, int index) {
    string key = "";
    int highest = -1;
    vector<string> keys;

    keys = Get_Keys(data);

    for (int i = 0; i < keys.size(); i++) {
        if (data[keys[i]] > highest) {
            highest = data[keys[i]];
            key = keys[i];
        }
    }

    if (index > 0) {
        data.erase(key);    //remove the 1st one to get to the next one until it reaches the wanted index
        key = Get_Key_Max(data, --index);
    }

    return key;
}

string Get_Pool(string item) {
    return Items[item].Pool;
}

void Swap(string item1, string item2) {
    string Get_Item;

    cout << "Swapped " << Items[item1].Name << " and " << Items[item2].Name << endl;

    Get_Item = Items[item1].Name;
    Items[item1].Name = Items[item2].Name;
    Items[item2].Name = Get_Item;
}

string Get_New_Location(string item, string pool) {
    vector<string> keys;
    int index;

    keys = Get_Keys(Items);

    //find a random item in the same pool
    do {
        index = Random(0, keys.size());
    } while (Items[keys[index]].Pool != pool);

    return keys[index];
}

//returns the most common missing item needed for the list of missing items
string Most_Common(vector<string> Missing_Items_Source, vector<string> Missing_Items, map<string, vector<vector<string>>> Logic, Inventory Inv, int index) {
    string Item;
    map<string, int> Items_Count;    //how may items a certain item can help get
    map<string, bool> Items_Need;    //if an item was found in one of the ways to get the item, this is to prevent from counting the item more than once for each item
    string Cur_Item;    //current item
    string Miss_Item;

    for (int i = 0; i < Missing_Items.size(); i++) {
        Items_Count[Missing_Items[i]] = 0;
        Items_Need[Missing_Items[i]] = false;
    }

    for (int i = 0; i < Missing_Items_Source.size(); i++) {
        Cur_Item = Missing_Items_Source[i];

        //each list of needed items
        for (int j = 0; j < Logic[Cur_Item].size(); j++) {
            //each item needed
            for (int k = 0; k < Logic[Cur_Item][j].size(); k++) {
                Miss_Item = Logic[Cur_Item][j][k];

                //if this item is not obtained
                if (!Inv.Items[Miss_Item]) {
                    Items_Need[Miss_Item] = true;
                }
            }
        }

        for (int j = 0; j < Missing_Items.size(); j++) {
            Miss_Item = Missing_Items[j];

            if (Items_Need[Miss_Item]) {
                Items_Count[Miss_Item]++;   //increase the count of the item

                Items_Need[Miss_Item] = false;
            }
        }
    }

    Item = Get_Key_Max(Items_Count, index);

    return Item;
}

bool Start_Map(map<string, Item> Items) {
    //if (Items["Deku Mask"].Address_Item_ID[0] == "" || Items["Song of Time"].Address_Item_ID[0] == "" || Items["Song of Healing"].Address_Item_ID[0] == "") {
    if (Items["Deku Mask"].Address_Item_ID[0] == "" || Items["Song of Healing"].Address_Item_ID[0] == "") {
        return true;
    }
    else {
        return false;
    }
}

vector<string> Get_All_Pools(map<string, Item> Items) {
    vector<string> Pools;
    vector<string> Item_Names;
    string pool;
    string item;

    Item_Names = Get_Keys(Items);

    for (int i = 0; i < Item_Names.size(); i++) {
        item = Item_Names[i];
        pool = Items[item].Pool;

        //if haven't added this pool yet
        if (IndexOf(Pools, pool) == -1) {
            //if is a pool, doesnt give an item and is randomized
            if (pool[0] != '#' && pool != "") {
                Pools.push_back(pool);
            }
        }
    }

    return Pools;
}

void Fix_Shuffled(map<string, Item> &Items, string Seed, string Logic_File, int Seed_Increase = 1) {
    Inventory Inv;
    map<string, vector<vector<string>>> Logic;
    vector<string> Items_Checked;
    vector<string> Pools;   //for pools of the missing items
    string pool;
    string New_Location;
    string Common_Missing_Item;    //the item that can get the most of the missing items
    vector<string> Missing_Items;    //items that are missing
    vector<string> Missing_Items_Source;    //the source of the missing items, (zora mask => deku mask, it would be the zora mask)
    string Common_Missing_Item_Source;
    int tries = 100;    //number of times the randomizer tries to make a seed possible
    string Second_Item;
    string source;

    Logic = Get_Logic(Logic_File);

    //while the 2nd curiosity shop item overwrites the first
    while (Locked_Trade_Item(Items)) {
        //swap the 2nd with a random item
        Second_Item = Items["Express Letter to Mama"].Name;
        pool = Get_Pool(Second_Item);
        source = Get_Source(Second_Item);
        New_Location = Get_New_Location(Common_Missing_Item, pool);
        Swap(source, New_Location);
    }

    //while deku mask or healing give a map (cannot start with map cause the dev is not smart enough to figure out how)
    while (Start_Map(Items)) {
        //swap deku mask with a random item
        Second_Item = Items["Deku Mask"].Name;
        pool = Get_Pool(Second_Item);
        source = Get_Source(Second_Item);
        New_Location = Get_New_Location(Common_Missing_Item, pool);
        Swap(source, New_Location);
    }

    //if curiosity shop guy 2nd item doesn't overwrite 1st item
    //and if deku mask does not give map
    //if (!Locked_Trade_Item(Items) && !Start_Map(Items)) {
        //get all items possible
        //while (Update_Inventory(Inv, Items, Logic, Items_Checked)) {

      //  }
    //}

    //run the update inv function until it does not get at least one item on a run-through
    while (Update_Inventory(Inv, Items, Logic, Items_Checked)) {

    }

    if (!Inv.All(Items)) {
        //Print_Vector(Inv.Get_Missing_Items(), "\n", "\t");

        //only try to fix the logic x times before quitting (stops infinite loop if current pools/items are impossible no matter what)
        if (Seed_Increase <= tries) {
            cout << "Failed to randomize items with logic, trying again\n";

            //Replace a random item with the most common useful missing item
            Missing_Items = Inv.Get_Missing_Items();

            if (Missing_Items.size() <= Tried_Items.size()) {
                Tried_Items.clear();
            }

            Missing_Items_Source = Inv.Get_Missing_Items_Source(Missing_Items, Items);
            Common_Missing_Item = Most_Common(Missing_Items_Source, Missing_Items, Logic, Inv, Tried_Items.size());
            pool = Get_Pool(Common_Missing_Item);

            Tried_Items.push_back(Common_Missing_Item);

            //bombs, quiver, or nuts
            if (pool == "") {
                if (Common_Missing_Item == "Bombs") {
                    pool = Get_Pool("Bomb Bag");
                }
                else if (Common_Missing_Item == "Quiver") {
                    pool = Get_Pool("Bow");
                }
                else if (Common_Missing_Item == "Nuts") {
                    pool = Get_Pool("Deku Nuts");
                }
            }

            New_Location = Get_New_Location(Common_Missing_Item, pool);
            Common_Missing_Item_Source = Get_Source(Common_Missing_Item);
            Swap(Common_Missing_Item_Source, New_Location);

            //see if all items are possible now
            Fix_Shuffled(Items, Seed, Logic_File, ++Seed_Increase);
        }
        else {
            Error("Failed to create rom with the current settings and logic");
        }
    }
}

///Gets the word at index 'word_index' from 'text' (0 based)
string Get_Word(string text, int word_index) {
    vector<string> words = Split(text, " ");

    if (words.size() > word_index) {
        return words[word_index];
    }
    else {
        return "";
    }
}

///Gets the wallet size (nothing, adult, or giant) needed to have the 'rupees_needed' amount of rupees
string Get_Wallet_Needed(int rupees_needed, map<string, string> wallet_sizes) {
    vector<string> wallet_keys = {"Small", "Medium", "Large"};
    vector<string> wallets = {"", "Adult Wallet", "Giant Wallet"};

    for (int w = 0; w < wallets.size(); w++) {
        string key = wallet_keys[w];
        string Size = wallets[w];
        string rupees_str = wallet_sizes[key];
        int max_rupees = string_to_dec(rupees_str);

        //this is the wallet that is needed
        if (max_rupees >= rupees_needed) {
            return Size;
        }
    }

    //the player cannot buy the item ever
    return "Error";
}

vector<string> Get_Items_Aval(map<string, Item> &Items, map<string, vector<vector<string>>> Items_Needed, vector<string> &Items_Gotten, map<string, string> wallets) {
    vector<string> items;
    vector<string> Items_Aval;
    bool Can_Get_Item = false;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++) {
        string Cur_Item = items[i];
        Can_Get_Item = false;

        for (int isn = 0; isn < Items_Needed[Cur_Item].size(); isn++) {
            vector<string> Cur_List = Items_Needed[Cur_Item][isn];
            bool Have_All = true;

            for (int in = 0; in < Cur_List.size(); in++) {
                string item_needed = Cur_List[in];

                //if this is a rupee amount
                if (IndexOf_S(item_needed, "Rupees") != -1) {
                    string rupees_str = Get_Word(item_needed, 0);   //gets the first word (gets the rupee amount)
                    int rupees = string_to_dec(rupees_str);
                    string Needed_Wallet = Get_Wallet_Needed(rupees, wallets);

                    //if the player needs a wallet (too many rupees for the starting wallet)
                    if (Needed_Wallet != "") {
                        //if the player does not have the wallet needed
                        if (IndexOf(Items_Gotten, Needed_Wallet) == -1) {
                            Have_All = false;
                        }
                    }
                }
                //if an item is not obtained, then that means the player cannot get the item with the current list we're checking
                else if (IndexOf(Items_Gotten, item_needed) == -1) {
                    Have_All = false;
                }
            }

            //if the player has all items for a row of needed items, then the player can get this item and skip checking the other rows
            if (Have_All) {
                Can_Get_Item = true;
                isn = Items_Needed[Cur_Item].size();
            }
        }

        //if the player can get this item, then add the location to the items available vector
        if (Can_Get_Item || Items_Needed[Cur_Item].size() == 0) {
            Items_Aval.push_back(Cur_Item);
        }
    }

    return Items_Aval;
}

vector<string> Get_Items_Left_Pool(string pool) {
    vector<string> Items_Pool;
    vector<string> items;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++) {
        string item = items[i];

        if (Items[item].Pool == pool) {
            if (!Items[item].can_get) {
                Items_Pool.push_back(item);
            }
        }
    }

    return Items_Pool;
}

void Remove_All_Items_Last(vector<string> &Items_Last, vector<string> &Items_Gotten) {
    //remove all items gotten from last run-through
    for (int ri = 0; ri < Items_Last.size(); ri++) {
        //remove from gotten vector
        string Item_Remove = Items_Last[ri];
        int Index_Remove = IndexOf(Items_Gotten, Item_Remove);
        Items_Gotten.erase(Items_Gotten.begin() + Index_Remove);

        //update the item gives flag that gave it
        string Item_Source = Get_Source(Item_Remove);
        Items[Item_Source].gives_item = false;
    }
}

vector<string> Get_First_Items_List(string item, map<string, vector<vector<string>>> Items_Needed, vector<string> &Items_Gotten) {
    vector<string> items;
    vector<string> empty_vector;
    bool yes = true;

    for (int i = 0; i < Items_Needed[item].size(); i++) {
        bool yes = true;
        items.clear();
        for (int j = 0; j < Items_Needed[item][i].size(); j++) {
            string Item_Needed = Items_Needed[item][i][j];

            items.push_back(Item_Needed);

            if (IndexOf(Items_Gotten, Item_Needed) == -1) {
                yes = false;
            }
        }

        if (yes) {
            return items;
        }
    }

    return empty_vector;
}

vector<string> Sort_Value(vector<string> Items_Pool) {
    vector<string> Sorted_List;
    map<string, int> Item_Values;
    string item;
    int value;

    for (int i = 0; i < Items_Pool.size(); i++) {
        item = Items_Pool[i];
        value = Items[item].value;

        Item_Values[item] = value;
    }

    while (Items_Pool.size() > 0) {
        int highest = -1;
        int Highest_Index = -1;
        string Highest_Item = "";

        for (int ip = 0; ip < Items_Pool.size(); ip++) {
            item = Items_Pool[ip];

            if (Item_Values[item] > highest) {
                Highest_Item = item;
                highest = Item_Values[item];
                Highest_Index = ip;
            }
        }

        Sorted_List.push_back(Highest_Item);    //store the current highest value

        Items_Pool.erase(Items_Pool.begin() + Highest_Index);   //remove the current highest value from pool
    }

    return Sorted_List;
}

string Get_Missing_Items(vector<string> &Items_Gotten) {
    string miss_items = "\t";
    vector<string> items;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++) {
        if (IndexOf(Items_Gotten, items[i]) == -1) {
            if (miss_items == "\t") {
                miss_items += items[i];
            }
            else {
                miss_items += "\n\t" + items[i];
            }
        }
    }

    return miss_items;
}

string Get_Missing_Locations(map<string, vector<vector<string>>> &Items_Needed, vector<string> &Items_Gotten) {
    string miss_loc = "";
    vector<string> items;
    bool Can_Get = true;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++) {
        string item = items[i];
        Can_Get = true;

        vector<vector<string>> needs = Items_Needed[item];
        for (int n = 0; n < needs.size(); n++) {
            vector<string> Item_List = needs[n];

            for (int il = 0; il < Item_List.size(); il++) {
                string Item_Needed = Item_List[il];
                if (IndexOf(Items_Gotten, Item_Needed) == -1) {
                    Can_Get = false;
                    il = Item_List.size();
                }
            }

            if (Can_Get) {
                n = needs.size();
            }
        }

        if (!Can_Get) {
            miss_loc += "\n\t" + item;
        }
    }

    return miss_loc;
}

bool Check_Curiosity_Items(string Cur_Item) {
    vector<string> deeds = {"Moon's Tear", "Land Title Deed", "Mountain Title Deed", "Swamp Title Deed", "Ocean Title Deed"};
    vector<string> key = {"Room Key", "Pendant of Memories"};
    vector<string> letter = {"Letter to Kafei", "Express Letter to Mama"};

    //if one of the items is being placed
    if (Cur_Item == "Keaton Mask" || Cur_Item == "Express Letter to Mama") {
        //if either items has already been placed
        if (Items["Express Letter to Mama"].gives_item || Items["Keaton Mask"].gives_item) {
            //if both items now contain items that overwrite each other
            if (IndexOf(deeds, Items["Keaton Mask"].Name) != -1 && IndexOf(deeds, Items["Express Letter to Mama"].Name) != -1) {
                return false;
            }
            else if (IndexOf(key, Items["Keaton Mask"].Name) != -1 && IndexOf(key, Items["Express Letter to Mama"].Name) != -1) {
                return false;
            }
            else if (IndexOf(letter, Items["Keaton Mask"].Name) != -1 && IndexOf(letter, Items["Express Letter to Mama"].Name) != -1) {
                return false;
            }
            //both items are good
            else {
                return true;
            }
        }
    }
    else {
        return true;
    }
}

string Global_Log = "";
int highest_items = 0;

bool Randomize(string Log, map<string, Item> &Items, string Seed, map<string, vector<vector<string>>> &Items_Needed, vector<string> &Items_Gotten, map<string, string> wallets, vector<string> Items_Last = {}) {
    vector<string> items;
    vector<string> Items_Aval;  //item locations that the player is able to get to currently according to logic
    vector<string> Items_This;
    bool Has_All = true;
    bool Placed_All = true;

    items = Get_Keys(Items);

    Items_Aval = Get_Items_Aval(Items, Items_Needed, Items_Gotten, wallets);

    //place an item at each available spot
    for (int ia = 0; ia < Items_Aval.size(); ia++) {
        string Cur_Item = Items_Aval[ia];
        int New_Item_Index;
        string New_Item;

        //if this item doesn't give anything yet, then make it give an item
        if (!Items[Cur_Item].gives_item) {
            string pool = Items[Cur_Item].Pool;
            vector<string> Items_Pool = Get_Items_Left_Pool(pool);

            random_shuffle(Items_Pool.begin(), Items_Pool.end());

            //put the most valuable items in the front if more than half the items have been placed
            //if (highest_items > (items.size()/2)) {
                //Items_Pool = Sort_Value(Items_Pool);
            //}


            //if ran out of items in the pool
            if (Items_Pool.size() == 0) {
                //if not using logic, then it doesn't matter
                if (Items_Needed.size() == 0) {
                    //make it where the item gives itself
                    Items[Cur_Item].gives_item = true;
                    Items[Cur_Item].can_get = true;
                    //go to start of the for loop
                    continue;
                }
                else {
                    Error(Cur_Item + " could not be placed - pool was empty - probably because of some manual placements?");
                }
            }

            for (int ip = 0; ip < Items_Pool.size(); ip++) {
                string New_Log = Log;

                New_Item = Items_Pool[ip];

                Items[Cur_Item].Name = New_Item;
                Items[Cur_Item].gives_item = true;
                Items[New_Item].can_get = true;

                //make the player acquire the item that is now placed here
                Items_Gotten.push_back(New_Item);

                //New_Log += Cur_Item + " => " + New_Item + "\n";
                if (Items_Needed.size() > 0) {
                    vector<string> IN = Get_First_Items_List(Cur_Item, Items_Needed, Items_Gotten);
                    Items[Cur_Item].Items_Needed = IN;
                }

                if (Items_Needed.size() > 0) {
                    if (Check_Curiosity_Items(Cur_Item) && Randomize(New_Log, Items, Seed, Items_Needed, Items_Gotten, wallets, Items_This)) {
                        return true;
                    }
                    else {
                        int IG_Index = IndexOf(Items_Gotten, New_Item);
                        Items_Gotten.erase(Items_Gotten.begin() + IG_Index);
                        Items[Cur_Item].Name = Cur_Item;
                        Items[Cur_Item].gives_item = false;
                        Items[New_Item].can_get = false;
                    }
                }
                else {
                    return Randomize(New_Log, Items, Seed, Items_Needed, Items_Gotten, wallets, Items_This);
                }
            }
        }

        //if this item hasn't been gotten yet, and it's been placed
        if (IndexOf(Items_Gotten, Items[Cur_Item].Name) == -1) {
            string Item_Name = Items[Cur_Item].Name;

            //make the player acquire the item that is now placed here
            Items_Gotten.push_back(Item_Name);

            //Log += Cur_Item + " => " + Item_Name + "\n";

            if (Items_Needed.size() > 0) {
                if (Get_First_Items_List(Cur_Item, Items_Needed, Items_Gotten).size() > 0) {
                    //Log += Get_First_Items_List(Cur_Item, Items_Needed, Items_Gotten) + "\n";
                    vector<string> IN = Get_First_Items_List(Cur_Item, Items_Needed, Items_Gotten);
                    Items[Cur_Item].Items_Needed = IN;
                }
            }

            return Randomize(Log, Items, Seed, Items_Needed, Items_Gotten, wallets, Items_This);
        }
    }

    for (int i = 0; i < items.size(); i++) {
        //check if every item is obtainable
        if (IndexOf(Items_Gotten, items[i]) == -1) {
            Has_All = false;
        }

        //check if all items have been placed
        if (!Items[items[i]].gives_item) {
            Placed_All = false;
        }
    }
    //can get all items, or no logic
    if (Has_All || Items_Needed.size() == 0) {
        //Global_Log = Log;

        //keep going if haven't placed all items, and using no logic
        if (!Placed_All && Items_Needed.size() == 0)  {
            return Randomize(Log, Items, Seed, Items_Needed, Items_Gotten, wallets, Items_This);
        }
        //cannot get all items somehow?
        else if (!Placed_All) {
            return false;
        }
        else {
            return true;
        }
    }
    //all items are placed, but cannot get all items
    else {
        return false;
    }
}

template<typename vec_type, typename var_type, typename Func>
void forEach(vector<vec_type> vec, Func f, var_type var1) {
    for (int i = 0; i < vec.size(); i++) {
        f(vec[i], var1);
    }
}

void Setup_NonRandom_Items(map<string, Item> &Items, string *Log) {
    vector<string> items;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++) {
        string item = items[i];

        //if item gives itself, then set the gives_item and can_get flags
        if (Items[item].Pool == "") {
            Items[item].gives_item = true;
            Items[item].can_get = true;
        }
        //if an item gives a specific item, then update the item's name to what it gives
        else if (Items[item].Pool[0] == '#') {
            string other_item = "";

            other_item = Items[item].Pool.substr(1); //remove # from start
            Items[item].Name = other_item;
            Items[item].gives_item = true;
            Items[other_item].can_get = true;   //the other item has been placed

            *Log += item + " => " + other_item + "\n\n";
        }
    }

    return;
}

void Setup_Item_Values(map<string, vector<vector<string>>> Items_Needed) {
    vector<string> items;
    string item;
    int item_count = 0; //how many times the item is needed for another item

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++) {
        item = items[i];
        item_count = 0;

        for (int oi = 0; oi < items.size(); oi++) {
            string Other_Item = items[oi];

            for (int in = 0; in < Items_Needed[Other_Item].size(); in++) {
                //if the current item is needed for this list of the other item, then increment item_count
                if (IndexOf(Items_Needed[Other_Item][in], item) != -1) {
                    item_count++;
                }
            }
        }

        Items[item].value = item_count;
    }
}

//void Randomize(map<string, Item> &Items, string &Seed, string Logic_File) {
  void Randomize(map<string, Item> &Items, map<string, map<string, string> > Custom_Settings) {
    map<string, vector<vector<string>>> Items_Needed;
    vector<string> Items_Gotten;
    string Log = "";

    string &Seed = Custom_Settings["settings"]["Seed"];
    string Logic_File = Custom_Settings["settings"]["Logic"];

    if (Logic_File != "" && Logic_File != "None") {
        Items_Needed = Get_Logic(Logic_File);
        Setup_Item_Values(Items_Needed);
    }

    //sets up vanilla and placed items
    Setup_NonRandom_Items(Items, &Log);

    if (Seed == "") {
        Seed = dec_to_string(time(0));
        //srand(time(0));
        srand(hex_to_decimal(string_to_hex(Seed)));
    }
    else {
        srand(hex_to_decimal(string_to_hex(Seed)));
    }

    Randomize(Log, Items, Seed, Items_Needed, Items_Gotten, Custom_Settings["wallets"]);

    return;
}

void Export_Spoiler_Log(map<string, Item> Items) {
    vector<string> keys;
    string item;
    string New_Item;

    keys = Get_Keys(Items);

    for (int i = 0; i < keys.size(); i++) {
        item = keys[i];
        New_Item = Items[item].Name;

        outFile << item << " => " << New_Item << endl << endl;
    }
}

string Get_Red(string colors) {
    string R;

    R = colors.substr(IndexOf_S(colors, "R=") + 2);
    R = R.substr(0, IndexOf_S(R, ","));
    R = dec_to_hex(string_to_dec(R));

    return R;
}

int Get_Red_Dec(string colors) {
    string R;

    R = colors.substr(IndexOf_S(colors, "R=") + 2);
    R = R.substr(0, IndexOf_S(R, ","));

    return string_to_dec(R);
}

string Get_Green(string colors) {
    string G;

    G = colors.substr(IndexOf_S(colors, "G=") + 2);
    G = G.substr(0, IndexOf_S(G, ","));
    G = dec_to_hex(string_to_dec(G));

    return G;
}

int Get_Green_Dec(string colors) {
    string G;

    G = colors.substr(IndexOf_S(colors, "G=") + 2);
    G = G.substr(0, IndexOf_S(G, ","));

    return string_to_dec(G);
}

string Get_Blue(string colors) {
    string B;

    B = colors.substr(IndexOf_S(colors, "B=") + 2);
    B = B.substr(0, IndexOf_S(B, "]"));
    B = dec_to_hex(string_to_dec(B));

    return B;
}

int Get_Blue_Dec(string colors) {
    string B;

    B = colors.substr(IndexOf_S(colors, "B=") + 2);
    B = B.substr(0, IndexOf_S(B, "]"));

    return string_to_dec(B);
}

string Color_To_Hex(string colors) {
    string R;
    string G;
    string B;

    R = Get_Red(colors);
    G = Get_Green(colors);
    B = Get_Blue(colors);

    return R + G + B;
}

//only positive numbers
//returns the index of the max number
//if there are more than one numbers, then it returns the index of the first one
//returns -1 if vector is empty or no values are greater or equal to 0
int Max(vector<double> numbers) {
    double Max_Number = -1;
    int Max_Index = -1;

    for (int i = 0; i < numbers.size(); i++) {
        if (numbers[i] > Max_Number) {
            Max_Number = numbers[i];
            Max_Index = i;
        }
    }

    return Max_Index;
}

//only positive numbers
//returns the index of the max number
//if there are more than one numbers, then it returns the index of the first one
//returns -1 if vector is empty or no values are greater or equal to 0
int Min(vector<double> numbers) {
    double Min_Number = -1;
    int Min_Index = -1;

    for (int i = 0; i < numbers.size(); i++) {
        if (numbers[i] < Min_Number || Min_Number == -1) {
            Min_Number = numbers[i];
            Min_Index = i;
        }
    }

    return Min_Index;
}

double Get_Hue(vector<double> Prime) {
    double Difference;
    int C_Max;
    int C_Min;
    int Hue;
    double temp;

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    Difference = Prime[C_Max] - Prime[C_Min];

    if (Difference == 0) {
        Hue = 0;
    }
    else if (C_Max == 0) {
        temp = Prime[1] - Prime[2];
        temp = temp/Difference;
        temp = Double_Mod(temp, 6);
        Hue = 60 * temp;
    }
    else if (C_Max == 1) {
        temp = Prime[2] - Prime[0];
        temp = temp/Difference;
        temp = temp + 2;
        Hue = 60 * temp;
    }
    else {
        temp = Prime[0] - Prime[1];
        temp = temp/Difference;
        temp = temp + 4;
        Hue = 60 * temp;
    }

    return Hue;
}

double Get_Lightness(vector<double> Prime) {
    int C_Max;
    int C_Min;

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    return ((Prime[C_Max] + Prime[C_Min])*100)/2;
}

double Get_Saturation(vector<double> Prime) {
    int C_Max;
    int C_Min;
    double Difference;
    double Light;
    double Sat;

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    Difference = Prime[C_Max] - Prime[C_Min];
    Light = Get_Lightness(Prime)/100;

    if (Difference == 0) {
        Sat = 0;
    }
    else {
        Sat = Difference/(1 - abs(2*Light - 1));
    }

    return Sat*100;
}

map<string, double> RGB_To_HSL(int red, int green, int blue) {
    map<string, double> HSL;
    vector<double> Prime;   // 0 = red, 1 = green, 2 = blue (indexes)
    int C_Max;
    int C_Min;
    double Difference;

    Prime.push_back(red/255.0);
    Prime.push_back(green/255.0);
    Prime.push_back(blue/255.0);

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    Difference = Prime[C_Max] - Prime[C_Min];

    HSL["H"] = Get_Hue(Prime);

    HSL["S"] = Get_Saturation(Prime);

    HSL["L"] = Get_Lightness(Prime);

    return HSL;
}

map<string, double> RGB_To_HSL(string Hex_Colors) {
    int red, green, blue;

    red = hex_to_decimal(Hex_Colors.substr(0, 2));
    green = hex_to_decimal(Hex_Colors.substr(2, 2));
    blue = hex_to_decimal(Hex_Colors.substr(4, 2));

    return RGB_To_HSL(red, green, blue);
}

//takes hex in form RRGGBB and converts it to rbg5a1
string hex_to_rgb5a1(string hex) {
    string rbga;
    string binary;

    binary = hex_to_binary(hex);

    rbga = binary.substr(0, 5); //first 5 red bits
    rbga += binary.substr(8, 5);    //first 5 green bits
    rbga += binary.substr(16, 5);   //first 5 blue bits
    rbga += "1"; //alpha bit

    rbga = binary_to_hex(rbga);

    return rbga;
}

string rgb5a1_to_hex(string rbga) {
    string hex;
    string binary;

    binary = hex_to_binary(rbga);

    hex = binary.substr(0, 5) + "000";
    hex += binary.substr(5, 5) + "000";
    hex += binary.substr(10, 5) + "000";

    hex = binary_to_hex(hex);

    return hex;
}

double round(double num, int places) {
    int New_Num;

    if (places == 0) {
        New_Num = num;
        return New_Num;
    }
    else  {
        return round(num*10, --places)/10.0;
    }
}

string HSL_To_RGB(double H, double S, double L) {
    string RGB = "";
    double Prime_R, Prime_G, Prime_B;
    double C, X, m;
    int t;
    double d;
    int Red, Green, Blue;
    int places = 2;

    S /= 100;
    L /= 100;

    S = round(S, places);
    L = round(L, places);

    d = 2*L - 1;
    d = abs(d);
    d = 1 - d;
    C = d * S;

    d = H / 60;
    d = Double_Mod(d, 2);  //d % 2
    d = d - 1;
    d = abs(d);
    d = 1 - d;
    X = C * d;

    m = L - C/2;

    if (H <= 60) {   //C, X, 0
        Prime_R = C;
        Prime_G = X;
        Prime_B = 0;
    }
    else if (H <= 120) { //X, C, 0
        Prime_R = X;
        Prime_G = C;
        Prime_B = 0;
    }
    else if (H <= 180) { //0, C, X
        Prime_R = 0;
        Prime_G = C;
        Prime_B = X;
    }
    else if (H <= 240) { //0, X, C
        Prime_R = 0;
        Prime_G = X;
        Prime_B = C;
    }
    else if (H <= 300) { //X, 0, C
        Prime_R = X;
        Prime_G = 0;
        Prime_B = C;
    }
    else { //C, 0, X
        Prime_R = C;
        Prime_G = 0;
        Prime_B = X;
    }

    Red = (Prime_R+m)*255;
    Green = (Prime_G+m)*255;
    Blue = (Prime_B+m)*255;

    RGB = dec_to_hex(Red);
    RGB += dec_to_hex(Green);
    RGB += dec_to_hex(Blue);

    return RGB;
}

vector<string> String_Split(string text, int split) {
    vector<string> Data;

    for (int i = 0; i < text.size(); i+=split) {
        Data.push_back(text.substr(i,split));
    }

    return Data;
}

void Change_FD(string Tunic_Color) {
    string Original_FD = "6D6B9D6B756B7D6BAD6B956BA56B8D6B856BB56B656BBD6BBDABCDEFBDAB9DAB";
    string Green_FD = "0A831B470A830AC323891B4723891305130523C90A4324492449250924491BC7";
    vector<string> FD = String_Split(Green_FD, 4);
    string FD_Location = "01155128";
    int red, green, blue;
    map<string, double> HSL;
    vector<double> H = {};  //no change in hue, keeps same hue throughout
    vector<double> S = {22.7273, 3.40909, 22.7273, 24.2424, -3.53535, 3.40909, -3.53535, 12.3377, 12.3377, -1.19617, 20.9091, 2.81385, 2.81385, 7.57576, 2.81385, 7.57576};
    vector<double> L = {-8.62745, -0.784314, -8.62745, -7.05882, 2.35294, -0.784314, 2.35294, -3.92157, -3.92157, 3.92157, -10.1961, 7.05882, 7.05882, 11.7647, 7.05882, 2.35294};
    double Hue, Sat, Light;
    string Data = "";

    //convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    //if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue) {
        for (int i = 0; i < S.size(); i++) {
            S[i] = 0;
        }
    }

    HSL = RGB_To_HSL(red, green, blue);

    for (int i = 0; i < FD.size(); i++) {
        //Hue = 120;
        //Sat += 50;
        //Light -= 35;

        Hue = HSL["H"];
        //Sat = RGB_To_HSL(rgb5a1_to_hex(FD[i]))["S"] + S[i];
        //Light = RGB_To_HSL(rgb5a1_to_hex(FD[i]))["L"] + L[i];
        Sat = HSL["S"] + S[i];
        Light = HSL["L"] + L[i];

        if (Sat > 100) {
            Sat = 100;
        }
        else if (Sat < 0) {
            Sat = 0;
        }

        if (Light > 100) {
            Light = 100;
        }
        else if (Light < 0) {
            Light = 0;
        }

        Data += hex_to_rgb5a1(HSL_To_RGB(Hue, Sat, Light));
    }

    Write_To_Rom(hex_to_decimal(FD_Location), Data);
}

void Change_Zora(string Tunic_Color) {
    //tunic and back of head - 1, 2, 3
    string Original_Zora_1 = "E7F9D7B529CB431100C10081CFB50347024504490307020504CB03C902C7054D048B0389028715910D4F25950185561F6EA53DDB6E655E6386EB5317ADED7EEB76A98EEFCFB9A73397310107D7BBAF77CEB5B5EFBE31A56B94E78CA584637C21";
    //zora fins
    string Original_Zora_2 = "C7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BAF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77A733A733A733A733A733A733A733A733A733A733A733A733A733A733A733A73397319731973197319731973197319731973197319731973197319731973197318EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF86EB86EB86EB7EEB86EB7EEB86EB7EEB86EB86EB7EEB7EEB7EEB86EB86EB86EB76A976A976A976A976A976A976A976A976A976A976A976A976A976A976A976A96EA56E656EA56E656EA56EA56EA56E656EA56E656EA56EA56E656EA56EA56EA55E635E635E635E635E635E635E635E635E635E635E635E635E635E635E635E63561F561F561F561F561F561F561F561F561F561F561F561F561F561F561F561F3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB2595259525952595259525952595259525952595259525952595259525952595259525952595259525952595259525952595259525952595259525952595259515911591159115911591159115911591159115911591159115911591159115910D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB044904490449044904490449044904490449044904490449044904490449044903C903C903C903C903C903C903C903C903C903C903C903C903C903C903C903C9034703470347034703470347034703470347034703470347034703470347034702C702C702C702C702C702C702C702C702C702C702C702C702C702C702C702C702450245024502450245024502450245024502450245024502450245024502450245024502450245024502450245024502450245024502450245024502450245";
    //next set of tunics - 4, 5, 6 (Same as other one? so gonna write to here as well instead of doing it a third time)
    string Original_Zora_3 = "E7F9D7B529CB431100C10081CFB50347024504490307020504CB03C902C7054D048B0389028715910D4F25950185561F6EA53DDB6E655E6386EB5317ADED7EEB76A98EEFCFB9A73397310107D7BBAF77CEB5B5EFBE31A56B94E78CA584637C21";
    string Zora_Location_1 = "01197120";
    string Zora_Location_2 = "011A2228";
    string Zora_Location_3 = "0119E698";
    string Zora_Location_4 = "010FB0B0";    //zora boomerang
    int red, green, blue;
    vector<string> Zora_1 = String_Split(Original_Zora_1, 4);
    vector<string> Zora_2 = String_Split(Original_Zora_2, 4);
    vector<double> H = {3, 3, 3, 3, 3, 3, 15, 16, 16, 17, 18, 18, 18, 18, 19, 20, 19, 20, 20, 20, 20, 23, 23, 24, 26, 25, 27, 28, 30, 33, 33, 33, 33, 39, 39, 40, 39, 48, 48, 48, 63, 63, 63, 63, 63, 63, 63, 63};
    vector<double> S = {4.06699, -7.47801, -42.4242, -39.0909, 40.9091, 40.9091, -1.94805, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 24.2424, 31.8182, 10.1399, 40.9091, -12.0321, -6.56566, -5.75758, -12.489, -8.64046, -6.07886, -50, -48.9643, -3.9185, -8.56459, -8.458, -1.94805, -8.29726, -2.75288, 40.9091, -7.47801, -0.909091, -51.2478, -53.7576, -53.1208, -54.6953, -55.3526, -55.6126, -55.8389, -55.8651};
    vector<double> L = {66.6667, 61.9608, -7.05882, 5.4902, -21.1765, -22.7451, 60.3922, -5.4902, -11.7647, 0.784314, -7.05882, -13.3333, 3.92157, -2.35294, -8.62745, 7.05882, 2.35294, -3.92157, -10.1961, 11.7647, 8.62745, 14.902, -16.4706, 27.451, 35.2941, 21.1765, 33.7255, 30.5882, 41.5686, 8.62745, 43.1373, 40, 36.8627, 43.1373, 60.3922, 49.4118, 46.2745, -19.6078, 61.9608, 52.549, 54.1176, 44.7059, 47.8431, 38.4314, 32.1569, 29.0196, 25.8824, 22.7451};
    map<string, double> HSL;

    //convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    //if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue) {
        for (int i = 0; i < S.size(); i++) {
            S[i] = 0;
        }
    }

    HSL = RGB_To_HSL(red, green, blue);

    //tunic and back of the head
    for (int i = 0; i < Zora_1.size(); i++) {
        H[i] += HSL["H"] - 20;
        if (H[i] > 360) {
            H[i] -= 360;
        }
        else if (H[i] < 0) {
            H[i] = 360 - H[i];
        }

        S[i] += HSL["S"];
        if (S[i] > 100) {
            S[i] = 100;
        }
        else if (S[i] < 0) {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100) {
            L[i] = 100;
        }
        else if (L[i] < 0) {
            L[i] = 0;
        }

        Write_To_Rom(hex_to_decimal(Zora_Location_1) + (i*2), hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i]))); //tunic 1, 2, 3
        Write_To_Rom(hex_to_decimal(Zora_Location_3) + (i*2), hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i]))); //tunic 4, 5, 6
    }

    //update the H, S, and L for zora fins
    H = {63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 30, 30, 30, 33, 30, 33, 30, 33, 30, 30, 33, 33, 33, 30, 30, 30, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 26, 27, 26, 27, 26, 26, 26, 27, 26, 27, 26, 26, 27, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
    S = {2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, 2.44755, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -8.02708, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -8.29726, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -2.75288, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -8.458, -6.07886, -6.07886, -6.07886, -3.9185, -6.07886, -3.9185, -6.07886, -3.9185, -6.07886, -6.07886, -3.9185, -3.9185, -3.9185, -6.07886, -6.07886, -6.07886, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -8.56459, -6.56566, -12.489, -6.56566, -12.489, -6.56566, -6.56566, -6.56566, -12.489, -6.56566, -12.489, -6.56566, -6.56566, -12.489, -6.56566, -6.56566, -6.56566, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -8.64046, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -12.0321, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, -5.75758, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 10.1399, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 24.2424, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 31.8182, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091};
    L = {58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 58.8235, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 55.6863, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 52.549, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 49.4118, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 46.2745, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 43.1373, 41.5686, 41.5686, 41.5686, 40, 41.5686, 40, 41.5686, 40, 41.5686, 41.5686, 40, 40, 40, 41.5686, 41.5686, 41.5686, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 36.8627, 35.2941, 33.7255, 35.2941, 33.7255, 35.2941, 35.2941, 35.2941, 33.7255, 35.2941, 33.7255, 35.2941, 35.2941, 33.7255, 35.2941, 35.2941, 35.2941, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 30.5882, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 27.451, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 21.1765, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 14.902, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 11.7647, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 8.62745, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 7.05882, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 3.92157, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -5.4902, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647};

    for (int i = 0; i < Zora_2.size(); i++) {
        H[i] += HSL["H"] - 20;
        if (H[i] > 360) {
            H[i] -= 360;
        }
        else if (H[i] < 0) {
            H[i] = 360 - H[i];
        }

        S[i] += HSL["S"];
        if (S[i] > 100) {
            S[i] = 100;
        }
        else if (S[i] < 0) {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100) {
            L[i] = 100;
        }
        else if (L[i] < 0) {
            L[i] = 0;
        }

        Write_To_Rom(hex_to_decimal(Zora_Location_2) + (i*2), hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
        Write_To_Rom(hex_to_decimal(Zora_Location_4) + (i*2), hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
    }
}

void Change_Goron(string Tunic_Color) {
    map<string, double> HSL;
    int red, green, blue;
    string Goron_Location_1 = "0117C780";
    string Goron_Location_2 = "01186EB8";   //when curled
    string Original_Goron_1 = "0185018501850185018501850185018501830183018301830185018501C501C501C501C50245020501C501C50245024501C501C301C501830205028702C7024501430143014301C3028503C903C903070143014302850347044B050B044B038903470389040904CB054D0409038903CB054D054D04CD048B040903070347040904490449034702C5024502050307044B01C301C301830143014302850389034701C501C501830245030704090409038902470287034903C9048B050B050B044B054D054D048B0409034702850387050B01C301430143018301C50287040B04CB020502C5034703C90409048B048B048B04CB048B050B054D054D054D048D04CB";
    vector<string> Goron_1 = String_Split(Original_Goron_1, 4);
    vector<double> H = {23, 23, 23, 23, 23, 23, 23, 23, 12, 12, 12, 12, 23, 23, 20, 20, 20, 20, 16, 18, 20, 20, 16, 16, 20, 11, 20, 12, 18, 20, 19, 16, 15, 15, 15, 11, 15, 18, 18, 18, 15, 15, 15, 16, 20, 18, 20, 20, 16, 20, 18, 18, 20, 18, 20, 23, 20, 20, 21, 19, 18, 18, 16, 18, 17, 17, 16, 13, 16, 18, 18, 20, 11, 11, 12, 15, 15, 15, 20, 16, 20, 20, 12, 16, 18, 18, 18, 20, 23, 20, 21, 18, 19, 18, 18, 20, 20, 20, 19, 18, 16, 15, 15, 18, 11, 15, 15, 12, 20, 20, 21, 18, 18, 13, 16, 18, 18, 19, 19, 19, 18, 19, 18, 20, 20, 20, 23, 18};
    vector<double> S = {40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091};
    vector<double> L = {-16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -16.4706, -14.902, -14.902, -14.902, -14.902, -11.7647, -13.3333, -14.902, -14.902, -11.7647, -11.7647, -14.902, -14.902, -14.902, -16.4706, -13.3333, -10.1961, -8.62745, -11.7647, -18.0392, -18.0392, -18.0392, -14.902, -10.1961, -2.35294, -2.35294, -7.05882, -18.0392, -18.0392, -10.1961, -5.4902, 0.784314, 5.4902, 0.784314, -3.92157, -5.4902, -3.92157, -0.784314, 3.92157, 7.05882, -0.784314, -3.92157, -2.35294, 7.05882, 7.05882, 3.92157, 2.35294, -0.784314, -7.05882, -5.4902, -0.784314, 0.784314, 0.784314, -5.4902, -8.62745, -11.7647, -13.3333, -7.05882, 0.784314, -14.902, -14.902, -16.4706, -18.0392, -18.0392, -10.1961, -3.92157, -5.4902, -14.902, -14.902, -16.4706, -11.7647, -7.05882, -0.784314, -0.784314, -3.92157, -11.7647, -10.1961, -5.4902, -2.35294, 2.35294, 5.4902, 5.4902, 0.784314, 7.05882, 7.05882, 2.35294, -0.784314, -5.4902, -10.1961, -3.92157, 5.4902, -14.902, -18.0392, -18.0392, -16.4706, -14.902, -10.1961, -0.784314, 3.92157, -13.3333, -8.62745, -5.4902, -2.35294, -0.784314, 2.35294, 2.35294, 2.35294, 3.92157, 2.35294, 5.4902, 7.05882, 7.05882, 7.05882, 2.35294, 3.92157};

    //convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    //if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue) {
        for (int i = 0; i < S.size(); i++) {
            S[i] = 0;
        }
    }

    HSL = RGB_To_HSL(red, green, blue);

    for (int i = 0; i < Goron_1.size(); i++) {
        H[i] += HSL["H"] - 5;
        if (H[i] > 360) {
            H[i] -= 360;
        }
        else if (H[i] < 0) {
            H[i] = 360 - H[i];
        }

        S[i] += HSL["S"];
        if (S[i] > 100) {
            S[i] = 100;
        }
        else if (S[i] < 0) {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100) {
            L[i] = 100;
        }
        else if (L[i] < 0) {
            L[i] = 0;
        }

        Write_To_Rom(hex_to_decimal(Goron_Location_1) + (i*2), hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
        Write_To_Rom(hex_to_decimal(Goron_Location_2) + (i*2), hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
    }
}

void Change_Deku(string Tunic_Color) {
    vector<string> Deku_Locations = {"011A9092", "011A9094", "011A9096", "011A9098", "011A909A", "011A909C", "011A909E", "011A90A0", "011A90A2", "011A90A4", "011A90A6", "011A90A8", "011A90AA", "011A90AC", "011A90AE", "011A90B0"};
    vector<int> H = {-24, -10, 13, 17, 17, 18, 19, 20, 19, 21, 20, 21, 21, 21, 0, 2};
    vector<double> S = {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, -100, 41};
    vector<double> L = {-15, -18, -9, -7, -1, -2, 2, 1, -4, 9, 5, 4, -1, -6, -100, -13};
    vector<string> H_Hex = {};
    vector<string> S_Hex = {};
    vector<string> L_Hex = {};
    vector<string> RBG5A1 = {};
    vector<string> RGB = {};
    int red;
    int green;
    int blue;
    map<string, double> HSL;

    //convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    //if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue) {
        S = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    HSL = RGB_To_HSL(red, green, blue);

    //apply base to each offset
    for (int i = 0; i < H.size(); i++) {
        //update the hues
        H[i] += HSL["H"] - 10;
        H[i] %= 360;
        if (H[i] < 0) { //if hue goes negative, then wrap it around
            H[i] = 360 - H[i];
        }

        //update the Saturations
        S[i] += HSL["S"];
        if (S[i] > 100) {
            S[i] = 100;
        }
        else if (S[i] < 0) {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100) {
            L[i] = 100;
        }
        else if (L[i] < 0) {
            L[i] = 0;
        }
    }

    //convert HSL back to hex
    for (int i = 0; i < H.size(); i++) {
        RGB.push_back(HSL_To_RGB(H[i], S[i], L[i]));
    }

    //convert new hex to rgb5a1
    for (int i = 0; i < RGB.size(); i++) {
        RBG5A1.push_back(hex_to_rgb5a1(RGB[i]));
    }

    //write values to rom
    for (int i = 0; i < RBG5A1.size(); i++) {
        Write_To_Rom(hex_to_decimal(Deku_Locations[i]), RBG5A1[i]);
    }
}

void Change_Link_Color(string color) {
    vector<string> Link_Locations = {"0116639C", "011668C4", "01166DCC", "01166FA4", "01167064", "0116766C", "01167AE4", "01167D1C", "011681EC"};
    string RGB;

    //change kokiri tunic color
    RGB = Color_To_Hex(color);
    for (int i = 0; i < Link_Locations.size(); i++) {
        Write_To_Rom(hex_to_decimal(Link_Locations[i]), RGB);
    }
}

void Change_Item_Screen(string color) {
    string RGB;
    string RG;
    string B;

    RGB = Color_To_Hex(color);
    RG = RGB.substr(0, 4);
    B = RGB.substr(4, 2);
    Write_To_Rom(13225082, RG);
    Write_To_Rom(13226478, RG);
    Write_To_Rom(13225086, B);
    Write_To_Rom(13226482, B);
}

void Change_Map_Screen(string color) {
    string RGB;
    string RG;
    string B;

    RGB = Color_To_Hex(color);
    RG = RGB.substr(0, 4);
    B = RGB.substr(4, 2);
    Write_To_Rom(13225378, RG);
    Write_To_Rom(13226762, RG);
    Write_To_Rom(13225382, B);
    Write_To_Rom(13226766, B);
}

void Change_Status_Screen(string color) {
    string RGB;
    string RG;
    string B;

    RGB = Color_To_Hex(color);
    RG = RGB.substr(0, 4);
    B = RGB.substr(4, 2);
    Write_To_Rom(13225802, RG);
    Write_To_Rom(13227490, RG);
    Write_To_Rom(13225806, B);
    Write_To_Rom(13227494, B);
}

void Change_Mask_Screen(string color) {
    string RGB;
    string RG;
    string B;

    RGB = Color_To_Hex(color);
    RG = RGB.substr(0, 4);
    B = RGB.substr(4, 2);
    Write_To_Rom(13226134, RG);
    Write_To_Rom(13227802, RG);
    Write_To_Rom(13226138, B);
    Write_To_Rom(13227806, B);
}

void Change_Nameplate(string color) {
    string RGB;
    string RG;
    string B;

    RGB = Color_To_Hex(color);
    RG = RGB.substr(0, 4);
    B = RGB.substr(4, 2);
    Write_To_Rom(13230330, RG); //nameplate
    Write_To_Rom(13230466, RG); //Highlighted Z
    Write_To_Rom(13230578, RG); //Highlighted R
    Write_To_Rom(13230334, B);  //nameplate
    Write_To_Rom(13230486, B);  //Highlighted Z
    Write_To_Rom(13230582, B);  //Highlighted R
}

void Change_Colors(map<string, string> colors) {
    Change_Link_Color(colors["Link"]);
    Change_Deku(colors["Deku"]);
    Change_Goron(colors["Goron"]);
    Change_Zora(colors["Zora"]);
    Change_FD(colors["FD"]);

    Change_Item_Screen(colors["Item"]);
    Change_Map_Screen(colors["Map"]);
    Change_Status_Screen(colors["Quest"]);
    Change_Mask_Screen(colors["Mask"]);
    Change_Nameplate(colors["Name"]);
}

void Change_Tunics(string Tunic_Color) {

    //change kokiri tunic color
    Change_Link_Color(Tunic_Color);

    //change deku link color
    Change_Deku(Tunic_Color);

    //change goron link color
    Change_Goron(Tunic_Color);

    //change zora link color
    Change_Zora(Tunic_Color);

    //change FD link color
    Change_FD(Tunic_Color);
}

void Change_Link_Kafei() {
    vector<string> Kafei = {"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001505050505050505050505050505050A140908000000000000000000000000000000000809140A0505050505050505050505050505150000000000000000000538440E0E0E0E0E0E0E0E0E6363544F532D38600514080000000000000000000008140560382D534F5463630E0E0E0E0E0E0E0E0E44380500000000000000054B4607070707070707221616687A6B676F6E0E55584A0900000000000000000000094A58550E6E6F676B7A6816162207070707070707464B05000000000005493D0747241818181870247B650707070722166C79373F0400000000000000000000043F37796C162207070707657B2470181818182447073D490500000000413B073E040406061111938C9F77784E515007070707071604000000000000000000000416070707070750514E78779F8C931111060604043E073B410000000031050A1405050504040404040D110C0101012339250707070600000000000000000000060707072539230101010C110D0404040404050505140A053100000000A909140A0406060D0D0D0D0606060D0D0D110119235C255B0608000000000000000008065B255C231901110D0D0D0606060D0D0D0D0606040A1409A9000000000B09A9050D110C0C0101300C0D04040404060611191F3A3C0608000000000000000008063C3A1F19110606040404040D0C3001010C0C110D05A9090B000000000BD8665E32121212121212121201300C060404040501191F0508000000000000000008051F190105040404060C30011212121212121212325E66D80B0000000092A0561DAD2A26C91D1D0202C926596261121105050401190A080000000000000000080A1901040505111261625926C902021D1DC9262AAD1D56A0920000000008AE910202020202020202020202021D26C0121105050506140000000000000000000014060505051112C0261D02020202020202020202020291AE08000000000892B602B5AF4EB50202020202020202020273120604050A0900000000000000000000090A050406127302020202020202020202B54EAFB502B69208000000000B92C6EC8B69F2891D029489020202520202022F120D050A1500000000000000000000150A050D122F020202520202028994021D89F2698BECC6920B000000000B0892F48975104A5A5A0F0F860202A5A50202027C1205141500000000000000000000151405127C020202A5A50202860F0F5A5A4A107589F492080B0000000008000011B09E10DE34B264575202024CF00285F8268A060509080000000000000000080905068A26F88502F04C0202525764B234DE109EB0110000080000000000000006978710B79C27171B6A020276A7A5865C347C010509000000000000000000000905017C345C86A5A77602026A1B17279CB7108797060000000000000000000004969DF629858017171717171B45D7CAB0FB7F5E04090000000000000000000009045E7FFBB0CAD7451B1717171717808529F69D9604000000000000000000000A1E814D0FA21A5F171B1B5D5FAAE740FAE68F8A060A0B00000000000000000B0A068A8FE6FA40E7AA5F5D1B1B175F1AA20F4D811E0A000000000000000000000B0128720FCF888EAA353545B3BFCF0FBDC883480D050B00000000000000000B050D4883C8BD0FCFBFB3453535AA8E88CF0F7228010B00000000000000000000000A0C28A60FE3C5901A1A1AE874BD29ABC384430D0A0B00000000000000000B0A0D4384C3AB29BD74E81A1A1A90C5E30FA6280C0A0000000000000000000000000B09112132C29D9D5757FEE2408D339AA11E1E04090800000000000000000809041E1EA19A338D40E2FE57579D9DC2322111090B0000000000000000000000000000001411011F21421F01040A14090909090A09080000000000000000000008090A09090909140A04011F42211F01111400000000000000000000000000000000000000000008080B0B0B0808000000000000080000000000000000000000000800000000000008080B0B0B08080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001505050505050505050505050505050A140908000000000000000000000000000000000809140A0505050505050505050505050505150000000000000000000538440E0E0E0E0E0E0E0E0E6363544F532D38600514080000000000000000000008140560382D534F5463630E0E0E0E0E0E0E0E0E44380500000000000000054B4607070707070707221616687A6B676F6E0E55584A0900000000000000000000094A58550E6E6F676B7A6816162207070707070707464B05000000000005493D0747241818181870247B650707070722166C79373F0400000000000000000000043F37796C162207070707657B2470181818182447073D490500000008413B073E040406061111938C9F77784E515007070707071604000000000000000000000416070707070750514E78779F8C931111060604043E073B4108000009310514140A04060404040404060D110C010123392507070706000000000000000000000607070725392301010C110D06040404040406040A141405310900000BA9150BA905040606060606040404040406060C12235C255B0608000000000000000008065B255C23120C0606040404040406060606060405A90B15A90B0000000B15090A0406060D11110C0C11110D060404040D121F3A3C0608000000000000000008063C3A1F120D040404060D11110C0C11110D0606040A09150B000000080B090A0406110C0101010C0C0C0C0C0D0D0604050501191F0508000000000000000008051F1901050504060D0D0C0C0C0C0C0101010C1106040A090B080000000B090504060C0C01010101010101010101110D06050530190A080000000000000000080A19300505060D11010101010101010101010C0C060405090B000000000015140611010101010101010101010101010C110604050614000000000000000000001406050406110C010101010101010101010101011106141500000000000008EB8CA10101010101010101010101010101011106050A1500000000000000000000150A050611010101010101010101010101010101A18CEB0800000000000008A0C9C9AD2A26C91D1D5656C9265962F90101010D041415000000000000000000001514040D010101F9625926C956561D1DC9262AADC9C9A00800000000000000EF9102020202020202020202020256B17D01010C040509080000000000000000080905040C01017DB15602020202020202020202020291EF000000000000000000A01DB5AFD534B26457020202020202027F010C060509000000000000000000000905060C017F020202020202025764B234D5AFB51DA0000000000000000000000496ABF6BD9C27171B02020276A7A55AC92F0106040900000000000000000000090406012FC95AA5A7760202021B17279CBDF6AB9604000000000000000000000A1E814D10858017171717171B45D74EFB9C7C1E060A0B00000000000000000B0A061E7C9CFB4ED7451B17171717178085104D811E0A000000000000000000000B0128720FA21A5F171B1B5D5FAAE710BD4E8F660D050B00000000000000000B050D668F4EBD10E7AA5F5D1B1B175F1AA20F7228010B00000000000000000000000A0C28A6E2888EAA353545B3BFCF29ABC378430D0A0B00000000000000000B0A0D4378C3AB29CFBFB3453535AA8E88E2A6280C0A0000000000000000000000000B091121329D64901A1AE5E8E340339AA11E1E04090800000000000000000809041E1EA19A3340E3E8E51A1A90649D322111090B0000000000000000000000000000001411011F21421F01040A14090909090A09080000000000000000000008090A09090909140A04011F42211F01111400000000000000000000000000000000000000000008080B0B0B0808000000000000080000000000000000000000000800000000000008080B0B0B0808000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
                            "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001505050505050505050505050505050A140908000000000000000000000000000000000809140A0505050505050505050505050505150000000000000000000538440E0E0E0E0E0E0E0E0E6363544F532D38600514080000000000000000000008140560382D534F5463630E0E0E0E0E0E0E0E0E44380500000000000000054B4607070707070707221616687A6B676F6E0E55584A0900000000000000000000094A58550E6E6F676B7A6816162207070707070707464B05000000000005493D0747241818181870247B650707070722166C79373F0400000000000000000000043F37796C162207070707657B2470181818182447073D490500000008413B073E040406061111938C9F77784E515007070707071604000000000000000000000416070707070750514E78779F8C931111060604043E073B4108000009310514140A04060404040404060D110C010123392507070706000000000000000000000607070725392301010C110D06040404040406040A141405310900000BA9150BA905040606060606040404040406060C12235C255B0608000000000000000008065B255C23120C0606040404040406060606060405A90B15A90B0000000B15090A0406060D11110C0C11110D060404040D121F3A3C0608000000000000000008063C3A1F120D040404060D11110C0C11110D0606040A09150B000000000B090A0406110C0C0C0C0C0C0C0C0C0D0D0604050501191F0508000000000000000008051F1901050504060D0D0C0C0C0C0C0C0C0C0C1106040A090B000000000B090504060C0C30010101010101010101110D06050530190A080000000000000000080A19300505060D11010101010101010101300C0C060405090B000000000015140601010101010101010101010101010C110604050614000000000000000000001406050406110C010101010101010101010101010106141500000000000008EB8C010101010101010101010101010101011106050A1500000000000000000000150A050611010101010101010101010101010101018CEB0800000000000008A02F32010101010101010101010101010101010D041415000000000000000000001514040D01010101010101010101010101010101322FA00800000000000000EF56D9F70101010101010101010101010101010C040509080000000000000000080905040C010101010101010101010101010101F7D956EF000000000000000000910262F901010101010101010101010101010C060509000000000000000000000905060C0101010101010101010101010101F9620291000000000000000000003AC90262190101010101010101010101010101060409000000000000000000000904060101010101010101010101010101196202C93A000000000000000000000AD90256D9EE010101010101010101011E5ED91E060A0B00000000000000000B0A061ED95E1E01010101010101010101EED95602D90A000000000000000000000B017F0202B17C485E12F761EE48D97DC07FCE660D050B00000000000000000B050D66CE7FC07DD948EE61F7125E487CB102027F010B00000000000000000000000A0CEEB1020202560202020202020202CE9C430D0A0B00000000000000000B0A0D439CCE020202020202020256020202B1EE0C0A0000000000000000000000000B0911218A2F5602020202020226EC66191E1E04090800000000000000000809041E1E1966EC26020202020202562F8A2111090B0000000000000000000000000000001411011F21421F01040A14090909090A09080000000000000000000008090A09090909140A04011F42211F01111400000000000000000000000000000000000000000008080B0B0B0808000000000000080000000000000000000000000800000000000008080B0B0B0808000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000DA3800030D000000D7000002FFFFFFFFD900000000230405010030060600A360010060120600E6D80100201606005D20DA3800030D000040E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005E00F5500000070D8140E600000000000000F3000000071FF400E700000000000000F5480400000D8140F20000000003C0FCFA000080FFFFFFFFD900000000230005010080260600A91006001618001A0200061C1E02000402200604220E00240012E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005B40F550000007090040E600000000000000F30000000707F400E700000000000000F548040000090040F20000000003C03C010110380600E1C8061416180006081A061C1E0A000A200C0622240C00080C2606282A08002C2E0A0610300A0014063205343606000000000100C01806008D600600020400060408060A0C0E0010081206120804000E140A0602001600040600E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01CD9000000002304050100A01406007E000600020400060208060A060800080200060C040E00040C0006100E040008120A060A120E000E100ADF00000000000000DA3800030D000380D7000002FFFFFFFFD9000000002304050100600C0600A790DA3800030D0003C0E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005740F5500000070D4050E600000000000000F3000000071FF200E700000000000000F5480800000D4050F20000000007C07CFA000080FFFFFFFF0100F02A06009EF006000C0E0010001206081416000A0018061A1C0600061E00060420220024020406040826000228060100E01C06005C40060002040006080A060A0C0E001008120610141600121410061804020018021A061614000004160006080612000A0E06E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600C398F5500000070CC030E600000000000000F30000000701F800E700000000000000F5480200000CC030F20000000001C01C0100D01A06006A00060002040006040206080602000A0C0E06100608000E0C120612140E0016141206141618001804140518000400000000E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005400F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03C0100F01E06007B80060002040006080A06000C02000E101206041400000A1606060C001400181A1CDF00000000000000DA3800030D0002C0D7000002FFFFFFFFD9000000002304050100600C06008BC0DA3800030D000300E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005740F5500000070D4050E600000000000000F3000000071FF200E700000000000000F5480800000D4050F20000000007C07CFA000080FFFFFFFF0100F02A060083A0060C0E0000100012061416080018000A06061A1C00001E06062022040004022406260804000628020100E01C0600A5D0060002040006080A060C0E0600100812061416120012161006020018001A0218060416140004140006100A08000A0C06E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600C398F5500000070CC030E600000000000000F30000000701F800E700000000000000F5480200000CC030F20000000001C01C0100D01A0600B258060002040002000606020608000A0C0E0608061000120C0A060A14120012141606181614001400180500041800000000E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005400F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03C0100F01E06008490060002040006080A06020C04000E101206041400000A16060614040C00181A1CDF00000000000000DA3800030D000100D7000002FFFFFFFFD9000000002304050100500A06008120DA3800030D000140E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000",
                            "E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01CFA000080FFFFFFFF0101102C0600BCA00604020A000C0E02060610120014160006000818001A020006021C1E002022040624260400060428052A0806000000000101C0380600B360060002040006020006000408000A0C0E061012140016181A061A1016000E121006101C0E0004021E061E200400042008060E1C0A000A1A1806180C0A0002221E06242628002A2C2E060622020014161006302A2E00242832061E22340036201EDF000000000000000349FF8701A90000FC2400FC6C172EFF0306FE9A019E0000FC3C020B36A334FF03D9FF0100740000FEF200D768D326FF032BFE74010D0000FD8F020536970FFF032BFE74FEF30000027102052891F1FF03D9FF01FF8C0000010E00D764C7E1FF01B0017D0000000000000200257200FF01F7009101BC0000040002001C5351FF02FD00460190000003AF00A9455828FF0350003B00BE00000215FFFD624016FF0349FF8701A90000046900206C172EFF02A8010900000000002800BC4A5D00FF0398FFA400B900000276FF7A6C2A1AFF0398FFA400B90000FE5300916C2A1AFF0349FF87FE57000003DC00FC6C17D2FF0398FFA4FF47000001AD00916C2AE6FF0306FE9AFE62000003C4020B36A3CCFF0349FF87FE570000046900206C17D2FF02FD0046FE70000003AF00A94558D8FF0350003BFF4200000215FFFD6240EAFF0398FFA4FF4700000276FF7A6C2AE6FF01F70091FE440000040002001C53AFFF0620CF100A0B05000000000000000000000000000000000000050B0A10CF20061F0606060606060C080D0C1753160000000000292B0E2E0D0D0D0808080C0C0CC30617111916130A000000000000000000000000000000000A131619111706C30C0C0C0808080D0D0D2E0E2B290000000000000029143F21110E182E080808080C0CC30606261014000000000000000000000000000000001410260606C30C0C080808082E180E11213F142900000000000000000000002929050B1B210E170D0D0D080C0C18595A0B00000000000000000000000000000B5A59180C0C080D0D0D170E211B0B052929000000000000000000000000000000000004223B971A113817180D080C1F1510470000000000000000000000004710151F0C080D181738111A973B22040000000000000000000000000000D003030309243D4E343A333330506A1A0E0D0C1F1E14050000000000000000000005141E1F0C0D0E1A6A503033333A344E3D2409030303D000000000000000000049040435232F3A46ADA3B5DD58925D851A18080C64132900000000000000002913640C08181A855D9258B5A3A3AD463A2F2335040449000000000000000000000339BB872F449C820101E1EBEEB55752552B0E0D0C250B00000000000000290B250C0D0E2B555257B5527801F1CD4889442F87BB39030000000000000000000000B0524B349CF701010101DCD5DEE48844092211181F132900000000000205131F18112209448852A15E01F5CBC4E1D5F8344B52B0000000000000000000000000002340677801010101E1A099A29A403042050BC977F447000202020202031677C90B054230425E0101F3DB019AA299A06740230000000000000000000000000000043441A101010101F083A48D485633AA140205F40200020303030203030204050214AA33A6010101E8AC56488DA483413404000000000000000000000000000000239EA6010101018C839463807B90962705020303030303030303050505050505279601010101017A907B806394839E2300000000000000000000000000000000045CA601010101BE327C73706E3295690A29050404D005050505050404040309699501010101017A326E70737C325C0400000000000000000000000000000000037D6D0101010101A89BA58132605F4D2202030404040404D0040404040403224D5F010101010101603281A59BA87D030000000000000000000000000000000000006F6501010101017491A798755F6114020504040404040404040404D00314615F7501010101010198A791746F0000000000000000000000000000000000000000004F5E01FB01010101017950715BF40304B0B004090909090404040403F45B715079010101010101015E4F0000000000000000000000000000000000000000000004848F0101015E658E72691602050404090909090909090909B0040405001669728E655E0101018F84040000000000000000000000000000000000000000000000D14382627E766B51B9B80002030409090904040404040409090404032900B8B9516B767E628243D100000000000000000000000000000000000000000000000000000003040405290500000203040404040405030303D0040404040302020005290504040300000000000000000000000000000000000000000000000000000000000000000000000000000203D004D0030302020202020305D0D005020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020303030202000000000002020303030200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000202020200000000000000000000020202000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001505050505050505050505050505050A140908000000000000000000000000000000000809140A0505050505050505050505050505150000000000000000000538440E0E0E0E0E0E0E0E0E6363544F532D38600514080000000000000000000008140560382D534F5463630E0E0E0E0E0E0E0E0E44380500000000000000054B4607070707070707221616687A6B676F6E0E55584A0900000000000000000000094A58550E6E6F676B7A6816162207070707070707464B05000000000005493D0747241818181818247B65DCEA222216166C79373F0400000000000000000000043F37796C16162222EADC657B2418181818182447073D490500000008413B073E04010101010101010C9F8B515BEA070707070716040000000000000000000004160707070707EA5B518B9F0C01010101010101043E073B410800000931050A0A010101010101010101010101011123392507070706000000000000000000000607070725392311010101010101010101010101010A0A05310900000892009201010101010101010101010101D1D11119235C255B0608000000000000000008065B255C231911D1D10101010101010101010101010192009208000008090AAC0101010101D1D1D10D0D0D0D0606040AC6931F3A3C0608000000000000000008063C3A1F93C60A0406060D0D0D0DD1D1D10101010101AC0A09080000080BD22A2F7C736DB12626266D2FC061300C0C110604C2121F0508000000000000000008051F12C20406110C0C3061C02F6D262626B16D737C2F2AD20B080000000BC63C0202020202020202020202022F481E010D0506EB010A000000000000000000000A01EB06050D011E482F0202020202020202020202023CC60B000000009292D802020202020202020202020202022AEE01050404059200000000000000000000920504040501EE2A0202020202020202020202020202D89292000000000892C6029CAFD58B1D02B58902020256020226610405050409000000000000000000000904050504612602025602020289B5021D8BD5AF9C02C69208000000000092C6598B69DE4A5A5A292986020252520202B13206050A0900000000000000000000090A050632B10202525202028629295A5A4ADE698B59C6920800000000000892618B75F2DE34B264575202024C4C34CE027C01050A1500000000000000000000150A05017C02CE344C4C0202525764B234DEF2758B6192080000000000000092A1897510B79C27171B6A0202801B8FDE5C26EE04141500000000000000000000151404EE265CDE8F1B8002026A1B17279CB7107589A19200000000000000000011B09E10298580171B1776761B3583BBB034C011050908000000000000000008090511C034B0BB83351B7676171B17808529109EB011000000000000000000000697870F29A21A5F171B1B5D5FAAA2E2CA857F1E04090000000000000000000009041E7F85CAE2A2AA5F5D1B1B175F1AA2290F8797060000000000000000000004969D0F0FCA888EAA353545B3BFFEF5DE847FEE0409000000000000000000000904EE7F84DEF5FEBFB3453535AA8E88CA0F0F9D9604000000000000000000000A1EAE334D4D57C5901A1A1AE87440F5FAB08F8A060A0B00000000000000000B0A068A8FB0FAF54074E81A1A1A90C5574D4D33AE1E0A000000000000000000000B012872330F0F8DE3747474FE0F0F0FFFC883480D050B00000000000000000B050D4883C8FF0F0F0FFE747474E38D0F0F337228010B00000000000000000000000A0C28A69D3340290F0F0F0F0F4DFF8DD384430D0A0B00000000000000000B0A0D4384D38DFF4D0F0F0F0F0F2940339DA6280C0A0000000000000000000000000B09112132C2AB33BDF54040408D879A191E1E04090800000000000000000809041E1E199A878D404040F5BD33ABC2322111090B0000000000000000000000000000001411011F21421F01040A14090909090A09080000000000000000000008090A09090909140A04011F42211F01111400000000000000000000000000000000000000000008080B0B0B0808000000000000080000000000000000000000000800000000000008080B0B0B0808000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
                            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001505050505050505050505050505050A140908000000000000000000000000000000000809140A0505050505050505050505050505150000000000000000000538440E0E0E0E0E0E0E0E0E6363544F532D38600514080000000000000000000008140560382D534F5463630E0E0E0E0E0E0E0E0E44380500000000000000054B4607070707070707221616687A6B676F6E0E55584A0900000000000000000000094A58550E6E6F676B7A6816162207070707070707464B05000000000005493D0747241818181870247B650707070722166C79373F0400000000000000000000043F37796C162207070707657B2470181818182447073D490500000008413B073E04040606060611939F77784E515007070707071604000000000000000000000416070707070750514E78779F93110606060604043E073B4108000009310A140A0406060D0D0D0D06060611010101233925070707060000000000000000000006070707253923010101110606060D0D0D0D0606040A140A310900000B0909A9050D110C0C0101300C0D040404060D0119235C255B0608000000000000000008065B255C2319010D060404040D0C3001010C0C110D05A909090B0000000B0504040C01010101010101010C110D0604040D121F3A3C0608000000000000000008063C3A1F120D0404060D110C01010101010101010C0404050B0000000092D2DF613261D92F26C956026DECDF8A010C11050501191F0508000000000000000008051F19010505110C018ADFEC6D0256C9262FD9613261DFD2920000000008AE9102AD1D0202020202020202021D598A121105040C190A080000000000000000080A190C040511128A591D0202020202020202021DAD0291AE08000000000892B6021DC91D5C86B0CACAB089FB1D020273120604050614000000000000000000001406050406127302021DFB89B0CACAB0865C1DC91D02B69208000000000B9292AD1D5A84B70F0F0F0F0F0F0F0F94F8022F120D050A0900000000000000000000090A050D122F02F8940F0F0F0F0F0F0F0FB7845A1DAD92920B000000000B080048B5AFB70F0F0F74E51A1AE80F0F86F8027C1205141500000000000000000000151405127C02F8860F0FE81A1AE5740F0F0FB7AFB54800080B000000000800001EFB750F0F0F744545353545BFFE0F89F8268A060515000000000000000000001505068A26F8890FFEBF4535354545740F0F0F75FB1E0000080000000000000011869E0F0FB7BF64571B1B5F45E8BDDE4E347C010509080000000000000000080905017C344EDEBDE8455F1B1B5764BFB70F0F9E8611000000000000000000000697870F0FBB5F0F0F86171B5FAABB0F89FB7F5E04090000000000000000000009045E7FFB890FBBAA5F1B17860F0F5FBB0F0F8797060000000000000000000004969D0F0F84279489FB02F0A7A7A20FCA397CEE0614000000000000000000001406EE7C39CA0FA2A7A7F002FB899427840F0F9D9604000000000000000000000A1E814D0F8F4C17B55A02021780860FFAE68F8A060A0B00000000000000000B0A068A8FE6FA0F86801702025AB5174C8F0F4D811E0A000000000000000000000B0128720F5A4C1702020202174C860FFF4A83480D050B00000000000000000B050D48834AFF0F864C1702020202174C5A0F7228010B00000000000000000000000A0C28B0AD4C4C020202024C4C83BBBBB084430D0A0B00000000000000000B0A0D4384B0BBBB834C4C020202024C4CADB0280C0A0000000000000000000000000B0911216DC9020202020252022F7DA1A11E1E04090800000000000000000809041E1EA1A17D2F02520202020202C96D2111090B0000000000000000000000000000001411011F21421F01040A14090909090A09080000000000000000000008090A09090909140A04011F42211F01111400000000000000000000000000000000000000000008080B0B0B0808000000000000080000000000000000000000000800000000000008080B0B0B080800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000092C3492D494A9D000000000000000000000000000000000000000000000000000000000000000000000000000000009D4A492D49C39200000000000000000000585537796746545338C3C60000000000000000000000000000000000000000000000000000000000000000C6C3385354466779375558000000000000000000386F07DC50EA072237794663534B38AB000000000000000000000000000000000000000000000000AB384B53634679372207EA50DC076F380000000000000000582584D8848418244750EA167A6746635349C3C600000000000000000000000000000000C6C349536346677A16EA504724188484D88425580000000000000000150815090A0531D8968470650707686B6F3B4449C20000000000000000000000000000C249443B6F6B68070765708496D831050A09150815000000000000000000150A05040D0C01110CAC774E25EA07167A67465438000000000000000000000000385446677A1607EA254E77AC0C11010C0D04050A1500000000000000000000150A05060D30010101010101018B250707077A6B67A208000000000000000008A2676B7A070707258B01010101010101300D06050A1500000000000000000000150A05060C01010101010101010101832507070716840000000000000000000084160707072583010101010101010101010C06050A1500000000000000000000150A0A0630010101010101010101010123510707071100000000000000000000110707075123010101010101010101010130060A0A15000000000000000000000B090A060C010101010101010101010101F439505B0A000000000000000000000A5B5039F40101010101010101010101010C060A090B000000000000000000000015090A0C01010101010101010101010101618523A900000000000000000000A9238561010101010101010101010101010C0A0915000000000000000000000000000B090C61627D7D8AF91212010101010101F7AC090000000000000000000009ACF70101010101011212F98A7D7D62610C090B000000000000000000000000000000096159020202022F7C7D8A61F9A10101110609000000000000000000000906110101A1F9618A7D7C2F02020202596109000000000000000000000000000000000B615902020202020202C92A7CD961F9A1060A0B00000000000000000B0A06A1F961D97C2AC90202020202020259610B00000000000000000000000000000000000000C2439102020202020202027391DF0D050B00000000000000000B050DDF917302020202020202029143C20000000000000000000000000000000000000000000000000000000000009209C27723F40D0900000000000000000000090DF42377C2099200000000000000000000000000000000000000000000000000000000000000000000000000150905061106041400000000000000000000000014040611060509150000000000000000000000000000000000000000000000000000000000080B1509140D01010101011104090B0000000000000000000000000B09041101010101010D1409150B08000000000000000000000000000000000000000000000015090401010101010C060A1415000000000000000000000000000015140A060C0101010101040915000000000000000000000000000000000000000000000000000B1404040404050A0A090B000000000000000000000000000000000B090A0A0504040404140B0000000000000000000000000000000000000000000000000000000B0B15150B0B0000000000000000000000000000000000000000000000000B0B15150B0B000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
                            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A41DB33372128010128213733DB410A00000000000000000000000000000028C904040404045B33335B0404040404C928000000000000000000000000000A331D0708A2D008071D1D0708D0A208071D330A0000000000000000003F170A211DF57405050505050505050505050574F51D210A173F0000000000373921CC20706E7664BF5F0B2B3F3F2B0B5FBF64766E7020CC21393700000000394C40C73673775F3F4116160A00000A1616413F5F777336C7404C39000000005E36898868D8CFDC0C160A0000000000000A160CDCCFD8688889365E000000001BE602BE1B0B412130210C0C0C0C0C0C0C0C213021410B1BBE02E61B0000002B0BA98F2A00000016581B29131809091813291B58160000002A8FA90B2B00002B0000000000000000172A1C1915090915191C2A1700000000000000002B0000000000000000000000000A161F143131141F160A00000000000000000000000000000000000000000000000017283F3F281700000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002F165737280101283757162F0000000000000000000000000000000000000A28370404045B33335B04040437280A000000000000000000000000000000000ACFA10FA208071D1D0708A20FA1CF0A0000000000000000000000000000002F1AED0F70616CABB5B5AB6C61700FED1A2F0000000000000000000000000A0A0A400F6165E13A323232323AE165610F400A0A0A0000000000000000002F0A0A580F61830303030303030303030383610F580A0A2F00000000000000171A2A3FA96CEE320303030303030303030332EE6CA93F2A1A1700000000002F52A008A690DE3203030303030303030303030332DE90A608A0522F000000000A4F083B9525F00303030303030303030303030303F025953B084F0A00000000164A3B0525258B67E7F03A3232323232323AF0E7678B2525053B4A160000000016D8AD7E7AFCFC4F998F4C71F3ADADF3714C8F994FFCFC7A7EADD8160000000000000B0B54540001010101010101010101010101010054540B0B000000000000000000001717002130210C0C0C0C0C0C0C0C2130210017170000000000000000000000000000002130210C0C0C0C0C0C0C0C21302100000000000000000000000000000000000016581B29131809091813291B581600000000000000000000000000000000000000172A1C1915090915191C2A170000000000000000000000000000000000000000000A161F143131141F160A000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A57372801012837570A000000000000000000000000000000000000000000DA04040404040404040404DA000000000000000000000000000000282A2A1A2AC107070707070707070707C12A1A2A2A28000000000000000000172AFCFCF78673252595646464649525257386F7FCFC2A17000000000000000028FCF4F465B39DEE8D3203030303328DEE9DB365F4F4FC2800000000000000001AFC4FFD029A7C7DE53A030303033AE57D7C9A02FD4FFC1A000000000000002F52C819880202636F6F80C47B7BC4806F6F6302028819C8522F000000000000172A4F85020202020202020202020202020202020202854F2A1700000000000A17FC4A68020202020202020202020202020202020202684AFC170A000000000A1AC8960202AF6AB74626755A5A5A5A752646B76AAF020296C81A0A000000000A1AFC9E025D6AD3C2BA4646262626264646BAC2D36A5D029EFC1A0A00000000171A2A1B685D9422D58ABBBBDD6666DDBBBB8AD522945D681B2A1A1700000000171A2AFC9B85C211060606221E1E1E1E2206060611C2859BFC2A1A170000000017282A2A20017811060606221E1E1E1E22060606117801202A2A2817000000000028522AACF701781111060606060606060611117801F7AC2A5228000000000000171A2AFC2001019CD99469699494696994D99C010120FC2A1A170000000000000028522A1B2001012A97C643434343C6972A0101201B2A522800000000000000000028281A2AB4010101010101010101010101B42A1A282800000000000000000000001717172A3F210C0101010101010C213F2A171717000000000000000000000000000000000A3030210C0C0C0C2130300A0000000000000000000000000000000000000000003F1B2913181813291B3F00000000000000000000000000000000000000000000172A1C191515191C2A1700000000000000000000000000000000000000000000000A161F14141F160A0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000172F0000000000000A0A28000101010100280A0A0000000000002F170000003F1F0C000000000A1FDA335BDA28282828DA5B33DA1F0A000000000C1F3F0000394C40281A3F1F14A6B1A1A1B140A6A640B1A1A1B1A6141F3F1A28404C3900005E36A392ACB4B11D08080808080808080808080808081DB1B4AC92A3365E00001B880205050505050587A77F2C2C2C2C2C2C7FA78705050505050502881B00000B71AD6825028BBCE2E2E7E7242424242424E7E7E2E2BC8B022568AD710B00000000C1FCEB8E4959E0E24D4D3A3A3A3A3A3A4D4DE2E059498EEBFCC1000000000000000000F99349E551EAF0323232323232F0EA51E54993F90000000000000000000000000101EC7259F2512E2E2E2E2E2E51F25972EC010100000000000000000000000000010100F9E16B3535353535356BE1F9000101000000000000000000000000000000000A2B010101010101010101012B0A00000000000000000000000000000000000000412B00010101010101002B410000000000000000000000000000000000000000001BD40C0C0C0C0C0CD41B000000000000000000000000000000000000000000171B98131809091813981B170000000000000000000000000000000000000000000A1C1915090915191C0A0000000000000000000000000000000000000000000000161F143131141F160000000000000000000000000000000000000000000000000017283F3F28170000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
                            "FDF3FFA901E20000FFD901E7A8E64CFFFED3FFB102890000010A01E41C0074FFFDDCFF29031F000000760127E13C63FFFDDCFF29031F0000004E00F5E13C63FFFF1700C801A9000000F002260E7221FFFDF3FFA901E20000FFF401D9A8E64CFFFF1700C801A90000012201E00E7221FFFDDCFF29031F000000A600DDE13C63FFFED3FFB102890000007601851C0074FFFDF3FFA901E20000FF8D0222A8E64CFFFF2FFCA9010A000000D500ABADBE36FF0048FC7C01D80000FFBC0081DFC864FFFFC0FF7402070000FFA401F3D74756FFFFF2FF9102BB00000018018CA7D744FF000B009102180000011A01EED87006FF00AFFFE902980000013B01BB5B1B48FF003FFF83039D0000009D00F0EE1E72FFFFF2FF9102BB0000FFCA0200A7D744FF003FFF83039D000000760104EE1E72FFFFF2FF9102BB000003020035A7D744FFFFC0FF74020700000283003AD74756FF0027FE8F0258000002B800AF08ED76FF000B009102180000016C0181D87006FF003FFF83039D0000002300F1EE1E72FF00AFFFE902980000005101825B1B48FF018FFF2E0230000001A801962C2768FF02C4FD3801B80000014E00A63CFD67FF0244FF5D01680000005102196237D8FF02E9FC2A00870000047E01FD37970CFF0365FC710084000004A001F173FEE0FF0316FC6E01400000040C01E94CC143FF0316FC6E01400000FFC8001B4CC143FFFDBA00D6FF13000000C601110276EFFFFF4800600001000000D302012A7000FFFFDBFFB0FF9A0000001701F10673DFFFFF4800600001000000E202C72A7000FFFDBA00D6FF130000011300760276EFFFFD73FF8800010000FF2800998CE300FFFFDBFFB0FF9A0000012D038F0673DFFFFF48006000010000007D022A2A7000FFFFDBFFB000670000FFEB039BDE6D21FFFDBA00D600EF000001130076027611FFFFDBFFB000670000001701F1DE6D21FFFDBA00D600EF000000C60111027611FFFDF3FFA901E20000FFDC0075A8E64CFFFF1700C801A90000019B008F0E7221FFFFDBFFB0006700000125025FDE6D21FFFDF3FFA901E2000001AC003BA8E64CFFFFDBFFB00067000000F902ACDE6D21FFFDBA00D600EF0000FFFD00B4027611FF009BFF9A00A70000000901F11B6DD7FFFFDBFFB000670000010A01E5DE6D21FFFF1700C801A9000000C900F40E7221FFFFC0FF740207000001610087D74756FF009BFF9A00A7000000C702471B6DD7FFFF1700C801A900000014009D0E7221FF009BFF9A00A70000009802841B6DD7FFFFC0FF7402070000FFB900A9D74756FF000B00910218000000EF00A6D87006FF009BFF9A00A70000011C02351B6DD7FF000B009102180000008D0080D87006FF0117FF87010F0000005901A4485DF1FF02AAFD2200FD000000580086083395FF0244FF5D01680000016E016F6237D8FF414B4606464B4B4606464B4B4606464BF3574ECA4E57574ECA4E57574ECA4E57774144BE44414144BE44414144BE4441564C44D7444C4C44D7444C4C44D7444C00005C9D5C00005C9D5C00005C9D5C000000585358000058535800005853580000563C533C56563C533C56563C533C5600563953395656395339565639533956005452D352545452D352545452D35254003A59A2593A3A59A2593A3A59A2593A003D49C4493D3D49C4493D3D49C4493D005B42A8425B5B42A8425B5B42A8425B005D3B063B5D5D3B063B5D5D3B063B5D27270606062727060606272706060627313106060631310606063131060606310606060606060606060606060606060603C000F400220000052D0129702800FF035D01920046000004BC0081416400FF0361FEF800320000071E009652A900FF03E4FFD500150000065E016A75E800FF00E7FF2BFFA7000000EF01E1F28FDCFF00EAFFEFFEFB0000021E01DDEEF08BFF01E4FFEAFEF200000218028CFCEE8AFF00E900BBFF570000030301DFF25DB7FF00E100DB005D0000042601E3F4643FFF01C200F40040000003F50280016047FF00D1FF9B0082000006D401D9FAD36EFF01B7FF9D00740000071B027A01B55DFF00E7FF2BFFA7000008EF01E1F28FDCFF01CDFF1DFFAA000008EC0283FA8AF0FF01CDFF1DFFAA000000EC0283FA8AF0FF01D100BFFF53000002FB0282FE5CB5FF005B00D0FFD000000383017FE974F0FF005900AD00560000042E0183F13E65FF014CFF6FFE15000005F0FFCB04FC89FF01FBFEAEFE48000004DEFFA1F5AEAAFF02B2FF46FF03000000D404415022AEFF0309FF3CFFBD00000326040C601EBFFF0308FEAB000100000400029376F100FF01DEFEDD015700000848036E191A72FF02C2FE04010C0000075700F562FD44FF02B2FF4600FE0000072B0441502252FF031EFDED000100000400009D77F900FF0309FF3C0045000004D9040C601E41FF02C3FCF100780000057DFE296E1D25FF02AAFD2200FD00000728FEB054F253FF02C2FE04FEF5000000A800F562FDBCFF02C3FCF1FF8900000282FE296E1DDBFF02AAFD22FF04000000D7FEB054F2ADFF01DEFEDDFEAA0000FFB8036E191A8EFF02AEFE9C004300000055080B258E00FF02AEFE9C004300000751FF6F258E00FF000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001F00000000000000000000000000000000000000000000000000000000000000FB250000000000000000000000000000000000000000000000000000000000001FED16000000000000000000000000000000000000000000000000000000000000162416000000000000000000000000000000000000000000000000000000000000FBCEFB250000000000000000000000000000000000141E1E1E25000000000000000F24EBFB00000000000000000000000000000DE1D0010101E3CFDF0C000014000000F4CD1A0000000000000000000000140C0101BA1A1E001E0C202000000F00000000FAD61E000000000000000000000DA601D516000000000025FF1E00FD0F0000001E011A000000000000000000001A01BA0F00000000000000251E000FDA2A0016C2010F00000000000000000000F701D90A00002B1DE02B25000000000C010101C50C0A000000000000000000002B01C502002BD31C1CDE0D0000000000F501C10F00001F0000000000000000001E01290C00D3010C001F1A0000000000160116000000F91F0000000000000000001ECD29EFF1010A00000000000000001429240000EF2616000000000000000000001EF12901010C0000000000000000142B01D7D82616250000000000000000000000140CF3011D1E00000000000000001E2B01C21614000000000000000000000000000025EC011D1E00000000000000002A010000000000000000000000020214021E0F0CCA0101F60C1E0A02020D0D0F0C01FF1A0F0D0202020202020201010101010101010101010101010101010101010101010101010101010101010202020202020202020202020202020202020202020202020202020202020202000000000000000000000000000000000000000000000000000000000000000001010101010101010101010101010101010101010101010101010101010101010202020202020202020202020202020202020202020202020202020202020202000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404043E1717040404040422170404040404043E0B3E1717223E1818182204040404170B310B18180B0B310B0B3E22223E3E0B073205233132070707310B18188E3107000905050509000000090505050914000600090506000606060009050600060605060005000605050506000500060505180500000005232218050000000523220506000500060505050600050006050506000905090006060600090509000606007105050509000000090505050900006B05050705053232070505070505053207070707070707070707070707070707FF7D01E1FF0D0000029C03FDD10393FF008B01E3FF2A000001A503FF45019EFF001B009DFEEA00000213031713FD8AFF00A601B700C50000071E03EF3BDD61FF0067FFE7007A0000071802A533EC6AFF00F0006DFFBE000008D502FE74EFE9FF00F301DBFFB9000008ED03FF6DFFCFFF011C01BE006F000007F103F16DE023FFFEF0FFED0044000003F802A79BEE3CFFFF8301BB00B30000048B03F1CDFA6CFFFF0F01BFFF9B0000033A03E98B07E8FFFF3D0078FF4F000002F50300ACFCABFF00F0006DFFBE000000D502FE74EFE9FF00F301DBFFB9000000ED03FF6DFFCFFF01C801CFFF4A00000C3A08190076EBFF01C801CFFF4A0000045BFDE60076EBFF032E00A1FFE60000084AF8BFFE7700FF0308FFA6FFE900000368FAC90B8BE9FF03260036FF6E00000636F7B7010789FF03290035005F0000065DFB4705F577FF0094FF65011F000002F607CD02D871FF0088009A0127000004A20874F7376AFF0094FF65011F000002F607CD02D871FF0102FE63FFFA000000E80818179125FF0094FF65011F000002F607CD02D871FF0102FE63FFFA000000E80818179125FF0088009A0127000004A20874F7376AFF0102FE63FFFA000000E80818179125FF01010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101013701010101010101010101010137A6597B27FE010101010101010137FE8D7C59E91A3201010101010101FE8D7C59E91A1A1A48370101010101FEBB7CE91A1A1A1A2A48A7A7FE010101A77B1A1A1A1A1A48FEBBFE4832010101FE7BE91A1A1A48FE74FE1A1A4801010137BB7CE91A48FE74E91A1A2A0101012A2AFE7C7C48FE9F7C1A1A48010101019E9E9E9E8B7C7B7CE91A480101010101E2E2E2E2CB9EDBC5E92A320101010101261FE9E91DE29E8B7CFE37010101010126261F1F1FE9499E8B7CFE01010101010101A6A62A7626E9498B74A601010101010101010101A62626E9DB2701010101050505A6010101011A769E7CA601010105050505CD2A01012A76E9C5FE0101010505050505DF1A01A676268BFE0101010505050505050501A676E98BA601010105050505050505011A269EDB32010101050505050505051A76E99EE9320101010505050505051A1A769E8B2A370101FE0505050505CD011A76DB7C320101016505050505D01F1A1AE9C5FE370101016505050505D0011A1AC57C32010101FE6505050505CD2A1A1AC55937010101276505050505031A1AE97CFE01010101396505050505011A1A5974A6010101016565D0D00505011A1A7CBB01010101FE6565",
                            "CDD005D0011AE99FBB010101015265A7CDCD05DF011AFEBB270101010165A7BBA1CD051F011A59BBFE010101FE65A774DFA10526012A52A7A6010101FEA7BB7403DF05260132BBA70101013727A7BB7C0303051F010165A701010132597B9F7C030305030101A73901010148597C7CC5030305DF0101BBBB0101011AE9DBDBDB0303D0D00148749F3201011A768B8BDB0303CD053748C574A601017626E29E8B0303A10537485974FE01017626E99E8B0303A1053748FE74270101762626CB9E0303A1053748FE7C7B3201762626499E0303DF0505321A7C74FE321A2626E9CB0303DF0505322AE97C7B322A762626490303DFD00537481AC5C5FE481A7626260303DFCD0505481AE98BDB2A481A7676030303A1D005321A76E99EE92A481A1A030303A1A105051A2626E99EE92A3232030303A1A1CD0548762626E9CBE91A32030303DFA1CD05051A262626E9CB49E903030303A1CDCD05051A26261FE9CB9E03030303A1A1CDCD05051A261F1FEDCB0303030303DFCDCDCD050576261F26E20303030303DFA1CDCDD00505762626E9030303030303DFDFA1CDCD050576262603030303030303DFDFA1CDCD05051A760303030303030303DFDFCDCDCDD0054803030303030303030303DFDFA1CDD0050303030303030303030303030303DFCD0303030303030303030303030303030303030303030303030303030303030303DA3800030D000440D7000002FFFFFFFFD9000000002304050100500A060083500100200E06006AD0DA3800030D0001C0E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01CFA000080FFFFFFFF0100F02C06008E90060E0010000002120614060000160018060C081A001C1E0C060C202200240A0C060A2628002A040AE700000000000000FA0000FFFFFFFFFFE300100100008000FD1000000600D1B0E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000008000000F550000007094260E600000000000000F3000000073FF100E700000000000000F548100000094260F2000000000FC07C0100E01C06005640060002040006080A060A0804000C040806040E0A0008100C06121008000806120614040C00041400060C16140014161806181A140000141AE700000000000000E300100100008000FD1000000600E4D8E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000009000000F550000007094250E600000000000000F3000000071FF200E700000000000000F548080000094250F20000000007C07C0100700E0600823006000204000600040606040800080A0605080C0A00000000E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD500000060090B0F5500000070D0030E600000000000000F30000000703F800E700000000000000F5480200000D0030F20000000001C03C01020040060094F0060002040006080A060C0E100012141606181A1C00161E1206200806000A220606242628002A2C2E0612303200121E3006343638003A3C3E0102004006005000060002040006080A060C0E100012141606181A1C001E2022061C1A240026282A062C2E300032343605383A3C00000000010200400600CCE0060002040002000606080A0C000E0206060C10120014161806041A1C001E20220624262800262A28062A262C002E2A2C0630323400363038053A3C3E000000000102004006006B00060002040006080A060C0E100012141606181A1C001E20220624262000282A2C06282E2A00302A2E06302E3200343638053A3C3E00000000010200400600DA80060002040006080A060C0E10000A1214060C160E00180E16061A1C1E000420000622242600282A2C062E3032003436380532303A0000000001020040060096F0060002040006080A060C0E1000121416060A1806001A1C1E06202202002420020626282A002C2E30063234360032383A0502223C000000000100F01E06007C70060002040006080A060C0E100012141605181A1C00000000E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01C0101D03A0600BDB006000204000600080600040A0004020C06020006000E00100612100A0008000E060A100000141618061A1012000C021C0616141E002022240624262000282A2C062C2E30003234360538363400000000E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600A230F5500000070D0030E600000000000000F30000000703F800E700000000000000F5480200000D0030F20000000001C03C010200400600AC30060002040004060006080A0C000C0E08060C100E0002120406141202000E101606121418001A1610061C1E200022242606282A2C002E303206343638003A3C3E0102004006005200060002040006080A060C0E10000A12060614021600181A1C061E202200242628062A2C2E003032340536383A000000000101502A06007EA006000204000006020608020600060A08060C0E100010120C061210140016181A061A1C16001C1A1E06201C1E002224260626282200142228E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006006900F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03C010040080600B7380600020400040600DF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000F3F300000000000000000000000077162C2C1677000000000000000000774C818181814C7700000000000056778881817272818188775600000056778872104010104010728877560077774C8179143C10103C1479814C7777F3162CF4F01D141010141DF0F42C16F31641107C1D1D147272141D1D7C10411616E1799E1D8D582C2C588D1D9E79E11616162929292916F3F316882929291616008B01E3FF2A000002010029C9335DFFFF7D01E1FF0D000000C2003215326AFF008D017EFF620000024A00A8C82D5FFFFF7C015CFF450000008400DE372065FFFF0F01BFFF9B0000001F00456B1B2EFFFF0F01BFFF9B0000061F00456B1B2EFFFF8301BB00B30000051800124248BBFFFF4700D2FF850000061B01914D51D8FFFF4700D2FF850000001B01914D51D8FF00A601B700C5000004170014F458B1FF00B700D4FFD70000035E017CA84FEEFF011C01BE006F0000039A001B9C3CE7FF00F301DBFFB9000002F000179D203AFF03D9FF0100740000006D00D968D326FF03D9FF01FF8C0000006D00D964C7E1FF19B800000000000000000000000000160117FF87010F0000FFC901EDB738B5FF018FFF2E02300000011501FE2C2768FF01C000A6013F0000007D01142948ABFF00AFFFE90298000000DB01775B1B48FF018FFF2E02300000002F01DF2C2768FF0117FF87010F0000015D01EB485DF1FF0117FF87FEF20000015D01EB485D0FFF018FFF2EFDD20000002F01DF2C2798FF00AFFFE9FD69000000DB01775B1BB8FF01C000A6FEC20000007D0114294855FF018FFF2EFDD20000011501FE2C2798FF0117FF87FEF20000FFC901EDB7384BFF00AFFFE9FD690000005101825B1BB8FF000B0091FDEA0000016C0181D870FAFF0117FF87FEF20000002E023A485D0FFFFF7CFBAC00010000021E0020C09B00FF00A5FBC7FEC300000076001BEB9EBFFF00EAFB680001000001D9FFCA028900FFFF2FFCA9FEF8000000D500ABADBECAFF0048FC7CFE290000FFBC0081DFC89CFF0027FE8FFDA90000FFCE017908ED8AFF0048FC7CFE29000000F40084DFC89CFFFED3FFB1FD780000002702251C008CFF0209FC7EFE480000FF9A004918BCA1FF00A5FBC7FEC300000122001AEB9EBFFF01E3FBB1FF030000003EFFE22196D5FF00EAFB68000100000000020A028900FF00A5FBC7FEC3000001B201DEEB9EBFFF01E3FBB1FF0300000335020C2196D5FF01E3FBB1FF030000019DFFF62196D5FF0209FC7EFE4800000102005218BCA1FF02E9FC2AFF7B00000012FFF83797F4FF000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000F000000000000000000000000000000000000000000000000000000000000001925002800282828282828000000000000000000000000000000000000000000170326474A3A090909090909090909091F28000000000000000000000000000417110303030303152A1A11080A1D193A0B1F1F2800000000000000000000000417081A03030303030303030303030C2A2608191F280000000000000000000004172D2D0A0837111A2A2A150303030303030303081238000000000000000000044239171750190D0D0D0D0D0D123422261A150303110A090F00000000000000041941410D0D0D0D0D0D0D0D0D0D0D0D0D4134242A03030A090F0000000000000005490D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D34210325090F000000000000490D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0A030909000000000000490D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0B0909000000000004490D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D0D092E00000000002E490D0D0D0D0D0D0D0D0D0D0D050D0D0D0D0D0D0D0D0D0D0D510900000000001B490D0D0D0D0D0D0D0D0D05050505050D0D0D0D0D0D0D0D0D0D0900000000001B0D0D0D0D0D0D0D0D0D0D0507070505050D0D0D0D0D0D0D0D0D0900000000001B0D0D0D0D0D0D0D0D0D05070E0E0E0705050D0D0D0D0D0D0D0D0900000000002E053B0D0D0D0D0D0D0D050E0E0E0E0E07050D0D0D0D0D0D01510900000000000F4B0D0D0D0D0D0D0D0D070E0E0E0E0E07050D0D0D0D0D0D120D4300000000000032460D0D0D0D0D0D0D070E0E0E0E0E07050D0D0D0D0D0D340D4F0000000000002E12124949490D014605070707070705050D0D0D0D0D0D290D4F00000000000004420A34341201050505050505050505050D0D0D0D3429080509000000000000001E08212408223412121212121212121212340A29211525320F000000",
                            "000000002E3C08111A1A112624080822222D082924212A150C08350F0000000000000000000F4A0A37111A2A2A1515151515151515150C110A350000000000000000000000000F45470A08111A2A2A2A2A2A1A1A1148254A0F04000000000000000000000000000000354A3944441D1D1D1D1D1D39450F0F0000000000000000000000000000000000000000000F0F0F040404040000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000143B22223B140000000000000000000000000000000000000000000509091C14050314140305141C09090500000000000000000000000000050809380D3B3B3B0914080814093B3B3B0D3809080500000000000000000524383B3B3B3B3B3B3B3809090909383B3B3B3B3B3B3B3824050000000005040B1A1A3D331313131335392F30302F393513131313333D1A1A0B04050000030D31130C1D1D1D400C0C3F2041212141203F0C0C401D1D1D0C13310D0300000026333D421A3B3B3B3B3B37293336363329373B3B3B3B3B1A423D332600000000020E0B080505050503081044111111114410080305050505080B0E020000000000050408442D240814030505050505050505031408242D440804050000000000000000000E2311440B081405050505050514080B4411230E00000000000000000000000000050E231717421A3B2D2D3B1A421717230E05000000000000000000000000000002040427450B0130303030010B4527040402000000000000000000000000000005020202040200001C1C0000020402020205000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000044083A3A08440000000000000000000000000000000000000000003107070C44330244440233440C070731000000000000000000073144440C070D01010D070707443131440707070D01010D070C4444310700001B04040801081C0F040801010D070707070D010108040F1C08010804041B0000152D3628283C3C1D1F1D3E3921183D3D1821393E1D1F1D3C3C2828362D150000021A341D3636363636361F36224324244322361F3636363636361D341A02000000070D0F182D2D2D2D3815352E342C2C342E3515382D2D2D2D180F0D070000000000333F2D080D0D0D0404040404040404040404040D0D0D082D3F330000000000000005452004453131443333333333333333443131450420450500000000000000000000054215452F314433333333333344312F451542050000000000000000000000000033194229203F0408070708043F2029421933000000000000000000000000000011050519372F092B2B2B2B092F3719050511000000000000000000000000000033111111051100000C0C0000110511111133000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005023737020500000000000000000000000000000000000000000000090D20052C050505052C05200D0900000000000000000000000000000000000C0C0202020D050909050D0202020C0C000000000000000000000000000000340C0C0C0C34340D0D0D0D34340C0C0C0C34000000000000000000000000001B02010B0B072626252525252626070B0B01021B00000000000000000000000002060B1219152933163A3A1633291519120B060200000000000000000000002D121110153A233204363636360432233A151011122D000000000000000000001207101516393232323232323232323239161510071200000000000000001F170E0E070B0207100E1918181818190E1007020B070E0E171F000000000000001C02020805050809141E0B0B0B0B1E14090805050802021C00000000000000002C1417140D0905052C2C2C2C2C2C2C2C0505090D1417142C0000000000000000002F172D1E1E0C0C0C0C0C2C2C0C0C0C0C0C1E1E2D172F000000000000000000000000001C2D0606120B020D0D020B1206062D1C00000000000000000000000000000000000000000000000000000000000000000000000000000000B44BB44BB44BB44BB44BD555FFA7CD11BC8DFF63DD95B44BB44BB44BB44BB44BB44BB44BBC8FC4CFBC8DF76BFFF5FF23D553FFF3FFF5F65BB44BB44BB44BB44BB44BD555FFF3FFEFC4CFFFE7FFA9FEDDDD95FFA3FFA7F619B44BB44BB44BB44BBC8DF65DFFF3FFF1CCD1FFA1FF21FE9BDD95FF1FFF21EE5BBC8DC4D1C48FB44BC4CFF619FF65FF63BC8DFF61FF23F6DDDD53FF21FF23F6DFCD13FFEBFF69B44BC4CFEE19FF21FF1FC4CFF6DFF721F69DD553FF1FFF21F6E1D553FFEBFFADBC8DC4CFEE19F6DFFEDDCD11F6DFF6DFF6DFD553F69DF69FF6DFD553FF61FFA7BC8FCCD1EE59EE9BF69BCD53F6DFFF23FF23D553FF21FF23F6E1D555FF61FF65C4CFD511F65BFF21FEDDD593FF23FF23FF63D553FF21FF23F6E1D595F69DF723CCD1CD11F69DFF21FF1FDDD5FF23FF23FF63DD95FF23FF23FF23DDD7FF21FF63CD11CCCFF65DFEE1FF1FEE59FF23FEE3FF63E5D7FF23FEE3FF23E5D9FF23FF23D553BC8DEE19F6DFFEE1F69DFF23F6E1FF23F69FF6E1F6E1FF23EE5BFF23FF23D593BC8DEE19F69DF69FFEE1F6E1EE9DF6E1FF23F69FEE9DFF23F69FEE5DFF21DD95C48DF69BFEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3F6E1FEE3E5D7C4CFF69FFEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3E5D7CD11F69FFEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3FEE3EE170308FF6E002F0000004E00E3079534FF02A70053005B000000A100B4142A6EFF0304FF6DFFAD0000004E00E20BA3B7FF02A10052FF6C000000A100B21B2D95FF02A200A3FFF4000000BD00B1187506FF005CFF5D01E90000059302DDFAD26EFF005CFF5D01E90000059302DDFAD26EFF006500A701E10000039D01FB00316DFF005C019E00E4000015740173097029FF005C019E0000000013FF0173087700FF005C019E0000000013FF0173087700FF005C019EFF1C0000128B01730970D7FF00EFFE4FFF2D00000B930253FB8FD9FF00EFFE4F00D30000086C0253FD8E22FF005C019E00E4000015740173097029FF00EFFE4F00D30000086C0253FD8E22FF006500A701E10000039D01FB00316DFF006500A701E10000039D01FB00316DFF005C019E00E4000001740173097029FF005CFF5D01E90000059302DDFAD26EFF00EFFE4F00D30000086C0253FD8E22FF005CFF5D01E90000059302DDFAD26EFF005CFF5DFE1700000E6C02DDFAD292FF00EFFE4FFF2D00000B930253FB8FD9FF007600BD01E500000554049EFA346BFF00B1019400CF000007C105A1F2673BFF005BFF6501DD00000355039301E073FF00B3FE5D00030000140003B3018900FF00BB01CA000300000A0005D8F17600FF00AF0194FF3700000C3E05A1F167C5FF007100BDFE2100000EAB049EF93495FF0056FF65FE2A000010AA039300E08DFF00A9FE7AFF26000012B903B1079BC1FF00ABFE7A00E10000154603B1089B3FFF00ABFE7A00E10000014603B1089B3FFF01F70091FE4400000747FFA31C53AFFF0306FE9AFE620000037FFF8536A3CCFF0349FF87FE57000001EFFF896C17D2FF02FD0046FE7000000083FF7B4558D8FF02FD0046FE7000000883FF7B4558D8FF005C0209FFB00000003900701758B3FF008D017EFF620000000301A85939C8FFFF7C015CFF450000020F0192D23CA3FFFF4700D2FF850000026B0216B4F65BFF00B700D4FFD70000FFBB023B4B055DFF006A0222000100000010FF70454346FF006A022200010000000C0004454346FF006A022200010000000B003A454346FF00B700D4FFD70000FFD402064B055DFF008D017EFF620000000F017D5939C8FFFF6801FBFFC7000002010015B65AE7FFFF6801FBFFC7000001F9FF9DB65AE7FFFF7C015CFF45000001EF019ED23CA3FFFF4700D2FF85000002520225B4F65BFFFF6801FBFFC7000001FA003EB65AE7FFFDF3FFA9FE1F000000E9022CA8E6B4FFFED3FFB1FD780000002702251C008CFF0048FC7CFE29000000F40084DFC89CFFFD73FF880001000001C401F78CE300FFFDF3FFA9FE1F0000FF8D0222A8E6B4FFFF2FFCA9FEF8000000D500ABADBECAFFFDF3FFA9FE1F0000FF0901E6A8E6B4FFFBF4FFD5FED40000005801368F1AE5FFFDBA00D6FF130000011101E90276EFFF00AFFFE9FD690000007701FA5B1BB8FF018FFF2EFDD20000FFE101862C2798FF0027FE8FFDA9000001ED017B08ED8AFF0316FC6EFEC10000010600264CC1BDFF02C4FD38FE4A0000012C00D33CFD99FF033FFDE1FEE90000FFC9011E712508FF0086FF6300470000004CFFDD0C9635FF0084FF62FF9D0000004BFFDD0592D3FF0113FF6100570000004C0036009F46FF0111FF60FF8B0000004C003501A8B0FF005C004F00A500000096FFA9F21A74FF0072004DFF0A00000095FFBAE8138DFF012B0066FF210000009E002B0A449EFF0133006D0090000000A1002D074164FF00B600E3FFF2000000C5FFD4F27705FF014C00D4FFF0000000C100330B7704FF0231FF6E004700000055007D0AC164FF0308FF6E002F0000004E00E3079534FF02A70053005B000000A100B4142A6EFF0304FF6DFFAD0000004E00E20BA3B7FF020AFF63FF9200000052006C0FA0BBFF02A10052FF6C000000A100B21B2D95FF023600620069000000AA007E12296EFF021B00BF0002000000CB00721B740BFF02A200A3FFF4000000BD00B1187506FF020B0068FF3E000000AD006C262D98FF02AAFD2200FD000000580086083395FF0244FF5D01680000016E016F6237D8FF033FFDE1011800000189008C7125F8FF01DEFEDD01570000009E016B13048AFF01C000A6013F000001A1021C2948ABFF0117FF87010F0000001501FDB738B5FF02AAFD2200FD0000FFB00147083395FF033FFDE101180000FFDE01817125F8FF0365FC7100840000FE80020573FEE0FF02C3FCF100780000FF2D01076E1D25FF0302FD1700010000FED200C56E2E00FF01C000A6FEC2000001A1021C294855FF0117FF87FEF20000001501FDB7384BFF01DEFEDDFEAA0000009E016B130476FF0244FF5DFE990000016E016F623728FF02AAFD22FF0400000058008608336BFF033FFDE1FEE900000189008C712508FF0365FC71FF7D0000FE80020573FE20FF033FFDE1FEE90000FFDE0181712508FF02AAFD22FF040000FFB0014708336BFF02C3FCF1FF890000FF2D01076E1DDBFF005CFF5DFE1700000E6C02DDFAD292FF",
                            "005C019EFF1C0000128B01730970D7FF006500A7FE1F0000106201FB003193FF006500A7FE1F0000106201FB003193FF00EFFE4FFF2D00000B930253FB8FD9FF005C019EFF1C0000128B01730970D7FF006500A7FE1F0000106201FB003193FF005CFF5DFE1700000E6C02DDFAD292FF005CFF5DFE1700000E6C02DDFAD292FF0163FF35007300000054007402C065FF0161FF37FF610000005500740B93D0FF02A1FF4100550000005BFFDE08932FFF029DFF40FF940000005BFFE00FB9A1FF01240026006E000000930097F62A6FFF02A70032008F0000009BFFE1002C6FFF00EB00A5FFF8000000B400B303770BFF0135003CFF260000009A008D12178CFF02A10030FF580000009BFFE20C4AA3FF02A10090FFF3000000B4FFE5057700FF02A4004FFF9D000000A100B4132A92FF0306FF6AFFC80000004E00E30795CCFF0304FF6A004A0000004E00E20CA349FF02A1004E008D000000A100B21C2E6BFF02A0009F0004000000BD00B11875FAFF018FFE6EFF590000131F05930C8BECFF02ADFE9CFFBE000013AA080B258E00FF02AEFE9C004300001455080B258E00FF035D0192004600001AF30B4A416400FF03C000F40022000014D20BBD702800FF03C000F4FFDD0000132D0BBD702800FF0361FEF8FFCE000013BB09C052A900FF03E4FFD500150000142B0B5B75E800FF0361FEF800320000144409C052A900FF035D01920046000006F30B4A416400FF035C0192FFBA00000D0C0B4A416400FF01C801CFFF4A00000C3A08190076EBFF0309FF3CFFBD00000107FF47601EBFFF02B2FF46FF030000FE61FFEF5022AEFF027C001AFFB0000000D8038F3B61DCFF0306FF6900010000020000005A4E00FF027C001A005100000326038F3B6124FF0309FF3C0045000002F8FF47601E41FF02B2FF4600FE0000059FFFEF502252FF007B013AFF10000002590433F6409CFF0199013AFF15000000F504361663C1FF019800AFFF24000000FE02AD30CE9FFF013F013B01660000012A040F076145FF0296013BFFDA0000FFA9042B642ACFFF026E013B00E40000FFC1041A58F750FF014600B001450000012E028A16C867FF02410093005600000011024C459F09FF0027013A012B0000028C040ED93B60FFFF270139006B000003DB0416B25420FFFF510139FF56000003C304289918C8FF033F005E00000000F4A404FF644200FF0398FFA400B90000F5A9048C6C2A1AFF0398FFA400B90000FFEB00DE6C2A1AFF0398FFA4FF470000F3D1048C6C2AE6FF0398FFA4FF470000FFEB00DE6C2AE6FF00F000740041000000D502FE74EF17FF001B00A4011500000213031713FD76FFFF3D007F00B0000002F50300ACFC55FF001B00A4011500000213031713FD76FF0067FFEDFF850000071802A533EC96FF00F000740041000008D502FE74EF17FF00F000740041000000D502FE74EF17FFFF3D007F00B0000002F50300ACFC55FFFEF0FFF3FFBB000003F802A79BEEC4FFFF3D007F00B0000002F50300ACFC55FFFEF0FFF3FFBB000003F802A79BEEC4FF0067FFEDFF850000071802A533EC96FFFEF0FFF3FFBB000003F802A79BEEC4FF0067FFEDFF850000071802A533EC96FFFEF0FFF3FFBB000003F802A79BEEC4FFFF7B016300BA0000020F0192D23C5DFF008D0185009D0000000301A8593938FF005C0210005000000039007017584DFF006A0228FFFE00000010FF704543BAFF00B700DB00280000FFBB023B4B05A3FFFF4700D8007B0000026B0216B4F6A5FF006A0228FFFE0000000C00044543BAFF008D0185009D0000000F017D593938FF00B700DB00280000FFD402064B05A3FF006A0228FFFE0000000B003A4543BAFFFF6802020038000002010015B65A19FFFF6802020038000001F9FF9DB65A19FFFF6802020038000001FA003EB65A19FFFF4700D8007B000002520225B4F6A5FFFF7B016300BA000001EF019ED23C5DFF0113FF5EFF9E0000004C0036009FBAFF0085FF5F005A0000004BFFDD06922DFF0086FF60FFAF0000004CFFDD0C96CBFF0112FF5D006A0000004C003503A850FF005A004CFF5200000096FFA9F11A8CFF012D006300D30000009E002B0A4462FF0074004900EC00000095FFBAE91374FF0131006AFF65000000A1002D06419CFF00B500E00004000000C5FFD4F277FBFF014B00D10004000000C100330B77FBFF012600A8016D000000B90015C22D5BFF011A015600FD00000080000AA84030FF01C2FEE0002B000000150093EE8A00FF013EFF0100A000000050002CBD9E0CFF016401BF0032000000180043D77005FF01F501C6002B0000001500B4F67700FF0134FF5A014E000000A90022A7C333FF0150000401CC000000EA0036B9FD60FF036D01FA00000000000001D8F07600FF02F5FEEA0000000000000181038900FFDA3800030D000280D7000002FFFFFFFFD9000000002304050100600C06009490DA3800030D0002C0E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005740F5500000070D4050E600000000000000F3000000071FF200E700000000000000F5480800000D4050F20000000007C07CFA000080FFFFFFFF0101102E0600A7F006060C0E0006081006000412001404060602001600181A06061C1E0A00200A020606222400260028052A022C000000000100E01C0600AF40060002040006020006080A06000C0A08060E0C100010120E0604140000001606060616080008100C06060A1800180A1A051A0A0C00000000DE00000006001D08DF00000000000000DF00000000000000021C018D001F000000720226F9653FFF00560156003D0000008100C7F26C32FF006B00D500DB000000D200D8ED4A5BFF020B010200C3000000C6021A014263FF0097FFF901520000010F00FAF3F776FF004DFF030088000000A700C2F7A147FF0207FF1800CA000000CA0219FFB25BFF0208FFFF0131000000FE02180AFF77FF0217FEA30003000000630226FA9029FF02A0FF3DFFA40000005BFFDE0893D1FF0163FF32009B0000005500740C9330FF0161FF30FF8800000054007402C09BFF029EFF3C00640000005BFFE011BA5FFF02A4002EFF6A0000009BFFE1FF2C91FF01210021FF8E000000930097F52A91FF00E9009F0005000000B400B30277F5FF0136003700D60000009A008D131774FF02A1002D00A00000009BFFE20C4A5DFF029F008C0005000000B4FFE5057700FFFFE00040FF2D000002B502A0F3DD8FFFFFD5FFACFEF80000025E00C1FD8A11FFFF01FFB9FFDC0000038900338AEC00FFFEDA014200710000FECC044CA4F54BFFFEC7013AFF590000FEBF043AA121C0FFFF320028FFD70000009900D08CE603FF02B70126FE8B000002E803F4294BADFF02BA01400104000002D104341B594BFF042100A200320000045B04004EC140FF009BFFFDFFDB00000184026370D908FF00F1FF7FFFDC000000B6011A76EF01FFFFCA0138FEAE0000FFCC0435ED3A9AFFFFE00040FF2D0000012B011DF3DD8FFFFF320028FFD70000039301BD8CE603FFFFCD014F00E60000FFB7046FEF4262FF01D700B3FF0D0000029A028603A3B5FF0260007BFFB50000037401CE0F8AFDFF009BFFFDFFDB00000246004270D908FF040E0097FF260000045203E6199AC8FF020F00A0008A000002F2023D07A34AFFFFE1004300780000012B0120F6E073FFFFE100430078000002B402A4F6E073FFFFD2FFB200A90000026300C6FC8AEBFF00AF0194FF3700000C3E05A1F167C5FF01C801CFFF4A00000C3A08190076EBFF012000C8FE1B00000EEB0617F83395FF00BB01CA000300000A0005D8F17600FF00B3FE5D00030000140003B3018900FF018FFE6EFF590000131F05930C8BECFF0191FE6E00AB000014E005930A8C19FF02A4004FFF9D000000A100B4132A92FF0306FF6AFFC80000004E00E30795CCFF022FFF69FFB300000055007D09C19BFF020AFF5F006800000052006C10A045FF0304FF6A004A0000004E00E20CA349FF02A1004E008D000000A100B21C2E6BFF021900BAFFF8000000CB00721A74F5FF0233005DFF90000000AA007E112991FF02A0009F0004000000BD00B11875FAFF020B006300BC000000AD006C272D67FF01E0FFEA010D00000218028CFCEE76FF01BD00F5FFC0000003F502800160B9FF01B2FF9DFF8C0000071B027A01B5A3FF01CD00BF00AC000002FB0282FE5C4BFF01C9FF1E0055000008EC0283FA8A10FF01C9FF1E0055000000EC0283FA8A10FF005BFF6501DD00000355039301E073FF0116FF8201E20000033B053208C266FF012500C801EA000005140617F9336CFF00ABFE7A00E10000014603B1089B3FFF0191FE6E00AB000000E005930A8C19FF0191FE6E00AB000014E005930A8C19FF00ABFE7A00E10000154603B1089B3FFF00B3FE5D00030000140003B3018900FF007600BD01E500000554049EFA346BFF00B1019400CF000007C105A1F2673BFF012000C8FE1B00000EEB0617F83395FF007100BDFE2100000EAB049EF93495FF00AF0194FF3700000C3E05A1F167C5FF0056FF65FE2A000010AA039300E08DFF00A9FE7AFF26000012B903B1079BC1FF0111FF82FE23000010C4053207C29AFF018FFE6EFF590000131F05930C8BECFF00BB01CA000300000A0005D8F17600FF01CA01CF00BA000007C50819FB741AFF01C801CFFF4A00000C3A08190076EBFF01A30196FF8B0000040000770D73E3FF029B0145FF54000003CC02142071ECFF02AC00E8FEB7000003160233FE3293FF008700A0FED7000002EDFEAAF63797FF0091FF6BFEE00000011DFEC101D88FFF0101FE690003000007A3FF7A1791DBFF0091FF6BFEE00000091DFEC101D88FFF0299FF1FFF5000000854021B0EA5B4FF0299FF1FFF5000000054021B0EA5B4FF02A7FFE4FEC0000001E3023002D98FFF029FFECF000E000007760223208F16FF02A00120001B00000494021A2D5B3FFF0398FFA5FF450000FFFF00286627D0FF033E0061000000000030FFF9644200FF0398FFA500BB0000FFFF0028662730FF03D9FF000076000000CB010371DC0DFF03D9FF0000760000FFABFFCE71DC0DFF03D9FF00FF8A000000CB010374EDECFF03D9FF00FF8A0000FFABFFCE74EDECFF013C002200010000F4C80457683A00FF009BFF9A00A70000F5C303A80D2E6DFF009BFF9A00A70000F5C303A80D2E6DFF009BFF9AFF5A0000F41903A80D2E93FF009BFF9AFF5A0000F41903A80D2E93FF013C002200010000F4C80457683A00FF009BFF9AFF5A0000002300A20D2E93FF009BFF9AFF5A0000002300A20D2E93FFFFDBFFB0FF9A0000008E00BC0673DFFFFFDBFFB0FF9A0000008E00BC0673DFFFFFDBFFB000670000008E00BCDE6D21FFFFDBFFB000670000008E00BCDE6D21FFFFDBFFB000670000008E00BCDE6D21FF009BFF9A00A70000002300A20D2E6DFF009BFF9A00A70000002300A20D2E6DFF02AD00E40141000003160233FF326DFF029A014100A4000003CC0214207114FF01A0019100700000040000770D731DFF0094FF65011F0000011DFEC102D871FF0088009A0127000002EDFEAAF7376AFF029AFF1B00A800000854021B0FA54CFF0094FF65011F0000091DFEC102D871FF0102FE63FFFA000007A3FF7A179125FF",
                            "02A9FFE10138000001E3023003D971FF029AFF1B00A800000054021B0FA54CFF029EFECBFFEA000007760223218FEAFF029D011CFFDD00000494021A2C5BC1FF017EFEE7FEAB0000001802E1150A8BFF01B8FE4AFE930000FFD1013F301094FF0281FD9EFEF1000000E6FF7A5BF6B4FF0281FD9E011000000719FF7A5BF64CFF01B8FE4A016F0000082F013F30106CFF017EFEE70156000007E702E1150A75FF025CFF8601250000075704863C2261FF030303030303030372031B03031B03039B1B897C6690721BB378B1B889248B78DB96B2D29E0EDC8A13BDEE0EC0110EB5131A2B13FF10131A0A1E1E0A1A120A13000000002A13000002000D002A130000100A0E2A110E12E2110EAAFCC2B711D10EAC95B9AC97DDB6B0A881B6A87C9B7C9D7203877203787C0303030303030303005C019EFF1C0000128B01730970D7FF005C019E0000000013FF0173087700FF01B0017D000000001400FEB9257200FF005C019E00E4000015740173097029FF005CFF5D01E90000059302DDFAD26EFF00EFFE4F00D30000086C0253FD8E22FF01FBFEAE01B80000073FFFDFF5AE56FF01F7009101BC0000045FFEB81C5351FF006500A701E10000039D01FB00316DFF014CFF6F01EB000005E300D804FC77FF00EFFE4FFF2D00000B930253FB8FD9FF032BFE74FEF300000B85FD822891F1FF032BFE74010D0000087AFD8236970FFF0306FE9A019E000007B4FDB836A334FF01F7009101BC0000185FFEB81C5351FF005C019E00E4000001740173097029FF01FBFEAEFE4800000CC0FFDFF5AEAAFF0306FE9AFE6200000C4BFDB836A3CCFF01F70091FE4400000FA0FEB81C53AFFF006500A7FE1F0000106201FB003193FF005CFF5DFE1700000E6C02DDFAD292FF014CFF6FFE1500000E1C00D804FC89FF000000000000000000000000000000000000100BED1D1DE21DED1D1D1D1DED1D0000000000000000000000000000000088106B0B1D9E9E8B9E9EE2E2E2E29E9E00000000000000000000000000002C106B0B0B1DE2DBBB7C7C7CDB9E9E8BDBC500000000000000000000772C723C6B0B0B0B1D9E74BBA7A7A7A7A774DBDBC57C00000000007716887210403C6B0B0B0B1DE29E74BBA7A765E8F1E4BBC5DBDB7C000077292C721058F06B0B0B0B0B1D9E9EC59FBBA7A7A7A7E8E4BB7BC5DB8B7C10F458F06B1DED0B0B0B0B1DE2C5DB7C9FBBBBBBA7A7A7A7E8A774747CC5C5BB0E0E0B0B0BEDED1D1D1DE2DB7CA7A7656565A7BBBBBBBBA7E8659F9F747C7CBB0E0E9E9E9E9EDB7C9F9F6565E8E46565A7BBBBBBBBBBA7E8F1E8BB747C7C7C740E0E8B7C7B9FBBA7A7A7A7A7A7BBBBBB9F749FBBBBBBA7E8F165A7BB7C7C7C7C0E0EDBC5BBBBBBBBBB7B9F9F7C7C7C74747C749F7B7BA76565A7A7A7747C7CBB0E0E8BDBC57CC5C5DBDBDB8B8B8BDBDBDBDBDBDBDB7CBBBB9F7BA7BB7CDB7C740E0EE29E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E8B747C8BDB7C7CDB9E8B8B0E0EEDEDED1D1D1DE2E2E2E21D1D1D1D1D1D1D1D1DE29EE21D1DE2E2E21DE2E20E0E0E0E0EDCEDEDEDEDEDDCDCDCDCDCDCDCDCDCDC0B0BDCDCDCDCDCDCDCEDED0E0EDCDCDCDCDCDCDCDCDCDCDCDCDCDC0E0EDCDCDCEDEDEDEDDCDC0E0E0E0E0E024D00A1FFA200000442014D123D9BFF0200FFBEFF9A0000066D01171DBFA0FF024300C5002E000003850141FD7223FF025D000F00CF0000024C014EF7F977FF01EDFF68005B0000012901030A8F26FF01EDFF68005B0000092901030A8F26FF033FFDE101180000FFC9011E7125F8FF02C4FD3801B80000012C00D33CFD67FF0316FC6E01400000010600264CC143FF0027FE8F0258000001ED017B08ED76FF018FFF2E02300000FFE101862C2768FF00AFFFE902980000007701FA5B1B48FFFDBA00D600EF0000011101E9027611FFFBF4FFD5012E0000005801368F1A1BFFFDF3FFA901E20000FF0901E6A8E64CFFFF2FFCA9010A000000D500ABADBE36FFFDF3FFA901E20000FF8D0222A8E64CFFFD73FF880001000001C401F78CE300FF0048FC7C01D8000000F40084DFC864FFFED3FFB102890000002702251C0074FFFDF3FFA901E2000000E9022CA8E64CFFFEDFFC7B000100000216008A9AC300FF02C4FD3801B80000FFB100833CFD67FFFFF2FF9102BB0000018F01F4A7D744FFFBF4FFD5012E00000086012E8F1A1BFFFDBA00D600EF0000012001F1027611FFFD73FF8800010000FF3201F88CE300FFFBF4FFD5012E0000FFF1012B8F1A1BFFFD73FF8800010000FEFA01ED8CE300FFFDF3FFA901E20000011201F1A8E64CFFFF7CFBAC00010000021E0020C09B00FF00A5FBC7013E00000076001BEB9E41FF0027FE8F02580000051D019B08ED76FFFFC0FF7402070000052D021BD74756FFFED3FFB102890000058002711C0074FFFED3FFB102890000007601851C0074FFFFC0FF7402070000FFDB0216D74756FFFF1700C801A90000012201E00E7221FF0048FC7CFE290000FFBC0081DFC89CFFFF2FFCA9FEF8000000D500ABADBECAFFFDF3FFA9FE1F0000FF8D0222A8E6B4FFFED3FFB1FD780000007601851C008CFFFDDCFF29FCE2000000A600DDE13C9DFFFF1700C8FE590000012201E00E72DFFFFDF3FFA9FE1F0000FFF401D9A8E6B4FFFF1700C8FE59000000F002260E72DFFFFDDCFF29FCE20000004E00F5E13C9DFFFDDCFF29FCE2000000760127E13C9DFFFED3FFB1FD780000010A01E41C008CFFFDF3FFA9FE1F0000FFD901E7A8E6B4FFFFC0FF74FDFB0000FFDB0216D747AAFFFED3FFB1FD780000058002711C008CFFFFC0FF74FDFB0000052D021BD747AAFF0027FE8FFDA90000051D019B08ED8AFFFF7CFBAC00010000021E0020C09B00FFFEDFFC7B000100000216008A9AC300FF00A5FBC7FEC300000076001BEB9EBFFFFDF3FFA9FE1F0000011201F1A8E6B4FFFD73FF8800010000FEFA01ED8CE300FFFBF4FFD5FED40000FFF1012B8F1AE5FFFD73FF8800010000FF3201F88CE300FFFDBA00D6FF130000012001F10276EFFFFBF4FFD5FED400000086012E8F1AE5FF0027FE8FFDA9000001ED017B08ED8AFFFFF2FF91FD470000018F01F4A7D7BCFF00AFFFE9FD690000007701FA5B1BB8FF018FFF2EFDD20000FFE101862C2798FF02C4FD38FE4A0000FFB100833CFD99FFFD73FF880001000001C401F78CE300FFFDF3FFA9FE1F000000E9022CA8E6B4FFD7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005E00F5500000070D8140E600000000000000F3000000071FF400E700000000000000F5480400000D8140F20000000003C0FCFA000080FFFFFFFFD9000000002304050101402806008C200600020400060802060A0C0E00020006060410000012100406141618001A1614061C1A1E000E1C20061E201C00141E1A06222426002212240100D01A0600BBD0D900000000230005060002040006080006060A0C000C0A0E060E1012001416180100300606009AA0D90000000023040505000204000000000100C01806008170D900000000230005060002040006080A0604020C000E100C060A080E001214160100700E06008AB0D900000000230405060002040002000605080A0C00000000DF00000000000000FFFF011B415730FF01B8FE4AFE930000FF8B01FC0F326BFF017EFEE7FEAB0000012701D1561351FF01B8FE4AFE9300000182022D0F326BFF012500C801EA000005140617F9336CFF01CA01CF00BA000007C50819FB741AFF00B1019400CF000007C105A1F2673BFF017EFEE701560000FF840105150A75FF01ABFF7B015C0000FF4B01C10A2671FF0076FF5D0148000001920206EA465EFF012BFE890182000000150089450D60FF003BFE3C02AE000001FCFFDD06A34BFF012BFE89FE7F000000150089EB3C9BFF003BFE3CFD54000001FCFFDD06A3B5FF0076FF5DFEB9000001920206EA46A2FF017EFEE7FEAB0000FF840105150A8BFF01ABFF7BFEA50000FF4B01C10A268FFFDA3800030D000440D7000002FFFFFFFFD9000000002304050100500A06009EA00100200E0600CEE0DA3800030D000280E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005740F5500000070D4050E600000000000000F3000000071FF200E700000000000000F5480800000D4050F20000000007C07CFA000080FFFFFFFF010120320600CAA8060E100000120C0A060A0214001600180602041A00061C04061E2002002224020626062800000C2A062C0C2E003008000100D01A0600A6C006000204000200060604080A000C060E06100A08000A000406120E1400161214060E120C00060C0205140E1800000000DF00000000000000DA3800030D000140D7000002FFFFFFFFD900000000230405010040080600A580DA3800030D000180E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600C3E8F5500000070D4350E600000000000000F3000000071FF200E700000000000000F5480800000D4350F20000000007C07CFA000080FFFFFFFF0100C0200600D52806080A000002040C0600020E001012040604141600180600061A040600001C1E0101702E0600BA60060002040006080A060C0E10001214040616180800041A0006040212001C0A08061C0818001E101806162022000E1810060E1C1800061608060C2422001E22240624101E0010240C06161E1800221E1606262822001C282606280E0C000C2228060A260600222026061C0E2800122A2C06001A2C000A1C26062C1412002C2A00DF0000000000000001A8006FD04C4EFF00380086FF580000008701D5B735B2FFFF7A009900AF000001C501D2E3D16AFFFF5C02020039000001A400B3BE63F8FF00460003FF9E000000DB022B1094CFFFFF860006FFC8000000B502B5CEA7C2FF00660001003F0000008C0247389E27FF00460003FF9E0000009802601094CFFF01F7009101BC00000747FFA31C5351FF0306FE9A019E0000037FFF8536A334FF0349FF8701A9000001EFFF896C172EFF02FD0046019000000083FF7B455828FF02FD0046019000000883FF7B455828FF001B009DFEEA00000213031713FD8AFF00F0006DFFBE000000D502FE74EFE9FF001B009DFEEA00000213031713FD8AFFFF3D0078FF4F000002F50300ACFCABFF00F0006DFFBE000008D502FE74EFE9FF0067FFE7007A0000071802A533EC6AFF00F0006DFFBE000000D502FE74EFE9FFFEF0FFED0044000003F802A79BEE3CFFFF3D0078FF4F000002F50300ACFCABFFFF3D0078FF4F000002F50300ACFCABFF0067FFE7007A0000071802A533EC6AFFFEF0FFED0044000003F802A79BEE3CFFFEF0FFED0044000003F802A79BEE3CFF0067FFE7007A0000071802A533EC6AFFFEF0FFED0044000003F802A79BEE3CFF0200FFBE00650000066D01171DBF60FF024E00A1005D00000442014D123D65FF",
                            "018B001200980000055400C72AED6EFF011700B600520000044400720B6F29FF00FAFF470022000007FC005C1F901CFF01EDFF68FFA50000092901030A8FDAFF00F40065FF68000002DC0051FE3997FF024300C5FFD1000003850141FD72DDFF00F4FF72FFAB0000092000530BAFA9FF00F4FF72FFAB0000012000530BAFA9FF025D000EFF300000024C014EF7F989FF01EDFF68FFA50000012901030A8FDAFF002DFFD2FF5E00000200FFC6FDF389FF0200FFBE00650000066D01171DBF60FF024E00A1005D00000442014D123D65FF024300C5FFD1000003850141FD72DDFF025D000EFF300000024C014EF7F989FF01EDFF68FFA50000012901030A8FDAFF01EDFF68FFA50000092901030A8FDAFF00FAFF470022000007FC005C1F901CFF018B001200980000055400C72AED6EFF011700B600520000044400720B6F29FF011700B600520000044400720B6F29FF00FAFF470022000007FC005C1F901CFF00F4FF72FFAB0000092000530BAFA9FF002DFFD2FF5E00000200FFC6FDF389FF002DFFD2FF5E00000200FFC6FDF389FF00F40065FF68000002DC0051FE3997FF002DFFD2FF5E00000200FFC6FDF389FF011700B600520000044400720B6F29FF00F40065FF68000002DC0051FE3997FF00F4FF72FFAB0000012000530BAFA9FF002DFFD2FF5E00000200FFC6FDF389FF018B001200980000055400C72AED6EFF011700B600520000044400720B6F29FF018B001200980000055400C72AED6EFF00F4FF72FFAB0000092000530BAFA9FF03030303030303030303030303030303030303030303030303030366031B030303030366037C03035C81031B0387031B598F54780390667859A0288403A01B845C01289959017290790115A4280103990890150115A45499081B15A4151B28870879159008794A81083408030828155C08080808084A0808080808080808080801EFFFB7FFBF000000FB00931EB1ABFF01F0FF92004A00000104005B21972EFF00F9FF43002B000000BF0068168DEBFF01B500A4FFF3000000EE007E0F72DFFF014D007500AC000000DD0033115452FF02520083004C0000011E00591C6D26FF02500061FFBE0000011500932441A3FF018B0012FF7A000000DB00B00F0E8AFF0252000B009C0000012300391BFA74FF014EFFB900AC000000DD003315C163FF011700B9FFFA000000C4007C0E72DFFF012000C8FE1B00000EEB0617F83395FF0111FF82FE23000010C4053207C29AFF018FFE6EFF590000131F05930C8BECFFDA3800030D000080D7000002FFFFFFFFD9000000002304050100400806005D40DA3800030D0000C0E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600C3E8F5500000070D4350E600000000000000F3000000071FF200E700000000000000F5480800000D4350F20000000007C07CFA000080FFFFFFFF0100C0200600BF880600080A000C0204060E04000002101206141602000006180606021A001C1E000101702E06008940060002040006080A060C0E10000012140608161800041A00061402000008061C0616081C00160C1E06202218000C160E06161C0E0008180A062024100024201E061E0C240010240C06161E1800181E20062026280028261C06100E2600262010060A28060028222006260E1C002A2C14062A1A0400281C060614122A00042C2ADF0000000000000001F0016C930231FF0038008000A70000008701D5B7354EFF006A022200010000005F003E414A43FF00C90168FFCC000000140159321B69FF011C00E90061000000B6012474EB15FF00CC014F0054000000920129D45FC8FF00E40157008D000000AF0103163F63FF010C014E006D000000A400F7504BD0FF032D00A000090000084AF8BFFE7700FF03270035008100000636F7B7020777FF0309FFA5000700000368FAC90B8B17FF03270034FF900000065DFB4704F589FF01F0FF91FFB60000000004492197D2FF001B00A4011500000213031713FD76FF008B01E900D6000001A503FF450162FFFF7D01E800F20000029C03FDD1036DFF00F000740041000008D502FE74EF17FF0067FFEDFF850000071802A533EC96FF00A601BDFF3B0000071E03EF3BDD9FFF011C01C5FF91000007F103F16DE0DDFF00F301E20047000008ED03FF6DFF31FFFF8301C2FF4C0000048B03F1CDFA94FFFEF0FFF3FFBB000003F802A79BEEC4FFFF3D007F00B0000002F50300ACFC55FFFF0F01C500640000033A03E98B0718FF00F000740041000000D502FE74EF17FF00F301E20047000000ED03FF6DFF31FF00A9FF57FFBA000000B40456FD8BE9FF018B0012FF680000055400C72AED92FF024D00A1FFA200000442014D123D9BFF0200FFBEFF9A0000066D01171DBFA0FF011700B6FFAD0000044400720B6FD7FF01EDFF68005B0000092901030A8F26FF00F9FF47FFDD000007FC005C1F90E4FF024300C5002E000003850141FD7223FF00F400650097000002DC0051FE3969FF00F3FF7200550000092000530BAF57FF025D000F00CF0000024C014EF7F977FF00F3FF7200550000012000530BAF57FF01EDFF68005B0000012901030A8F26FF002DFFD200A100000200FFC6FDF377FF01E4FFEAFEF200000218028CFCEE8AFF01C200F40040000003F50280016047FF01B7FF9D00740000071B027A01B55DFF01D100BFFF53000002FB0282FE5CB5FF01CDFF1DFFAA000008EC0283FA8AF0FF01CDFF1DFFAA000000EC0283FA8AF0FF00E6FFEF01050000021E01DDEEF075FF00E500BC00A80000030301DFF25D49FF00E2FF2B0058000000EF01E1F28F24FF005600D0003000000383017FE97410FF005600D0003000000383017FE97410FF005500ADFFA90000042E0183F13E9BFF00E2FF2B0058000000EF01E1F28F24FF00E6FFEF01050000021E01DDEEF075FF00CDFF9CFF7D000006D401D9FAD392FF00E2FF2B0058000008EF01E1F28F24FF00CDFF9CFF7D000006D401D9FAD392FF00E500BC00A80000030301DFF25D49FF005600D0003000000383017FE97410FF005500ADFFA90000042E0183F13E9BFF005600D0003000000383017FE97410FF00CDFF9CFF7D000006D401D9FAD392FF005500ADFFA90000042E0183F13E9BFF002DFFD2FF2E0000021CFFBBC7FD97FF008700A0FED700000F5D0874F63797FF0091FF6BFEE00000110907CD01D88FFF0091FF6BFEE00000110907CD01D88FFF0091FF6BFEE00000110907CD01D88FFF0101FE6900030000131708181791DBFF0101FE6900030000131708181791DBFF0101FE6900030000131708181791DBFF008700A0FED700000F5D0874F63797FF00F9FF43FFD4000000BF0068168D15FF01F0FF91FFB600000104005B2197D2FF01EFFFB70040000000FB00931EB155FF02520083FFB30000011E00591C6DDAFF014D0075FF54000000DD00331154AEFF01B600A4000C000000EE007E0F7221FF018B00120085000000DB00B00F0E76FF02500061004100000115009324415DFF014EFFB9FF53000000DD003315C19DFF0252000BFF630000012300391BFA8CFF011700B90005000000C4007C0E7221FF014EFFB9FF530000001C00BE6B0034FF01850011FEE200000088005C6B0034FF014D0075FF54000000D300B36B0034FF008DFFBD000100000163018D000077FF008B00640001000002B70199000077FFFFFC00160001000002220203000077FF03CA00BCFFF400000334FF1B1276FFFFFF660002FFD0000002030276FF0089FFFF6700AD0002000003600280FF77FFFF03CD0003FFC1000001BCFF0D0AFF89FFFE5EFFFE00080000020C034389FE02FFFEF8FF10000B0000001C02BBA7B104FFFF6A0000007E000001FE0273F60177FF03CCFF5CFFF600000066FF02128A00FF04C4FFC8FFF300000133FE495BB3FFFF04BF0042FFF20000022CFE555D4BFEFFFF69FF4E0004000000930268018900FFFEE8FFFCFFC60000020002D701FF89FFFCA3FFF1FFC70000020E0499A9FCAEFFFCE0005F0009000002EB0471DC7202FFFCE1FF8A000D000001380462E18D03FFFEEAFFB000040000016302D0088900FFFEE9004B00040000029F02DB047700FFFF65FFFEFF83000001FB0276F4FD89FFFEF70100FFF80000041102DDA64EFFFF03CC0003002B000001BCFF0E0D0077FFFF650002003A000002030277010077FFFF67FEB700060000FF60025E6CCE00FFFEEBFFFE00420000020202D5030077FFFCA5FFEF00410000020B0497AEFF56FFFF6701570001000004BA028C6D30FCFF01E3FBB1FF030000FF81FEE92196D5FF02E9FC2AFF7B00000081002D3797F4FF01EBFBA700010000FF2E00EE329300FF00EAFB6800010000FE2800C9028900FF01E3FBB100FE0000FF67FEEE21962BFF00EAFB6800010000FE0D00CE028900FF01EBFBA700010000FF1400F3329300FF02E9FC2A008700000066003137970CFF02D9FC4C00010000003C015F53AB00FF02D9FC4C000100000057015B53AB00FF0365FC71FF7D00000114006173FE20FF0365FC710084000000F9006673FEE0FF0302FD170001000000E301E675E900FF0302FD170001000000C901EB75E900FF000B0091FDEA000000EF00A6D870FAFFFFC0FF74FDFB0000FFB900A9D747AAFF009BFF9AFF5A0000009802841B6D29FF0117FF87FEF20000005901A4485D0FFF000B0091FDEA0000008D0080D870FAFF009BFF9AFF5A0000011C02351B6D29FFFF1700C8FE5900000014009D0E72DFFF009BFF9AFF5A000000C702471B6D29FFFFC0FF74FDFB000001610087D747AAFFFF1700C8FE59000000C900F40E72DFFFFFDBFFB0FF9A0000010A01E50673DFFF009BFF9AFF5A0000000901F11B6D29FFFFDBFFB0FF9A00000125025F0673DFFFFF1700C8FE590000019B008F0E72DFFFFDF3FFA9FE1F0000FFDC0075A8E6B4FFFDBA00D6FF130000FFFD00B40276EFFFFFDBFFB0FF9A000000F902AC0673DFFFFDF3FFA9FE1F000001AC003BA8E6B4FF00E2FF8CFFF1000000550129168B02FF00E2FF8CFFF1000000550129168B02FF0107FFAE003800000060013A08B960FF007C0093FFEF000000B400F5F37702FF00790036FF780000009400F6E81F8FFF00620038005E0000009500EAEE2171FF007C0093FFEF000000B400F5F37702FF007C0093FFEF000000B400F5F37702FF00620038005E0000009500EAEE2171FF0107FFAE003800000060013A08B960FF00620038005E0000009500EAEE2171FF0105FFADFF9B00000060013906AFA9FF00E2FF8CFFF1000000550129168B02FF00790036FF780000009400F6E81F8FFF0105FFADFF9B00000060013906AFA9FF00790036FF780000009400F6E81F8FFF007C0093FFEF000000B400F5F37702FF01E0FFEA010D00000218028CFCEE76FF00E6FFEF01050000021E01DDEEF075FF00E2FF2B0058000000EF01E1F28F24FF00E500BC00A80000030301DFF25D49FF01BD00F5FFC0000003F502800160B9FF00DD00DBFFA30000042601E3F464C1FF00CDFF9CFF7D000006D401D9FAD392FF00E2FF2B0058000008EF01E1F28F24FF01B2FF9DFF8C0000071B027A01B5A3FF01C9FF1E0055000008EC0283FA8A10FF01C9FF1E0055000000EC0283FA8A10FF01CD00BF00AC000002FB0282FE5C4BFF",
                            "005600D0003000000383017FE97410FF005500ADFFA90000042E0183F13E9BFF000000000000000000162C10102C16000010401414402C000010148D8D1410000010148D8D141000002C401414402C0000162C10102C16000000000000000000FFDF0184FF890000FF5301FBC15E25FF01EF01A0FFDC000000D6FFDC3764DEFF0148018E00820000FFFC0204E15C46FF0313006901B30000087E021E19475DFF034C007000C100000858000C692EDEFF0321FE45012200000BF400092D95E4FF007AFDFE00230000000D0208D79F38FF0095FE04FF900000FFA10011068CE2FF0335FE5F01CE00000D4A020116BD60FFDA3800030D000040D7000002FFFFFFFFD9000000002304050100500A06007900DA3800030D000080E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01CFA000080FFFFFFFF0101102C0600AE30060A000400000C0E0610120600021416061808020002001A061C1E000004202206042426002804060506082A000000000101C0380600B888060002040004020606080004000A0C0E061012140016181A061A14160014120A060A1C14001E02000600201E00082000060E1C0A0018160E060E0C18001E220206242628002A2C2E0602220600141A10062A2E30003224280634221E001E2036DF00000000000000008D0185009D0000024A00A8C82DA1FFFF7D01E800F2000000C20032153296FF008B01E900D6000002010029C933A3FFFF7B016300BA0000008400DE37209BFFFF0F01C500640000001F00456B1BD2FFFF4700D8007B0000061B01914D5128FFFF8301C2FF4C000005180012424845FFFF0F01C500640000061F00456B1BD2FFFF4700D8007B0000001B01914D5128FF00A601BDFF3B000004170014F4584FFF00B700DB00280000035E017CA84F12FF011C01C5FF910000039A001B9C3C19FF00F301E20047000002F000179D20C6FFFFE6007B692EDEFF0370FF8001C1000000F601AB3F1363FF03CBFEDC00F70000FFF601FF6EDAE8FF0313006901B300000103007019475DFF007B008F0008000000E4FFAAF377FFFF014300D00052000000CA003BF06D2DFF014400D3FFC5000000C9003CF26CD0FF007A0033007F00000106FFACE91F71FF00600035FF9900000106FF99EC218FFF032D00A00009000000B10071FE7700FF0220009E0004000000A5FFB22871FEFF01FD002100880000007CFFA2130776FF0309FFA500070000005F00690B8B17FF0106FFAA005A0000004BFEFB07AF57FF00E2FF8900040000003FFEE5178BFEFF0106FFABFFBD0000004CFEFC07B9A0FF0202002DFF9100000080FFA50B268FFF03270034FF900000008E007504F589FF0327003500810000008E0074020777FF0220009E0004000000DA00E32871FEFF01310071FF62000000ED003208218EFF012D006B00D6000000F0002F072073FF0106FFABFFBD00000132005307B9A0FF00600035FF9900000101FFD3EC218FFF01310071FF62000000E9006C08218EFF012D006B00D6000000A20149072073FF007A0033007F0000009400F6E91F71FF0106FFAA005A00000060013907AF57FF01FD002100880000008301AC130776FF0202002DFF9100000100010B0B268FFF01FD002100880000010900CD130776FF0202002DFF910000010400D10B268FFF060E1006000E0C10060C1210000A120CDF00000000000000FF6B501B5281280DFEDF00013881FE176B4100019B8149835281938141859B81DDDB5A815A8152819BCF5A81F6A59BC15A8100014015AA839B816207A3C1300F5A8162C162C15A8162C162C1301183114015EEA540175A81E65F6AC19341938162C140C348198B417B4140156AC1501938819B8FEEA74905AC0FDE2573019381C517EEA159855019AB8B8B41B3CB628148176A09F6A5DDD5F6E39381B38B40158B8100018B0D89C3F6E962C1FF2BE619B4917A8BFF2BCDA1B40FB4DBAB8B7B0193413011481940159341AC0B6B015A816AC1401562C1728962C162C15AC173019B815AC1E5DB7B01934973013813FF297B01CD576B01938B8B49938130116AC19381EE1D481762818B419B819B819381F6A373018B817AC983417ACB40157B41401548178B81FF2B52817B018B4193416AC18B497B018B81938179837287934B8B41180969835AC1481983415017A3CB40C183419B818B418B4972877B017B418341730130118B49301173017B0140178B419BC18B419B8B6AC18B81594389C37B01934B73017287514383099A87AC0B72877B0148C16A877B411005000172C108037287730181838B813813000169436B019349DDDB82C949858B41A3CB20092009E65D6A478B49AC0D9A877AC96AC1B40D514D83418B49DDDB51C58B4138138B8DB44D8341F6E5C5556A47FF296A47728762C19B8B72876A478B4179D34817036FFF1B0001000000F3003F476000FF0309FF3CFFBD0000FFB301E5601EBFFF0306FF69000100000109021F5A4E00FF0309FF3C00450000025901D7601E41FF00E900BBFF570000030301DFF25DB7FF00EAFFEFFEFB0000021E01DDEEF08BFF00E7FF2BFFA7000000EF01E1F28FDCFF005B00D0FFD000000383017FE974F0FF005B00D0FFD000000383017FE974F0FF005900AD00560000042E0183F13E65FF00EAFFEFFEFB0000021E01DDEEF08BFF00E7FF2BFFA7000000EF01E1F28FDCFF00E7FF2BFFA7000008EF01E1F28FDCFF00D1FF9B0082000006D401D9FAD36EFF00D1FF9B0082000006D401D9FAD36EFF005B00D0FFD000000383017FE974F0FF00E900BBFF570000030301DFF25DB7FF005B00D0FFD000000383017FE974F0FF005900AD00560000042E0183F13E65FF005900AD00560000042E0183F13E65FF00D1FF9B0082000006D401D9FAD36EFF014600D6002F000000C9003CF36C30FF014200D3FFA2000000CA003BF06DD3FF007C0093FFEF000000E4FFAAF37702FF00790036FF7800000106FFACE81F8FFF00620038005E00000106FF99EE2171FF01FB0023FF6A0000007CFFA211078AFF022100A0FFEE000000A5FFB2287002FF032E00A1FFE6000000B10071FE7700FF00E2FF8CFFF10000003FFEE5168B02FF0105FFADFF9B0000004BFEFB06AFA9FF0308FFA6FFE90000005F00690B8BE9FF03290035005F0000008E007505F577FF0204002F006100000080FFA50D2671FF0107FFAE00380000004CFEFC08B960FF03260036FF6E0000008E0074010789FF022100A0FFEE000000DA00E3287002FF013400730092000000ED0032092172FF012C006DFF1E000000F0002F06208DFF013400730092000000E9006C092172FF00620038005E00000101FFD3EE2171FF0107FFAE003800000132005308B960FF0105FFADFF9B00000060013906AFA9FF00790036FF780000009400F6E81F8FFF012C006DFF1E000000A2014906208DFF01FB0023FF6A0000008301AC11078AFF0204002F006100000100010B0D2671FF01FB0023FF6A0000010900CD11078AFF0204002F00610000010400D10D2671FF0606100E00100C0E0610120C000C1208DF00000000000000FF07FFBDFFF20000038900338BEAF7FFFFC8FFBB00E60000025E00C10089F3FFFFD5004C00AD000002B502A0EAE071FFFF36002DFFF70000009900D08DE4F5FFFEBD014000620000FEBF043A9C2137FFFEE70140FF4D0000FECC044CABF1AEFF042600B7FFFE0000045B040054C0C9FF02CF0149FF0B000002D104341F56B4FF029901440181000002E803F4214F53FF00F6FF8B001C000000B6011A76F108FF009E0007001200000184026371DA02FFFFD5004C00AD0000012B011DEAE071FFFFB2014801220000FFCC0435E43D62FFFF36002DFFF70000039301BD8DE4F5FFFFE2014DFEEB0000FFB7046FF63E9BFF01C600C900F20000029A0286FFA64EFF009E0007001200000246004271DA02FF025D008D00580000037401CE118A08FF03FE00B501080000045203E6169C3EFFFFF10046FF640000012B012000DC8EFF021D00AAFF7C000002F2023D0FA1BAFFFFE8FFB2FF370000026300C6FC8B18FFFFF10046FF64000002B402A400DC8EFF02AEFE9C004300001455080B258E00FF0191FE6E00AB000014E005930A8C19FF018FFE6EFF590000131F05930C8BECFF0361FEF8FFCE000013BB09C052A900FF0361FEF800320000144409C052A900FF03E4FFD5FFE9000013D40B5B75E800FF03E4FFD500150000142B0B5B75E800FF03C000F4FFDD0000132D0BBD702800FF035C0192FFBA00000D0C0B4A416400FF035D0192004600001AF30B4A416400FF01C801CFFF4A00000C3A08190076EBFF01CA01CF00BA000007C50819FB741AFF035D01920046000006F30B4A416400FF00E2FF890004000000550129178BFEFF0106FFABFFBD00000060013A07B9A0FF00E2FF890004000000550129178BFEFF007A0033007F0000009400F6E91F71FF007B008F0008000000B400F5F377FFFF007B008F0008000000B400F5F377FFFF00600035FF990000009500EAEC218FFF007B008F0008000000B400F5F377FFFF00600035FF990000009500EAEC218FFF00600035FF990000009500EAEC218FFF0106FFABFFBD00000060013A07B9A0FF00E2FF890004000000550129178BFEFF0106FFAA005A00000060013907AF57FF0106FFAA005A00000060013907AF57FF007A0033007F0000009400F6E91F71FF007A0033007F0000009400F6E91F71FF007B008F0008000000B400F5F377FFFF0158000C00010000F4CF043A007700FF01DCFFE200D50000F5EA0404FF5C4CFF027C001A00510000F532044D3B6124FF009BFF9A00A70000F5C303A80D2E6DFF013C002200010000F4C80457683A00FF027C001AFFB00000F463044D3B61DCFF02B2FF4600FE0000F64A033C502252FF009BFF9AFF5A0000F41903A80D2E93FF01DCFFE2FF2D0000F3CB0404FF5CB4FF02B2FF46FF030000F3C1033C5022AEFF0117FF87FEF20000004F0056F452ABFF01DCFFE2FF2D00000010FFF3FF5CB4FF01DEFEDDFEAA0000FFF100DB191A8EFF01DEFEDDFEAA0000F3770281191A8EFF01DEFEDD01570000F6E40281191A72FF009BFF9AFF5A0000009600050D2E93FF0302FD1700010000F5A0006E6E2E00FF02C3FCF1FF890000F512003D6E1DDBFF031EFDED00010000F565018177F900FF02C3FCF100780000F643003D6E1D25FF036FFF1B00010000F511030432D262FF0309FF3C00450000F55F032F601E41FF0308FEAB00010000F530027576F100FF0309FF3CFFBD0000F4B1032F601EBFFF036FFF1B00010000F511030432D29EFF01DEFEDD01570000FFF100DB191A72FF01DCFFE200D500000010FFF3FF5C4CFF0117FF87010F0000004F0056F45255FF009BFF9A00A70000009600050D2E6DFFDF00000000000000FF01FFB9FFDC0000097EF8D18AEC00FFFFD5FFACFEF800000569F6EAFD8A11FFFFD5FFACFEF800000569F6EAFD8A11FFFFD5FFACFEF800000569F6EAFD8A11FFFFD5FFACFEF800000569F6EAFD8A11FF00F1FF7FFFDC0000FFFCFC9A76EF01FF00F1FF7FFFDC0000FFFCFC9A76EF01FFFFD2FFB200A90000",
                            "05C5FD45FC8AEBFFFFD2FFB200A9000005C5FD45FC8AEBFFFFD2FFB200A9000005C5FD45FC8AEBFFFFD2FFB200A9000005C5FD45FC8AEBFFFF01FFB9FFDC0000097EF8D18AEC00FFDA3800030D000000D7000002FFFFFFFFD9000000002304050100B01606007A80DA3800030D000440E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005E00F5500000070D8140E600000000000000F3000000071FF400E700000000000000F5480400000D8140F20000000003C0FCFA000080FFFFFFFF0101303C060079500616000200181A04061C1E06002022060624260800061228062A02080014042C062E3014000400320602343600383A0A0100902806007FF006160C0E0010181A061C0E1000080A1E06201006000E2224050A0C2600000000E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01C0100600C0600E16806000204000602000600080A00040800E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006009290F5500000070D0150E600000000000000F3000000070FF200E700000000000000F5480800000D0150F20000000007C03C0101602C060020F0060002040006080A060A040600020604060C0E100012101406101216001812140610160C00041A00061C1E0A000A0820060A201C002224260622262800162624060C162400242A0CE700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005E00F5500000070D8140E600000000000000F3000000071FF400E700000000000000F5480400000D8140F20000000003C0FC0101602C06009130060002040002060406080A0C000E1012060A1416000A1618060A181A00121008061C0406000C1208061E100E001A0C0A061420220024260006282A20000004240628262A00221614062A262400201428DF00000000000000000000000000000000000000000000001F1F1F251F161616E8F8F8F8F8E6E4E48DE4E78D8D6158622E4F5756564F2D4405442E2D07050505050505050505050506080A0C00080E0ADF0000000000000002020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020221111111020202020202020202020202020202020202020202020202213E3E3E216E111111020202020202020202020202020202020202020211212150505034211121216602020202020202020202020202020202111121215F348C50335F6621213E8C0D02020202020202020202021121213E5F34348C8C0D0D0D0D3E2D21212D5F0D17020202020202021111116E665F8C500D0A0A0D50338C5F110202022D5F330D33111111116E6E6E2121216634340D0D0A0D508C5F3E1102021121668C330D0A3334116E212D663E0D5033500A0A0A0D338C343E110202026E3E5F3E668C0D0A170A0A170A2E0D0A17170A0A0A0D0D8C342D1102111111213E8C5F21213E33330D0A1E1E170A0A0A0A0A0A0A0A338C5F21111111472166345034216E212D5F330D0A170A0A0A0A0A0D0A0A0D335F2D2D2D212121668C50508C2D1111112D5F2E0D0A0A2E0A0A17170A0D0D335F3E3E5F5F8C8C8C0D0A505F2D11110211215F0D1E0A2E2E0D0A0A0A0A0D338C33333333330D0A170D5F2D21111102026E2D330A0A1E0A2E2E2E0D50500D0A0D0D33330D170A0A333E21110202021111215F330A1E170D0D0D2E2E33330D0A0D0D0D0D0A335F3E2D111102020211212D5F330D0D1E1E0A0A2E0A0A0D2E2E0A0D0D33333E211111111111021111212D5F502E0D0D1E0A0A170D0D170A3333335F3E2D211102021102020211212D2D3E3333332E2E0D0A0A0A0D33335F2D211111110202020202111121212D3E5F8C0D338C33332E0A0A2E3333343E470202020202110211116E472D2D3E34505033335F3E2D5F0D0A170A33343E2D21212121212121212D3E3E5F8C5050508C343E2D4711115F33330A0A0D8C5F5F3E5F8C508C0D50500D500D0D508C3466211111020202112D5F332E0A0D335033508C34348C330A0A0D503E216E476E11110202116E47112166340A0D8C8C8C5F5F345F3E3E6666CF2D2111020202020202111111212111212D3E2E8C5FCFCFCFCF3ECF2D2D212D21214711020202111147214747212121212D3E08080808080808080808080808080808080808080808080808080808080808081B1B1B1B1B1B1B1B1BC6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6C6040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040707070707070707070707070707070707070707070707070707070707070707DA3800030D000000D7000002FFFFFFFFD900000000230405010030060600CA780100400E06005500010020120600D3B00100201606005720DA3800030D000100E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005E00F5500000070D8140E600000000000000F3000000071FF400E700000000000000F5480400000D8140F20000000003C0FCFA000080FFFFFFFFD9000000002300050100802606005D80061618020002001A06001C1E00200004060E022200122404E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005B40F550000007090040E600000000000000F30000000707F400E700000000000000F548040000090040F20000000003C03C010110380600E73806161810001A0608060A1C1E000C200A060C222400260C060606282A000A2C2E060A30140032081005083436000000000100C01806008F800600020400060008060A0C0E0010061206000610000E140A0616040200040800E700000000000000E300100100008000FD1000000600B538E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000600B020F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01CD9000000002304050100A01406008B2006000204000602080606080A00040206060C000E00040E0006000C10000A1206060C120A000A100CDF000000000000000116FF8201E20000033B053208C266FF012500C801EA000005140617F9336CFF0191FE6E00AB000000E005930A8C19FF018B0012FF680000055400C72AED92FF00F9FF47FFDD000007FC005C1F90E4FF011700B6FFAD0000044400720B6FD7FF011700B6FFAD0000044400720B6FD7FF00F3FF7200550000092000530BAF57FF00F9FF47FFDD000007FC005C1F90E4FF002DFFD200A100000200FFC6FDF377FF002DFFD200A100000200FFC6FDF377FF002DFFD200A100000200FFC6FDF377FF00F400650097000002DC0051FE3969FF00F400650097000002DC0051FE3969FF011700B6FFAD0000044400720B6FD7FF002DFFD200A100000200FFC6FDF377FF00F3FF7200550000012000530BAF57FF018B0012FF680000055400C72AED92FF018B0012FF680000055400C72AED92FF011700B6FFAD0000044400720B6FD7FF00F3FF7200550000092000530BAF57FFD7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD10000006006200F5100000070D4350E600000000000000F3000000073FF100E700000000000000F5101000000D4350F20000000007C07CFA000080FFFFFFFFD9000000002304050101C0380600AA70060002040006080A06060C08000E1012060C1408000C16140606180C000C181606141A08001C1E20061C221E0024221C0620261C00280E2A0628100E00142C2E06162C1400182C160618062C00060A2E06142E1A00062E2C0628301000223234063220340026203206222432001E3420061E223400283630062A36280012303606121030002A1236050E122A00000000DF000000000000000316FC6E01400000FFC8001B4CC143FF0209FC7E01B90000010200521BBF60FF02E9FC2A008700000012FFF837970CFF02C4FD3801B80000FFB100833CFD67FF0316FC6E01400000010600264CC143FF0365FC71008400000019000973FEE0FF033FFDE101180000FFC9011E7125F8FF0027FE8F0258000001ED017B08ED76FF0244FF5D01680000FFE0027F6237D8FF02C4FD3801B80000012C00D33CFD67FF0244FF5D01680000005102196237D8FF01C000A6013F0000FFE902ED2948ABFF018FFF2E0230000001A801962C2768FF0209FC7E01B900000102005218BC5FFF01E3FBB100FE0000019DFFF621962BFF01E3FBB100FE00000335020C21962BFF00A5FBC7013E000001B201DEEB9E41FF00EAFB68000100000000020A028900FF01E3FBB100FE0000003EFFE221962BFF0209FC7E01B90000FF9A004918BC5FFF00A5FBC7013E00000122001AEB9E41FF0048FC7C01D8000000F40084DFC864FF0027FE8F02580000FFCE017908ED76FFFED3FFB102890000002702251C0074FF00A5FBC7013E00000076001BEB9E41FF0048FC7C01D80000FFBC0081DFC864FFFF2FFCA9010A000000D500ABADBE36FF00EAFB680001000001D9FFCA028900FFFF7CFBAC00010000021E0020C09B00FF0117FF87010F0000002E023A485DF1FF000B009102180000016C0181D87006FF00AFFFE902980000005101825B1B48FF01FBFEAE01B8000004DEFFA1F5AE56FF014CFF6F01EB000005F0FFCB04FC77FFDF00000000000000DA3800030D000440D7000002FFFFFFFFD9000000002304050100500A06007B300100200E06005620DA3800030D000340E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005740F5500000070D4050E600000000000000F3000000071FF200E700000000000000F5480800000D4050F20000000007C07CFA000080FFFFFFFF010120320600A11006000E10000C0A120614020C00160018061A040200041C0606021E200002222406260628002A0A00",
                            "062C0A2E000008300100D01A06009FE0060002040006040206080A00000C060E060A08100000040806120C1400121416060E140C00020E0605180C1200000000DF00000000000000050E120400000000DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD10000006005000E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006006A00F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03CFA000080FFFFFFFFD9000000002304050101F03E0600A390060002040006080A0604020C000E0A080610000400120C0206141618001A040C061C1418001E1420062212240026222406261A28001E1614061A0C280026280A060A0E26002A2C2E06280C1200003012061230240022061206260E3200281206060A2806002E342A0608320E0036141C061C2038003A382006363C3A00383A3C063C361C001C383C063A20140014363A06041A2400241A260632222600020012DF00000000000000F729B4D70001F729CDDDD5DFCD9D001FEEE7E6A3DE21EEE5BD59C59B513BFFFFF7FFC55BB4D5001FDE61EEA5082160016AA1AC95C8016801E6A30843A453AC93AC93A44F00218BD54A1F291B18C340019C0FF7BD2945C55BA453937300013147BD19C55FA495DEB12909980118C31065AC6B52177B555137835550FB5A9D28EBEF39B4EDA4516B116A37A00148FD315F734F9371BCEB82F33001FFFD62DB6A3918DD4A1B10017AB761BB50FD0801D56B72B539871885419B5259700194119001C5639413730D597BA39D20DF839130EF1063FCBF200120AB18672105493740F35A616001BDA33187CCE7FE3F58019C59839928AD18A739614A095A8BAC6B41C75001A4974A0972D78BDD6A559463E7359A119CE7838F739BB4D9EF37A04351CDB1CF2907EEA7BD1B6319EEA7A49DAD2508430843C5A19001418DCE29F73BAC99ACDBACD7ACA7ACD7838F2801BD6188015001E663B001D66DB5192905C5A59AE7AD6920C52085B8019413294B9C5BEF3BB001EF39F7BDDEB5EEA7F77B6801C14B524B9CE7C5A3BD65F77B9B9DDE251001CDF11083C631DE71EF3B800120C7CE310801C559BD5DD66F2801C5F1B4D950C5B51B6ACD08011001189FB517CE33628D10836ACDE739DE31A801C843A4699319CB19A001085FCD9F398920C58BD1CDE548011885FFBF597B9415EF7BEFBFA49320C99C53E6F94A13AC959C53DE73FFBD01CA01CF00BA000007C50819FB741AFF01CA01CF00BA0000045BFDE6FB741AFFDF00000000000000DA3800030D000340D7000002FFFFFFFFD9000000002304050100600C0600A0B0DA3800030D000380E700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD1000000600DF68E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006005740F5500000070D4050E600000000000000F3000000071FF200E700000000000000F5480800000D4050F20000000007C07CFA000080FFFFFFFF0101102E0600B778060C0E06001008060612040200060414061602000006181A060A1C1E00000A200622240600260228052A002C000000000100E01C0600554006000204000402060606080A000A080C060E0C100010120E0604140000061604060A1606000C0E0A06180806001A0818050C081A00000000DE00000006001A88DF00000000000000DF00000000000000FFC8FFBB00E600000569F6EA0089F3FFFF07FFBDFFF20000097EF8D18BEAF7FFFFC8FFBB00E600000569F6EA0089F3FFFFC8FFBB00E600000569F6EA0089F3FF00F6FF8B001C0000FFFCFC9A76F108FFFFC8FFBB00E600000569F6EA0089F3FFFFE8FFB2FF37000005C5FD45FC8B18FF00F6FF8B001C0000FFFCFC9A76F108FFFFE8FFB2FF37000005C5FD45FC8B18FFFFE8FFB2FF37000005C5FD45FC8B18FFFF07FFBDFFF20000097EF8D18BEAF7FFFFE8FFB2FF37000005C5FD45FC8B18FF0612141600161812061A1C1E001E141A061A121C00121A1406181C12001C181E061E161400200A2206242628002A2C2E062A002C000A200606002A08000E3000060030020000322C062834240006202E062E2C06002C320C06163638002C0C06060E0806000E000806181638003A161E0632100C003A3616060432000010023C06222E2000320410062E222A00100402DF000000000000005D3F553F54FF54FF54FF54FF54FF54FF54FF54FF54FF5D3F657F5D3F5D3F5D3F5D3F5D3F5D3F657F5D3F54FF54FF54FF54FF54FF5D3F657F5D3F54FF553F5D3F553F4CBF447F447F447F44BF44BF44BF44BF44BF44BF54FF657F54FF4CBF4CBF4CBF4CBF54FF657F653F44BF44BF44BF44BF447F54FF657F54FF447F44BF553F54FF447F3C3F3C3F3C7F447F447F447F447F447F447F54FF6D7F54FF4CBF4CBF4CBF3BB3336D657F6D7F4C7D4C7F447F4C7F443D54FF657F4CBF3C3F447F54FF5D3F447F3C3F3C7F447F447F4CBF4CBF4CBF4CBF4CBF5CFF75BF5D3F5CFF54FF4C352109210954736D3B294B321754BD4C79214F329F6D7F54FF3C7F447F54FF657F657F4CBF447F4C7F4CBF54BF54BF54BF54FF54BF653F75BF6D7F653F653F5CF93A593A596D3B85BD4A554ADB64B75C2F39CF31D16D3954FF447F44BF553F5D3F657F657F657F54FF54BF5CFF5CFF5CFF64FF64FF6D7F85FF75BF6D7F757F75BF7D7D7DBD85FF9E3F95FB8DFF85BF7D7D642D6CB585FF6D3F4CBF4CBF5D3F54FF4CBF5D3F6D7F75BF6D7F653F64FF653F757F7DBF8DFF8E3F8E3F85FF7DBF85FF963F9E3FA67FAEBFB6BFB6BFAE7FA67FA67F9E7F963F85FF75BF657F657F54FF44BF447F54FF6D7F7DBF7DFF7DBF85FF8DFF8E3F8DFF8DFF963F963F9E3F9E7FAEBFAEBFB6BFB6BFBEFFBEFFBEFFBEFFAEBF9E7F963F85FF7DBF75BF6DBF54FF447F447F4C7F4CBF653F7DFF8E3F85FF85FF7DBF7DBF7DBF85FF9E7FA67FAEBFAEBFAEBFB6BFB6BFB6BFBEFFC73FBEBFAE7F9E3F8DBF757F653F653F6D7F54FF447F3BFB3BF74CBF54BF653F85FF7DBF6D7F757F757F7DBF7DBF8E3FA67F9E7F9E7FA67FAEBFB6BFBEBFBEFFC6FFBEBFAE7F9E3F8DFF7D7F653F5CFF653F54FF3BF9084708433BF74CBF4C376D3775BF653D4BEF5CB5757F75BF8DFF963F963F963FA67FA67FAE7FB6BFBEBFBEFFB6BFA67F9E3F8DBF757F653F5CFF653F54FF3BF9008B08473C3B3BF918C718C764F943F12109294B653D6D3D3A154A997DBF8DFF95FF9E3F9E3FA67FAE7FB6BFA67F9E3F8DFF7DBF6D3F5CFF5CFF653F54FF447D2BB733B53C3F3BFB108B10CF653D43F518C7190F5CFF54B9294B318D6CF97DBF85BF85BF8DFF8DFF9E3FA67F963F85BF7D7F753F64FF54BF54FF5D3F54FF3C7F343F343F3C3F3C3D33734433657F54FF4C7B4CBF54FF5CFF43254BEB653F653F6D7F757F757F7D7F85BF963F85BF757F6D3F64FF54FF4CBF4CBF5D3F553F44BF3C7F447F447F447F447F54FF657F54FF54FF54FF54FF5D3F6DBF653F5D3F5CFF5D3F653F653F6D7F757F85FF757F653F5CFF5CFF54BF4CBF4CBF5D3F5D3F553F54FF54FF54FF54FF54FF5D3F657F5D3F5D3F5D3F5D3F657F6DBF657F5D3F653F657F657F6D7F657F75BF75BF6D7F657F653F653F5D3F5D3F5D3F657F018FFF2EFDD2000001A801962C2798FF01C000A6FEC20000FFE902ED294855FF0244FF5DFE99000000510219623728FF02C4FD38FE4A0000012C00D33CFD99FF0244FF5DFE990000FFE0027F623728FF033FFDE1FEE90000FFC9011E712508FF02C4FD38FE4A0000FFB100833CFD99FF0209FC7EFE480000010200521BBFA0FF0027FE8FFDA9000001ED017B08ED8AFF0365FC71FF7D00000019000973FE20FF0316FC6EFEC10000010600264CC1BDFF0316FC6EFEC10000FFC8001B4CC1BDFF02E9FC2AFF7B00000012FFF83797F4FF0316FC6EFEC10000040C01E94CC1BDFF0365FC71FF7D000004A001F173FE20FF02E9FC2AFF7B0000047E01FD3797F4FF02C4FD38FE4A0000014E00A63CFD99FF00AFFFE9FD690000005101825B1BB8FF003FFF83FC640000002300F1EE1E8EFF000B0091FDEA0000016C0181D870FAFF0027FE8FFDA9000002B800AF08ED8AFFFFC0FF74FDFB00000283003AD747AAFFFFF2FF91FD47000003020035A7D7BCFF003FFF83FC64000000760104EE1E8EFFFFF2FF91FD4700000018018CA7D7BCFF000B0091FDEA0000011A01EED870FAFFFFF2FF91FD470000FFCA0200A7D7BCFF003FFF83FC640000009D00F0EE1E8EFF00AFFFE9FD690000013B01BB5B1BB8FFFFC0FF74FDFB0000FFA401F3D747AAFF0048FC7CFE290000FFBC0081DFC89CFFFF2FFCA9FEF8000000D500ABADBECAFF01C204A0AF1CADFF0027021C003E000001520098236F1DFF002C0224FFFE00000159008D0E3897FFFF6B020400390000009700BCBF61E6FF00C7017D0033000001DA014A3C3EADFFFF3C0170008D00000054016A9808C5FFFF55016E00E00000006D016BC51067FFFF7902070067000000A500B7CF5A3DFF00B90188009D000001CE013D502E4BFFFF3C0170008D0000004301A69808C5FFFF2700C300610000004101E1961ED0FFFF55016E00E00000005A01ABC51067FFFF2700C30061000000280235961ED0FF00D20106FFE40000003D022D640B40FF00380087FF58000000BC037DAF1CADFFFF2700C30061000001D60253961ED0FFFF2700C30061000001710255961ED0FF00380087FF580000004E0292AF1CADFFFF860006FFC80000010D034DC2A5D2FFFF6A009B00B50000008301FBD0C55CFFFF860006FFC80000018703EFC2A5D2FFFF6A009B00B5000001B6023BD0C55CFFFF6A009B00B5000000650260D0C55CFF00A6008D008E00000199026350D950FF00A6008D008E0000009F024E50D950FF00B7005BFF5B0000005803AD44B9BDFF00D20106FFE40000003D022D640B40FF00B7005BFF5B0000004202B544B9BDFF00460003FF9E000000BC03351194CFFFFF860006FFC8000001870340C2A5D2FF00B90188009D000001CE013D502E4BFF00A6008D008E00000199026350D950FF00D20106FFE4000001D501D4640B40FFFF55016E00E00000006D016BC51067FF00C7017D0033000001DA014A3C3EADFFFF2700C30061000000280235961ED0FF00380087FF58000000C90284AF1CADFF00660001003F000000E0036F29982BFFFF6A009B00B5000001B6023BD0C55CFF00660001003F00000099033429982BFFFF860006FFC80000018703EFC2A5D2FFFFB10005006300000184035FDD9D3AFFFF6B020400390000009700BCBF61E6FFFF7902070067000000A500B7CF5A3DFF0027021C003E000001520098236F1DFFFF3C0170008D00000054016A9808C5FFFFB1000500630000015A033EDD9D3AFFFFFF003FF5BF001F280D000342150007000B4213FE3F1009B3FFE53F897FD47FF9BFF17FFBBFE37FFEBF000D9CBF200D1809000D92FF0023383F38FFDDBFCE7F70FF1809200B0805317FD6BF31BF00230013107FF53F947F000B0009084D084D0009080700050005000F180908070007080700050007000700071809180B0005200B1809200B200B084B084B180B200D0807000D00110011",
                            "00130013000B084F100900030005100900171009109110931095001900190019001B00070805180B180B2119211B180B0807200B0021002100210021200B0849200D0023200D0023000D210D002500250025002500250013002700150027002700291091002900290029002B002B002B002D001B001B002F001F006F082D106B180B39E11007003100330033003300230023007108330871000D0037003700750075007700B7084F0039003910091009003D0029083D100900B900B900FB180900FF107F002F180B017F117B19BD307539E138B720B928B928BB28FB20FF0033003338B9200D38FF293B213F297F297F297F313F313F317F317F0015397F0015001739BF39FF403F40BF40FD50FD413F413F417F497F41BF41FF51FF59BF78FB30B378FD617F693F61FF717F793F303F4A654A654A675AE7632B00230023423F4A3F523F52BF5ABF0027627F00276A7F6ABF6AFF727F72BF7BF1633F733F7BFF813F817F000781FF033F005E00000000F4A404FF644200FF0350003BFF420000F3B604E96240EAFF02A8010900000000F47B05C14A5D00FF0398FFA4FF470000F3D1048C6C2AE6FF0350003B00BE0000F59D04E9624016FF0398FFA400B90000F5A9048C6C2A1AFF01A30196FF8B0000040000770D73E3FF008700A0FED7000002EDFEAAF63797FF02A00120001B00000494021A2D5B3FFF0101FE690003000007A3FF7A1791DBFF029FFECF000E000007760223208F16FF02A5FF240085000006E3022B18D46CFF02A5FF240085000006E3022B18D46CFF02A9005A009E0000058F022A141474FF02A9005A009E0000058F022A141474FF02A9005A009E0000058F022A141474FF02A00120001B00000494021A2D5B3FFF029FFECF000E000007760223208F16FF02A5FF240085000006E3022B18D46CFF0101FE690003000007A3FF7A1791DBFF01A30196FF8B0000040000770D73E3FF02A00120001B00000494021A2D5B3FFF01A30196FF8B0000040000770D73E3FF060002040006080ADF00000000000000FF86FFFE0038000001870340C2A52EFFFFB1FFFDFF9D0000015A033EDD9DC6FF0066FFF9FFC10000009903342998D5FFFF3C0168FF7300000060018F98083BFFFF6B01FCFFC70000009B0127BF611AFFFF7901FFFF99000000AB0123CF5AC3FF0066FFF9FFC10000014402A32998D5FFFFB1FFFDFF9D0000009E029DDD9DC6FFFF6A0093FF4B00000073022AD0C5A4FF00270214FFC20000014C0118236FE3FF0046FFFB0062000000BC0335119431FFFF86FFFE00380000007002A2C2A52EFF00B7005300A50000004202B544B943FF00A60085FF7200000194023A50D9B0FF00B7005300A500000190026C44B943FF0038007F00A8000000C90284AF1C53FF00C70175FFCD000001CD018F3C3E53FFFF2700BBFF9F00000035020F961E30FF00D200FE001C000001C501E9640BC0FF00B90180FF63000001C60182502EB5FFFF550166FF200000007B018DC51099FF002C021C00020000014F01140E3869FF0038007F00A800000120024BAF1C53FF0103018D002F00000137026E5143C8FF011900E80082000000AB03856D0131FF00D200FE001C00000127037D640BC0FF00B00195002A000001BD0285B957D7FF0038007F00A8000001C204A0AF1C53FF008F00FB00BC0000018B03A1EF296FFF00B7005300A5000000E304B444B943FF00DB0197004F0000017B026F185D48FFFF6BFFB140C1FFFFDDD53881B247B491AC4FA451FF29E65FFEE3FFFFFEE3B48FFF29A207FEE3B4D5DE1DB517FF27F729A491CD9DF6E7D5DDD61FC4D3BA87EEE3BC93FEE1BA47FEE1BD6949037083FF29F727C559EEA5F6E36907FEE3C527FF69FE9FD5DBEEF7EE5DF7298A4D8B0DEE9DD5DDDDDDDE317249FF6BF727AC4FF6E5DDDBFEE5FF699185F727FF297883F6E7F7275985C4D7EEA5A413CD6BFF27D59DF729BCA5EEE5F727EEE7F6E5AC4FF6A1EEA192D36843E61BFF2770C5CD5BBCD9F6E57AC9F72960C5620B830F81038315724BA20589859B1372477247514369499B8D9B91939559C76207688349458B91BB513881728D821182518A9340C358C5798D724B49038355830FA417A45740816A0961C7A1C5598759879C1F8B11B495830D4903BCD7A41999C55145B451BC17BD17BD1961899BD5CC9B8317BCD5BB51C517CD15A40F934F99C56A09D59750C3724BC519C5196A07CD597A8B7A8B59015145CD55CD9D7A53D59959C559C789457A8B898591059145A41FD59D7ACB93D1D5DDD5DFA9C5A9C5820FDDDB9A09CD53DE1FE619620BA40FEE5B6A099B8DE65D9BCFEE5DEE5DB205EE5FB207EE9D7AC9CD9BA249EE9FF69DF69F81058311FE9FAC61B4E1B4A5B4A5B4A593179391BD27BD279B91C4E7C561C563C51393DBDE1FC5EDC5EDA39B8B0FDE21938D40C1DE23DE23D6274903A413E661A415E661E661035C0192FFBA000004BC0081416400FF03C000F4FFDD0000052D0129702800FF0361FEF8FFCE0000071E009652A900FF03E4FFD5FFE90000065E016A75E800FF02ADFE9CFFBE000013AA080B258E00FF02ADFE9CFFBE00000751FF6F258E00FF0088009A0127000002EDFEAAF7376AFF01A0019100700000040000770D731DFF029D011CFFDD00000494021A2C5BC1FF029EFECBFFEA000007760223218FEAFF0102FE63FFFA000007A3FF7A179125FF02A3FF20FF74000006E3022B17D494FF02A50057FF5A0000058F022A13148CFF02A3FF20FF74000006E3022B17D494FF02A50057FF5A0000058F022A13148CFF029D011CFFDD00000494021A2C5BC1FF02A50057FF5A0000058F022A13148CFF02A3FF20FF74000006E3022B17D494FF029EFECBFFEA000007760223218FEAFF0102FE63FFFA000007A3FF7A179125FF01A0019100700000040000770D731DFF01A0019100700000040000770D731DFF029D011CFFDD00000494021A2C5BC1FF060002040006080ADF00000000000000FF86FFFE0038000001870340C2A52EFFFFB1FFFDFF9D0000015A033EDD9DC6FF0066FFF9FFC10000009903342998D5FFFF3C0168FF73000001B90165981039FFFF830225FF970000018900E0C16410FFFF920222FF680000017B00DBD254B9FFFF6A0093FF4B000001A40278D0C5A4FF0037020BFFAC000000D100B1256AD8FF0046FFFB0062000000BC0335119431FF00B7005300A50000004202B544B943FF00A60085FF7200000054027250D9B0FF0038007F00A8000000C90284AF1C53FF00C70175FFCD0000003801213C3E53FFFF2700BBFF9F000001D101FC961E30FF00D200FE001C0000002F0184640BC0FF00B90180FF63000000450119502EB5FF00A60085FF720000005C01F250D9B0FFFF550166FF20000001A00163C51099FF00B7005300A50000004C021B44B943FFFF6A0093FF4B0000018F0214D0C5A4FF00B90180FF630000004501195225B1FFFF550166FF20000001A00163C60798FF003B0218FFE0000000CC00A80E4262FFFF2700BBFF9F000001DF0234961E30FFFF6A0093FF4B000001970264D0C5A4FFFF86FFFE00380000017B032CC2A52EFF0038007F00A8000000BC0270AF1C53FF0103018D002F00000137026E5143C8FF011900E80082000000AB03856D0131FF00D200FE001C00000127037D640BC0FF00B00195002A000001BD0285B957D7FF0038007F00A8000001C204A0AF1C53FF008F00FB00BC0000018B03A1EF296FFF00B7005300A5000000E304B444B943FFFF2700BBFF9F0000004A01D4961E30FF0038007F00A8000001620252AF1C53FF00D200FE001C000001FF01E1640BC0FF00C70175FFCD0000003801213C3E53FFFF3C0168FF73000001B9016598083BFFFF2700BBFF9F000001D101FC961E30FF0037020BFFAC000000D100B1256AD8FF003B0218FFE0000000CC00A80E4262FF00C70175FFCD0000003801213D454CFFFF550166FF20000001A00163C51099FF00B90180FF630000004501195225B1FFFF920222FF680000017B00DBD254B9FFFF550166FF20000001A00163C60798FFFF3C0168FF73000001B90165981039FFFF830225FF970000018900E0C16410FF011900E80082000000AB03856D0131FF00D200FE001C00000127037D640BC0FF0103018D002F00000137026E5143C8FF00DB0197004F0000017B026F185D48FFFE38026F0060000000F900ACD752B3FFFF19026C0064000001E901582071EBFFFEC8022BFFDC000001F50055F34EA6FFFE84019DFFC2000001F6FFD7C1F49BFFFFC901AAFF9200000367008B2341A2FFFDEF02A2015F0000001301D6225849FFFD40026101980000FF55018CAA1650FFFDBC023C01940000FFEC01D907D971FFFDD002AC010D000000170156E071ECFFFDF2029100BC0000007000FED55DC3FFFD59024901100000FFC000E59309D0FFFDBB0205007B000000930056A1F6B8FFFFF001ED00370000031E01955850F9FFFF8B01FE00AE0000026E01ED1D643BFFFED2015E0099000001F80119DB921CFFFF82018A00FF0000026D022EF7D56FFF002F01C801D70000029F03DC256D20FFFFFE015500C300000328022C35AB42FF002F01C801D70000029F03DCEA9530FF007F010300400000041901C557C83CFFFFAC011DFF69000003990015EDBAA1FF007200EBFFC80000045301160390D7FF007200EBFFC80000045301165F3FDDFFE700000000000000E300100100008000D7000002FFFFFFFFFD5000000600DB08F550000007090240E600000000000000F30000000707F400E700000000000000F548040000090240F20000000003C03CFD1000000600DA80E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FC127E03FFFFFDF8E200001CC8112078D9F3FFFF00000000D9FFFFFF00030400FA000000FFFFFFFF010200400600E858060002040006080A0604020C000E0A0806100004000C0200061210040014040C060414120000121606181A1C001E181C061E2022001C201E06001012001C24200620262200282A0A060A0E28002C0806062E30320022261A0632342E0036383A0101702E0600EA38060002040004020606080A0C000E101206141618001A121006181C14001E2022062218160024161406260628002802000606260400262A2C062C042600042C000628002A00002C2AE700000000000000E300100100000000FD1000000600D680F510000007090250E600000000000000F3000000071FF100E700000000000000F510100000090250F20000000007C03C0101702E0600EBA806000204000004060608060400080402060A0C0E0010121406140C10000A100C06161200001412160618021A0016061C061600060000120206180802000212100602100A001C141606020A1A001E1A0A061E0A0E000E1C1E06141C0C000C1C0E06181A20001820220622241E001E201A06221E1C00221C26061C282A001C2A26060828060006281C061822260018262C06182C08002A2808DF00000000000000008F0103FF440000017B026FFD258EFF00DB0192FFD100000137026EE26F21FF011900F0FF7E000000AB038574FFE3FF008F0103FF440000018B03A1FD258EFF00B7005BFF5B000000E304B444B2C4FF00DB0192FFD1000001BD0285E26F21FF00380087FF58000001C204A0AD24B2FF00D20106FFE400000127037D701822FF002C0224FFFE000000CC00A829339CFF00C7017D00330000",
                            "003801212A2B99FFFF3C0170008D000001B90165A6FAB1FFFF55016E00E0000001A00163C41465FFFF6B020400390000017B00DBC75E2FFF002C0224FFFE000000D100B129339CFF00B90188009D000000450119503546FFFF2700C30061000001D101FC961ED0FF00D20106FFE4000001FF01E1701822FF00380087FF58000001620252AD24B2FFFF2700C300610000004A01D4961ED0FFFF2700C30061000001DF0234961ED0FF00380087FF58000000BC0270AD24B2FFFF860006FFC80000017B032CC1A8CEFFFF6A009B00B50000018F0214CEBF57FFFF6A009B00B5000001970264CEBF57FFFF6B020400390000018900E0C75E2FFF00A6008D008E0000005C01F24AD051FF00B7005BFF5B0000004C021B44B2C4FF00D20106FFE40000002F0184701822FF00B7005BFF5B0000004202B544B2C4FF00460003FF9E000000BC03351990DEFFFF860006FFC8000001870340C1A8CEFF00380087FF58000000C90284AD24B2FF00B7005BFF5B0000004202B544B2C4FF00A6008D008E0000005402724AD051FF00060001003F0000009903341A952FFFFF6A009B00B5000001A40278CEBF57FF00460003FF9E000000BC03351990DEFFFF860006FFC8000001870340C1A8CEFF00060001003F0000015A033E1A952FFFE700000000000000E300100100008000D7000002FFFFFFFFFD5000000600DB08F550000007090240E600000000000000F30000000707F400E700000000000000F548040000090240F20000000003C03CFD1000000600DA80E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FC127E03FFFFFDF8E200001CC8112078D9F3FFFF00000000D9FFFFFF00030400FA000000FFFFFFFF010200400600EF680600020400060408060A0C0E000E08040610121400141618061A1C1200141E16061E14120020222406080C0600060C0A060E04020026282A061E2C16002A2E2606143010001C1A180618161C00162C320632343600383A3C061C32360016321C0636121C00361E12053E383C000000000100700E0600F168060002040006040206040800000A0C06DF0000000000000000AD01670098000001D401A4474345FF004901F3FFB0000001DD00D81771E0FFFF5A018E00530000005C0184A04702FF004901F3FFB00000003B011D1771E0FF0076011EFF320000001F00402E349FFFFF5A018E0053000001C80124A04702FF0076011EFF32000001C701CD2E349FFF00AD01670098000001BD0165474345FF00C8008300740000018602B358E84DFF004901F3FFB0000001C7009A1771E0FF0060FFFE00360000008204252B9829FF00C8008300740000012D043058E84DFFFF6900830098000000BC02A3BBD357FFFF7D012900AF000001720247E30F73FFFF7D012900AF0000010103E9E30F73FF00C80083007400000163027858E84DFF00AD016700980000008402C6474345FFFF7D012900AF000000400218E30F73FFFF5A018E0053000001DB01F7A04702FFFF90FFFFFFBD0000004B0360C1ABC9FFFF2E0088FFF3000000BA028C8EF5DFFF00C00081FF490000014004792FC8A1FFFF880100FFBF0000015D02AFBF0E9DFF00C00081FF49000001D602AF2FC8A1FFFF880100FFBF0000006E0210BF0E9DFFFF880100FFBF000000D50253BF0E9DFFFF2E0088FFF30000008B02BE8EF5DFFFFF5A018E005300000170029BA04702FF0076011EFF32000000BF01722E349FFFFFAAFFFF0057000000460353DB9D38FF0060FFFE0036000000E1037D2B9829FF0046FFFFFF9C000000F0037B1A97CEFFFF90FFFFFFBD0000008C0276C1ABC9FF00C00081FF49000001B6057F2FC8A1FF0046FFFFFF9C0000015A033A1A97CEFFE700000000000000E300100100008000D7000002FFFFFFFFFD5000000600DB08F550000007090240E600000000000000F30000000707F400E700000000000000F548040000090240F20000000003C03CFD1000000600DA80E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FC127E03FFFFFDF8E200001CC8112078D9F3FFFF00000000D9FFFFFF00030400FA000000FFFFFFFF010200400600F318060002040006080A060C0E1000120E0C06141618001A1816061C1E20000422000624181A00261828062A262C002C2628062E300C00323436063236380024281806183A140018263A06102E0C003C2E10053E2E3C00000000010030060600F5180500020400000000DF00000000000000FF6A0093FF4B000001A40278CEBFA9FF0006FFF9FFC10000015A033E1A95D1FFFF86FFFE0038000001870340C1A832FF00B7005300A50000004202B544B23CFF0046FFFB0062000000BC0335199022FF0006FFF9FFC10000009903341A95D1FF00A60085FF720000005402724AD0AFFF0038007F00A8000000C90284AD244EFF00C70175FFCD0000003801212A2B67FFFF2700BBFF9F000001D101FC961E30FF00D200FE001C0000002F01847018DEFF00B90180FF630000004501195035BAFF00A60085FF720000005C01F24AD0AFFFFF550166FF20000001A00163C4149BFF00B7005300A50000004C021B44B23CFFFF6A0093FF4B0000018F0214CEBFA9FFFF6B01FCFFC70000017B00DBC75ED1FF002C021C0002000000D100B1293364FF002C021C0002000000CC00A8293364FFFF6B01FCFFC70000018900E0C75ED1FFFF3C0168FF73000001B90165A6FA4FFFFF2700BBFF9F000001DF0234961E30FFFF6A0093FF4B000001970264CEBFA9FFFF86FFFE00380000017B032CC1A832FF0038007F00A8000000BC0270AD244EFF00DB018A002F00000137026EE26FDFFF011900E80082000000AB038574FF1DFF00D200FE001C00000127037D7018DEFF00DB018A002F000001BD0285E26FDFFF0038007F00A8000001C204A0AD244EFF008F00FB00BC0000018B03A1FD2572FF00B7005300A5000000E304B444B23CFFFF2700BBFF9F0000004A01D4961E30FF0038007F00A8000001620252AD244EFF00D200FE001C000001FF01E17018DEFF00C70175FFCD0000003801212A2B67FFFF3C0168FF73000001B90165A6FA4FFFFF2700BBFF9F000001D101FC961E30FFFF550166FF20000001A00163C4149BFF00B90180FF630000004501195035BAFF002C021C0002000000D100B1293364FFFF6B01FCFFC70000017B00DBC75ED1FF002C021C0002000000CC00A8293364FF011900E80082000000AB038574FF1DFF00B7005300A5000000E304B444B23CFF00D200FE001C00000127037D7018DEFF0038007F00A8000001C204A0AD244EFF00DB018A002F000001BD0285E26FDFFF008F00FB00BC0000018B03A1FD2572FF00DB018A002F00000137026EE26FDFFF008F00FB00BC0000017B026FFD2572FFE700000000000000E300100100008000D7000002FFFFFFFFFD5000000600DB08F550000007090240E600000000000000F30000000707F400E700000000000000F548040000090240F20000000003C03CFD1000000600DA80E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FC127E03FFFFFDF8E200001CC8112078D9F3FFFF00000000D9FFFFFF00030400FA000000FFFFFFFF010200400600F668060002040006080A060C0A00000A0C060604060E00101214061610140016181A061418160004080606141C1800181E1A06161A200020221606242628002A2C2E061A1E12002E302A0632343600383A3C053C3A3E00000000010130260600F868060002040006080A060C0A0800060E1006120C08000806140616181A001A1C1E0618162000162224DF00000000000000FF5A018EFFAD0000005C0184A047FEFF004901F30050000001DD00D8177120FF00AD0167FF68000001D401A44743BBFFFF5A018EFFAD000001BC012AA047FEFF0076011E00CE0000001300452E3461FF004901F300500000002F0122177120FF00C80083FF8C0000018602B358E8B3FF00AD0167FF68000001BD01654743BBFF0076011E00CE000001C701CD2E3461FF004901F30050000001C7009A177120FFFF690083FF68000000BC02A3BBD3A9FF00C80083FF8C0000012D043058E8B3FF0060FFFEFFCA0000008204252B98D7FFFF7D0129FF51000001720247E30F8DFF00AD0167FF680000008402C64743BBFF00C80083FF8C00000163027858E8B3FFFF7D0129FF510000010103E9E30F8DFFFF7D0129FF51000000400218E30F8DFFFF5A018EFFAD000001DB01F7A047FEFFFF2E0088000D000000BA028C8EF521FFFF90FFFF00430000004B0360C1AB37FFFF88010000410000015D02AFBF0E63FF00C0008100B70000014004792FC85FFFFF88010000410000006E0210BF0E63FF00C0008100B7000001D602AF2FC85FFFFF5A018EFFAD00000170029BA047FEFFFF2E0088000D0000008B02BE8EF521FFFF8801000041000000D50253BF0E63FF0076011E00CE000000BF01722E3461FFFFAAFFFFFFA9000000460353DB9DC8FF0060FFFEFFCA000000E1037D2B98D7FF0046FFFF0064000000F0037B1A9732FF0046FFFF00640000015A033A1A9732FF00C0008100B7000001B6057F2FC85FFFFF90FFFF00430000008C0276C1AB37FFE700000000000000E300100100008000D7000002FFFFFFFFFD5000000600DB08F550000007090240E600000000000000F30000000707F400E700000000000000F548040000090240F20000000003C03CFD1000000600DA80E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FC127E03FFFFFDF8E200001CC8112078D9F3FFFF00000000D9FFFFFF00030400FA000000FFFFFFFF010200400600FAD8060002040006080A060C0E1000100E12061416180016141A061C1E2000042200061A142400261428062A282C0026282A06102E3000323436063832360014262406183A14003A28140610300C000C303C053C303E00000000010030060600FCD80500020400000000DF00000000000000FF86FFFE0038000001870340C2A52EFFFFB1FFFDFF9D0000015A033EDD9DC6FF0066FFF9FFC10000009903342998D5FFFF3C0168FF73000001B90165981039FFFF830225FF970000018900E0C16410FFFF920222FF680000017B00DBD254B9FFFF6A0093FF4B000001A40278D0C5A4FF0037020BFFAC000000D100B1256AD8FF0046FFFB0062000000BC0335119431FF00B7005300A50000004202B544B943FF00A60085FF7200000054027250D9B0FF0038007F00A8000000C90284AF1C53FF00C70175FFCD0000003801213C3E53FFFF2700BBFF9F000001D101FC961E30FF00D200FE001C0000002F0184640BC0FF00B90180FF63000000450119502EB5FF00A60085FF720000005C01F250D9B0FFFF550166FF20000001A00163C51099FF00B7005300A50000004C021B44B943FFFF6A0093FF4B0000018F0214D0C5A4FF00B90180FF630000004501195225B1FFFF550166FF20000001A00163C60798FF003B0218FFE0000000CC00A80E4262FFFF2700BBFF9F000001DF0234961E30FFFF6A0093FF4B000001970264D0C5A4FFFF86FFFE00380000017B032CC2A52EFF0038007F00A8000000BC0270AF1C53FF0103018D002F00000137026E5143C8FF011900E80082000000AB03856D0131FF00D200FE001C0000",
                            "0127037D640BC0FF00B00195002A000001BD0285B957D7FF0038007F00A8000001C204A0AF1C53FF008F00FB00BC0000018B03A1EF296FFF00B7005300A5000000E304B444B943FFFF2700BBFF9F0000004A01D4961E30FF0038007F00A8000001620252AF1C53FF00D200FE001C000001FF01E1640BC0FF00C70175FFCD0000003801213C3E53FFFF3C0168FF73000001B9016598083BFFFF2700BBFF9F000001D101FC961E30FF0037020BFFAC000000D100B1256AD8FF003B0218FFE0000000CC00A80E4262FF00C70175FFCD0000003801213D454CFFFF550166FF20000001A00163C51099FF00B90180FF630000004501195225B1FFFF920222FF680000017B00DBD254B9FFFF550166FF20000001A00163C60798FFFF3C0168FF73000001B90165981039FFFF830225FF970000018900E0C16410FF011900E80082000000AB03856D0131FF00D200FE001C00000127037D640BC0FF0103018D002F00000137026E5143C8FF00DB0197004F0000017B026F185D48FFFE38026F0060000000F900ACD752B3FFFF19026C0064000001E901582071EBFFFEC8022BFFDC000001F50055F34EA6FFFE84019DFFC2000001F6FFD7C1F49BFFFFC901AAFF9200000367008B2341A2FFFDEF02A2015F0000001301D6225849FFFD40026101980000FF55018CAA1650FFFDBC023C01940000FFEC01D907D971FFFDD002AC010D000000170156E071ECFFFDF2029100BC0000007000FED55DC3FFFD59024901100000FFC000E59309D0FFFDBB0205007B000000930056A1F6B8FFFFF001ED00370000031E01955850F9FFFF8B01FE00AE0000026E01ED1D643BFFFED2015E0099000001F80119DB921CFFFF82018A00FF0000026D022EF7D56FFF002F01C801D70000029F03DC256D20FFFFFE015500C300000328022C35AB42FF002F01C801D70000029F03DCEA9530FF007F010300400000041901C557C83CFFFFAC011DFF69000003990015EDBAA1FF007200EBFFC80000045301160390D7FF007200EBFFC80000045301165F3FDDFFE700000000000000E300100100008000D7000002FFFFFFFFFD5000000600DB08F550000007090240E600000000000000F30000000707F400E700000000000000F548040000090240F20000000003C03CFD1000000600DA80E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FC127E03FFFFFDF8E200001CC8112078D9F3FFFF00000000D9FFFFFF00030400FA000000FFFFFFFF010200400600FE28060002040006080A0604020C000E0A0806100004000C0200061210040014040C060414120000121606181A1C001E181C061E2022001C201E06001012001C24200620262200282A0A060A0E28002C0806062E30320022261A0632342E0036383A0101702E06010008060002040004020606080A0C000E101206141618001A121006181C14001E2022062218160024161406260628002802000606260400262A2C062C042600042C000628002A00002C2AE700000000000000E300100100000000FD1000000600D680F510000007090250E600000000000000F3000000071FF100E700000000000000F510100000090250F20000000007C03C0101702E0601017806000204000004060608060400080402060A0C0E0010121406140C10000A100C06161200001412160618021A0016061C061600060000120206180802000212100602100A001C141606020A1A001E1A0A061E0A0E000E1C1E06141C0C000C1C0E06181A20001820220622241E001E201A06221E1C00221C26061C282A001C2A26060828060006281C061822260018262C06182C08002A2808DF000000000000000365FFA100250000FFFC002E02037732023BFF570000000000000200B45C00FF0271FF220000000002000200FB8900320365FFA1FFDB0000FFFC002E0203897E035DFF4E00000000000F010BABAC007E0365FFA1FFDB0000FFC000100203897E03EBFEA7FFDD0000020E00BDF2FA899203EBFEA700230000020E00BDF2FA77320365FFA100250000FFC0001002037732030CFFB700000000FEAA008289F200FC035DFF4E00000000026500A0ABAC007E03DEFE64000000000242019FA9AE00840407FEEA0000000001FEFFA04E5B0032FBEAFEA700230000020E00BD0EFA7732FBF6FE64000000000242019F58AF0032FC75FF4E00000000000F010B55AC0032FC6DFFA1FFDB0000FFC00010FE038986FBEAFEA7FFDD0000020E00BD0EFA8960FBCDFEEA0000000001FEFFA0B15A00FFFC6DFFA100250000FFC00010FE0377320498FE0200000000008D0299E38C003203DEFE64000000000232017EA9AE008403EBFEA7FFDD0000024D00D7F2FA899203EBFEA700230000024D00D7F2FA773204FFFE4700000000007800F877F500320487FF0400000000016D000B0D77008A0407FEEA00000000024900324E5B003204FFFE47000000000000020077F50032FC6DFFA100250000FFFC002EFE037732FC75FF4E00000000026500A055AC0032FD61FF22000000000200020005890032FC6DFFA1FFDB0000FFFC002EFE038986FC6DFFA100250000FFFC002EFE037732FD97FF5700000000000002004C5C0032FCC6FFB700000000FEAA008277F20032FC6DFFA1FFDB0000FFFC002EFE038986FD61FF22000000000200020005890032FB3CFE0200000000008D02991D8C0032FAD6FE47000000000000020089F500FFFBEAFEA7FFDD0000024D00D70EFA8960FBF6FE64000000000232017E58AF0032FBEAFEA700230000024D00D70EFA7732FB4EFF0400000000016D000BF37700B6FBCDFEEA0000000002490032B15A00FFFAD6FE4700000000007800F889F500FFFF48026F00000000FEE30902BC6200FFFE9201B800000000FFEE066EB75F00FFFEDF015B00280000018607470E16753202E900C200000000002E00AC525700320297009E00280000012101AAFEF777320365FFA10025000002DBFEAE020377320365FFA1FFDB000002DBFEAE0203897E0297009EFFD80000012101AAFFFE8980FC6DFFA1FFDB000002DBFEAEFE038986FD3B009EFFD80000012101AA01FE897CFCC6FFB7000000000328FFD977F20032005E0229001B000000010978AAC53B7200F3015BFFD8000001860747DD118FCA00F3015B0028000001860747F2167532005E0229FFE5000000010978C4DB9FBA014001B800000000FFEE066E495F0032008A026F00000000FEE309024462003200E2011F00360000025A076ADA1A6E480407FEEA000000000404FC5D4E5B0032FCE900C200000000002E00ACAE5700FFFBCDFEEA000000000404FC5DB15A00FFFC6DFFA10025000002DBFEAEFE037732FD3B009E00280000012101AA02F77732FEDF015BFFD800000186074723118F4CFEF1011FFFCA0000025A076A0FF88A5EFE24007C0000000003050495209ACA32FEF100A80000000003C7073F0E933032FEF1011F00360000025A076A1EF87332030CFFB7000000000328FFD989F200FC01AE007C0000000003050495E09ACA3200E200A80000000003C7073FF58E243200E2011FFFCA0000025A076AEEF38B9400E200A80000000003C907C8F58E2432FEF1011FFFCA000006070DED0FF88A5E00E2011FFFCA000002C80823EEF38B94FEF100A80000000007080D910E933032FEF1011F0036000005840E1C1EF8733200E2011F0036000002450852DA1A6E48FCC6FFB7000000000328FFD977F20032FE24007C0000000003050495209ACA32FD3B009E00280000012101AA02F77732FD3B009EFFD80000012101AA01FE897CFEF1011F00360000025A076A1EF87332FEDF015B00280000018607470E1675320365FFA10025000002DBFEAE020377320297009E00280000012101AAFEF77732030CFFB7000000000328FFD989F200FCFF740229001B00000001097856C53B32FF48026F00000000FEE30902BC6200FFFF740229FFE50000000109783CDB9F32FEDF015BFFD800000186074723118F4CFC6DFFA10025000002DBFEAEFE037732FCE900C200000000002E00ACAE5700FFFE9201B800000000FFEE066EB75F00FFFF61FFC8FFAF0000009C0250A6F1B3FAFF62FFDE00280000008C025391EB26D0FF2E004BFF62000000470285B8CEAEBAFF970089FF3F000000460239FBF7898000250095FF5B0000FFF101E21DE98E32FFF60036FF5D0000003B01F61BDB9132FF030050FFF00000004902A4ABAFE990FF970089FF3F0000000D0244FBF78980FF3F0120FF5A0000FFAE0297DE018DBAFFAA0124FF3E0000FF9E024CFAF58980007300D7003C0000FFB801B46311BF32007A011200070000FF8E01B775071632009900F9003B0000FF9B019E37E39A3200A90119FFD60000FF82019762E9413200BB0111FF9B0000FF86018973E0FB3200D60170FFB60000FF3F018368CF2132009901B1FF7B0000FF1901B73F35A93200DF019CFFB00000FF1F01827303DF3200C30174FF730000FF3F01915E06B6320094004E008100000014018A4ED85132005D0027008D0000003701AB1DC66432006DFFF2004C0000005A01994CB5363200A800F600A30000FF9B01932B216B32003F00E300950000FFB701DAEA2071320068009700A20000FFE701B312F27532000EFFDD00560000007601D930C55C3200140005007E0000005901DA0BBE6332005DFFB5000C00000087019C69DC2B320041FFBEFFB80000008501B15AE0B832007F000000190000004E018F6FD7EF32000AFFB600600000009201D623F8723200A20111FF600000FF89019B47EBA232007A0123FF520000FF8201B92CE69432008F0182FF5F0000FF3C01B72F149436008F0088FF960000FFEC019663DAC932FF2E004BFF62000000470285B8CEAEBAFF970089FF3F0000000D0244FBF78980FF970038FF4B000000460239F2D0936600420068FF6D0000000D01C72CDD97320041FFBEFFB80000008501B15AE0B8320002FFD6FF8F0000007C01E128EF9132FFF60036FF5D0000003B01F61BDB913200610112FF4F0000FF9101C82DE1963200250095FF5B0000FFF101E21DE98E32FFAA0124FF3E0000FF9E024CFAF58980003E0114FF360000FF9401E11CDB9232FFA0FFCAFF91000000920224F1EF8B8A007F000000190000004E018F6FD7EF3200750027FFA900000034019A63D5CC32006DFFF2004C0000005A01994CB53632005D0027008D0000003701AB1DC66432000EFFDD00560000007601D930C55C3200B00061005900000003017972DEF232005DFFB5000C00000087019C69DC2B32008F0054FFE400000011018E76ECFB320091006B00280000000001906DFBCF3200BB0111FF9B0000FF86018973E0FB320094004E008100000014018A4ED8513200BD00B000660000FFCA017A76ED0932009C00B200920000FFCD01924FE65632009200B400370000FFCD01996014BB3200B000B2004A0000FFCA018457FDAE3200C30174FF730000FF3F01915E06B63200BA0131005B0000FF70018E4560F13200A800F600A30000FF9B01932B216B3200C800FF00700000FF91017E7501183200BB0105004F0000FF8E018861F6BA3200B70180FFF10000FF38019B59B1083200D2019DFFFB0000FF20018C63C0123200BE017600240000FF3E019455B6DA32009900F9003B0000FF9B019E37E39A32009200B400370000FFCD01996014BB32007300D7003C0000FFB801B46311BF32006F013200640000",
                            "FF7901C3F96D3186003F00E300950000FFB701DAEA20713200A200E9FFCE0000FFA5019571F0223200BB0111FF9B0000FF86018973E0FB3200A90119FFD60000FF82019762E9413200BD00B000660000FFCA017A76ED093200B000B2004A0000FFCA018457FDAE3200610112FF4F0000FF9101C82DE19632007A0123FF520000FF8201B92CE69432005E0057FF800000001501B13ADC9E3200170050009A0000002301E3FBE7753200140005007E0000005901DA0BBE6332005D0027008D0000003701AB1DC66432FFE3FFED00740000007001F901CF6D32FFB8FFCF004F0000008B0213D2EE6D32000AFFB600600000009201D623F87232FF62FFDE00280000008C025391EB26D00078012800240000FF7F01BB2DB2B232006D0183FFFE0000FF4001CFCDAABF6A00A20166FFF60000FF4D01A61F901B32000EFFDD00560000007601D930C55C3200750027FFA900000034019A63D5CC32008F0054FFE400000011018E76ECFB32007F000000190000004E018F6FD7EF3200C30174FF730000FF3F01915E06B63200D60170FFB60000FF3F018368CF213200BB0111FF9B0000FF86018973E0FB32005E0057FF800000001501B13ADC9E3200420068FF6D0000000D01C72CDD973200610112FF4F0000FF9101C82DE196320041FFBEFFB80000008501B15AE0B8320068009700A20000FFE701B312F27532005D0027008D0000003701AB1DC664320094004E008100000014018A4ED85132006DFFF2004C0000005A01994CB5363200B00061005900000003017972DEF232008F0088FF960000FFEC019663DAC932009200B400370000FFCD01996014BB320091006B00280000000001906DFBCF3200A200E9FFCE0000FFA5019571F02232007A0123FF520000FF8201B92CE6943200BE017600240000FF3E019455B6DA320078012800240000FF7F01BB2DB2B23200B70180FFF10000FF38019B59B1083200250095FF5B0000FFF101E21DE98E32003E0114FF360000FF9401E11CDB923200B000B2004A0000FFCA018457FDAE3200BB0105004F0000FF8E018861F6BA32009900F9003B0000FF9B019E37E39A32007A011200070000FF8E01B77507163200170050009A0000002301E3FBE77532009C00B200920000FFCD01924FE65632FF970038FF4B000000460239F2D09366FF970089FF3F0000000D0244FBF78980FFF60036FF5D0000FFF101E21BDB9132007A011200070000FF8E01B775071632009600A5FFFF0000FFB901977316173200A200E9FFCE0000FFA5019571F02232007300D7003C0000FFAD01A86311BF32009200B400370000FFCD01996014BB32FFA0FFCAFF91000000920224F1EF8B8AFF61FFC8FFAF0000009C0250A6F1B3FA00C30174FF730000FF3F01915E06B632008F0182FF5F0000FF3C01B72F149436009901B1FF7B0000FF1901B73F35A932FF2E004BFF62000000470285B8CEAEBA00BA0131005B0000FF70018E4560F13200BB0105004F0000FF8E018861F6BA3200A90119FFD60000FF82019762E9413200D60170FFB60000FF3F018368CF213200A0015EFFE90000FF5301A639D56032FFF60036FF5D0000003B01F61BDB913200250095FF5B0000FFF101E21DE98E3200420068FF6D0000000D01C72CDD9732006F013200640000FF7901C3F96D31860021011600290000FF9701F6FA70D6C0009C00B200920000FFCD01924FE6563200BD00B000660000FFCA017A76ED093200C800FF00700000FF91017E7501183200A800F600A30000FF9B01932B216B32005DFFB5000C00000087019C69DC2B32007F000000190000004E018F6FD7EF32006DFFF2004C0000005A01994CB5363200D60170FFB60000FF3F018368CF213200B70180FFF10000FF38019B59B1083200A20166FFF60000FF4D01A61F901B3200B000B2004A0000FFCA018457FDAE32009200B400370000FFCD01996014BB32009900F9003B0000FF9B019E37E39A3200BB0105004F0000FF8E018861F6BA3200A0015EFFE90000FF5301A639D560320078012800240000FF7F01BB2DB2B23200C30174FF730000FF3F01915E06B63200DF019CFFB00000FF1F01827303DF32006D0183FFFE0000FF4001CFCDAABF6A0064017DFFF00000FF4601D5EBC2643200250095FF5B0000FFF101E21DE98E32FF970089FF3F0000000D0244FBF78980FFAA0124FF3E0000FF9E024CFAF58980005E0057FF800000001501B13ADC9E3200750027FFA900000034019A63D5CC320041FFBEFFB80000008501B15AE0B83200BD00B000660000FFCA017A76ED093200B00061005900000003017972DEF232008F0054FFE400000011018E76ECFB32009600A5FFFF0000FFB901977316173200A200E9FFCE0000FFA5019571F0223200BE017600240000FF3E019455B6DA32009E013500450000FF7001A24DBBC53200A90119FFD60000FF82019762E94132007A011200070000FF8E01B77507163200D2019DFFFB0000FF20018C63C01232008F0182FF5F0000FF3C01B72F14943600A20111FF600000FF89019B47EBA23200170050009A0000002301E3FBE775320068009700A20000FFE701B312F27532003F00E300950000FFB701DAEA20713200A0015EFFE90000FF5301A639D560320064017DFFF00000FF4601D5EBC26432007A011200070000FF8E01B77507163200A90119FFD60000FF82019762E9413200D2019DFFFB0000FF20018C63C0123200B70180FFF10000FF38019B59B1083200D60170FFB60000FF3F018368CF21320042014E00210000FF6B01E7E4D1967E006D0183FFFE0000FF4001CFCDAABF6A0078012800240000FF7F01BB2DB2B232009C00B200920000FFCD01924FE6563200A800F600A30000FF9B01932B216B320017FE8FFF780000016001A55BF3B3320002FFD6FF8F0000007C01E128EF91320041FFBEFFB80000008501B15AE0B832FFC1FEA9FF4E0000015901E6180B8B5EFFDCFE09FF62000001C501BD1FA4BA32005DFFB5000C00000087019C69DC2B320032FE7FFFD900000167019072DE0B32FFE3FE8700570000016C01C92DDB6832FF62FFDE00280000008C025391EB26D0FF0EFEA8000E0000017202639AFE3EC2FFB8FFCF004F0000008B0213D2EE6D32FF1FFEC9FF8D00000158025C9719CDFF000AFFB600600000009201D623F87232FF58FEC2FF57000001560233D61692DAFFA0FFCAFF91000000920224F1EF8B8AFF98FE20FF44000001BF01F0F2A9B032FFE3FE8700570000016C01C92DDB6832FF6FFE0B0005000001D3020ADA9B3432FFC9FDF8000F000001D401C822983032FF58FEC2FF57000001560233D61692DAFF98FE20FF44000001BF01F0F2A9B032FF3FFE2DFF55000001C10230C1E79ECE0032FE7FFFD900000167019072DE0B32FF1FFEC9FF8D00000158025C9719CDFFFFC1FEA9FF4E0000015901E6180B8B5EFFDCFE09FF62000001C501BD1FA4BA32FFF4FDFDFFB0000001CB01AA59B2F1320017FE8FFF780000016001A55BF3B332FF0AFE2FFF84000001C702569FC1E2BCFF0EFEA8000E0000017202639AFE3EC2FF1DFE23FFCE000001CE0247BAAA2C46FFA0FFCAFF91000000920224F1EF8B8A0002FFD6FF8F0000007C01E128EF9132FF61FFC8FFAF0000009C0250A6F1B3FAFF62FFDE00280000008C025391EB26D0FFB8FFCF004F0000008B0213D2EE6D32FF87FEA4005700000164020DD8EA6E32000AFFB600600000009201D623F872320041FFBEFFB80000008501B15AE0B832FF6FFE0B0005000001560157DA9B3432FFDCFE09FF620000012801001FA4BA32FFC9FDF8000F00000163015722983032FFF4FDFDFFB000000145012659B2F132FF98FE20FF440000011400F6F2A9B032FF1DFE23FFCE0000013A0141BAAA2C46FF0AFE2FFF840000011E011D9FC1E2BCFF3FFE2DFF55000001110103C1E79ECEFBEAFEA7FFDD0000024D00D70EFA8960FBF6FE64000000000232017E58AF0032FB3CFE0200000000008D02991D8C0032FB4EFF0400000000016D000BF37700B6FAD6FE4700000000007800F889F500FFFBEAFEA700230000024D00D70EFA7732FBCDFEEA0000000002490032B15A00FFFAD6FE47000000000000020089F500FFFD61FF22000000000200020005890032FD97FF5700000000000002004C5C0032FC6DFFA100250000FFFC002EFE037732FC6DFFA1FFDB0000FFFC002EFE038986FCC6FFB700000000FEAA008277F20032FC75FF4E00000000026500A055AC003200E200A80000000000000200F58E243200E2011F003600000000016CDA1A6E48FEF1011F003600000200016C1EF87332FEF100A800000000020002000E933032FEF1011FFFCA00000200019E0FF88A5E00E2011FFFCA00000000019EEEF38B9403EBFEA7FFDD0000024D00D7F2FA899204FFFE47000000000000020077F500320498FE0200000000008D0299E38C0032035DFF4E00000000000F010BABAC007E03DEFE64000000000242019FA9AE008403EBFEA700230000020E00BDF2FA773203EBFEA700230000024D00D7F2FA773203DEFE64000000000232017EA9AE00840407FEEA00000000024900324E5B00320487FF0400000000016D000B0D77008A04FFFE4700000000007800F877F500320407FEEA0000000001FEFFA04E5B003203EBFEA7FFDD0000020E00BDF2FA89920365FFA1FFDB0000FFC000100203897EFBEAFEA700230000020E00BD0EFA7732FBF6FE64000000000242019F58AF0032FC75FF4E00000000000F010B55AC0032FC6DFFA1FFDB0000FFC00010FE038986FBEAFEA7FFDD0000020E00BD0EFA8960FBCDFEEA0000000001FEFFA0B15A00FFFC6DFFA100250000FFC00010FE0377320365FFA100250000FFC000100203773203EBFEA700230000020E00BDF2FA773203DEFE64000000000242019FA9AE0084035DFF4E00000000000F010BABAC007E0271FF220000000002000200FB890032035DFF4E00000000026500A0ABAC007E0365FFA100250000FFFC002E020377320365FFA1FFDB0000FFFC002E0203897E030CFFB700000000FEAA008289F200FC023BFF570000000000000200B45C00FFFEF1011FFFCA00000200019E0FF88A5E00E2011F003600000000016CDA1A6E4800E2011FFFCA00000000019EEEF38B94FEF1011F003600000200016C1EF87332005E0229000000000001097890D600D600F3015BFFD8000001860747DD118FCA00F3015B0028000001860747F21675320297009E00280000012101AAFEF7773201AE007C0000000003050495E09ACA32030CFFB7000000000328FFD989F200FC0365FFA1FFDB000002DBFEAE0203897E0297009EFFD80000012101AAFFFE898002E900C200000000002E00AC525700320365FFA10025000002DBFEAE02037732FF48026F00000000FEE30902BC6200FFFE9201B800000000FFEE066EB75F00FFFEDF015B00280000018607470E167532FEDF015BFFD800000186074723118F4CFCE900C200000000002E00ACAE5700FFFD3B009EFFD80000012101AA01FE897CFD3B009E00280000012101AA02F77732FEF1011F00360000025A076A1EF8733200E2011FFFCA0000025A076AEEF38B9400E200A80000000003C7073FF58E243200E2011F00360000025A076ADA1A6E48FE24007C0000000003050495209ACA32FCC6FFB7000000000328FFD977F20032FEF1011FFFCA0000025A076A0FF88A5EFEF100A80000000003C7073F0E933032FC6DFFA100250000",
                            "02DBFEAEFE037732FC6DFFA1FFDB000002DBFEAEFE038986FBCDFEEA000000000404FC5DB15A00FFFF740229000000000001097870D60032FF740229000000000001097870D60032008A026F00000000FEE30902446200320407FEEA000000000404FC5D4E5B003200F3015BFFD8000001860747DD118FCA005E0229000000000001097890D600D6008A026F00000000FEE309024462003200F3015B0028000001860747F216753200E2011F00360000025A076ADA1A6E480297009E00280000012101AAFEF7773202E900C200000000002E00AC525700320297009EFFD80000012101AAFFFE8980014001B800000000FFEE066E495F003200F3015BFFD8000001830825DD118FCA00E2011FFFCA0000024E0815EEF38B9400E2011F00360000024E0815DA1A6E4800F3015B0028000001830825F2167532FEF1011F00360000024E08151EF87332FEF1011FFFCA0000024E08150FF88A5EFEDF015BFFD800000183082523118F4CFEDF015B00280000018308250E167532006D020500880000FFFE006738A83A3200A90229004A000000EE00E900AD563200CE025D007D000000C9019E4AD55332008501F50018000001130034178B003200F6028800180000020002006C33003200B8028800D500000000020059045032FFFE01A5001800000035FED1FF890032FFFE020500AD0000012F006608A54D32FFFE01A5001800000037FED1FF890032006D020500880000FFFC006638A83A32FFFE02880113000002000200FBFA773200B8028800D500000000020059045032FF6702050018000001210067ABAC0080FFFE01A5001800000019FECFFF890032FF8D020500880000FFF10067CDA84032FF42028800D5000000000200A3F64AA2FF0402880018000002000200890F00FFFF8D020500880000FFEF0068CDA84032FFFE01A500180000001FFED2FF890032FFFE020500AD00000122006808A54D32FF42028800D5000000000200A3F64AA2FFFE035D001800000003FD18007700A0FFFE02880113000000000200FBFA773200B8028800D500000200020059045032FFFE035D00180000FFFEFD2B007700A000F6028800180000000002006C330032FFFE035D00180000FFFEFD17007700A0FF0402880018000000000200890F00FFFF42028800D5000002000200A3F64AA2FFFE035D001800000011FD17007700A0FFFE02880113000000000200FBFA7732FF8D0205FFA80000FFEF0068CDA8C066FF420288FF5B000000000200A3F6B6FFFFFE0288FF1C000002000200FBFA8982FFFE0205FF8200000122006808A5B332FFFE01A500180000001FFED2FF890032006D0205FFA80000FFFC006638A8C632FFFE01A5001800000037FED1FF890032FFFE0205FF820000012F006608A5B33200B80288FF5B0000000002005904B032FF6702050018000001210067ABAC0080FF0402880018000002000200890F00FFFF420288FF5B000000000200A3F6B6FFFF8D0205FFA80000FFF10067CDA8C066FFFE01A5001800000019FECFFF890032006D0205FFA80000FFFE006738A8C632008501F50018000001130034178B0032FFFE01A5001800000035FED1FF89003200B80288FF5B0000000002005904B03200F6028800180000020002006C33003200CE025DFFB3000000C9019E4AD5AD3200A90229FFE5000000EE00E900ADAA32FF420288FF5B000002000200A3F6B6FFFF0402880018000000000200890F00FFFFFE035D00180000FFFEFD17007700A0FFFE0288FF1C000000000200FBFA8982FFFE035D001800000011FD17007700A000B80288FF5B0000020002005904B032FFFE0288FF1C000000000200FBFA8982FFFE035D001800000003FD18007700A000F6028800180000000002006C330032FFFE035D00180000FFFEFD2B007700A0FF4802AA00180000FFF4012789F900FFFF48046C0018000000000200880000FFFF9803B1FFA50000006301B6ABFDACFFFF9803B1008A0000006301B6ABFE5494000802AA00DA000000ED014D05FC77320008035700DA000000F201A00B017732000802AAFF56000000ED014D05FC897200080357FF56000000F201A00AFE896C00A90229004A000000B6004900AD5632008501F50018000000C30107178B003200BF01FD003700000121008ED5C55E3200E401AC0018000001E301089BBF00AC00BF01FDFFF900000121008ED5C5A288008501F50018000000C30107178B003200A90229FFE5000000B6004900ADAA3200E401AC0018000001E301089BBF00AC00C802AA0018000001E701727800003200C803190018000001EA01A877FE0032FEDF00940050000000EE0143F7DD723200DF004B001A000008EAF9740E8A0F3200C8007B0050000008D5FB200AEF7632FEC2007D001A0000005100EBD492EF32012800EBFFE7000001CE008FF8FE898C016700EA001A000000D3000677FD0332013C0078001A00000238FEE35BB30232012800EB0048000001CE008FF6FF7732FE8700FAFFDB00000015053FC0039BEAFED401A2FFDC000002560947CE1896E6FEFD0148FFDF00000279062F49FFA132FEFD0148005000000279062F49FF5F32FE8700FA005500000015053FCEF86C40FF0B00D600180000020C02B3623FE83200C8007BFFDF000008D5FB2007EA8B5CFEDF0094FFDF000000EE0143F3D2936600F200E50018000002B200EF943400FF00FC013300180000020B01E289F800FF00F80204FFD9000000CA04A90EE48D4C0117019CFFE7000000F80308FBFA898400E401AC0018000001AA03AA9BBF00AC0117019C0049000000F80308F8F9773200C700B50018000003AC00A9D66E14D800C8007B005000000403FFE70AEF7632010800A5004C000002BFFFEAEE177432010800A5FFE3000002BFFFEAEF168CB200C8007BFFDF00000403FFE707EA8B5C00F802040056000000CA04A90DE47432014601B4001A0000001402F4741C023200DF004B001A000003F7FF120E8A0F3200CE025D007D000000E4062D4AD5533200F6028800180000FFFE06696C33003200F80204FFD9000000CA04A90EE48D4C00CE025DFFB2000000E4062D4AD5AD3200F6028800180000FFFE06696C33003200F802040056000000CA04A90DE47432014601B4001A0000001402F4741C0232FF0B00D600180000020C02B3623FE83200C700B5001800000925FCCED66E14D800C8007BFFDF000008D5FB2007EA8B5CFEDF00940050000000EE0143F7DD723200C8007B0050000008D5FB200AEF763200F200E50018000002B200EF943400FF012800EBFFE7000001CE008FF8FE898C010800A5FFE3000002BFFFEAEF168CB200A90229FFE5000001CA05CD00ADAA3200BF01FDFFF9000001BD050BD5C5A2880117019CFFE7000000F80308FBFA8984016700EA001A000000D3000677FD033200CE025D007D000000E4062D4AD5533200A90229004A000001CA05CD00AD563200BF01FD0037000001BD050BD5C55E320117019C0049000000F80308F8F9773200BF01FD0037000001BD050BD5C55E3200E401AC0018000001AA03AA9BBF00AC00FC013300180000020B01E289F800FF012800EB0048000001CE008FF6FF7732FEFD0148005000000279062F49FF5F32FED401A20054000002560947BA1F5C92FE8700FA005500000015053FCEF86C4000BF01FDFFF9000001BD050BD5C5A288FEB701C2001A0000020B0A85932EEDFFFE8700FAFFDB00000015053FC0039BEAFE6200F0001A0000FF6A055D8DE40FE0FEB701C2001A0000020B0A85932EEDFFFED401A2FFDC000002560947CE1896E6FE8700FAFFDB00000015053FC0039BEAFED401A20054000002560947BA1F5C92FE6200F0001A0000FF6A055D8DE40FE0FE8700FA005500000015053FCEF86C40FEDF0094FFDF000000EE0143F3D29366FEC2007D001A0000005100EBD492EF32FEDF00940050000000EE0143F7DD723200C8007BFFDF000008D5FB2007EA8B5C00DF004B001A000008EAF9740E8A0F32FF61FFC8FFAF0000009C0250A6F1B3FAFF62FFDE00280000008C025391EB26D0FF2E004BFF62000000470285B8CEAEBAFF970089FF3F000000460239FBF7898000250095FF5B0000FFF101E21DE98E32FFF60036FF5D0000003B01F61BDB9132FF030050FFF00000004902A4ABAFE990FF970089FF3F0000000D0244FBF78980FF3F0120FF5A0000FFAE0297DE018DBAFFAA0124FF3E0000FF9E024CFAF58980007300D7003C0000FFB801B46311BF32007A011200070000FF8E01B775071632009900F9003B0000FF9B019E37E39A3200A90119FFD60000FF82019762E9413200BB0111FF9B0000FF86018973E0FB3200D60170FFB60000FF3F018368CF2132009901B1FF7B0000FF1901B73F35A93200DF019CFFB00000FF1F01827303DF3200C30174FF730000FF3F01915E06B6320094004E008100000014018A4ED85132005D0027008D0000003701AB1DC66432006DFFF2004C0000005A01994CB5363200A800F600A30000FF9B01932B216B32003F00E300950000FFB701DAEA2071320068009700A20000FFE701B312F27532000EFFDD00560000007601D930C55C3200140005007E0000005901DA0BBE6332005DFFB5000C00000087019C69DC2B320041FFBEFFB80000008501B15AE0B832007F000000190000004E018F6FD7EF32000AFFB600600000009201D623F8723200A20111FF600000FF89019B47EBA232007A0123FF520000FF8201B92CE69432008F0182FF5F0000FF3C01B72F149436008F0088FF960000FFEC019663DAC932FF2E004BFF62000000470285B8CEAEBAFF970089FF3F0000000D0244FBF78980FF970038FF4B000000460239F2D0936600420068FF6D0000000D01C72CDD97320041FFBEFFB80000008501B15AE0B8320002FFD6FF8F0000007C01E128EF9132FFF60036FF5D0000003B01F61BDB913200610112FF4F0000FF9101C82DE1963200250095FF5B0000FFF101E21DE98E32FFAA0124FF3E0000FF9E024CFAF58980003E0114FF360000FF9401E11CDB9232FFA0FFCAFF91000000920224F1EF8B8A007F000000190000004E018F6FD7EF3200750027FFA900000034019A63D5CC32006DFFF2004C0000005A01994CB53632005D0027008D0000003701AB1DC66432000EFFDD00560000007601D930C55C3200B00061005900000003017972DEF232005DFFB5000C00000087019C69DC2B32008F0054FFE400000011018E76ECFB320091006B00280000000001906DFBCF3200BB0111FF9B0000FF86018973E0FB320094004E008100000014018A4ED8513200BD00B000660000FFCA017A76ED0932009C00B200920000FFCD01924FE65632009200B400370000FFCD01996014BB3200B000B2004A0000FFCA018457FDAE3200C30174FF730000FF3F01915E06B63200BA0131005B0000FF70018E4560F13200A800F600A30000FF9B01932B216B3200C800FF00700000FF91017E7501183200BB0105004F0000FF8E018861F6BA3200B70180FFF10000FF38019B59B1083200D2019DFFFB0000FF20018C63C0123200BE017600240000FF3E019455B6DA32009900F9003B0000FF9B019E37E39A32009200B400370000FFCD01996014BB32007300D7003C0000FFB801B46311BF32006F013200640000FF7901C3F96D3186003F00E300950000FFB701DAEA20713200A200E9FFCE0000FFA5019571F0223200BB0111FF9B0000FF86018973E0FB3200A90119FFD60000",
                            "FF82019762E9413200BD00B000660000FFCA017A76ED093200B000B2004A0000FFCA018457FDAE3200610112FF4F0000FF9101C82DE19632007A0123FF520000FF8201B92CE69432005E0057FF800000001501B13ADC9E3200170050009A0000002301E3FBE7753200140005007E0000005901DA0BBE6332005D0027008D0000003701AB1DC66432FFE3FFED00740000007001F901CF6D32FFB8FFCF004F0000008B0213D2EE6D32000AFFB600600000009201D623F87232FF62FFDE00280000008C025391EB26D00078012800240000FF7F01BB2DB2B232006D0183FFFE0000FF4001CFCDAABF6A00A20166FFF60000FF4D01A61F901B32000EFFDD00560000007601D930C55C3200750027FFA900000034019A63D5CC32008F0054FFE400000011018E76ECFB32007F000000190000004E018F6FD7EF3200C30174FF730000FF3F01915E06B63200D60170FFB60000FF3F018368CF213200BB0111FF9B0000FF86018973E0FB32005E0057FF800000001501B13ADC9E3200420068FF6D0000000D01C72CDD973200610112FF4F0000FF9101C82DE196320041FFBEFFB80000008501B15AE0B8320068009700A20000FFE701B312F27532005D0027008D0000003701AB1DC664320094004E008100000014018A4ED85132006DFFF2004C0000005A01994CB5363200B00061005900000003017972DEF232008F0088FF960000FFEC019663DAC932009200B400370000FFCD01996014BB320091006B00280000000001906DFBCF3200A200E9FFCE0000FFA5019571F02232007A0123FF520000FF8201B92CE6943200BE017600240000FF3E019455B6DA320078012800240000FF7F01BB2DB2B23200B70180FFF10000FF38019B59B1083200250095FF5B0000FFF101E21DE98E32003E0114FF360000FF9401E11CDB923200B000B2004A0000FFCA018457FDAE3200BB0105004F0000FF8E018861F6BA32009900F9003B0000FF9B019E37E39A32007A011200070000FF8E01B77507163200170050009A0000002301E3FBE77532009C00B200920000FFCD01924FE65632FF970038FF4B000000460239F2D09366FF970089FF3F0000000D0244FBF78980FFF60036FF5D0000FFF101E21BDB9132007A011200070000FF8E01B775071632009600A5FFFF0000FFB901977316173200A200E9FFCE0000FFA5019571F02232007300D7003C0000FFAD01A86311BF32009200B400370000FFCD01996014BB32FFA0FFCAFF91000000920224F1EF8B8AFF61FFC8FFAF0000009C0250A6F1B3FA00C30174FF730000FF3F01915E06B632008F0182FF5F0000FF3C01B72F149436009901B1FF7B0000FF1901B73F35A932FF2E004BFF62000000470285B8CEAEBA00BA0131005B0000FF70018E4560F13200BB0105004F0000FF8E018861F6BA3200A90119FFD60000FF82019762E9413200D60170FFB60000FF3F018368CF213200A0015EFFE90000FF5301A639D56032FFF60036FF5D0000003B01F61BDB913200250095FF5B0000FFF101E21DE98E3200420068FF6D0000000D01C72CDD9732006F013200640000FF7901C3F96D31860021011600290000FF9701F6FA70D6C0009C00B200920000FFCD01924FE6563200BD00B000660000FFCA017A76ED093200C800FF00700000FF91017E7501183200A800F600A30000FF9B01932B216B32005DFFB5000C00000087019C69DC2B32007F000000190000004E018F6FD7EF32006DFFF2004C0000005A01994CB5363200D60170FFB60000FF3F018368CF213200B70180FFF10000FF38019B59B1083200A20166FFF60000FF4D01A61F901B3200B000B2004A0000FFCA018457FDAE32009200B400370000FFCD01996014BB32009900F9003B0000FF9B019E37E39A3200BB0105004F0000FF8E018861F6BA3200A0015EFFE90000FF5301A639D560320078012800240000FF7F01BB2DB2B23200C30174FF730000FF3F01915E06B63200DF019CFFB00000FF1F01827303DF32006D0183FFFE0000FF4001CFCDAABF6A0064017DFFF00000FF4601D5EBC2643200250095FF5B0000FFF101E21DE98E32FF970089FF3F0000000D0244FBF78980FFAA0124FF3E0000FF9E024CFAF58980005E0057FF800000001501B13ADC9E3200750027FFA900000034019A63D5CC320041FFBEFFB80000008501B15AE0B83200BD00B000660000FFCA017A76ED093200B00061005900000003017972DEF232008F0054FFE400000011018E76ECFB32009600A5FFFF0000FFB901977316173200A200E9FFCE0000FFA5019571F0223200BE017600240000FF3E019455B6DA32009E013500450000FF7001A24DBBC53200A90119FFD60000FF82019762E94132007A011200070000FF8E01B77507163200D2019DFFFB0000FF20018C63C01232008F0182FF5F0000FF3C01B72F14943600A20111FF600000FF89019B47EBA23200170050009A0000002301E3FBE775320068009700A20000FFE701B312F27532003F00E300950000FFB701DAEA20713200A0015EFFE90000FF5301A639D560320064017DFFF00000FF4601D5EBC26432007A011200070000FF8E01B77507163200A90119FFD60000FF82019762E9413200D2019DFFFB0000FF20018C63C0123200B70180FFF10000FF38019B59B1083200D60170FFB60000FF3F018368CF21320042014E00210000FF6B01E7E4D1967E006D0183FFFE0000FF4001CFCDAABF6A0078012800240000FF7F01BB2DB2B232009C00B200920000FFCD01924FE6563200A800F600A30000FF9B01932B216B320017FE8FFF780000016001A55BF3B3320002FFD6FF8F0000007C01E128EF91320041FFBEFFB80000008501B15AE0B832FFC1FEA9FF4E0000015901E6180B8B5EFFDCFE09FF62000001C501BD1FA4BA32005DFFB5000C00000087019C69DC2B320032FE7FFFD900000167019072DE0B32FFE3FE8700570000016C01C92DDB6832FF62FFDE00280000008C025391EB26D0FF0EFEA8000E0000017202639AFE3EC2FFB8FFCF004F0000008B0213D2EE6D32FF1FFEC9FF8D00000158025C9719CDFF000AFFB600600000009201D623F87232FF58FEC2FF57000001560233D61692DAFFA0FFCAFF91000000920224F1EF8B8AFF98FE20FF44000001BF01F0F2A9B032FFE3FE8700570000016C01C92DDB6832FF6FFE0B0005000001D3020ADA9B3432FFC9FDF8000F000001D401C822983032FF58FEC2FF57000001560233D61692DAFF98FE20FF44000001BF01F0F2A9B032FF3FFE2DFF55000001C10230C1E79ECE0032FE7FFFD900000167019072DE0B32FF1FFEC9FF8D00000158025C9719CDFFFFC1FEA9FF4E0000015901E6180B8B5EFFDCFE09FF62000001C501BD1FA4BA32FFF4FDFDFFB0000001CB01AA59B2F1320017FE8FFF780000016001A55BF3B332FF0AFE2FFF84000001C702569FC1E2BCFF0EFEA8000E0000017202639AFE3EC2FF1DFE23FFCE000001CE0247BAAA2C46FFA0FFCAFF91000000920224F1EF8B8A0002FFD6FF8F0000007C01E128EF9132FF61FFC8FFAF0000009C0250A6F1B3FAFF62FFDE00280000008C025391EB26D0FFB8FFCF004F0000008B0213D2EE6D32FF87FEA4005700000164020DD8EA6E32000AFFB600600000009201D623F872320041FFBEFFB80000008501B15AE0B832FF6FFE0B0005000001560157DA9B3432FFDCFE09FF620000012801001FA4BA32FFC9FDF8000F00000163015722983032FFF4FDFDFFB000000145012659B2F132FF98FE20FF440000011400F6F2A9B032FF1DFE23FFCE0000013A0141BAAA2C46FF0AFE2FFF840000011E011D9FC1E2BCFF3FFE2DFF55000001110103C1E79ECEFFFE035D0018000001300216007700A0FF0402880018000001EDFFD3890F00FFFFFE028801130000FF1A001DFBFA7732FFFE035D001800000174020C007700A0FFFE028801130000031F0022FBFA773200F6028800180000FFC8001F6C330032FFFE02880113000000000200FBFA7732FF0402880018000002000200890F00FFFF670205001800000121005FABAC0080FFFE020500AD0000FFEF005E08A54D32FFFE01A5001800000019FEE5FF890032FFFE01A500180000002AFEE0FF890032008501F50018000001160045178B0032FFFE020500AD00000001006108A54D3200CE025D007D0000017802004AD55332FFFE02880113000000000200FBFA773200CE025D007D000000B9FFB44AD5533200A90229004A00000147012200AD5632FFFE0288FF1C000000000200FBFA8982FFFE0205FF820000FFEF005E08A5B332FFFE0205FF8200000001006108A5B332FFFE0288FF1C000000000200FBFA898200CE025DFFB30000017802004AD5AD3200A90229FFE500000147012200ADAA32FFFE0288FF1C0000FF1A001DFBFA8982FFFE0288FF1C0000031F0022FBFA898200CE025DFFB3000000B9FFB44AD5AD32FF9803B1008A0000006301B6ABFE5494FF4802B100180000FFF4012789FA00FFFFFE02B600DD000000ED014D05FE7732FFFE02BBFF59000000ED014D04FB8974FF9803B1FFA50000006301B6ABFDACFF00080357FF56000000F201A00AFE896C00C803190018000001EA01A877FE003200C802B10018000001E701727700FF320008035700DA000000F201A00B01773200A90229004A00000121008E00AD5632008501F50018000000C30107178B003200CF01B20018000001E301089DBD00A8FF48046C0018000000000200880000FF00A90229FFE500000121008E00ADAA3200F80204FFD9000000A006090EE48D4C00A90229FFE5000001F4080F00ADAA3200CE025DFFB3000000FD081F4AD5AD3200F6028800180000000008006C33003200CE025D007D000000FD081F4AD5533200F802040056000000A006090DE4743200C800670050000003A400ED0ACC6B32015000EB00480000005D004524FC7232010600E50018000001D20195B3571DFC00CF01B20018000001D7059C9DBD00A8015000EBFFE70000005D004524FC8E3800C80067FFDF000003A400ED08FA896CFEF30094005000000102020CEFE7743200C800670050000008E1F83A0ACC6B32010600E5001800000AFBFA14B3571DFCFF1F00D600180000025302CF6439DE32FF110148FFDF000002FD05D2481EA632FF1101480050000002FD05D2481E5A32FEF30094FFDF00000102020CECCB976A00C80067FFDF000008E1F83A08FA896CFEE801A200180000030308C2E97500C8FE9B00FA00180000004B062C890000FFFE9B00FA00180000004B062C890000FFFEE801A200180000030308C2E97500C800A90229004A000001F4080F00AD5632FC0E0000FFF2000000000000B6A200620000FCE0FFF2000000000000008800320000FCE0000E00000000000000880032FC0E0000000E000000000000B6A2006203F20000FFF20000000000004AA2003203F20000000E0000000000004AA2003203CCFF5CFFF60000001E106A1A8C089C03CC0003002B00000200107F110876A4FF690002003A000002000000C9076ACC03CB00A2FFE00000001E106A1A74F83AFF6900ADFFEC0000FFFCFFF79E45FB3CFF69FFF9FFB7000002000000C7F9973A03CAFFFAFFAA00000200107F0EF88A3AFF69FF4D00040000FFFCFFF79EBB05C603CCFF5CFFF60000FF8F00051A8C089C03CAFFFAFFAA0000010000000EF88A3A04C4FFFFFFE80000022903757700FF3A03CB00A2FFE00000FF8F00051A74F83A03CC0003002B0000",
                            "01000000110876A404C4FFFFFFE80000022903757700FF3AFF690002003A000000000000C9076AF6FF69FFF9FFB7000000000000C7F99732FF69FF4D00040000000000009EBB05ECFF6900ADFFEC0000000000009E45FB3401A200B7FEEF0000010000006DED2D3A01410129000500000001025057E14C3A014100AEFFD50000FFFCFFFB32D8653A01A20148FF2D0000010002636DEC2E3A0141012900050000025A025357E14C3AFF150127003E0000FFFE025CEFD56E70014100AEFFD50000025CFFED32D8653AFF1500AC000E00000001FFF6D1D86688FDFD0146FF690000FFF40279B8DD599AFDFC00B5FF300000FFEDFFFCB8DD599AFF1500AC000E000000FAFFF6D1D86688FF150127003E000000FD0263EFD56E70FF81FC43FFE7000001D10217FAD06D60025EFC3EFFE6000001D400C22CB0B23AFF81002AFF1C000000000217000077960417002AFF1F00000000FFF57100263AFD65FCACFFE7000001A00312DCACB33AFAEB002AFF1800000000043A8D0020BAFF810411FFE7000001D10217FA306DC6FD6503A8FFE7000001A00312DC54B378025E0416FFE6000001D400C22C50B23AFF8102F7FEFF000002CA0000F8369632FF81002AFE4900000800000000008932FAEB002AFF180000080004458D0020DCFD6503A8FFE70000018001F6DC54B386FF810411FFE7000000BC0000FA306DEEFF81FD5DFEFF000002CA0000F8CA9632FD65FCACFFE70000018001F6DCACB332FF81FC43FFE7000000BC0000FAD06D66018CFD5DFF2B000002C9021911D89132FF81002AFE4900000800040000008932030F002AFE9E0000080000B123008E32025EFC3EFFE6000000B201562CB0B232FF81FD5DFEFF000002CA0400F8CA9632018C02F7FF2B000002C9021911289132FF8102F7FEFF000002CA0400F8369632025E0416FFE6000000B201562C50B232FF810411FFE7000000BC0400FA306DEE0417002AFF1F00000800FFBB71002632FF81FC43FFE7000000BC0400FAD06D66050E0040FFF3000000A802004D5BF93203FB00C4FFE80000033702001E73F74E0447000700480000025A00230A0E76A803FB00C4FFE80000FCF002001E73F74EFF6900C5FFE8000002F90200000078E80447000700480000FC8C00090A0E76A8FF6900040048000002F90000000078FF05A9FF3E00080000003D020067C305320447000700480000021E00100A0E76A804D2FF3C00080000002A003E01890A320447FFFAFFA80000FC8C00090AFB8932FF69FFF7FFA8000002F900000000786E0447FFFAFFA80000025A00230AFB89320447FFFAFFA80000020300010AFB8932050E0040FFF300000200020B4D5BF93205A9FF3E00080000000101DD67C3053204D2FF3C00080000007D004D01890A32050E0040FFF3000002D302004D5BF9320447FFFAFFA8000002000D160AFB893204D2FF3C0008000000080E8D01890A32FF69FF3500090000FFFE000000007860FF69FFF7FFA80000020000000000786E044700070048000002000D160A0E76A8FF6900040048000002000000000078FFFF69FF35000900000000000000007860FF6900040048000000000000000078FFFF69FFF7FFA80000000000000000786EFF6900C5FFE8000000000000000078E8073100430022000002A600ED0389088A059500E0FFD70000019201FC00F6898A08BD00E2FFED00000199FFA703F7898601AA00E1FFEA0000018B049316F78B6C03E8005C001F000002760319FA890A9408BD00E8004400000199FFA70309773A0D1400E50016000001A0FD087700FA3A0A49016B000C000000B8FEE10C76F53A0A49005E00200000027EFEE10C89077E03E80176000A000000990318F977F73A01AA00E800400000018B04931708753A059500E900530000019201FC0008773A0731018B00090000007D00EC0377F63A03E80176000A000001680647F977F73A059500E0FFD7000000B804F400F6898A01AA00E1FFEA0000006B081316F78B6C0731018B0009000001C603A90377F63A08BD00E2FFED000000FE022503F789860A49016B000C000001D601330C76F53A0D1400E500160000014CFEFD7700FA3A08BD00E80044000000FE02250309773A0A49005E00200000005501390C89077E059500E90053000000B804F40008773A0731004300220000FFF103B00389088A03E8005C001F0000FFD4064DFA890A9401AA00E800400000006B08131708753A0731004300220000015FFFC40389089C08BD00E2FFED00000240FEC003F789960A49005E00200000019EFDB70C89078E03E8005C001F0000015A01F2FA890AAA059500E0FFD70000022B00D800F6899C01AA0085001D00000173036F59B1063201AA00E1FFEA000001F9037116F78B760A49016B000C000002D9FDB90C76F5320731018B0009000002DFFFC60377F63208BD00E8004400000214FEBD0309773203E80176000A000002A501F5F977F732059500E90053000001EC00D50008773201AA0144000F000002530371594FFA3201AA00E80040000001CE036F1708753208BD00E2FFED0000FFF3FD8503F789960731018B0009000000A9FE7F0377F6320A49016B000C00000084FC7F0C76F532059500E0FFD70000FFEFFF9100F6899C03E80176000A0000009A00A1F977F73201AA00E1FFEA0000FFFE021B16F78B7601AA0144000F0000006E0216594FFA320A49005E00200000FF7FFC880C89078E08BD00E8004400000017FD82030977320731004300220000FF6AFE8A0389089C059500E9005300000024FF8D0008773203E8005C001F0000FF8700AAFA890AAA01AA00E800400000002302181708753201AA0085001D0000FFB4021D59B10632000D012C001A0000006700590377F832FF2E00E90069000000F9021C1E087332011100E8004C000000F6FE48E2087332010E00E1FFE3000000F6FE4EDDF88ED2FF2A00E0FFE3000000F902241CF88C68011100E8004C000000F6FE48E2087332FF2E00E90069000000F9021C1E087332000D009D00240000018900580489089AFF2A00E0FFE3000000F902241CF88C68010E00E1FFE3000000F6FE4EDDF88ED200D30282FF7E00000054FEEDF00B8AA2013E0182FFC2000000ABFE00180F8C58010E013D001100000129FD268A1001EC013F01E6000400000129FEA17126FD320140018D0054000001A6FE001A1F703200D60295006F000001F2FEF1F01E733201380264FFFD0000012BFF6A34950A3A012502C6FFED0000011CFFEE0A77F43201EE028BFFF700000129010ABD62F74C01FC025EFFC0000000C800E10EB8A2AE023D02E6FF8F00000087024ABE54CA7C02B202E3FF6E00000050032A18FB8B6E02B402F70060000001EE032B1D15723202EB0301FFE40000011D03B87127FC32023F02F40045000001BD024CC25E283201FD026800390000019700E110CC6A3200D8FF4700AE000001F2FEF1F0F476320127FF0300350000011CFFEE0B89078E01FFFF6F00710000019700E10F44613201F0FF42003500000129010ABE9D06FF0242FEE60092000001BD024CC3A936C602B7FEE900AD000001EE032B1DFD74320240FED9FFDD00000087024ABFA4D7FF02B5FED8FFBB00000050032A18F38C7402EEFECD00340000011D03B871DA023201FDFF66FFF8000000C800E10D39983800D5FF36FFBC00000054FEEDF0E48DC6013FFFE5002A00000129FEA172DB0232013AFF6800350000012BFF6A336CF932010E008B001D00000129FD268AF003FF01400049006C000001A6FE001AF27432013E003EFFD9000000ABFE0018E08F8401380264FFFD000000C601DD34950A3A01FD026800390000003C013210CC6A3200D60295006F0000FFE9022AF01E733201FC025EFFC000000129013B0EB8A2AE00D30282FF7E000001C2023BF00B8AA202EB0301FFE4000000B3006D7127FC3202B202E3FF6E000001A200A518FB8B6E02B402F700600000FFC900941D15723202EEFECD0034000000B3006D71DA023201FFFF6F00710000003C01320F44613202B7FEE900AD0000FFC900941DFD743201FDFF66FFF800000129013B0D39983802B5FED8FFBB000001A200A518F38C74013AFF680035000000C601DD336CF93200D5FF36FFBC000001C2023BF0E48DC600D8FF4700AE0000FFE9022AF0F47632FF2A00E0FFE30000010E01AD1CF88C68FE62012FFFBB000000510213EC349678FEA90182001F0000FF98012D1C70E232FEA9004A00340000027201011A8CF184FE620090FFC6000001C601FDCFC3A5FFFE68013E0092000000400031AC374156FF2E00E900690000010300801E087332FE68009E009D000001B5001BC8C65982FF3D0086002C000001E5010A1E8C076CFF3D0143001F0000002C01241F73F73201AA00E1FFEA000002EE000016F78B76010E00E1FFE30000FFE30092DDF88ED2013E0182FFC2000001100304180F8C58010E013D00110000000002008A1001EC013F01E600040000FFEA03037126FD3201AA0144000F0000FFE90002594FFA32013E0182FFC2000001D602FE180F8C5801AA00E1FFEA000001D8FFFB16F78B760140018D0054000001DF03041A1F703201AA00E80040000001E4000617087532000D012C001A0000000603730377F832FF3D0143001F0000FFFA00001F73F732FF2E00E900690000020000001E087332010E013D00110000FFF4000A8A1001EC000D012C001A0000000903750377F832011100E8004C000002000000E2087332FF3D0086002C0000FFFA00001E8C076C000D009D00240000000603730489089A000D009D00240000000903750489089A010E008B001D0000FFF4000A8AF003FFFF2A00E0FFE30000020000001CF88C68010E00E1FFE3000002000000DDF88ED2011100E8004C0000FFE30092E20873320140018D00540000011003041A1F703201AA00E80040000002EE000017087532010E008B001D0000000002008AF003FF013E003EFFD900000110030418E08F84013E003EFFD9000001D602FE18E08F8401AA0085001D0000FFE9000259B10632013FFFE5002A0000FFEA030372DB023201400049006C000001DF03041AF2743201400049006C0000011003041AF27432007BFF4D00020000023802D1188B07D2FEFDFF59FFBC000000410006DEEE8F32FF69FFFCFFA50000FFA201C4DAFA8F32FF690002004B0000FFBF01C7B6045EE2FEFFFF64004F000000520010F2F476E2FEFEFF00000C00000168FFFC62BC0546007B00B1FFF80000023802D11975FB32FEFE0100FFE700000168FFFC6245FB32FEFF00A70037000000520010F11D73A4FEFD009CFFA4000000410006D9018F3205E3FFF7FFCC000001FE040002FA89320A6E0077FFF0000000F4FF97376AF9320AD2FFFEFFE5000001ECFE9B4BF9A33204F100C3FFF70000005206000577FB320405FFF6FFBF00000200040002FB8932007BFF4D00020000001A002D188B07D2FF69FFFCFFA5000002000000DAFA8F3204F0FF360006000000000400058905E40AD2000000180000020203044C065C580AD2FFFEFFE50000020004004BF9A3320A6E0077FFF00000008203D5376AF9320AD2FFFEFFE5000001F803FB4BF9A3320AD200000018000001F403144C065C580A6EFF870001000000F003C6379604A40A6EFF8700010000010305F1379604A404F0FF3600060000007F0051058905E405E3FFF7FFCC000001FF008502FA89320AD2FFFEFFE5000001EB05DE4BF9A332007B00B1FFF80000",
                            "001A002D1975FB3204F100C3FFF70000000004000577FB3205E3FFFB0031000001FF0085030677BC0A6EFF8700010000010905F1379604A40AD200000018000001EB05DE4C065C5804F0FF3600060000008A0052058905E405E3FFFB0031000001FE0400030677BC0AD200000018000001ECFE9B4C065C580405FFFB003E000002000400010477C0FF690002004B000002000000B6045EE2FF690002004B0000002D00FCB6045EE2FEFDFF59FFBC000001BC0000DEEE8F32FEFFFF64004F000000440000F2F476E2FF69FFFCFFA5000001D600F8DAFA8F32FEFD009CFFA4000001BC0000D9018F32FEFD009CFFA4000001BC0101D9018F32FEFE00A2FFEE0000010001008DDF04D0FEFF00A700370000004400FFF11D73A4FEFE0100FFE700000100000D6245FB32FEFF00A70037000000440000F11D73A4FEFE00A2FFEE0000010000008DDF04D0FEFEFF00000C00000100000D62BC0546FEFFFF64004F0000004400FFF2F476E2FEFDFF59FFBC000001BC0101DEEE8F320405FFFB003E0000012B065A010477C005E3FFFB00310000012B004C030677BC04F100C3FFF7000001F9034A0577FB320405FFF6FFBF00000154065802FB893205E3FFF7FFCC00000148004A02FA89320405FFF6FFBF00000154050D02FB893205E3FFF7FFCC00000147FF0002FA893204F0FF3600060000007301EF058905E40405FFFB003E0000012A0510010477C005E3FFFB00310000012AFF02030677BC109201C7005C0000028D01C30311768C12B9022E00200000012C0201237200320C1402B2002000000569026100780032056F022E0020000009A80219DD720032079601C7005C0000084901D3FD1176860C1401C7005C0000056B01CB000A779407960019005C0000084C00C1FEF277B6056FFFB20020000009AC0083DD8E00C00C14FF2E0020000005700023008800EE0C140019005C0000056E00B900F177BA10920019005C0000029000B002F277BC12B9FFB2002000000131006B238E00FF10920019FFE40000029000B002F289320C140019FFE40000056E00B900F1893207960019FFE40000084C00C1FEF289320C1401C7FFE40000056B01CB000A8932109201C7FFE40000028D01C303118A32079601C7FFE40000084901D3FD118A3202C30302002000001CC3FFEB7517008E019700F000CF00001ECF036A000000C0029501DA002000001D1801E10000004402140285007800001DF000BC28156FA601FB03C5002000001E13FE9B5356003201FBFE1B002000001E36083D53AA00FF0214FF5BFFC900001E04061E28EB913202C3FEDD002000001CDD06F875E900D6019700F0FF7100001ECF036A0000003202950005002000001D2404FF000000FF040F018F002000001A97026B0B770032040F0051002000001A9F04880B8900F8114400F0005C0000022803D4070077AC157A00F000200000FCFB03E7780000B412B9022E0020000001A101B923720032109201C7005C0000054D025A0311768C06E400F0005C000017C50384FD0077A0079601C7005C000014950222FD117686056F022E00200000183C0165DD720032056FFFB200200000184C059FDD8E00C006E400F0FFE4000017C50384FD00893207960019FFE4000014A004FEFEF28932114400F0FFE40000022803D40700893212B9FFB20020000001B105F3238E00FF10920019FFE400000558053602F2893212B9FFB200200000188D046E238E00FF157A00F00020000013D70262780000B4114400F0005C00001904024F070077AC10920019005C00001C3403B102F277BC06E400F0005C00002EA101FEFD0077A0056FFFB2002000002F28041ADD8E00C007960019005C00002B7C0378FEF277B6040F005100200000317B03030B8900F8019700F000CF000035AB01E4000000C0029500050020000034000379000000FF02C3FEDD0020000033B9057275E900D60214FF5B0078000034E0049828EB6FE801FBFE1B00200000351206B853AA00FF12B9022E00200000187E003323720032114400F0FFE400001904024F07008932109201C7FFE400001C2900D503118A3206E400F0FFE400002EA101FEFD008932056F022E002000002F18FFDFDD720032079601C7FFE400002B72009CFD118A32040F018F00200000317300E50B770032019700F0FF71000035AB01E400000032029501DA0020000033F4005C0000004402C3030200200000339FFE657517008E02140285FFC9000034CCFF372815913201FB03C50020000034EFFD1653560032079601C7005C000000000400FD11768606E400F0005C0000FCB00200FD0077A007960019005C000000000000FEF277B610920019005C0000FF77000002F277BC0C1401C7005C000008D10400000A77940C140019005C000008D8000000F177BA109201C7005C0000FF7004000311768C10920019005C00000000040002F277BC114400F0005C0000FCB00200070077AC109201C7005C0000000000000311768C07960019005C0000FF770000FEF277B6079601C7005C0000FF700400FD117686FC2400F0005C00000380F509B6005E3CFC88014400200000FEADFD8CE8750032FC2400F0FFE500000845FB9FB600A232FC88009C0020000004A7F938E88B00D0FC88009C0020000002C201D6E88B00D0014F00F0FFE5000008560154C90096320104009C00200000080EFFDFA4B40046FC2400F0FFE50000023C0398B600A2320104014400200000080EFFDFA44C0032FC8801440020000002C201D6E8750032FC2400F0005C0000023C0398B6005E3C014F00F0005C000008560154C9006A5C015A023F0078000005290154DA186F4E014F00F0005C000008050149C9006A5C019700F000CF000007FB02AA000000C0015A023FFFC9000005290154DA189132019700F0FF71000007FB02AA00000032014F00F0FFE5000008050149C900963201040144002000000758FFCFA44C003201C202D60020000003D3033D5E4A0032019700F0FF71000007FB02AA0000003201C202D60020000003D3033D5E4A0032020101E0001F000005E30494741EFF80015A023F0078000005290154DA186F4E019700F000CF000007FB02AA000000C0015AFFA0FFC9000005290154DAE891320104009C002000000758FFCFA4B40046014F00F0FFE5000008050149C9009632014F00F0005C000008050149C9006A5C015AFFA00078000005290154DAE86F9802010000001F000005E3049474E2FFDE01C2FF090020000003D3033D5EB600FF00BAFEC2002000000193026C1F8D00FF015AFFA0FFC9000004840308DAE8913201C2FF0900200000034201495EB600FF01C202D600200000034201495E4A0032015A023FFFC9000004840308DA18913200BA031E002000000193026C1F730032015AFFA00078000004840308DAE86F98015A023F0078000004840308DA186F4E07960019FFE4000000000000FEF2893206E400F0FFE40000FCB00200FD008932079601C7FFE4000000000400FD118A3207960019FFE40000FF770000FEF28932079601C7FFE40000FF700400FD118A320C1401C7FFE4000008D10400000A89320C140019FFE4000008D8000000F1893210920019FFE40000FF77000002F28932109201C7FFE40000FF70040003118A32109201C7FFE400000000000003118A32114400F0FFE40000FCB002000700893210920019FFE400000000040002F289320058FCDCFF6700000330FF1403D09366FE4FFCDCFF67000003450107FDCB9568FF540032FE6D00000073002B0000889A02800260FF0800000220FD0E1530949200580389FF6700000330FF14033093B4FC27FE05FF08000002620322EBD0948E0280FE05FF0800000220FD0E15D0944403530032FE6D0000004AFC5716008A72FE4F0389FF67000003450107FD3595C0FC270260FF08000002620322EB3094DCFB550032FE6D0000009C03FFEA008ABEFB4002BEFFB400000782019CC916674EFC270260FF08000003FE00E4EB3094DCFA490032FF1900000B3200DF9B00C0FFFB550032FE6D000007240029EA008ABEFDC5045100140000FDCE0229DC6FE5E4FE4F0389FF670000FBB2016CFD3595C000E3045100140000F1CB02511157B0A000580389FF670000F3D90187033093B402800260FF080000EB8601361530949200E3045100140000F1CB02511157B0A0036802BEFFB40000E80F020537166732045F0032FF190000E453016270002B3203530032FE6D0000E853009016008A720280FE05FF080000EB86013615D0944403530032FE6D0000E853009016008A72045F0032FF190000E453016270002B320368FDA6FFB40000E80F020537EA673200E3FC1400140000F1CB025111A9B0320058FCDCFF670000F3D9018703D09366FE4FFCDCFF670000FBB2016CFDCB956800E3FC1400140000F1CB025111A9B032FDC5FC1400140000FDCE0229DC91E532FC27FE05FF08000003FE00E4EBD0948EFB40FDA6FFB400000782019CC9EA6732FA490032FF1900000B3200DF9B00C0FFFB550032FE6D000007240029EA008ABE045F0032FF1900000062021A70002B32FB4002BEFFB4000001B5FBA0C916674EFA490032FF1900000035FAFC9B00C0FF036802BEFFB4000001DA016337166732FA490032FF1900000035FAFC9B00C0FFFB40FDA6FFB40000FEBDFBB3C9EA6732045F0032FF1900000062021A70002B3200E304510014000002BAFF951157B0A0FDC504510014000002ABFD62DC6FE5E40368FDA6FFB40000FEE2017637EA6732FDC5FC1400140000FDDDFD81DC91E53200E3FC1400140000FDEBFFB411A9B032FEA3FD64FF400000FFE7FD44D02198FF0005FD64FF400000FFE100AB4C14A632FFE2FCDCFF0300000070005A3E099A32FEC6FCDCFF0300000074FDA1C3F19BEAFEC6FBC2FF67000001FDFDB2BCA8D488FEA3FC1C000C000001D6FD59AEAD176CFFE2FBC2FF67000001F9006A2999D2320005FC1C000C000001D100C068C6043200050301FF400000FFE100AB4CECA632FFE20389FF0300000070005A3EF79A3200050448000C000001D100C0683A0432FFE204A3FF67000001F9006A2967D268FEC604A3FF67000001FDFDB2BC58D4FFFEC60389FF0300000074FDA1C30F9BFFFEA30448000C000001D6FD59AE5317F4FEA30301FF400000FFE7FD44D0DF98C6FF5A014B00690000FF6901C4DADA6B32FE45018EFF6C0000FF720276AFE75562FF5B00CC003C0000FFC901C6DBDD6C320184016200330000FF74003744E15D32018400CF00000000FFE3003B43DF5D32FE4500DAFF410000FFF5027BB1EB576201E400CAFF3E00000017FFEB6BF1333201E4017EFF690000FF93FFE56CF43232FF540032FE6D00000400041D0000889A0058FCDCFF670000FFBA02EC03D09366FE4FFCDCFF670000FFBA054EFDCB95680280FE05FF0800000137006615D09444FC27FE05FF080000013707D4EBD0948EFB550032FE6D0000040008CBEA008ABE03530032FE6D00000400FF7016008A7202800260FF080000013700661530949200580389FF670000FFBA02EC033093B4FE4F0389FF670000FFBA054EFD3595C0FC270260FF080000013707D4EB3094DCD7000002FFFFFFFFE700000000000000FC11FE04FFFFF3F8FA0000FF464646FFE200001CC8104DD8E700000000000000E300100100000000FD9000000601CB00F5900000070D8350E600000000000000F3000000073FF200E700000000000000F5880800000D8350",
                            "F20000000007C0FCD9000000002304050100B01606015EE806000204000006020608000400080A0006000C06000E0C0006100E00001210000612001400000A14DF00000000000000DE00000006015F98DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD1000000601C800F51000000700C040E600000000000000F30000000707F200E700000000000000F51008000000C040F20000000003C01CFA000080FFFFFFFFD9000000002304050101002006015D6806000204000004060606080A0000060A060C0E0A00080C0A0606040C00060C08060E0C04000E040206101214001216140618161A0016121A061C1618001C1416061C1A1E001C181A061A121E0012101EE700000000000000E300100100008000FD10000006019040E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601BE80F550000007010030E600000000000000F30000000703F800E700000000000000F548020000010030F20000000001C03C0100801006015E680600020400060004060408060004020A06060C0E0006080CDF00000000000000D70000020E740384E700000000000000FC4196045FFCFFF8FA0000FF805A14FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D300F59000000704C133E600000000000000F30000000701F800E700000000000000F58802000004C133F20000000001C01CD9000000002F04050100C01806015CA8060002040000060206080A0C00060E1006061002000A120C060A141200141612DF00000000000000D700000208980384E700000000000000FC4196045FFCFFF8FA0000FF805A14FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D440F590000007057D52E600000000000000F3000000071FF200E700000000000000F588080000057D52F20000000007C07CD9000000002F04050100D01A06015BD806000204000600040608000600080A00060C0A0E00100C0E06120C10001412100616121400161812DF00000000000000D700000208980384E700000000000000FC4196045FFCFFF8FA0000FF805A14FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D440F590000007057D52E600000000000000F3000000071FF200E700000000000000F588080000057D52F20000000007C07CD9000000002F04050100D01A06015B0806000204000206040608020000080A02060C0A08000C0E0A06100E12001410120616101400161810DF00000000000000D70000020FA00384E700000000000000FC4196045FFCFFF8FA0000FF787878FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050142E600000000000000F30000000707F400E700000000000000F588040000050142F20000000003C03CD9000000002F04050100B01606015A58060002040006040806020A040000040C06040E0C00060E04060410080004121006041412000A1404DF00000000000000DE000000060163D0DE00000006016320DE00000006016270DE000000060161C8DE00000006016058DE00000006016048DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD1000000601A380F5100000070D4360E600000000000000F3000000077FF080E700000000000000F5102000000D4360F2000000000FC07CFA000080FFFFFFFFD9000000002304050102004006015658060002040006080A06060C08000E1012060A08140008161406181A1C001E181C06202224002220260628222A00262A22062C202E002A2E28062E2A2C00242E20D900000000230005063032340036383A06303C32003A3C36053E3836000000000102004006015858060002040006080206020804000A0C0E060E000A00100C1206081012001408160614160000160812060A001600181A1C061E2022001C24180522261E00000000D90000000023040506282A2C002E3032062E32340034323606323836003A3C3EDF00000000000000D70000020BB80384E700000000000000FC4196045FFCFFF8FA0000FF5A1E80FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050543E600000000000000F30000000707F400E700000000000000F588040000050543F20000000003C03CD9000000002F040501019032060154C8060002040006000406080A0C00080E0A0608100E000E1012D9000000002F000506121014001416180514101600000000D9000000002F0405061A021C001E1A1C06202224002026220620282600282A26D9000000002F000506282C2A002C2E30052C282E00000000DF00000000000000D70000020FA00384E700000000000000FC4196045FFCFFF8FA0000FF5A1E80FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050543E600000000000000F30000000707F400E700000000000000F588040000050543F20000000003C03CD9000000002F00050101903206015338060002040006020006080600000A0C0E060C100E00120E10D9000000002F0405061404020012101606181A1C00181C1E06140220002224200624142000261628062A262800161028062C1A2E002C2E30DF00000000000000D70000020FA00384E700000000000000FC4196045FFCFFF8FA0000FF781E28FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050543E600000000000000F30000000707F400E700000000000000F588040000050543F20000000003C03CD9000000002F040501012024060152180600020400040608060A04080000040A060C0E10000C10120612101400101614061816100018101A061A101C00100E1C061E04200022041E0622060400040220DF00000000000000DE000000060167D0DE000000060166F8DE00000006016608DE000000060164B8DF00000000000000D70000020BB80384E700000000000000FC4196045FFCFF78FA0000FF808000FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050D43E600000000000000F30000000707F400E700000000000000F588040000050D43F20000000003C03CD9000000002F04050100500A060151C80600020400060408DF00000000000000D7000002057803E8E700000000000000FC4196045FFCFF78FA0000FF808000FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050D44E600000000000000F30000000707F400E700000000000000F588040000050D44F20000000003C03CD9000000002F04050100500A060151780600020400060408DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD1000000601C900F5100000070D0340E600000000000000F3000000070FF200E700000000000000F5100800000D0340F20000000003C03CFA000080FFFFFFFFD9000000002304050100A01406014E3806000204000006080602000A00080A00060C0E10000E0C120610060C0004120CE700000000000000FD1000000601C400F5100000070D4340E600000000000000F3000000071FF200E700000000000000F5100800000D4340F20000000003C07C0101C03806014ED8060002040000060206080A0C000E0A08061012140016181A061C1E2000221C2006082426000C240806282A2C00282E2A0602063000320230063424360026243406340A0E00360A34E700000000000000FD1000000601C380F5100000070CC130E600000000000000F30000000703F400E700000000000000F5100400000CC130F20000000001C01C0100E01C06015098060002040000060206080600000A0C0E060A0E1000121408061208000016181ADF00000000000000DE000000060169E0DE00000006016950DE000000060168C0DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD1000000601C900F5100000070D0340E600000000000000F3000000070FF200E700000000000000F5100800000D0340F20000000003C03CFA000080FFFFFFFFD9000000002304050102004006014C38060002040004020606080A0C000A0E0C0610120A00100A0806141618001A1C1E06182022001E2426062220280026242A06281614002A1C1A06062C2E002E2C30063202340034020006360E380036383A063A383C003E2C320638123C00302C3EDF00000000000000D700000207D0044CE700000000000000FC4196045FFCFF78FA0000FF404080FFFB00000000003CFFE200001CC8112078E300100100000000FD9000000601D340F590000007050942E600000000000000F30000000707F400E700000000000000F588040000050942F20000000003C03CD9000000002F04050100A01406014B980600020400060800060008020000020406040A0C000A0E0C0606080000100600060008020002080A06080E0A00020A04060E0806000004120612040C000C0E06050C061000000000DF00000000000000D700000207D009C4E700000000000000FC4196045FFCFFF8FA0000FF3C3C80FFFB00000000003CFFE200001CC8112078E300100100000000FD9000000601D300F59000000704C533E600000000000000F30000000701F800E700000000000000F58802000004C533F20000000001C01CD9000000002F04050101002006014A98060002040000060206080600000A060C0602060A000E020A061012140010161206181610001A161C0612161A001E121ADF00000000000000D700000207D009C4E700000000000000FC4196045FFCFFF8FA0000FF000028FFFB000000808000FFE200001CC8112078E300100100000000FD9000000601D300F59000000704C533E600000000000000F30000000701F800E700000000000000F58802000004C533F20000000001C01CD9000000002F0405010200400601489806000204000006020608060A0004080A06040A000006000C060C0A0600000A0E060E1012000E1200061014120014161206181A1C001A141C061A16140014101C061E181C001E1C10061E100E001E0E0A06202224002226240626282400282A240628262C002C2E3006282C300028302A06322E2C00322C2606343222003226220622203400362038063834360034203A06203C3A0020363C063E3634003A3E34DF00000000000000D70000020BB80384E700000000000000FC4196045FFCFF78FA0000FF808000FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050D43E600000000000000F30000000707F400E700000000000000F588040000050D43F20000000003C03CD9000000002F04050100500A060148480600020400040608DF00000000000000D7000002057803E8E700000000000000FC4196045FFCFF78FA0000FF808000FFFB000000000000FFE200001CC8112078E300100100000000",
                            "FD9000000601D340F590000007050D44E600000000000000F30000000707F400E700000000000000F588040000050D44F20000000003C03CD9000000002F04050100500A060147F80600020400060800DF00000000000000DE00000006016FC8DE00000006016F38DE00000006016E10DE00000006016D58DE00000006016C88DE00000006016BA0DF00000000000000D700000207D009C4E700000000000000FC4196045FFCFFF8FA0000FF3C3C80FFFB00000000003CFFE200001CC8112078E300100100000000FD9000000601D300F59000000704C533E600000000000000F30000000701F800E700000000000000F58802000004C533F20000000001C01CD9000000002F04050100E01C06014718060002040002060806080A0C000E1012061214160016181ADF00000000000000D700000207D009C4E700000000000000FC4196045FFCFFF8FA0000FF3C3C80FFFB00000000003CFFE200001CC8112078E300100100000000FD9000000601D300F59000000704C533E600000000000000F30000000701F800E700000000000000F58802000004C533F20000000001C01CD9000000002F04050100E01C06014638060002040000060806060A0C000E1012061014160014181ADF00000000000000D70000020FA00384E700000000000000FC4196045FFCFF78FA0000FF808000FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050543E600000000000000F30000000707F400E700000000000000F588040000050543F20000000003C03CD9000000002F04050100D01A060145680600020400060802060A0C08000E100C060E121400161218DF00000000000000D70000020BB80384E700000000000000FC4196045FFCFF78FA0000FF808000FFFB000000000000FFE200001CC8112078E300100100000000FD9000000601D340F590000007050D43E600000000000000F30000000707F400E700000000000000F588040000050D43F20000000003C03CD9000000002F04050100D01A060144980600020400060208060A0C0E000C1004061214160018160ADF00000000000000DE00000006017270DE000000060171D0DE00000006017130DE00000006017090DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD1000000601C180F510000007010040E600000000000000F3000000070FF200E700000000000000F510080000010040F20000000003C03CFA000080FFFFFFFFD90000000023040501012024060142D8060002040006080A06080C0A000E1012061416080014080606180200001A1C1E06201A1E000E2210E700000000000000FD1000000601BF80F5100000070D0340E600000000000000F3000000070FF200E700000000000000F5100800000D0340F20000000003C03C0100600C060143F8060002040000040606040208000A0408FA000080000000FF01004008060144580600020400020604DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD10000006019040E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601BE80F550000007010030E600000000000000F30000000703F800E700000000000000F548020000010030F20000000001C03CFA000080FFFFFFFFD9000000002300050100C01806014058060002040002000606080A0C000C0A0E0610121400101416E700000000000000FD10000006018E40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601BC80F5500000070D4340E600000000000000F3000000070FF400E700000000000000F5480400000D4340F20000000003C07CD900000000230405010090120601411806000204000206040608000A000A0004060A0C0E00040C0A0604100C00040610E700000000000000FD10000006018C40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601B480F5500000070D4360E600000000000000F3000000073FF100E700000000000000F5481000000D4360F2000000000FC07C01008010060141A806000204000006080604020A000C040A060E0C0A00000406E700000000000000FD10000006018C40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019280F5500000070D4360E600000000000000F3000000073FF100E700000000000000F5481000000D4360F2000000000FC07C0100B016060142280600020400060800060A020C000E0C10060A0C0E0004020A06040A0E0012040E06140806000802000606041200060004DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100000000FD1000000601BF80F5100000070D0340E600000000000000F3000000070FF200E700000000000000F5100800000D0340F20000000003C03CFA000080FFFFFFFFD9000000002304050100801006013F3806000204000402060608040600060A0806060C0A000A0C00060E0A000000040EE700000000000000FD1000000601BF00F5100000070CC330E600000000000000F30000000703F400E700000000000000F5100400000CC330F20000000001C01C0100600C06013FB806000204000402060606080A000A0800FA000080000000FF01004008060140180600020400060200DF00000000000000E700000000000000FCFFFFFFFFFFFF3EE200001CC8112078D9000000002300050100600C06013ED80600020400000406060402080004080ADF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601B380F550000007010340E600000000000000F30000000707F400E700000000000000F548040000010340F20000000003C03CFA000080FFFFFFFFD9000000002304050101B03606013AB8060002040006080A060C0E10000C1012061014120016181A061C1E1A00200A08061A1822001A221C06100E2400261024062614100028181606282A2C002C2E28062E182800300200060A320600320A34E700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601A280F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03CD9000000002300050100E01C06013C68060002040006020806060A0C00060C0E060C100400040E0CD9000000002304050512141600000000D9000000002300050500180200000000D9000000002304050516141A00000000D900000000230005060218080004100005080A0600000000E700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019A80F5500000070D8350E600000000000000F3000000073FF200E700000000000000F5480800000D8350F20000000007C0FCD9000000002304050101903206013D48060002040000040606080A06000C0E100612100E000E0A12060014120010141606181A1C001E202206201E240014101206181E22001A1824061A2426000C1614060C140E00282220062A241800060A0E06140006000E140606242C20001E262406222A18002C2E20061202000022282A06181C1E000A30120608300A001E1C26DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601B380F550000007010340E600000000000000F30000000707F400E700000000000000F548040000010340F20000000003C03CFA000080FFFFFFFFD900000000230405010200400601217806000204000006020604080A00040A00060C0600000E10120614121600140E1206181A1C001E181C061E2018002224260614222600142822062A2C2E00302E3206343638003A383C0101F03E06012368060002040006000406060800000A0C0E060A0E0400100A04061214160018121606181A12001C1E2006222426001C22260626281C00281E1C062A2C2E00302A3206343638003A343CE700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601A280F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03C0101202406012558060002040006020006080A06000C00040606000800040E0C061012140014121606181A1C001E1A180608202200220A08060C2220000C0E22E700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019A80F5500000070D8350E600000000000000F3000000073FF200E700000000000000F5480800000D8350F20000000007C0FC0102004006012678060002040000060206080A0C000C0A0E0610121400161800061A1C1E001E101406001A160008202206141A1E00242628061A1416000E2A2206222826002C2E300622200E00322C200632342C0030202C06362A3800300E20060C343200302E0C060C0E30002E3A0C0632080C000C3A34053C363E0000000001020040060128780600020400040608060A0C0E00100C0A0610120C000004080614161800001A0206001C1A001E08200622240600081E00062426060020082806062A2C001E162E0620161E002C280606282C2E0028302006323436002C3800053A3C3E000000000100B01606012A78060002040006080A0606000800080C0E0608040C0010080E06100A08000E1214050E0C1200000000E700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019240F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01C0102004006012B28060002040006080A060C0402000E0410060E101200141618061A1C1E002022240626282A002C2E30063228340036383A0536323C000000000102004006012D18060002040002000606080A0C000E1012060E121400160E18061A1C180014121E06141E0C0020102206242628002A24200624282C002E30200606003200342A360634363800303A3C0532003E000000000102004006012F18060002040000040606080A0C000E10120614160200181A1C0606041E00061E200622242600282A2C062E3032002E3430",
                            "062E2A280036383A062E323C003C2A2E0102004006013108060002040006080A060C0E1000120E0C06141618001A1C18061E000C0002001E06202202001E0A240604221C000C261E061E240200282A2C062E3010001C2232063436380016143A05183C140000000001020040060132F8060002040006080A06060C08000C0E0806001012001416180600121A00061C1E062022240026282A062C1C2E00062E1C063032340030343606383A3C00361C2C01020040060134E8060002040006080A06060A0C00040E00060402100000121406041618001A1C1E062022240006262806082A2C002A2E2C06180E0400303210062E343600001438053A123C0000000001020040060136D8060002040006080A06060A0C000E10120614161800021A1C061E202200201E2406241E2600282A2206282C2A002E30320634302E0028362C0624383A00243C380101F03E060138C8060002040006080A060C0004000E060A06101208000C04140616141200181A0E060E0A18001C1A18061E20100022060E0624220E00221E060626282A001A28260628002A001C281A06280200001C0228060C162C000C1416062E303200303432062E3630002E383606383A36003A3C36DF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601A280F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03CFA000080FFFFFFFFD9000000002304050102004006011AF8060002040006080A060A0C06000A0E04060600080000060C0604020A00040E000610121400161210061812160014121806101A1600141A10061C1E20001C2022061C2224001C242606282A2C002E30320634362C00383A28063C283A002C2A34063A383400343C3A052C3628000000000101803006011CE8060002040006080A060C0E1000100612060A080E000A1206060E0C0A001416000602181A001C1E2006221E1C00242620062226240016141A061A0402001C26220620261C00282A2C05282E2A00000000E700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019A80F5500000070D8350E600000000000000F3000000073FF200E700000000000000F5480800000D8350F20000000007C0FC0102004006011E68060002040006080A060C0E100010061206141618001A161406161C18001A1C16061A1E1C001C201806202218000A080E060E2402002608060606282600240E0806082624001E2A2C062C2A20002E302A062A1E2E0030222006202A30001A2E1E0632201C001C1E340634361C001C363206341E2C002C203206181A3800143A1A06183814000A0E0C0612060A003C000406123E1000103E0C0101102206012068060002040006080A06060A0C000C0E0006100C000004100006060C10000610040612141600121618061A1C1E00201A1EDF00000000000000D7000002FFFFFFFFE700000000000000FC127E60FFFFF3F8E200001CC8112078E700000000000000E300100100008000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD5000000601A280F5500000070D0340E600000000000000F30000000707F400E700000000000000F5480400000D0340F20000000003C03CFA000080FFFFFFFFD9000000002304050102004006010538060002040004020606080A0C000E100806060212001202000606140400041400060C160800100E18061A1C1E0020222406241A26001E1C22061E261A0022201E06180C0A00282A2C062E30320032342E0628362E00302C320634322C002E2A280608160E002C362806383A3C003C3A3E0100D01A0601073806000204000402060606020800080200060A0C0E000A1012060E141600140E1806120C0A0012161406141812000E100AE700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019A80F5500000070D8350E600000000000000F3000000073FF200E700000000000000F5480800000D8350F20000000007C0FC0102004006010808060002040006080A060C0E06001012140616181A00161C18061A1E20001A061E06201E18001E061806060E18001A0806061A220800181C200606240C000A24060626282A0010282606261210002A2C26062E3012002C32340634362C00383A0E06083A380032123006380E0C003A3C3E063E0E3A0008223C063C3A08000E3E180620161A003034320101602C06010A0806000204000006020600080600000A08060C0E1000120E0C0610141600181A1C06161E20002022240616241E0024221E060C102600281016062412280024282A062A281600242A20E700000000000000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019240F5500000070CC330E600000000000000F30000000701F800E700000000000000F5480200000CC330F20000000001C01C0102004006010B68060002040006080A060C0402000E0410060E101200141618061A1C1E002022240626282A002C2E30063228340036383A0536323C000000000102004006010D58060002040002000606080A0C000E1012060E121400160E18061A1C180014121E06141E0C0020102206242628002A24200624282C002E30200606003200342A360634363800303A3C0532003E000000000102004006010F58060002040000040606080A0C000E10120614160200181A1C0606041E00061E200622242600282A2C062E3032002E3430062E2A280036383A062E323C003C2A2E0102004006011148060002040006080A060C0E1000120E0C06141618001A1C18061E000C0002001E06202202001E0A240604221C000C261E061E240200282A2C062E3010001C2232063436380016143A05183C14000000000102004006011338060002040006080A06060C08000C0E0806001012001416180600121A00061C1E062022240026282A062C1C2E00062E1C063032340030343606383A3C00361C2C0102004006011528060002040006080A06060A0C00040E00060402100000121406041618001A1C1E062022240006262806082A2C002A2E2C06180E0400303210062E343600001438053A123C000000000102004006011718060002040006080A06060A0C000E10120614161800021A1C061E202200201E2406241E2600282A2206282C2A002E30320634302E0028362C0624383A00243C380101F03E06011908060002040006080A060C0004000E060A06101208000C04140616141200181A0E060E0A18001C1A18061E20100022060E0624220E00221E060626282A001A28260628002A001C281A06280200001C0228060C162C000C1416062E303200303432062E3630002E383606383A36003A3C36DF0000000000000030C141010001FFE5000130C16AC3B381FEE3410110410841390128C118811081188120C1390128C1B44B0841288128C10801080120C1BB8130C128C118810001634139017BC172C3104118813101FFD5104128C193C52081BBC1FFD520813143FFD7084131435203BBC1FF23FF93FFC321037AC193C5AD41ABC331011881E707FE838B0130C120C3F683FF23C40339831041F643B38130C1B3818B859C05EE4330C172819B41D4C1DD434A05B441FF233983830118C138C1DD0372C5FFC5F6E10001D4C15A457B4518C30001F683B3816AC5A381AC8720C1E583B4874183B485F70FFF1FE605E5C7C60351818B8529017305B40138C1D4837B05FEC3FF93CCC1CD03EDC3E541CCC5C3C3FF21C5072901AC85BBC1D5476A85C3C1FF454141834549C139835205BC41C5054981E5C38B85F6C349C1A3C120C39341E64562434A058B41EE03A445F6C9FF43C403EE8B9BC572C37B03C3C1A3415A019C0549C5BC41EE19CD45B4C5CC43AC458AC16A81CC81DD458B853143E649F6C5FF0338C1FFCF9341B403DD43EE033141DE05BC41CD855245DD01FEE1CCC36A01C483EE07290362837B4549C33101830341C5AC03FFF3EECDF69FB4012943D553290359C1E5C3D585AB81CC434183B4011083FF85CC8393C5B3818301FF938385C545FF097281CD052903A405EE878345C4032903BC8DD585C4414141DD43AC4372815A45BC417429FFFF73E900DD742900DF7CF100DF00E17C6B7C298CADDEF9D0019D2FADB3846B01219D2FE77BADB17C69A571F7BD6BA3FFFF001194ED00A500DD00118CEDD801009BA57184AB000D7429CEB7D6B90099000FB5F3952FD6F9CE77001173E710D1DF3BB5F3D00100578CAB94EFA571BE35005984AD005FD6F97CAD0969B8016BE7846B001500DFC801FFFF000F18D1B5F5009B846710D912355B21012B00635B1FEF7D846960010015FF0158016BE5C675DEFB84AB009DA80194EDF7BF009F0921A001742908DF009FC0011235BE35A52BC001C63574270895FFFF0051952D000BC677E73BE73B09F3C67709F12999009D39D700590059001F8001EF7D0055BDF373E5425B8CED0055009BD6B70013005BD2530053C909009B94ED0015005DFF7B08E1005B005D215900990099E73B6A99005773E3005900DFBE35009BB00100536B63092709F300E100190123009D0055085110D39D6FC801001394A9C39D001B9D2F7427001B73E7096B08E784B5CE751917215900E3B801B5F1A56D9571C631FFBD0891A001980109AD8CAD09ADDEF97C69846BEFFF8801000D90017C6B632173E5746B7C2BCE75089963A300E900137C690967CEB72157096B00E5BE3109F10925B5F300DBFFFB7C6BB5F332673A1708E5B001FFFF635F52996361009700E90055012973A5A5710A3509B142AB00E1009D00A3009D096F001B84ADC633B5F3ADF3B5F1C675A56F8CEB6BE574256BE36C256BA1BE75C6B7C677BE35952D742773E1D6B5ADF1CEF984A97C2518012801494B735D94E9B633A5B17C67294B380128C741CF8423A56DCEFBDF3BCEB794EB39018A4369015081314B4A117BDFAD6FBE33CEB994A5628DA3099AC779C158C120C751CF7B9DADAF9D2B8B51C411AB4B92856149B38DA30B8243408138836B19A56BD6F99CE7FEA5FEE5FFABFE63FF29EE63D55DB4159B91A3915AD9C673ADB1D4D9BC13C4973001BC53BC55C455CCD74211FFFF9D29EE23B3D1DD9FC453CC97318B635BC495A34DB3CFBBD192CD418D40412001400152955A0969C59287820169416ACFAB8DBBCF50C1B5AF5B13BC0F480181C1510352CFBC11A34B8A855905ADADA5AF7B0DB3CD61018203FEA3FF6BFEA7FE65DD1BE55BD497C413D519B38F7141ED9FCC95DD5DDD5BBDEFB5B1D4D7DD19CC55CC53DCD98A8B60C179C79353414171818A45824DA4E5394158819AC9930DA45B9D2D8CE949857189AB93B56B5B618245A3939D6D72478BD9A4E39D6FEFBF952B84690123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123",
                            "0123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012390017001780188018001680160015001380148014001484120013001284138035801584160415043580360437043684301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230123012301230303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030300000000000000000000000000000000000000000000150910230B1F1B2B0E0E1616140F0F2A2A4838386A58712D2D262627272C2C2C0C0C0C0C0C595959590C00000000000000000000000000000000000000000A0910230B1F1B360E0E1616140F2A2A4838386A71712D2626272C2C0C595931317272727213131313131313000000000000000000000000000000000000001509100B1F1B360E0E1616140F2A2A80386A58712D26272C2C0C593172727213515151515117171717171717170000000000000000000000000000000000001510230B1F1B360EAB16160F2A2A80383858712D26272C0C593172131351515117175E5E01010101010101010101000000000000000000000000000000000A0910230B1B1B0E0E1616140F2A2A383858712686272C0C317272135151175E5E010101010101010101010101010101000000000000000000000000000000000910230B1F1B0E0E1616140F2A80386A712D26272C0C3172725151175E01010101010101010101010101010101BEEE770000000000000000000000000000001510230B1F1B0E0E1616140F2A8038587126272C0C317213511701015E010CBDC0010101010101010101010101BE1AA58800000000000000000000000000001509230B1F1B0E0E1614142A2A3838587126272C5913175151170CBE68F35094B947C001010101010101010101014AA5B6B5000000000000000000000000000A09100B1F1B2B0E16160F2A2A483858262C72313172DF144AA1827703B08F8FB0B0427931010101010101010101011740FC76000000000000000000000000001510230B1B360EAB16140F2A6A265886C0BEAE4A827794633B08FA4F1CB6F2B5B5B608B0820C010101010101010101010117710000000000000000000000000A09230B1F1B0E0E16160F2A58686BEE829494B08F081CB6F2F2D7DAF7F7E7E9D7FCF63EDE7C82E40101010101010101010101010000000000000000000000001510230B1B360E1616140F2A6A68AA87B03BDE3EC5C576767674161701010186A108F2FCB5B63B4DF3A1A1A1A1A1A1A11818181800000000000000000000000009100B1F1B2B0E16162A3871712DB8AE77B0DEC576744CF5F62D010101010101014D3BB5F6C5F2B6E908055F5F5F5F5F2121212100000000000000000000000A09100B1F1B0E0E162A6F4A4AAEDF2C0C2CAE77FAFC7474F574A10C0101010101BD94FAC5744C747476FC4E4E4E4E4ED7D7D7D74E00000000000000000000000A09100B1F1B0E1616E8AA1A1A6CB94A592713720EE7FC744C4C4CE1FF010101AE7F7CB6F64C4C7476FCB7B7B7B7B7B7B7B7B7B7B700000000000000000000000A09100B1F1B2B489A293921055BA5472C2C2C13E4F3C5744CF5FCB7010172EE343BB6F674763EE18F0A010101010101010101010100000000000000000000000A09100B1F1B0E16AE6C08D7F2BBA5EC0C271386501CF674FC74380101B882B0FAB5764C74C5A208949A68DF0C010101010101010100000000000000000000000A09100B361616160F0E40F7F7F710262D72DF824FB5C5D7F7590101AE4B3BB6C5744C4C7476C5B5B5A208949450F368BE310101010000000000000000000000003A9AE85081184A35AE68DFDFB8867127B84BFA3E3E1C942C010168B04FB6F64C4C4C4C4C4C76FCFCFCC5B5F2B6A68F4B778218BE00000000000000000000000036EE70292424C2B9B9B97777774DD0A1823BA2A2DE28B8010151944FB6B5F674744C4C4C76B7B7F74C76FCFCFCFCC5B5F2DE085B0000000000000000000000021BEC247F7F34398888888F8FB021D5885FA2A2E9B018015E010138387171384DD7764C744C170190B83848B74C76F6FCFCF6C5C50000000000000000000000021FEC1A3905A6E1DADAA2BB7757D04B08DA3EE98F6CBDBDBDBDDFB8B8B886684BBBC574FC380101890D8BAFB8905948B7F77476FC0000000000000000000000023AEC1A2160E1DADAA25FA114B8DC5BE13E3E0888399494949494949494774B88A6C5FCB70101890D20200D0D8B89C101011738B70000000000000000000000023AEC1A2108A6A6084BF31648AE8705A2B5B5A2BB085F5F5F5F5F5F3B3B8F7CFAB5F6F70101890D200D0D202020448B899090E4E40000000000000000000000023AEC1A28030328874A160E389A1A4BE1DAE1916391E1B5C5FCC5F7F7F7B7B7B748715E01AF0D200D89890D20200D8B8989898B8B0000000000000000000000000052C23428EF1A792A0E162A0FAE2A802A2A2ABD475BE14EB7310C72AFAFAFAFAF8989890D20200DAF86442020200D89018944DF0000000000000000000000002F6F477F7F8A24EC3581189A6B18ED506A2A48B8473960D7B71372BE9F3F3F3F65654444448B440D8901DF440D2020898989DF01000000000000000000000000021BDC427F7F8AAAAAA9242424298747FF140FB8797F031CD27259AF5C9F3F3F3F8B3F656551896565AF01908944440D0D448901000000000000000000000000023AEC1A2821E34B9482824DA14D576F141616719A1A21DEE1862C68615C9F5CAFCB8B3F9FDFCB8965658BDF86C0AF898B444489000000000000000000000000023AEC1A34287F4A1B0A6B6FAB4A6BEE0E16AB389A1A2811E1B86A2D61615C61AFCBAF5C9F8B86CBAF3F3F3F8B72CBCBAF3F656500000000000000000000000000156B8A7F8A707950EDED47473047709A0F2B489A2928633B152D58C46161C4AF722C8B5C5C5CDFCB86898BB87272AF9F9F899F000000000000000000000000002F1BED1A8A6E292429296E6EA9DCE84A0E1B2BDF3034035F4DB838AECEC4C4C4DF2CBD8B616161AF68272C2C3186AFAFAF0CAFA4A4A4A4A4BD44494949547B0707000000000D0D162B0F0F0F0B0B0B02020202A4A4BDBD4444494FA16C5CB307000000000D0D0D0D111111110F0F0A02020202A4BD98444449A1A1815C7B46070000000D0D0D0D11110D0D11110E0A0B020202BD9844444FA18196547B462C000000000D0D0D0D0D0D0D0D11110E0B0B0202027D444F81A18196547B462C070000000D0D0D0D0D0D0D0D0D110E0A0B02020202A444A196966C547B468C07000000000D0D0D0D0D0D260D11110A0B0B02020202444F6C6C6C545CB3841B070000000D0D00000000260D11110F0A0A0B02020202989654545C7BB3842C070000000000000000002611110E0F0A0B0B0B020202029896545C53E7462C070000005B00000000002611110E0E0F0A0B0B020202020A44545C5C7BB32C1B0700005B0C5B0C00000D11115A0E0F0A0B0B0202022690AC495C5C7BB3461B0700005B0C0C0C0C0C26115A0E0E0F0A0B0202118EC49A93374F537BB346840707005B0C0101010C260D115A0E0F0A0B0202778E806CBD98376C7BE7B3842C0700005B01010101260D11110E0F0A0B02C4904FA4373737375E54E7B346841B07005B010101010C0D11115A0F0B021147B6BDBDE65E5E5E5E5E53B3B3462C070700010101010C260D115A0F0B020E9544BDBCBDE6E6E6E6E65E464646462C07075B01010101260D11110E0B0B0AD4A1F3F3EEEEEEBCBDBDBD8D074646842C07000C0101010C2611110E0A0B47B6E04470363670F39D9DC5E0CB2C842C2C1B07000C01010C2611115A0F02779E804F3636273670BBBBB16F69AB072C2C2C1B070001010C0C2611110F0B0B47564F7027272736D8BB8A6A393941072C2C1B070F0C01010C260D110E0F18475954D830273670A6BB86F22A789E51072C2C1B070D0101010C2611110E0A029AF99630273670BBC76F4E7C5D6290DE071B1B1B070D0C01010C0D115A0E0B5193C9D8273636BB866D4E63D0C847C48E071B1B1B070D0C01010C0D0D0E0F0A56535470363670866D2A638B336ECF430D07071B07072600010101260D5A0A77C954D83636709D94A78F8B336E2F5A0E0EE107070707900D0101012626110A518254D83670A6E06FEC78626E2F380F0B0EABE1070707410A0101010C0D5A0B82546CCE70A3BBB22A8FC8D22FCF430F0B0F52ABE10707E7770C01010C260E0B82C9CECEEEBB944E4D5D3347CF38640B020A9C52E107077B950D01010C0C0E0B825CCECEA3C5B22A8F62D22F2F430E0B02025952AB0707B3590E0101010C0E02C96C6C6CA3C5B22A6392D52FCF640B0B0202399CAB07078C2C770001010C0E0253B8B8B8F3866D2A7C926E2F385A0B020202399CAB070707C9900D0101010E18B754B8B8F3866A2A6855472F385A0B020202399CAB07070753A02B010101110B935C5C5CF3B16A2A8B55472F380E0B020202399CAB070707B3B70E0101010D0A3CB37B83F3E06D2A5D55472F430E0B020202D49C690707078C82770C01010C0E9CC1CD83739DB22A7C55472F430E0B020202B69C9AC12C2C07E7900001010C0E51C146CDB8F3862A8F556E2F43E50B020202B69C3CC1C12C2C8C590E01010C2BDE3C84F98373ED2A4D556E2F43E50B020202232AD63CF993840793770C01010D8E692CC1938AB12A2A92D52FCF640A020202D02AA2D6ED83464681900001010D2B41E10793F0E0B42A8BD547DB43E5020202D02AB4FC8A8ACDF982A00D01010D0E8E5207C18372B2A28F92472F43E5020202234E6FB2C7BB8ACDB7B70F0C01260E0DB507C183E0866D2A685547CF64020202A9F294EDE0A673CDCD4F470D010C0D0A5107C1CD83E0B22A5DC86E2F430B0202634EEDC7BBD8BB8ACB549E2B01010C0ECC07C1CD83E0B26A4D62D547430A0202634EEDE0BB70D8A6CBCDF90A01010C2B8E07C18383F0866F4E8B92D5CFE502027CA794E0BB70367080CBC9770C01010D0EB507F98A80B1B26AECC8D52FE50202",
                            "782A94E0C5D82736D880CB90000101000FCCE1937354F0866AEC62552FE50B025D4D6DB1C5D8272736E0809A0D0101000E0C52C173CEE0B26A8FD05547640B02684DB494E09D362727A6E0CB0E0101262B2BB53C8AD8F3F0A22A7862D5640B189E5D4EB2C79D3627303672800E00010C0D0E393C837370736F2A7862AE430B0BD0234DA294C570273027A6E04E00010C000ADE52838A36CECB2A7CD055430B0A9E234DF2B2E09D70302736BB93000101260A26513C83D870E04E636855CF0B0ED0234D4EB2C79DD8303636A6CB0001010C2B0ADE9CCDD836A6CD8F68AECF0A110623632A88B172A3302736A6E00001010C0D1875B5CDD82770C7EC68AECF0F110623632AA2EDC5F3362736709600010101000B8ECCCDBB3636E02A5D55CF0E0D062363EC2A88C79DA33636EE9600010101000B11CC938A3636734E7892CF0E0D062323784DB4EDC5F33636EE9600010101000A02753C83D836BB4E7C92CF640D060623234DA294C5F3EEBFEE9600010101000A02953C83D836A6FC8FC8CF6477060623237C4D88B19DEE70BD9600010101000A028E9C83D83670ED4DC8DB432606062323A94DA29472EEEEBC4F00010101000A028EB5F070BF70E02A622F432606062323784DF2B2E0F3BCBC4400010101000A028EB58373EE70734E622F382606060623237C4DB4B296447DBD00010101000A028E39CD73EE49E0FC622F38260606062323A94D4E88B1A17DBD00010101000A028E517F73A349E0CDD04738260606060623A94D4EFC94967D7D000C0101000A180CB6B76C4040A1CBD047CF0C060606060623A94D2AFC807D7D005B0C01000A188ECCF95440404F80B647C49506060606062378A9D44DB27D7D000000002B0A18CC517953404081B7B647C47500000000000000000000000000000D000E2B00000000000000000000000D2B2B0A0E160000000000000000000D162B0E0A0E2B00000000000000000D0D2B0E0A0A0E0E16000000000000000D0D2B2B0F0B0A0E2B0D0D0000000D0D0D0D2B0E0A180A0A2B2B160D0D0D0D0D0D0E0E0A0B020A0A0E0E0E0E0E0E0E0E0E0E0A0A180202020B0B0B0B0B0B0B0202180B0B025653534F8DE6273030BF8D827F525190F9C9548DBFBF27303036EEF37369519079535449EEBFBFBFBFBFEE726C7959DE9AB77F5354A3EEBFEEA3A3967F939CAC59937F53C996F3A3C5E0726C7F93519ECC9C93B7F979795680966C7F799C51B690AC903939599C9A5693799A9C59395197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE957E74FE547E537E337E32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D306930651061105CF058F054D050D04CD048B044B040B03C9038903470345030502C50283024301C3018301810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE957E747E547E53FE337E32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707950755071506D306930651061105CF058F054D050D04CD048B044B040B03C903C903870347030502C502430243028101C101810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE94FE747E547E53FE337E32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D306930651061105D1058F054F050D04CD048B044B040903C903C90387030703050285028306810F81050101810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE957E757E74FE73FE53FE32FE32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D306930651061105CF058F054F050D04CD048B044B040903C903890347030703050285068106810241020101810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE757E74FE53FE53FE337E32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D306930651061105CF058F054F050D04CD048B044B040B03C903890387030702C703030F41028101C301C10181010100C300C101010101034197F197F197F197F197F197F197F1A7F3AFF3A7F387EF7FED77EB6FEB67EB5FE957E74FE747E537E32FE32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D306930651061105CF058F054F050D04CD048B044B040B03C903870347030702C703030641024101C3018301050181014105810EC1054104C197F197F197F197F197F1AFF3A7F31FDF071507555FE977EB67EB57E74FE547E53FE347E547E537E32FE32FE127E11FDF17DD0FDD07DB07DB07D907D907D907D707950755071506D306930651065305CF058F054F050D04CD048B044B040903C903C903470307030502C5044103C10201020104810F810F01060104810381034197F197F197F19FF197F12FE1044B000100010001000106930693010101410141014105CF4FE73FE52FE32FE127E11FDF17DD0FDD07DB07DB07D907D907D706C307530755071506D306930651054505CF058F054F050D04CD048B044B040903C903890385034302C70285024305C106410E8117C106010381024101810001018197F197F197F1A7F30755000100010001000101C106D307D70715008100010001000100C105CF47E537E32FE127E11FDF17DD0FDD07DB07DB07DB07D906C106C507D70755071506D30693050105470611058F054F050D04CD048B044B040903C9038903C50641030502C5024503C127C10E8104410281018100C100010001000197F197F1BFF5075500C1000100010001020357E72FE307952FE117DF0101000100010001000105D147E52FE127E11FDF17DD0FDD07DB07DB07D91F070481075507D70755071506D30651040106110611058F054F050D04CD048B044B040903C9038903870F010541020302850243060103C101C10141010100C100010001000197F1A7F327E101410001000101C100C1030702450001000100011FDF06D30001061106D305CF06934FE52FE127E11FDF17DD0FDD07DB07DB07D90F0102C1079507D707550715075506910381034105D105D1054F050D04CD048B044B040903C90389034903090541040101C1020102C101C101810141010100C100010001000197F1BFF7065100010001054F07DB010100010001000100C10181014107D71FDF57E70715065107D72FE137E31FE11FDF17DD0FDD07DB07DB07D9050102C107D707D70755071304C70693058702810341058F058F054F04CD050B048103C904090389034903070305058104010301024101C101810141010100C100010001000197F127E10141000104091FDF0141000100C1050D069307D97FED05D105CF17DF0181000100010001014107552FE31FDF17DD0FDD07DB07DB07D9050102C1064F07D707970713044103C1054B048102C1024103010449050B070104C103C9040903890387030702C702C504C103C1020101C301810141010100C10001000100C197F10693000101012FE102010001014107DD0795028501C1044B77ED069300810001000100010001008107552FE317DF17DF0FDF07DB07DB07D906410341050907D70651075706D30481030103C1048103C1034103410541060102C1038703CB03C704010309030702810381030101C101C30181014100C100C101010341048197F10755048B1FDF47E502430001014104090001000100010001048B0611000100010001000100C1079747E72FE327E10FDD07D507DD07DB07DB07D905C106011F0D1F050709058103810241028104850601058104410441048103C103C104C106C10401024102C5024103010241020301C3018101410101034104810481048197F187EF2FE1044B0409069100C101810101018101810101010104CB06D300C100010001000101032FDF1793074D0F4F06C1064B17DF07DB07DB07D907D717810E010481044103C10201034105CF0611058F02C102C104CD048504810481054105C1034102C10281034103410201020301C1014102810381048104C10481048197F107D700010001000100C1000107DB7FED57E767EB054D050D6FEB67EB06510001000100010201060104C103C10501064106011797074B074F07D904C30581038104890489040102410611065105D105D104470383050D048F0541054103C70387038702810281044103010203020301C30181020102410301038103C1044197F107130001000100010001044B3FE507D701810181010101010181069367E9048B0141020106050FDB17DD07DB068B050305410F011FC105C104C104C103C104C907D9079705CD02810611065105D1058F054D0489050D048D0681048703CB0389034702410281040103010201020301C101810141010101810201024102C197F1071300010001000101419FF107DB00C10001000100010001000100C10611075527E13FE337E32F55060104C102C105490FDD0603054104C104C103C1018104C907D70755069302C1060F065105D1058F054F054D048D05C51781044903C90389034702C302C7034104410201020301C101810141010100C100C101C1020197F1079500C10001000101817FED03C9000100010001000100010001000100C101413FE34FE73F1116C1038102C10281028107DB0FDD0751064907D70797038104C707D7075506C303410611065105D1058F054F050D05C527C10F41040903C903C90387030702C702C504C103C1020101C301810141010100C10001000100C197F1A7F3075500C1000101816FEB040900010001000100010001000100010001058F5FE947E31F8103C102410281020104030FDD07DD07DD07DB07D907D907D707550757074D17C10301050B065105CF058F054D0F010E8104810E010381040B03C9034903070305058104010301024101C101810141010100C100010001000197F19FF1A7F3075500C1000107D70FDD00C100010001000100010001000100C1069367E94FE50681028102810281024107D517DF07DB07DB07D907D907D707D70797075717091FC103010445065305D1058F068117410401024104C103C102C10387034903090541040101C1020102C101C101810141010100C100010001000197F197F19FF1A7F3071501410243061100C100010001000100010001000106115FE94FE74FE50581024102810241079327E10FDD07DB07DB07D907D907D707D70797075717091FC10541060F065105CF058F058B050103010281030103410241034503870F010541020302850243060103C101C10141010100C100010001000197F197F197F19FF18FEF7FED87EF07DB00C100010001000100010001040957E75FE74FE74FE5058102410201044327E117DF0FDD07DB07DB07D907D907D707D707950757074D1FC10601060F061105CF058F054D04810341028102810281030103C903C50641030502C5024503C127C10E8104410281018100C100010001000197F197F197F197F197F19FF19FF1AFF51FE107550409014103C9071557E767E957E74FE757E706CB0201024107D727E117DD0FDD07DB07DB07D907D907D707D70795075507131F8105C10611061105D105CF04870301030102810281020102C103C90385034302C70285024305C106410E8117C1060103810241018100010181",
                            "97F197F197F197F197F197F197F197F19FF3A7F37FED5FE96FEB87EF67E957E757E74FE547E52FDD0481028117DF1FDF17DF0FDD07DB07DB07D907D907D707D707970755071707070441050B065105D1058F044503C70281028102810241034503C903470307030502C5044103C10201020104810F810F01060104810381034197F197F197F197F197F197F197F197F18FEF87EF87EF7FED77EB67EB5FE95FE957E74FE53FE54FE707D5044327E11FDF17DF0FDD07DB07DB07D907D907D707D70795075507150713044103C3069305CF058F05D104050241024102410341040B03890347030702C703030641024101C3018301050181014105810EC1054104C197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE957E757E747E53FE53FE327E107D92FE11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D3069303C1050B0611058F058F04C904C902C102010301040B03C90387030702C703030F41028101C301C10181010100C300C101010101034197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE757E747E547E53FE337E337E327E11FDF17DD0FDD07DB07DB07D907D907D707D707950755071506D306D3065105CF05CF058F054F054F050D048B02C1024103C903C90347030703050285068106810241020101810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE957E747E547E53FE337E327E127E11FDF17DD0FDD07DB07DB07D907D907D707D707950755071506D306930651065105CF058F054F050D04CD04CD044B030103C703890387030703050285028306810F81050101810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE957E74FE74FE73FE53FE337E327E127E11FDF17DD0FDD07DB07DB07D907D907D707D707950755071506D306930651061105D1058F054F050D04CD048B048B044903C9038903870347030502C502430243028101C101810141010100C100010001000197F197F197F197F197F197F197F197F18FEF87EF7FED77ED6FEB67EB5FE95FE957E74FE747E53FE337E32FE127E11FDF17DD0FDD07DB07DB07D907D907D707D707970755071506D306930651061105D1058F054F050D04CD048B044B040903C9038903470345030502C50283024301C3018301810141010100C100010001000102020202020202020202020202020202181818181818181818181818181818180A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E16161616161616161616161616161616000000000000000000000000000000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0101010101010101010101010101010101000000160A02020202020202020202015653C944E6BF303027BFA1C96951900CF9C96CE6BF27303027BFA37279519016795382BCEEBFBFBFBFEEF3819341DE169AB77F5396EEBFBFA3A39D53F99CAC1859937F5354A1A3BBE0C5C553F9519E02B669F9B7937979F9E096C9939C51B60290ACDE3959A09A7993569A6941395100000000000000000000000000003AEE1A342839944D4DA1D61F0A234A151F0B0FEC87E308E3BD143898CECECE8B3826388BC461C4892C7186EDDC8282E877770000000000000000000000000000006FDC7F28397735158150EDEDDC24E80E100BBD4734038FD02A164898CCCECECE0F7138AE8BAF2D38BDED244242424242B90000000000000000000000000000023AEC1AEF7F29797930477070246EDC1BD3D30EED1AEF21394A0FBF6F7D7DCCCCCC98BE482A4848BEDC1A34210308FAEDAF000000000000000000000000000000109A1A34EF7F8A1A1A6EB9824D6B09406BEEED791A7F285B4B6B0EBFBF987D7D7D7D7D6F16486847873903A6B6FCAF5555000000000000000000000000000000021BDC4228E35B4BA16B10090A8150E84730291A7FEF28E35F39EE0E1BBF4A7D7D7D0EBF164A30422803DEB5EC5598794B000000000000000000000000000000000052DC7F28287F821550EDDC30A924291A7F282821030305053982520B3ABF183A3A36EE6E392105E9E75555555555790000000000000000000000000000000002239A1A7F7F8A2447A97024246EC2DC7728051160609108050588AAEC6BD33D0052E81A392163DEE79898EDEDED9898000000000000000000000000000000000002106BA9427F871A29A9DC825057405750945B11A611116008055B7FDC505781E81A3921056011BB1C1C1C1C1C1C1C000000000000000000000000000000000000023A4A298AAAE850812F000000D300006B825F0811A6A61108080588EFB97942210303038894E7E7E7E7E7E7E7E7000000000000000000000000000000000000002F3AECD65215100000D3092FEED6150952D64B880560A6A6111160055B21030503E342E8356B3D06063D3D3D6B000000000000000000000000000000000000000002D30000000000092FDC303030478209236B82EF39030811A6A611916305050342E82BD2982F0606060600560000000000000000000000000000000000000000000000000000021F30291A8A7F8724EC3A00236B826C42210591916305050342E8BF3D9856982F06063D535300000000000000000000000000000000000000000000000000000023DC1A395B03888AEE0900000023EEEC6E39030505050342E8BF9A5656565656569853535300000000000000000000000000000000000000000000000000000040818294284B82502F2F00002F4D164A29210305050342791BD29898989898989898535353000000000000000000000000000000000000000000000000000000000002816B6B5700000000402FFFEE1A39030505037FED000000D20606060606060698535300000000000000000000000000000000000000000000000000000000000000000000000000D602BDED29390305050321C29AD62F2F0AD2060606060606069853000000000000000000000000000000000000000000000000000000000000000000000000402BBEDC1A3921050503E3283487C2DCED502F09090606060606D25600000000000000000000000000000000000000000000000000000000000000000000002F40BD3029423421056305050503032139426CAADC50D609D209060698000000000000000000000000000000000000000000000000000000000000000000000000026BD650DC7F42422105089163630505035B21347FAAB9E84D6B153D00000000000000000000000000000000000000000000000000000000000000000000000000020010526B4D826C34395B0508916305050505032121346CAADC50000000000000000000000000000000000000000000000000000000000000000000000000000000000000092315184D94D5215B0508919105050505035B2121340000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000023526B4D774BE35B0505089163050503030300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000151515578294D5215B050891916363000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A15156B4D774BD5885B05080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000091515578294EF3900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A15526B4D0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010203040506070608080808090A0B0C0D0B0D030E000F10080808060711120213020E0D0E0B140B0115060610161718191A1B02020B030C0C1C1D091E1F18202021222324020B0C25261427281F292A2B2C172D2E2F30311C0C320C313334353637173638393A3B1B1314033C3D3E3F40414243444546474849264A274B4C4D4D4E4F5051525354555612035758595A5B5C5D5B5E5F205A6061623102635A5F5A646565655B66676869620E3C6A6B6C6B5F656D6E65446C6F70120B3C6A71725C5C1773732073172074120D3175767735787378783779722C49120231757A7B7C6D73362A7837177D7E120231757F7B807B81362A788272837E7E023175847B856D81863641877288898A0202758B438C418D372A8E79738D4902023275504E4E8F4F90915092935C7E0202037594955A73969798677236990D027E02759A9B6E9C6E8596679D98991C029E9F75A0A194A2A3A0A4989898A50D577E9F75722081201773735CA6A7A81C033C7575A979AA8D20AA37ABACAD310B02307575AEAAAA78AFB0B1B20D023CB31EB47575B58678B0B6B7B8000213B3B9080775757535BABBBC02131CBCB4B90808087575BD40BEBF02131DC01EB908080808757502BCBC02580F280808080808080802C1B3241CC2C310080808080808080858C1138A1E1E060808080808080808080458071E07080606060606060608080815B3000000010202020303030402010105040607080709060A0B0C0C0D0E0D0D080C080D08080808080D08080D0D0D0F080A07090A080A0809091007070909090909061007070909090A060610070909070906110607100709091007051207070907100506130907141115161717171517150509050601100110020D06010210021029F32A353A7742FB4B3B4B7D53BF53FF29F529F329F529F5323532353A773A7729F529B321B1216D196D19732179217729F521B3216F192710E1214D214D214D29F521B1196D10E5214D7387BDC1BDC129F521B1192B10A3214DBDC139C139C129F5216D1927089F214DBDC14A41F7C529F5196D10A3085D214DBDC17BC3F7C5294B421152956B5B7BDF84218C638C638C638C637BDF6B5B5AD74211318D294B6B5BA529CE73DEF7EF7BF7BDF7BDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF318D4A53739D8C639CE7B5ADC631CE73D6B5DEF7E739EF7BF7BDFFFFFFFFFFFF18C7294B318D39CF318D210921092109294B294B294B294B318D318D318D4211",
                            "8C638C63739D5AD752955AD76319739D7BDF8C6394A59CE7A529A529AD6BAD6BFFFFCE73AD6B94A58C638C6394A59CE7A529A529A529A529AD6BBDEFBDEFBDEFFFFFE739DEF7DEF7BDEFAD6BA529AD6BAD6BAD6BAD6BB5ADBDEFBDEFBDEFCE73F7BDD6B5CE73CE73CE73BDEFB5ADB5ADBDEFBDEFC631C631CE73CE73C631D6B5F7BDD6B5CE73C631C631BDEFBDEFB5ADB5ADBDEFD6B5CE73CE73CE73CE73CE73F7BDD6B5C631C631BDEFB5ADB5ADB5ADB5ADBDEFCE73E739E739E739DEF7D6B54A536319739D84218C6394A59CE7A5299CE79CE79CE79CE79CE79CE79CE79CE76B5BC631DEF7EF7BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4A536319739D739D739D739D739D739D739D739D739D739D739D739D739D739D0847084B084B088D088F08910891109310D510D910DB10DB10DB10DB10DB10DB088F084D10D5191B216321AB29F129F32A3532393ABB433D4B3F4B7F53BF53FF10D3216729F329F529F529F532373237323732393ABB433D4B3F4B7F53BF53FFFFFFE739D6B5D6B5BDEFAD6BA529AD6BAD6BAD6BB5ADB5ADBDEFBDEFBDEFCE73F7BDD6B5CE73CE73CE73BDEFB5ADB5ADBDEFBDEFCE73CE73CE73CE73CE73D6B5739D739D739D739D739D739D739D739D739D739D739D739D739D739D739D739D5AD78C639CE7B5ADC631BDEFBDEFBDEFBDEFC631C631CE73CE73CE73CE73CE735AD78C639CE7A5298C63735B5295318D41CF5253739DA529A529A529A529A529C101C101C101C101A90168C138C110C120C138C161018101B101C101C101C101FC01F341E301DAC1C28199C1594139016941A201B281CAC1E341FC01FC41FC41FE01FE01FE01FD81FCC1D34179C149419A81D3C1FD01FD81FDC1FE01FE01FE01FE01FE01FE01FE01FDC1EC0182015981A2C1DC41FD81FE01FE01FE01FE01FE01FE01FE01FE01FE01FDC1FC818A416981B301E441FD81FDC1FE01FE01FE01FE01FE01FE01FE01FE01FE01FD01A2C179C1BB41EC81FDC1FE01FE01FE01FE01FE01FE01FDC1FD81FD41ED01A34159C151417A81BBC1D481ECC1FD41FD81FE01FE01C101C101C1019101910170C158C170C170C1910191019101C101C101C101C1015AD78C639CE7A5298C63735B5295318D41CF5253739DA529A529A529A529A5295AD78C639CE7B5ADC631BDEFBDEFBDEFBDEFC631C631CE73CE73CE73CE73CE73739D739D739D739D739D739D739D739D739D739D739D739D739D739D739D739DDEF7DEF7DEF7DEF7000100010001000121092109210921090001000100010001B5AD8C636B5B5AD70001000100010001DEF7DEF7DEF74A530001000100010001DEF7DEF7DEF739CF00010001000100018C638C639CE7318D00010001000100015AD76B5B739D294B000100010001000121092109210921090001000100010001C001C001C001C001C00198017001580150015001500160018001B00100010001C001C001C001C001A80180016001500150015001500160018801B00100010001C001C001C001B001900168015001500148014801500160017801A00100010001C001C001B801A801880168015001480148014801500160017801A00100010001C001C001C001B001900170015001480148015001500160017801A00100010001C001C001B801A001880168015001480148015001500158018001A00100010001C001B801B0019001780160014801480148014801480150017801A00100010001C001B801A8018801700158015001400140014801380148017801A00100010001C001B80198018801700158015001400140014001380148017001A80100010001C001B80198018801700158015001400140014001380148017001A80100010001C001B80198018801700160015001400140014001400148016801A00100010001C001B801A0019001700168016001500150014801480148016001900100010001C001B801A0018801700190017801680160016801600168017801A00100010001C001B001A0018001700140014001400140014001400140014001400100010001C001B801A8018801700100010001080100010001000100010801000100010001C001B801B001980180019801B001B001B001B001B001B001B001B00100010001C001B801B001A80190019001980188017001680178018801A001B00100010001C001C001B801A801900188018001700158015801600170019801B00100010001C001B801B001A00188018001700160015801500158017801A001B00100010001C001B801B0019801800170016801580150014801480168019801B00100010001C001B801A8019001780190017801680160016801600168017801A00100010001C001C001A8019001700140014001400140014001400140014001400100010001C001C001B801A001800100010001000100010001000100010001000100010001C001C001B801A00188019801B001B001B001B001B001B001B001B00100010001C001C001B801A80190019001980188017001680178018801A001B00100010001C001C001C001B801980188018001700158015801600170019801B00100010001C001C001C001B001A00180016001500148014801480158016801B00100010001C001C001C001B001A80180016801580148014801480148014801B00100010001C001C001C001B801A00188017001580150014801480148014801B00100010001C001C001C001C001A80188017001580150014801480148014801B00100010001C001C001C001C001B80190017001580150014801480148014801B00100010001B801C001C001C001C00198017801600150015001480148014801B0010001000130C130C130C12881184100010001000100010001000100010001000100010001AC41CC81D4C1EE01F743FF8BFFD3FFD7FFD5FFD3F749E581CC8193416A414181BC01D4C1DD41F703FFCBFFD3FFD5FFD7FFD5FFD1F74BEE87D583A3816A414181ABC1CC81DD01EE43F6C7FF8DFFCFFFCFFFCFFF8BF6C7EE45E541ABC182C151819B81C441CC81CC81DD41F685FF8DFFCFF74BF687EE87DDC3CC81BC018B0151C17AC1B401C481CC81D501E581F685EE89D5C7D583DE05D583CC81B3C16A41624362419B41BC01CC81BC41B401ABC1B401CCC3DDC3E581D4C1AC018B016A416A8149C15201518172817AC183019381ABC1B401B441AC019B819B4182C172817281294B421152956B5B7BDF84218C638C638C638C637BDF6B5B5AD74211318D294B6B5BA529CE73DEF7EF7BF7BDF7BDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF318D4A53739D8C639CE7B5ADC631CE73D6B5DEF7E739EF7BF7BDFFFFFFFFFFFF18C7294B318D39CF318D210921092109294B294B294B294B318D318D318D42118C638C63739D5AD752955AD76319739D7BDF8C6394A59CE7A529A529AD6BAD6BFFFFCE73AD6B94A58C638C6394A59CE7A529A529A529A529AD6BBDEFBDEFBDEFFFFFE739DEF7DEF7BDEFAD6BA529AD6BAD6BAD6BAD6BB5ADBDEFBDEFBDEFCE73F7BDD6B5CE73CE73CE73BDEFB5ADB5ADBDEFBDEFC631C631CE73CE73C631D6B5F7BDD6B5CE73C631C631BDEFBDEFB5ADB5ADBDEFD6B5CE73CE73CE73CE73CE73F7BDD6B5C631C631BDEFB5ADB5ADB5ADB5ADBDEFCE73E739E739E739DEF7D6B54A536319739D84218C6394A59CE7A5299CE79CE79CE79CE79CE79CE79CE79CE76B5BC631DEF7EF7BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4A536319739D739D739D739D739D739D739D739D739D739D739D739D739D739D00010001000100010001000100010001000100010001000100010001000100010801C00178015801500150015001500150015801580168017801A801C001C0012801C0019801600150015001500150015001580170019001A001B001B801C001231D18140E0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C221D17130D0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C221D17120D0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C221C16120C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C211C16120D0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C211C17120D0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C211C17120D0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0D0D0C0C0C0C0C221D17130D0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0D0D0E0D0D0C0C0C0C221D18130F0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0D0D0E0F0F0F0E0E0E0C0C241E1815100C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0D0D1118FA161613110F0F0D251F1A16100C0C0C0C0C0C0C0C0C0C0C0C0C0C0D0E131AADFCAD261E1916131126201C16110C0C0C0C0C0C0C0C0C0C0C0C0C0C0E1227D5FDFDAD42362B25211C27221D18130E0C0C0C0C0C0C0C0C0C0C0C0C0D1430CCFCFDFDAF68584C443F3428231E1915100C0C0C0C0C0C0D0E0E0F11171A47FDFDFEFEF393887A726D685C2A24201B16120C0C0C0C0C0E11151C2043AEAFFDFDFEFEFAB19F99908E8C887B2B27221D18140E0C0C0C0D111F3060DBFDFDFDFDFEFED1A2A2A19E99989690892D29241F1A16110D0F0F1A3987E9FDFDFDEDE8E5D69F9BA1A2A19F9B9790867B2F2A26221C1E1A446287AAE8FDFDF5C9846F6364707D90999FA09C948778675B312D28241E2A2FBFE4FDFEE2A5854E3F30292629303F6689989B94846D533D31332F2A26221C1C42888F917B4E2D1D152A47462D10162A5B8793886D4F301F1736312D29241F1C29598A906724103BBEFFFFFFFFFF8E142B60827857321C120E39342F2B27221E1B2A577148172AFFFFFFFFFFFFFFFFFF1D3F6C725124140E0C3C36322D2A26211D1B212A1D0FFFFFFFFFFFFFFFFFFFFFFF2D5F6F5023100D0C3E3A35312C2824201C181612A3FFFFFFFFFFFFFFFFFFFFFF564F6F5A240F0D0D403D38332F2C2824201C1612FFFFFFFFFFFFFFFFFFFFFFFFEB3764632B0F0D0C433F3B37332E2B2723201811FFFFFFFFFFFFFFFFFFFFFFFFF42456672D0E0C0C46423F3A36322E2A27221910FFFFFFFFFFFFFFFFFFFFFFFFC51A33411E0E0C0C4A45413E3935322E2A261C10FFFFFFFFFFFFFFFFFFFFFFFF84141716100E0C0C4D4845413E3A35322D292112A9FFFFFFFFFFFFFFFFFFFFF82815140E0C0C0D0C504D4844403D3936322E251684FFFFFFFFFFFFFFFFFFFF511316160E0C0C0D0D53504C4744403E3936322B1E2DB4D4FFFFFFFFFFFFFFA4111019150F101416135753504C4844413E3B3631281A1F84F0FEFFFFFFFF7E0E0E1016171824323C355A56534F4B4845413F3B3730271E143CBAC7BD530E0E0E10141C294276908B7B5D5A5653504C4945423F3C38332B231B171615151313162A433F88FDFEFDA29E615D5A5753504D494643403E3A36322B272423201D1D285C8496F1FEFEFEA3A265625E5A5653514E4A4745413F3C39322D29241E1B232F598597FDFEFEDE9C9C6866625E5B5754524F4C494643403B3329211A16222F323D647DCBFDFD86878C6C6867635F5C595553504E4B48433A2A1F18161A2F3C322F3342477A555C6168716D696764615D5A5754534F4B4333201614192C4A4330241E1A1A1B1F262A3475706D6A6765625F5C5956534D3F28161214204862422619120F0E0E0E0F1115",
                            "7975716F6B686663605E5A57503B1F141525446C603119100D0C0C0C31696A6B7C7976736F6C696765625F5C5234181520497772471F110D0C0C3A9EBFFAFAFA807D7A7774716E6B6967656053301B1B3A77876633170E0C0C5ACDFAFAFAFAFA84817E7B797573706D6B6865522D25366A9084542A140D0CA9EBFAFAFAFAFAFA8885827F7C7B7775726F6D6749272F60929A804D28130DA0FAFAFAFAFAFAFAFA8C8A8684807D7B7977757267402A42849E9C7C4D2A1668FAFAFAFAFAFAFAFAFA918F8B8885827F7D7B797763372B5491A19C7E523257FAFAFAFAFAFAFAFAFBFB9491908D8A878482807E7C67352E6198A29B815C3EE1FAFAF8F2F0EFEEEEF0F2999693908F8C8A87868483744037709CA29F886651FBF7F2ECE9E6E6E5E7E9EB9D9B989592908F8E8C8B8B8459497E9FA3A08E769FF6F0EAE5E1E0E0DFE1E1E3A29F9D9A9694939291919291745B8AA0A3A19382E0F2ECE7E2DFDCDBDADBDBDCA3A3A19E9B9998979798999A846A8EA0A3A3988EF3EFEAE5E2DEDBD8D6D5D4D6A3A3A3A3A19D9C9B9B9D9D9D8E768FA0A2A39C92DEECE8E5E1DDD9D5D2D0CFD0A3A3A3A3A3A2A19F9FA0A0A0998490A0A2A39D90A9E3E30000DBD7D3D0CDCCCCA3A3A3A3A3A3A3A2A2A1A2A2A097919CA0A2A18C61C5D5939BD70000D00000CCA3A3A3A3A3A3A3A3A3A3A2A3A3A29B9CA1A2A197724BA6C4CFD39D9CD39996CFA3A3A3A3A3A3A3A3A3A3A3A3A3A3A2A1A2A2A3A093663130ABC8D19AD39B9AD1A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A2A1884B282987ADC3C7CCCFD0A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A29C81532F262A3C76939DA2A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A39F9072513B322B2A2729A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A29F947F736A665C53A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A2A2A1A1A09D9A93A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A1A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A3A30F1A242F34281C111A2C3E55574C341F2B446C9098734B2E375D9CFFFFA56037375FA8FFFFA45C362D4D79A29F71472A1E314856523E2C1B101B242F2C23180E004DEFB36D0E001F593838B6705E6D0006687F3370592ECFAC0100526D61AC1006000048B66D6BFFFF4F00000400729513000081AC485EEFFFE000000000005900000009292200ACFFA21A06290900337F2C5938337F9FEDB3130070B36D6DB8B36DB3FF68245233000000B6FFFCCFFC272290FF93000001000059EAFFB81070000000909000009061010081B33B0000000000010E0154AC2C00000038610000B1CC90330000100600001A0B13B36D6DFFFFFFFF4500000000009AB333523D6170FFF99A220000275CCCFFFF2C000000006DCF0E000000279DCAFFB80000000000002E3B10000000004F9A70102ED4590000386D100000062ECF6870706DA900000000000000000000000000000000000000000000000000000000000000000000000000000000000000111114210000001126071C000000000000000000000000000000000000001C26020000000000000000000019280500000000000000000000000000000C3F0C00000000000000000000000000002D35000000000000000000000000163F0000000000000000000000000000000000193C000000000000000000002637000000000000000000000000000000000000000C49000000000000000011440000000000000000000000000000000000000000001C44000000000000005D0F000000000000000000000000000000000000000000003C3C0000000000492D070000000000000000000000000000000000000000000011550200000007641402000000000000000000000000000000000000000000000A3A530000004B50160000000000000000000000000000000000000000000000072871000000733C190000000000000000000000000000000000000000000000072B5F4100166C3A1900000000000000000000000000000000000000000000000A2D5358004B803F1607000000000000000000000000000000000000000000001428558C00538A3F230C000000000000000000000000000000000000000000001632559E0041874B2D110000000000000000000000000000000000000000000A1C3F5F8A0041995F3F1C0500000000000000000000000000000000000000000F2B4E788F005AAA71462B0F0000000000000000000000000000000000000007193A5887AF0064BC825D3C190F00000000000000000000000000000000000216324B7394BE0041B996764E32160C000000000000000000000000000000000F23415F87A89B0000C6A08A6E4E32160F00000000000000000000000000071123415D8096C8760000ADC19E876E4E35230F07000000000000000000000C1423415D7B94A8E43500004EEEB4A0876E4E3F2B190F0A0A000000020C0F162335445D7B94A8D0D000000000D7E6B49E8C785F4E3C2D2616191919191C2B3244556C8599A5C6FF53000000003FFFDAB4A39485765F4E41413C3C3C3F414655697D8C9BA8C6FACB00000000000099FFE4C6A89B94877671675855555F7371808F9BA5B4D7F8FD1600000000000000C8FFEEDAC1AAA39B94878C8C8C8A919E9BA5B4CDE4FFFF41000000000000000000D5FFFFEEDCC8BEAAA8A8A8A8AAA8B4BECBE6F5FFFF580000000000000000000000B2FFFFFFF3EEDFDADADADADADAE9F0FAFFFFFF350000000000000000000000000050FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC3000000000000000000000000000000000076FFFFFFFFFFFFFFFFFFFFFFC3260000000000000000000000000000000000000000006EAFB4B2B2B49B300000000000000000000000000094004D00CD0000015F02357314187E000000B600E500000062FFEC007700F00000000202910000FF09056D00DF73A20079FFBA00260000039F04EF5ECCCB320094FFBC00BE0000029D056668C712320000FF5500BE0000037007BA00890D32FF87FFBA00260000039F04EFA2CCCB320000FF7400260000042D0683009FBA32FF6CFFBC00BE0000029D056698C71232FF6C004D00CD0000015F02358D14186A0000008C0026000001F300330060B968FF87004600260000028201C7AD3EC53A0079004600260000028201C7533EC548FFABFFBD012F000001E105C8AFCC48400000FFF70171000000FE04B200BA614E0055FFBD012F000001E105C851CC484E0000FF7F012C00000263072700A7503200000000FFCE000003A2030E00008832D7000002FFFFFFFFE700000000000000E200001CC8112078FC127E60FFFFF3F8E300100100008000FD10000006018A40E800000000000000F500010007000000E600000000000000F0000000073FC000E700000000000000FD50000006019A80F5500000070D8350E600000000000000F3000000073FF200E700000000000000F5480800000D8350F20000000007C0FCFA000080FFFFFFFFD900000000230405010120240601D840060002040006080A060C0E0A000C101206000806000214160618140200121A1C060618000012160C060A100C00020018061A100A001E200A0604021200041C000614182200001C1E061E08000018062206201E1C000A081E060A0E0600060E22061A201C000A201A060E0C220012101A060C16220004121C0616120200161422DF00000000000000DF00000000000000000100000000000000000001000000000000000000010000FDBEFF23FFE000010000000000000000000000000000000000000000000000000000000000000000DE00000006017700DA3800000601DA98DE000000040021A8D838000200000040DF00000000000000000100000000000000000001000000000000000000010000FE20FF10FFEC00010000000000000000000000000000000000000000000000000000000000000000DA3800000601DB00DE00000004001D00D838000200000040DE01000006017338DE00000006016B80DA3800000601DA98DE00000006017058D838000200000040DF00000000000000FFFFFFFF000000000000FFFF00000000000000000001000002280000000000010028F7110000000008EF00280000000000000000000000000000000000000000DA3800010601DB88DE01000006017458FFFFFFFF000000000000FFFF00000000000000000001000002280000FFCE00010028F7110000000008EF00280000000000000000000000000000000000000000DA3800010601DBD8DE01000006016480DE00000006008808DE01000006017458DE00000006008808DE01000006017458DE00000006008808DE01000006016480DE00000006008808DE01000006016480DE00000006008808DE01000004003050DE00000006008808DE01000004003050DE00000006008808DE01000004003060DE00000006008808DE01000004003060DE00000006008808DE00000006017058DE01000006017310DE00000006008808DE00000006017058DE01000006017310DE00000006008808DE01000006016898DE00000006008808DE01000006016898DE00000006008808DE01000006017858DE00000006008808DE01000006017858DE00000006008808DE010000060181C8DE00000006008808DE010000060181C8009F0165FEFB00000052021FF8F589D800C50114FF1000000025025D61EABEFF008D006EFF4B0000005602C3F9D591F8009F0165FEFB000000C7006CF8F589D800E40168FF5A000000FF00806FFED5E200C50114FF10000000A200CA61EABEFF00DA00F400150000011D012873F21D94FF6A015101060000019600E5C3E86432FF68008A00840000018D01D7C4C25332005C0150010E0000009701032AE56C3200D80041FFCC0000008801DB5EB7F4FF008D006EFF4B0000005A018AF9D591F8008F0080008C0000005402023ECA5646FFEE021B010B000001170049F90B7732002F011BFFB2000000C301EAD9139188FF180064FF84000001DF027AA6DFB862FF83011FFFA70000017A01D5D42C9A56FF180064FF8400000098024EA6DFB862FF1E0118002D0000009601548AF81432FF83011FFFA70000002D01A2D42C9A56008F0080008C0000011401BE3ECA56460000000EFF6C000000E502EFFEB9A0FF0078020200F800000084007B45026232FF3101E300D8000001DC007BB0FC5832FF68008A0084000000C30182C4C25332FF1E0118002D000001E401848AF8143200DA00F400150000000C01EB73F21D94FFF2FFDB003A0000007E0259F98F268C0000000EFF6C0000002D01F4FEB9A0FFFFF2FFDB003A000000F002A7F98F268CFF68008A0084000000BE0149C4C25332FF180064FF84000000940215A6DFB862FFF2FFDB003A0000009701F7F98F268CFF180064FF84000001DF027AA6DFB8620000000EFF6C000000E502EFFEB9A0FFFFF2FFDB003A000000F002A7F98F268C0000000EFF6C0000002D01F4FEB9A0FF00D80041FFCC0000008801DB5EB7F4FFFFF2FFDB003A0000007E0259F98F268CD7000002FFFFFFFFE700000000000000E200001CC8112078FC127E60FFFFF3F8E300100100000000FD10000006007700F5100000070D0340E600000000000000F3000000070FF200E700000000000000F5100800000D0340",
                            "F20000000003C03CFA000080FFFFFFFFD900000000230005010200400601DD38060002040006080A060C0A08000E1012060A141600101812061A0E12001C1E2006222426000C2814062A1C0400122C1A061C2A1E001A2E0E06223024000E3210060E2E3200341218062C123400041C000636142800140A0C0638161400103A18010090120601DF18060002040006080A050C0E1000000000DF0000000000000000000948000001FF0000000000000000FFFCFF9800000209060098F0060098F0025F0000000003FF0000000000000000FF540032FF420406060018000600180002B90000000005FF0600B0F00600B0F003390005000BFFFF0600A3900600A390FF56003900C007FF0600C7E80600C7E802B70000000008FF06001F8806001F88033100080004FFFF06009CB006009CB00000FF99FFF90AFF000000000000000003E4FF37FFFF0B0C0600620006006200FE93FD620000FFFF0600880806008808000000000000FF0D0600BF800600BF8002B8FF5101D20E1006009B7006009B700245000000000FFF060086C0060086C0020200000000FFFF06001D0806001D0802B8FF51FE2E11130600CF080600CF0802410000000012FF0600D3D80600D3D8020D00000000FFFF06001A8806001A880291FDDA016FFF140601DAD80601DAD8000000000000FFFF0600C0480600C0480601E0A00601E0B00601E0C00601E0D00601E0E00601E0F00601E1000601E1100601E1200601E1300601E1400601E1500601E1600601E1700601E1800601E1900601E1A00601E1B00601E1C00601E1D00601E1E00601E1F01500000012000000"
    };
    int Location = 18198528;

    vector<string> Locs = {"00EAB444", "00C5655C",
                            "00C567E4", "00C5680C",
                            "00C56DBC", "00C56674",
                            "00C5669C", "00C56704",
                            "00C5672C", "00C56778",
                            "00C567F8", "00C567CC"
    };
    vector<string> Data = {"003E80FF", "060098F0060098F0",
                            "060086C0", "0600CF08",
                            "0600000006000000", "0600880806008808",
                            "0600880806008808", "0600880806008808",
                            "0600880806008808", "0600EE780600EE78",
                            "06008808", "0600880806008808"
    };

    for (int i = 0; i < Kafei.size(); i++) {
        Write_To_Rom(Location, Kafei[i]);
        Location += (Kafei[i].size()/2);
    }

    for (int i = 0; i < Locs.size(); i++) {
        Write_To_Rom(hex_to_decimal(Locs[i]), Data[i]);
    }
}

void Change_Kafei_Color(string Colors) {
    //how I got from kafei's original to what looks like a good green tunic base
    //string Original_Code = "FFFF003FF5BF001F280D000342150007000B4213FE3F1009B3FFE53F897FD47FF9BFF17FFBBFE37FFEBF000D9CBF200D1809000D92FF0023383F38FFDDBFCE7F70FF1809200B0805317FD6BF31BF00230013107FF53F947F000B0009084D084D0009080700050005000F180908070007080700050007000700071809180B0005200B1809200B200B084B084B180B200D0807000D0011001100130013000B084F100900030005100900171009109110931095001900190019001B00070805180B180B2119211B180B0807200B0021002100210021200B0849200D0023200D0023000D210D002500250025002500250013002700150027002700291091002900290029002B002B002B002D001B001B002F001F006F082D106B180B39E11007003100330033003300230023007108330871000D0037003700750075007700B7084F0039003910091009003D0029083D100900B900B900FB180900FF107F002F180B017F117B19BD307539E138B720B928B928BB28FB20FF0033003338B9200D38FF293B213F297F297F297F313F313F317F317F0015397F0015001739BF39FF403F40BF40FD50FD413F413F417F497F41BF41FF51FF59BF78FB30B378FD617F693F61FF717F793F303F4A654A654A675AE7632B00230023423F4A3F523F52BF5ABF0027627F00276A7F6ABF6AFF727F72BF7BF1633F733F7BFF813F817F000781FF";
    //Kafei_Hair_Color_HSL[i]["H"] = 118;
    //Kafei_Hair_Color_HSL[i]["S"] -= 30;
    //average saturation is 70.2022
    //average lightness is 32.1691
    //vector<string> Original_Colors = String_Split(Original_Code, 4);

    //kafei green that looks good based off green tunic
    //string Kafei_Green = "FFBF2E89BF6F13050101000151D5008100C15195CF7300C18F23B76B4E939F2756954E938721871FD7750101A727010100C1010176DB13852E893E8FBF6FCF733E8F00C100C100414E93D7755695138509C3368BB76B9F2700C100C10903090300C1008100410041094100C1008100810081004100810081008100C100C1004100C100C100C100C108C308C300C10101008101010983098309C309C300C1114500C10001004100C10A4300C1198719C719C70A430A430A4312830081004100C100C1324D328D00C1008100C1134513451345134500C1088301011385010113850101290B1BC51BC51BC51BC51BC509C31BC50A031BC51BC51C0519871C051C051C051C471C471C471C87128312831CC713051CC71C87244900C153150081250725472547254713851385250725472D090101258925892547254725892589114525C925C900C100C12E491C052E4900C125C925C9260900C12E89368B1CC700C12E893E0D3E4F2D4B5315358D3DCD3DCD3E0D460F3E8F254725473DCD01013E8F4611468F4E934E934E93468F468F4E934E930A034E930A030A4356955ED52E89368D3E4F3E4F468F468F4E934E9356955ED55ED55695460F350D3E4F4E93468F5ED54E93468F2E896B596B596B9B7B9F84211385138566D766D766D76EDB6EDB1BC566D91BC566D96EDB76DB66D96EDB95257EDD7EDD8F23468F4E9300815ED5";
    //vector<string> Kafei_Colors = String_Split(Kafei_Green, 4);

    vector<double> Saturations = {-22.7273, 14.2424, -8.02708, 12.3377, 40.9091, -59.0909, -41.4439, 40.9091, 40.9091, -34.0909, -18.0653, 40.9091, -0.424242, -0.909091, 0.0395257, -5.35957, -1.43325, 0.0395257, 1.66858, 3.55969, -24.8052, 40.9091, -5.35957, 40.9091, 40.9091, 40.9091, -0.143541, 15.9091, 14.2424, 2.69771, -8.02708, -18.0653, 2.69771, 40.9091, 40.9091, 40.9091, 0.0395257, -24.8052, -1.43325, 15.9091, 15.9091, 8.65103, -0.909091, -5.35957, 40.9091, 40.9091, 0.909091, 0.909091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, -9.09091, -9.09091, 40.9091, 40.9091, 40.9091, 40.9091, 12.3377, 12.3377, 15.9091, 15.9091, 40.9091, -16.2338, 40.9091, -59.0909, 40.9091, 40.9091, 20.9091, 40.9091, -25.7576, -19.0909, -19.0909, 20.9091, 20.9091, 20.9091, 22.7273, 40.9091, 40.9091, 40.9091, 40.9091, -39.0909, -34.0909, 40.9091, 40.9091, 40.9091, 14.2424, 14.2424, 14.2424, 14.2424, 40.9091, -25.7576, 40.9091, 15.9091, 40.9091, 15.9091, 40.9091, -47.9798, 17.3797, 17.3797, 17.3797, 17.3797, 17.3797, 15.9091, 17.3797, 18.6869, 17.3797, 17.3797, 18.6869, -25.7576, 18.6869, 18.6869, 18.6869, 10.9091, 10.9091, 10.9091, 12.3377, 22.7273, 22.7273, 13.6364, 12.3377, 13.6364, 12.3377, 2.81385, 40.9091, -50, 40.9091, 14.8221, 15.9091, 15.9091, 15.9091, 15.9091, 15.9091, 14.8221, 15.9091, 7.57576, 40.9091, 10.1399, 10.1399, 15.9091, 15.9091, 10.1399, 10.1399, -16.2338, 11.2795, 11.2795, 40.9091, 40.9091, 13.3229, 18.6869, 13.3229, 40.9091, 11.2795, 11.2795, 12.3377, 40.9091, 14.2424, 8.65103, 13.6364, 40.9091, 14.2424, 0.909091, -2.398, 2.44755, -50, -1.94805, -0.470219, -0.470219, 0.909091, -4.2522, 2.69771, 15.9091, 15.9091, -0.470219, 40.9091, 2.69771, -8.69721, 2.69771, 0.0395257, 0.0395257, 0.0395257, 2.69771, 2.69771, 0.0395257, 0.0395257, 18.6869, 0.0395257, 18.6869, 20.9091, -1.43325, 4.46049, 14.2424, 3.90122, -2.398, -2.398, 2.69771, 2.69771, 0.0395257, 0.0395257, -1.43325, 4.46049, 4.46049, -1.43325, -4.2522, -5.24476, -2.398, 0.0395257, 2.69771, 4.46049, 0.0395257, 2.69771, 14.2424, -55.0909, -55.0909, -55.3872, -55.6426, -59.0909, 15.9091, 15.9091, 3.04501, 3.04501, 3.04501, -0.143541, -0.143541, 17.3797, 1.51515, 17.3797, 1.51515, -0.143541, -0.143541, 1.51515, -0.143541, -51.3239, -1.94805, -1.94805, -0.424242, 2.69771, 0.0395257, 40.9091, 4.46049};
    vector<double> Lightness = {69.8039, 21.1765, 55.6863, -3.92157, -19.6078, -25.8824, 0.784314, -22.7451, -21.1765, -0.784314, 58.8235, -21.1765, 44.7059, 52.549, 29.0196, 47.8431, 30.5882, 29.0196, 43.1373, 41.5686, 60.3922, -19.6078, 47.8431, -19.6078, -21.1765, -19.6078, 36.8627, -0.784314, 21.1765, 25.8824, 55.6863, 58.8235, 25.8824, -21.1765, -21.1765, -24.3137, 29.0196, 60.3922, 30.5882, -0.784314, -13.3333, 22.7451, 52.549, 47.8431, -21.1765, -21.1765, -18.0392, -18.0392, -21.1765, -22.7451, -24.3137, -24.3137, -18.0392, -21.1765, -22.7451, -22.7451, -22.7451, -24.3137, -22.7451, -22.7451, -22.7451, -21.1765, -21.1765, -24.3137, -21.1765, -21.1765, -21.1765, -21.1765, -19.6078, -19.6078, -21.1765, -19.6078, -22.7451, -19.6078, -14.902, -14.902, -13.3333, -13.3333, -21.1765, -14.902, -21.1765, -25.8824, -24.3137, -21.1765, -10.1961, -21.1765, -11.7647, -10.1961, -10.1961, -10.1961, -10.1961, -10.1961, -8.62745, -22.7451, -24.3137, -21.1765, -21.1765, -2.35294, -0.784314, -21.1765, -22.7451, -21.1765, -2.35294, -2.35294, -2.35294, -2.35294, -21.1765, -21.1765, -19.6078, -0.784314, -19.6078, -0.784314, -19.6078, -11.7647, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, -13.3333, 0.784314, -11.7647, 0.784314, 0.784314, 2.35294, -11.7647, 2.35294, 2.35294, 2.35294, 5.4902, 5.4902, 5.4902, 7.05882, -8.62745, -8.62745, 8.62745, -3.92157, 8.62745, 7.05882, 7.05882, -21.1765, 8.62745, -22.7451, 10.1961, 11.7647, 11.7647, 11.7647, -0.784314, -0.784314, 10.1961, 11.7647, 11.7647, -19.6078, 14.902, 14.902, 11.7647, 11.7647, 14.902, 14.902, -14.902, 16.4706, 16.4706, -21.1765, -21.1765, 19.6078, 2.35294, 19.6078, -21.1765, 16.4706, 16.4706, 18.0392, -21.1765, 21.1765, 22.7451, 8.62745, -21.1765, 21.1765, 21.1765, 24.3137, 14.902, 8.62745, 18.0392, 19.6078, 19.6078, 21.1765, 22.7451, 25.8824, 11.7647, 11.7647, 19.6078, -19.6078, 25.8824, 24.3137, 25.8824, 29.0196, 29.0196, 29.0196, 25.8824, 25.8824, 29.0196, 29.0196, -11.7647, 29.0196, -11.7647, -10.1961, 30.5882, 32.1569, 21.1765, 24.3137, 24.3137, 24.3137, 25.8824, 25.8824, 29.0196, 29.0196, 30.5882, 32.1569, 32.1569, 30.5882, 22.7451, 14.902, 24.3137, 29.0196, 25.8824, 32.1569, 29.0196, 25.8824, 21.1765, 13.3333, 13.3333, 16.4706, 19.6078, 24.3137, -0.784314, -0.784314, 33.7255, 33.7255, 33.7255, 36.8627, 36.8627, 0.784314, 35.2941, 0.784314, 35.2941, 36.8627, 36.8627, 35.2941, 36.8627, 33.7255, 38.4314, 38.4314, 44.7059, 25.8824, 29.0196, -22.7451, 32.1569};
    string RGB;
    map<string, double> HSL;
    map<string, double> HSL2;
    double H, S, L;
    vector<map<string, double>> Updated_HSL;
    string New_Colors = "";
    string New_Colors_2 = "";

    RGB = Color_To_Hex(Colors);
    HSL = RGB_To_HSL(RGB);

    for (int i = 0; i < Saturations.size(); i++) {
        H = HSL["H"];

        S = HSL["S"];
        S += Saturations[i];
        if (S < 0) {
            S = 0;
        }
        else if (S > 100) {
            S = 100;
        }

        L = HSL["L"];
        L += Lightness[i];
        if (L < 0) {
            L = 0;
        }
        else if (L > 100) {
            L = 100;
        }

        New_Colors += hex_to_rgb5a1(HSL_To_RGB(H, S, L));

        //invert hue and lightness
        H += 180;
        if (H > 360) {
            H = H - 360;
        }
        L = 100 - L;

        New_Colors_2 += hex_to_rgb5a1(HSL_To_RGB(H, S, L));
    }

    Write_To_Rom(18255720, New_Colors);
    Write_To_Rom(19210752, New_Colors_2);
}

void Make_Bottles_Work() {
    string Jump_Function = "0C061912";
    vector<string> New_Function = {
        "27BDFFE8", //ADDIU	SP, SP, 0xFFE8  Get enough free memory to store stuff
        "AFBF0014", //RA	0x0014 (SP)     Store the return value
        "28A60027", //SLTI	A2, A1, 0x0027  A2 = (Item < 27) ? 1 : 0;
        "10060007", //BEQ	R0, A1          Branch if ID > 27 (A2 = 1, is Item)
        "28A60012", //SLTI	A2, A1, 0x0012  A2 = (Item < 12) ? 1 : 0;
        "14C00005", //BNEZ	A2              Branch if ID < 12 (A2 = 0, Is Item)
        "00000000", //NOP                   Previous command runs nothing here
        "0C0453F4", //JAL	giv bottl item  Jump to the normal bottle function (only gets here if bottled item)
        "91C60146", //LBU	A2, 0x0146 (T6) Load something into A2 before jump, the original jump did this
        "10000003", //BEQ	R0, R0          Jump to the end to skip the item function
        "00000000", //NOP                   Don't wanna run anymore commands for bottle
        "0C044BA0", //JAL	giv item pass   Jump to the give item passive function (only runs if not bottle content)
        "91C60146", //LBU	A2, 0x0146 (T6) Load something into A2
        "8FBF0014", //LW	RA, 0x0014 (SP) Load the return Address
        "27BD0018", //ADDIU	SP, SP, 0x0018  Put the SP back to where it was
        "03E00008"  //JR	RA              Return
    };

    Write_To_Rom(12296876, Jump_Function);

    for (int i = 0; i < New_Function.size(); i++) {
        Write_To_Rom(12700040 + (i*4), New_Function[i]);
    }
}

void Fix_Tingle_Maps() {
    int Pass = 12227676;
    int Maps = 12342348;
    string starting_maps_add = "C1CAD4";

    vector<string> Pass_New = {
        "00000000",
        "2401006D",
        "0C04BBC3"
    };
    vector<string> Maps_Fun = {
        "27BDFFE8", //ADDIU	SP, SP, 0xFFE8  Get enough free memory to store stuff
        "AFA50010", //SW	A1, 0x0010 (SP)	Store A1
        "AFA6000C", //SW	A2, 0x000C (SP)	Store A2
        "AFA70008", //SW	A3, 0x0008 (SP)	Store A3
        "AFBF0004", //SW	RA, 0x0004 (SP)	Store RA
        "28E600B4", //SLTI	A2, A3, 0x00B4  A2 = (Item < B4) ? 1 : 0;
        "14C0001B", //BNEZ	A2, 		Branch if < B4 (A2 = 1)
        "00000000", //NOP
        "28E600B5", //SLTI	A2, A3, 0x00B5  A2 = (Item < B5) ? 1 : 0;
        "14C0000E", //BNEZ	A2, A		Branch if B4 (A2 = 1)
        "24060003", //ADDIU	A2, R0, 0x0003
        "28E600B6", //SLTI	A2, A3, 0x00B6  A2 = (Item < B6) ? 1 : 0;
        "14C0000B", //BNEZ	A2, A		Branch if B5 (A2 = 1)
        "2406001C", //ADDIU	A2, R0, 0x001C
        "28E600B7", //SLTI	A2, A3, 0x00B7  A2 = (Item < B7) ? 1 : 0;
        "14C00008", //BNEZ	A2, A		Branch if B6 (A2 = 1)
        "240600E0", //ADDIU	A2, R0, 0x00E0
        "28E600B8", //SLTI	A2, A3, 0x00B8  A2 = (Item < B8) ? 1 : 0;
        "14C00005", //BNEZ	A2, A		Branch if B7 (A2 = 1)
        "24060100", //ADDIU	A2, R0, 0x0100
        "28E600B9", //SLTI	A2, A3, 0x00B9  A2 = (Item < B9) ? 1 : 0;
        "14C00002", //BNEZ	A2, A		Branch if B8 (A2 = 1)
        "24061E00", //ADDIU	A2, R0, 0x1E00
        "24066000", //ADDIU	A2, R0, 0x6000
        "3C05801F", //LUI	A1, 0x801F              label: A
        "24A505D0", //ADDIU	A1, A1, 0x05D0	A1 = 801F05D0
        "8CA70000", //LW	A3, A1		A3 = Current map values
        "00C73825", //OR	A3, A2, A3
        "ACA70000", //SW	A3, A1
        "27BD0018", //ADDIU	SP, SP, 0x0018  Put the SP back to where it was
        "8FBF0014", //LW	RA, 0x0014 (SP) Load previous RA
        "27BD0040", //ADDIU	SP, SP, 0x0040
        "03E00008", //JR	RA	Return
        "00000000", //NOP
        "8FA50010", //LW	A1, 0x0010 (SP) Load A1     (not a map jumps to here)
        "8FA6000C", //LW	A2, 0x000C (SP) Load A2
        "8FA70008", //LW	A3, 0x0008 (SP) Load A3
        "8FBF0004", //LW	RA, 0x0004 (SP) Load RA
        "27BD0018", //ADDIU	SP, SP, 0x0018  Put the SP back to where it was
        "03E00008", //JR	RA	Return
        "00000000"  //NOP       Make sure not to do a random command
    };


    Maps_Fun = {
        "27BDFFD8",
        "AFA50010",
        "AFA60014",
        "AFA70018",
        "AFBF001C",
        "AFA40020",
        "28E600B4",
        "14C00067",
        "00000000",
        "28E600B5",
        "1006000D",
        "24060003",
        "AFA60024",
        "3C061000",
        "AFA60000",
        "24062000",
        "AFA60004",
        "3C060008",
        "24C60009",
        "AFA60008",
        "3C060002",
        "24C6F000",
        "AFA6000C",
        "10000043",
        "28E600B6",
        "1006000C",
        "2406001C",
        "AFA60024",
        "24060001",
        "AFA60000",
        "3C064000",
        "24C60880",
        "AFA60004",
        "24060060",
        "AFA60008",
        "24060000",
        "AFA6000C",
        "10000035",
        "28E600B7",
        "1006000C",
        "240600E0",
        "AFA60024",
        "24060000",
        "AFA60000",
        "3C060004",
        "AFA60004",
        "3C067C01",
        "24C62100",
        "AFA60008",
        "24060800",
        "AFA6000C",
        "10000027",
        "28E600B8",
        "1006000C",
        "24060100",
        "AFA60024",
        "3C060001",
        "AFA60000",
        "3C060020",
        "24C60004",
        "AFA60004",
        "24060006",
        "AFA60008",
        "24060400",
        "AFA6000C",
        "10000019",
        "28E600B9",
        "1006000C",
        "24061E00",
        "AFA60024",
        "3C060010",
        "AFA60000",
        "3C060988",
        "24C60020",
        "AFA60004",
        "24060400",
        "AFA60008",
        "24060000",
        "AFA6000C",
        "1000000B",
        "24066000",
        "AFA60024",
        "3C062008",
        "AFA60000",
        "24060000",
        "AFA60004",
        "3C060300",
        "24C60800",
        "AFA60008",
        "24060000",
        "AFA6000C",
        "3C05801F",
        "24A505D0",
        "8CA70000",
        "8FA60024",
        "00C73825",
        "ACA70000",
        "24A5FF50",
        "27A4000C",
        "8C860000",
        "8CA70000",
        "00C73825",
        "ACA70000",
        "24A5FFFC",
        "149DFFFA",
        "2484FFFC",
        "27BD0028",
        "8FBF0014",
        "27BD0040",
        "03E00008",
        "00000000",
        "8FA50010",
        "8FA60014",
        "8FA70018",
        "8FBF001C",
        "8FA40020",
        "27BD0028",
        "03E00008",
        "00000000"
    };

    vector<string> starting_maps_func = {
        "27BDFFC0",
        "AFA70018",
        "AFBF0014",
        "0C051224",
        "00000000",
        "0C061972",
        "27BDFFC0",
        "0C061978",
        "27BDFFC0",
        "8FA70018",
        "8FBF0014",
        "03E00008",
        "27BD0040",

        "AFBF0014",
        "0C04BBC3",
        "24070000", //deku mask map id
        "8FBF0014",
        "03E00008",
        "27BD0040",
        "AFBF0014",
        "0C04BBC3",
        "24070000", //song of healing map id
        "8FBF0014",
        "03E00008",
        "27BD0040"
    };

    for (int i = 0; i < Pass_New.size(); i++) {
        Write_To_Rom(Pass + (i*4), Pass_New[i]);
    }
    for (int i = 0; i < Maps_Fun.size(); i++) {
        Write_To_Rom(Maps + (i*4), Maps_Fun[i]);
    }

    Write_To_Rom(hex_to_decimal("BDD008"), "0C061965"); //jump to starting tingle maps custom function in the creation of a new file
    for (int i = 0; i < starting_maps_func.size(); i++) {   //custom starting tingle maps
        Write_To_Rom(hex_to_decimal(starting_maps_add) + (i*4), starting_maps_func[i]);
    }
}

void Fix_Song_Text() {
    //Sonata of Awakening first byte of the text id
    Write_To_Rom(41394852, "00");

    //Goron Lullaby first byte of the text id
    Write_To_Rom(40090672, "00");

    //New Wave Bossa Nova first byte of the text id
    Write_To_Rom(39854656, "00");

    //Elegy of Emptiness first byte of the text id
    Write_To_Rom(45553208, "00");

    //oath to order first byte of the text id
    Write_To_Rom(47725068, "00");

    //epona song first byte of the text id
    Write_To_Rom(40440860, "00");

    //song of soaring first byte of the text id
    Write_To_Rom(42098348, "00");

    //song of storms first byte of the text id
    Write_To_Rom(33041348, "00");
}

string Get_Song_Text(string song) {
    string text = "";

    text += "0200FEFFFFFFFFFFFFFFFF";   //header
    text += string_to_hex("You learned "); //pre-white text
    text += "01";   //change color to red
    text += string_to_hex(song);  //get song text in red
    text += "00";   //change color back to white
    text += string_to_hex("!"); //end with an exclamation mark
    text += "BF"; //footer

    return text;
}

void Songs_Text_Offset() {
    vector<string> Songs_Text = {
        "Epona's Song",    //70
        "Song of Soaring", //71
        "Song of Storms",  //72
        "Sonata of Awakening", //73
        "Goron Lullaby",   //74
        "New Wave Bossa Nova", //75
        "Elegy of Emptiness",  //76
        "Oath to Order"    //77
    };
    string Text_Hex = "";

    for (int i = 0; i < Songs_Text.size(); i++) {
        Text_Hex += Get_Song_Text(Songs_Text[i]);
    }

    //the new song text overlaps deku mask and goron mask text, so making new short ones
    Text_Hex += "020178FFFFFFFFFFFFFFFF596F7520676F7420746865200144656B75204D61736B0021BF";   //you got the deku mask text
    Text_Hex += "020179FFFFFFFFFFFFFFFF596F7520676F74207468652001476F726F6E204D61736B0021BF"; //you got the goron mask text

    Write_To_Rom(11352914, Text_Hex);

    //create song of healing text
    Text_Hex = Get_Song_Text("Song of Healing");   //94
    Text_Hex += "020095FFFFFFFFFFFFFFFF596F7520676F7420612001736561686F7273650021BF"; //you got a seahorse text, 95

    Write_To_Rom(11357024, Text_Hex);

    //update the text offsets in the text table
    Write_To_Rom(12964854, "2B52"); //70
    Write_To_Rom(12964862, "2B79"); //71
    Write_To_Rom(12964870, "2BA3"); //72
    Write_To_Rom(12964878, "2BCC"); //73
    Write_To_Rom(12964886, "2BFA"); //74
    Write_To_Rom(12964894, "2C22"); //75
    Write_To_Rom(12964902, "2C50"); //76
    Write_To_Rom(12964910, "2C7D"); //77
    Write_To_Rom(12964918, "2CA5"); //78    deku mask
    Write_To_Rom(12964926, "2CC9"); //79    goron mask
    Write_To_Rom(12965142, "3B8A"); //95    (94 is same offset as before)
}

void Fix_Goron_Lullaby() {
    vector<string> Code = {
        "3C01801F", //AT = 801F0000
        "8425F72C", //A1 = X1XX OR XXXX (0x801EF72C)
        "30A20100", //V0 =A1 or AT = 0100 (Intro Bit)
        "10400008", //Branch to 70 if no intro
        "00000000", //Replace the test of the normal instructions with nothing
        "00000000", //
        "00000000", //
        "00000000"  //
    };
    int Location = 16475616;

    for (int i = 0; i < Code.size(); i++) {
        Write_To_Rom(Location + (i*4), Code[i]);
    }
}

void Fix_Showing_Items() {
    map<string, string> Index;
    string base = "CD7614";
    vector<string> Item_Names = Get_Keys(Items);
    string item;
    string source;
    string ind;
    int address;
    string id;

    Index["Bow"] = "09";
    Index["Fire Arrow"] = "0A";
    Index["Ice Arrow"] = "0B";
    Index["Light Arrow"] = "0C";
    Index["Deku Stick"] = "07";
    Index["Deku Nuts"] = "12";
    Index["Magic Beans"] = "2E";
    Index["Powder Keg"] = "0F";
    Index["Pictograph Box"] = "13";
    Index["Lens of Truth"] = "52";
    Index["Hookshot"] = "0D";
    Index["Great Fairy's Sword"] = "06";
    Index["Red Potion"] = "23";
    Index["Green Potion"] = "25";
    Index["Blue Potion"] = "24";
    Index["Fairy"] = "29";
    Index["Deku Princess"] = "1A";
    Index["Milk"] = "26";
    Index["Fish"] = "16";
    Index["Bugs"] = "20";
    Index["Poe"] = "21";
    Index["Big Poe"] = "22";
    Index["Spring Water"] = "17";
    Index["Hot Spring Water"] = "18";
    Index["Zora Egg"] = "19";
    Index["Gold Dust"] = "1B";
    Index["Mushroom"] = "1E";
    Index["Seahorse"] = "1D";
    Index["Chateau Romani"] = "28";
    Index["Moon's Tear"] = "2A";
    Index["Land Title Deed"] = "2B";
    Index["Swamp Title Deed"] = "2F";
    Index["Mountain Title Deed"] = "30";
    Index["Ocean Title Deed"] = "31";
    Index["Room Key"] = "2C";
    Index["Express Letter to Mama"] = "33";
    Index["Letter to Kafei"] = "2D";
    Index["Pendant of Memories"] = "36";
    Index["Deku Mask"] = "51";
    Index["Goron Mask"] = "4F";
    Index["Zora Mask"] = "50";
    Index["Fierce Deity Mask"] = "4E";
    Index["Mask of Truth"] = "3A";
    Index["Kafei's Mask"] = "3B";
    Index["All-Night Mask"] = "3C";
    Index["Bunny Hood"] = "3D";
    Index["Keaton Mask"] = "3E";
    Index["Garo Mask"] = "3F";
    Index["Romani's Mask"] = "40";
    Index["Circus Leader's Mask"] = "41";
    Index["Postman's Hat"] = "42";
    Index["Couple's Mask"] = "43";
    Index["Great Fairy's Mask"] = "44";
    Index["Gibdo Mask"] = "45";
    Index["Don Gero's Mask"] = "46";
    Index["Kamaro's Mask"] = "47";
    Index["Captain's Hat"] = "48";
    Index["Stone Mask"] = "49";
    Index["Bremen Mask"] = "4A";
    Index["Blast Mask"] = "4B";
    Index["Mask of Scents"] = "4C";
    Index["Giant's Mask"] = "4D";

    for (int it = 0; it < Item_Names.size(); it++) {
        item = Item_Names[it];
        source = Get_Source(item);
        ind = Index[item];
        address = hex_to_decimal(base) + hex_to_decimal(ind);   //base + offset
        id = Items[source].Get_Item_ID;

        Write_To_Rom(address, id);
    }
}

bool All_Songs_Same_Pool() {
    vector<string> Songs = {"Song of Healing", "Song of Soaring", "Epona's Song", "Song of Storms", "Sonata of Awakening", "Goron Lullaby", "New Wave Bossa Nova", "Elegy of Emptiness", "Oath to Order"};
    string song;
    string Song_Gives;
    string pool = "";

    for (int s = 0; s < Songs.size(); s++) {
        song = Songs[s];
        Song_Gives = Items[song].Name;

        if (IndexOf(Songs, Song_Gives) == -1) {
            return false;
        }
    }

    //all songs gives songs
    return true;
}

string Char_To_String(char chr[], int size) {
    string str = "";
    int index = 0;

    for (index = 0; index < size; index++) {
        str += chr[index];
    }

    return str;
}

void Write_Cutscene_Rom(int address, string filename) {
    int limit = 2;
    char data[limit+1];
    string String_Data = "";
    ifstream file;
    int cur = 0;

    filename = ".\\cutscenes\\" + filename + ".zcutscene";

    file.open(filename, fstream::binary | fstream::out | fstream::in);

    while (file.good()) {
        file.seekg(cur);
        file.read(data, limit);
        String_Data += Char_To_String(data, limit);
        cur += limit;
    }

    file.close();

    Write_To_Rom(address, string_to_hex(String_Data));
}

void Write_Short_Giant() {
    vector<string> instructions = {
        "27BDFFD8",
        "AFA50004",
        "AFA60008",
        "AFA40008",
        "AFBF0010",
        "3C05801F",
        "8CA5F72C",
        "2406000F",
        "00A62824",
        "54A6000F",
        "00000000",
        "3C058040",
        "3C068071",
        "10000002",
        "24C66178"
    };
    vector<string> in_2 = {
        "ACA6F380",
        "8FA40004",
        "3C068071",
        "2405C800",
        "A4C56192",
        "0C1D4A4D",
        "2405000D",
        "240500C8",
        "A4C56192",
        "3C058042",
        "8CA68A24",
        "24C6FFF0",
        "ACA68A24",
        "8FBF0010",
        "8FA4000A",
        "8FA50004",
        "8FA60008",
        "03E00008",
        "27BD0028"
    };

    string data = "";

    for (int i = 0; i < instructions.size(); i++) {
        data += instructions[i];
    };

    Write_To_Rom(15923716, data);

    data = "";
    for (int i = 0; i < in_2.size(); i++) {
        data += in_2[i];
    };

    Write_To_Rom(15923780, data);
}

string Open_From_Rom(int address, int size) {
    //int address = string_to_dec(start);
    char *data = new char[size];

    try {
        inFile.open(Rom_Location, fstream::binary | fstream::out | fstream::in);
        inFile.seekg(address);
        inFile.read(data, size);
        inFile.close();
    }
    catch(exception e) {
        err_file << "Error opening the decompressed rom";
        err_file.close();
        exit(0);
    }

    return string_to_hex(Char_To_String(data, size));
}

void Remove_Actor(string room_offset, int actor_index) {
    string header = "";
    string header_code = "";
    string actors = "";
    int actors_offset = 0;
    int room_offset_int = hex_to_decimal(room_offset);
    int offset = room_offset_int;
    int num_actors = 0;
    int actor_num_offset = 0;

    header = Open_From_Rom(offset, 8);
    header_code = header.substr(0, 2);

    while(header_code != "14" && header_code != "01") {
        offset += 8;
        header = Open_From_Rom(offset, 8);
        header_code = header.substr(0, 2);
    }

    //found the header command for the actors
    if (header_code == "01") {
        num_actors = hex_to_decimal(header.substr(2, 2));
        actor_num_offset = offset+1;

        actors_offset = hex_to_decimal(header.substr(10, 6)) + room_offset_int;

        for (int a = 0; a < num_actors; a++) {
            //if this is not the actor we're removing
            if (a != actor_index) {
                actors += Open_From_Rom(actors_offset+(a*16), 16);
            }
        }
        actors += "00000000000000000000000000000000";

        Write_To_Rom(actors_offset, actors);

        //take 1 away from the number of actors in the file
        num_actors--;
        Write_To_Rom(actor_num_offset, dec_to_hex(num_actors));
    }
    //there are no actors
    else {

    }
}

void Shorten_Igos_CS() {
    Write_To_Rom(14857239, "0A");   //Frames of first camera angle
    Write_To_Rom(14857759, "30");   //Frames of showing the curtains
    Write_To_Rom(14857835, "00");   //Frames of curtains with a different link animation?
    Write_To_Rom(14859019, "20");   //Frames of showing one guy walk in
    Write_To_Rom(14859655, "20");   //Frames of showing the other guy walk in
    Write_To_Rom(14861167, "00");   //Frames of closeups

    Write_To_Rom(14857500, "00000000"); //remove first textbox
    Write_To_Rom(14858408, "00000000"); //remove second textbox
    Write_To_Rom(14859140, "00000000"); //remove third textbox
    Write_To_Rom(14860016, "00000000"); //remove fourth textbox

    Write_To_Rom(14858572, "1000000C"); //remove Igos mouth from moving in first angle
    Write_To_Rom(14859252, "1000000C"); //remove Igos mouth from moving in second angle
    Write_To_Rom(14860252, "10000006"); //remove Igos mouth from moving in third angle
}

void Remove_Cutscenes(bool Songs_Same_Pool) {
    string Sonata = Items["Sonata of Awakening"].Name;
    string Deku_Mask = Items["Deku Mask"].Name;
    string Oath = Items["Oath to Order"].Name;
    string Couples = Items["Couple's Mask"].Name;
    string GFM = Items["Great Fairy's Mask"].Name;
    string Goron = Items["Goron Mask"].Name;
    string Zora = Items["Zora Mask"].Name;
    string Lullaby = Items["Goron Lullaby"].Name;
    string NW = Items["New Wave Bossa Nova"].Name;
    string GFS = Items["Great Fairy's Sword"].Name;
    string Gibdo = Items["Gibdo Mask"].Name;
    string Elegy = Items["Elegy of Emptiness"].Name;

    //Write_To_Rom(47995767, "00");           //make the east clock town intro cs 0 frames

    //remove the east clock town into cs from the entrances
    Write_To_Rom(47996414, "FF");
    Write_To_Rom(47996422, "FF");
    Write_To_Rom(47996430, "FF");

    //Write_To_Rom(48354523, "00");           //make the north clock town intro cs 0 frames

    //remove the north clock town into cs from the entrances
    Write_To_Rom(48354914, "FF");
    Write_To_Rom(48354922, "FF");

    //Write_To_Rom(48223863, "00");           //make the west clock town intro cs 0 frames

    //remove the west clock town into cs from the entrances
    Write_To_Rom(48224334, "FF");
    Write_To_Rom(48224342, "FF");

    //Write_To_Rom(39388527, "00");           //make the west termina field intro cs 0 frames
    //Write_To_Rom(39389279, "00");           //make the north termina field intro cs 0 frames
    //Write_To_Rom(39389807, "00");           //make the east termina field intro cs 0 frames
    //Write_To_Rom(39390335, "00");           //make the south termina field intro cs 0 frames

    //remove the termina field intro cs from the entrances
    //Write_To_Rom(39390846, "FF"); this one's flag is FE, might be a credits cs or something
    //Write_To_Rom(39390878, "FF"); this one's flag is FE, might be a credits cs or something
    Write_To_Rom(39390886, "FF");
    Write_To_Rom(39390894, "FF");
    Write_To_Rom(39390902, "FF");
    Write_To_Rom(39390910, "FF");

    //Write_To_Rom(39606036, "E000");         //change the termina field skull kid cutscene actor to link spawn point actor to make it do nothing

    //remove actor that spawns the skull kid cs
    Remove_Actor("025C5000", 103);

    //Write_To_Rom(42098403, "00");           //make the southern swamp intro cs 0 frames

    //remove southern swamp intro cs from the entrance
    Write_To_Rom(42098938, "FF");

    //Write_To_Rom(39011735, "00");           //make the deku palace intro cs 0 frames

    //remove the daku palace intro cs from the entrance
    Write_To_Rom(39012318, "FF");

    //sonata
    Write_To_Rom(41386310, "0000");                 //make the first part of sonata cs 0 frames
    Write_Cutscene_Rom(41389520, "sonata");         //shorten sonata cutscene
    Write_To_Rom(41393633, Items[Sonata].Text_ID);  //fix sonata text
    if (Songs_Same_Pool) {
        Write_To_Rom(41393585, Items[Sonata].Song1_ID); //fix monkey singing right song, if randomizing the songs that are actually played
        Write_To_Rom(41393609, Items[Sonata].Song2_ID); //fix what the player plays, if randomizing the songs that are actually played
    }

    //hms
    Write_Cutscene_Rom(46985680, "hms_1");              //shorten first part of the happy mask cutscene
    Write_Cutscene_Rom(46988304, "hms_2");              //shorten second part of the happy mask cutscene
    Write_To_Rom(47001596, "C002");                     //make first cs jump to 2nd cs
    Write_To_Rom(47001604, "FFFF");                     //make 2nd cs not jump to another
    Write_To_Rom(46991505, Items[Deku_Mask].Text_ID);   //fix deku mask text

    //Write_To_Rom(42439947, "00");                       //make the woodfall intro cs 0 frames

    //remove woodfall intro cs from the entrance
    Write_To_Rom(42440514, "FF");

    Write_Cutscene_Rom(42439444, "woodfall_rising");    //shorten the woodfall rising cs

    //Write_To_Rom(35501459, "00");                       //make the woodfall temple intro cs 0 frames

    //remove the woodfall temple intro cs
    Write_To_Rom(35504954, "FF");

    //remove the woodfall intro cs from back room
    Write_To_Rom(35504946, "FF");

    //moon's tear
    Write_Cutscene_Rom(39375352, "tear_falling");   //shorten the tear falling cs
    Write_Cutscene_Rom(39381640, "tear_falling");   //shorten the tear falling cs for 3rd day
    Write_Cutscene_Rom(39383720, "tear_falling");   //shorten the tear falling for final hours

    //roof
    //Write_To_Rom(35258981, "39");   //change the rooftop first audio to the same as the 2nd one
    //Write_To_Rom(35258983, "00");   //the frame the music starts
    //Write_To_Rom(35254918, "000A"); //make the cs last 0 frames

    //Remove_Actor("21B8000", 1);
    //prevent the skull kid cs from playing on the clock tower roof
    Write_To_Rom(15758392, "00000000"); //removes the cs
    Write_To_Rom(35246095, "38");   //fixes the background music
    Write_To_Rom(35356789, "95"); //fixes skull kid's y position

    //moon
    Write_Short_Giant();            //remove the giants moon cs

    //Write_To_Rom(41857440, "E00B"); //make cucco shack not crash by replacing the 1AB actor with dodongo actor

    //remove the cucco shack 1AB actor to prevent a crash that occurs because of the giant's cs skip
    Remove_Actor("27EB000", 20);

    Write_To_Rom(11478999, "4E6F7420796F7520616761696E21BF");   //make owl's textbox for SoS short

    //oath
    Write_Cutscene_Rom(47718668, "oath_cs");        //make the oath cs shorter
    Write_To_Rom(47724913, Items[Oath].Text_ID);    //fix oath's text id
    if (Songs_Same_Pool) {
        Write_To_Rom(47724865, Items[Oath].Song1_ID); //fix oath right song, if randomizing the songs that are actually played
        Write_To_Rom(47724889, Items[Oath].Song2_ID); //fix what the player plays, if randomizing the songs that are actually played
    }

    //Write_Cutscene_Rom(35496572, "woodfall_tatl_end");  //shorten tatl cs after beating woodfall to 0 frames, and remove the motion blur
    Write_To_Rom(35492834, "0000");                     //remove the deku princess cs after beating woodfall
    Write_Cutscene_Rom(42455024, "woodfall_cleaning");  //shorten the woodfall cleaning cs after beating woodfall

    //couples mask
    Write_Cutscene_Rom(46642260, "couples");            //shorten the couple's mask cs
    Write_To_Rom(46644305, Items[Couples].Text_ID);     //fix the text for the shorter couples

    //Returning Deku Princess
    Write_To_Rom(41399030, "0000"); //make the cs terminator at 0 frames in returning the deku princess cs
    Write_To_Rom(41399254, "0000"); //make the 2nd phase of the princess cs 0 frames

    //Lullaby Intro
    Write_Cutscene_Rom(44660088, "lullaby_intro");  //day 3
    Write_Cutscene_Rom(46240568, "lullaby_intro");  //other days
    Write_Cutscene_Rom(44664824, "lullaby_intro2"); //day 3 opposite angle
    Write_Cutscene_Rom(46245304, "lullaby_intro2"); //other days opposite angle

    //Great Fairy in Clock Town
    Write_Cutscene_Rom(38001628, "great_fairy_magic_mask");     //Shorten Great Fairy Mask + Magic CS
    Write_To_Rom(38005181, Items[GFM].Text_ID);         //fix the text id
    Write_Cutscene_Rom(38011356, "great_fairy_mask");     //shorten Great Fairy Mask on same cycle
    Write_To_Rom(38014181, Items[GFM].Text_ID);         //fix the text id
    Write_Cutscene_Rom(37946092, "great_fairy_magic_1");    //shorten Magic CS
    //Write_Cutscene_Rom(37971580, "great_fairy_magic_1");    //shorten Magic CS on same cycle
    Write_Cutscene_Rom(38007244, "great_fairy_magic_2");    //shorten phase 2 of the magic cs
    //Write_To_Rom(38092857, "04");
    Remove_Actor("2454000", 4);  //remove the tatl text trigger in the fountain

    //Write_To_Rom(44659663, "00");                   //make mountain village intro cs 0 frames

    //remove the mountain village intro cs from the entrance
    Write_To_Rom(44669566, "FF");

    //Write_To_Rom(40090815, "00");                   //make goron shrine intro cs 0 frames

    //remove the goron shrine into cs from the entrance
    Write_To_Rom(40091518, "FF");

    //Write_To_Rom(46177479, "00");                   //make snowhead intro cs 0 frames

    //remove the snowhead intro cs from the entrance
    Write_To_Rom(46178166, "FF");

    //Write_To_Rom(36492391, "00");                   //make snowhead temple intro cs 0 frames

    //remove the snowhead temple intro cs from the entrance
    Write_To_Rom(36496622, "FF");

    Write_Cutscene_Rom(46175216, "goron_sleep");    //make the giant goron in snowhead sleep faster

    //Write_To_Rom(40638227, "00");                   //make great bay intro cs 0 frames

    //remove the great bay coast into cs from the entrance
    Write_To_Rom(40639162, "FF");

    //Write_To_Rom(40231683, "00");                   //make zora hall intro cs 0 frames

    //remove the zora hall intro cs from the entrances
    Write_To_Rom(40232106, "FF");
    Write_To_Rom(40232114, "FF");

    Write_Cutscene_Rom(40879224, "turtle");         //shorten the turtle rising cs
    Write_To_Rom(40892756, "8C10");                 //skip pirate cs altogether when entering gbt
    //Write_To_Rom(42834238, "0000");                 //make great bay temple intro cs 0 frames

    //remove the great bay temple intro cs from the entrances
    Write_To_Rom(42838550, "FF");
    Write_To_Rom(42838542, "FF");

    Write_To_Rom(40887446, "0001");                 //make riding the turtle into gbt 1 frame
    Write_To_Rom(40887983, "01");                   //shorten the 2nd time entering gbt with turtle

    //Write_To_Rom(42833486, "0000");                 //shorten the 2nd gbt intro cs
    //Write_To_Rom(43429603, "00");                   //make waterfall rapids cs 0 frames

    //Goron Mask
    Write_Cutscene_Rom(44479080, "goron_mask");     //shorten the goron mask cs
    Write_To_Rom(44480645, Items[Goron].Text_ID);   //fix the goron mask text

    //Zora Mask
    Write_Cutscene_Rom(40631980, "zora_mask");      //shorten zora mask cs
    Write_To_Rom(40633689, Items[Zora].Text_ID);    //Fix zora mask text

    //Goron Lullaby
    Write_Cutscene_Rom(40087880, "goron_lullaby");  //shorten goron lullaby cs
    Write_To_Rom(40091500, "5E20");                 //skip phase 2 of the cs
    Write_Cutscene_Rom(40088936, "goron_lullaby2");  //shorten goron lullaby cs phase 3
    Write_To_Rom(16480271, "68");                   //make zfg stay asleep on frame 0x68 in the phase 3 cutscene
    Write_To_Rom(40090225, Items[Lullaby].Text_ID); //fix lullaby text
    if (Songs_Same_Pool) {
        Write_To_Rom(40088625, Items[Lullaby].Song1_ID);    //fix lullaby display song
        Write_To_Rom(40088649, Items[Lullaby].Song2_ID);    //fix lullaby play song
    }

    //shorten Mikau walking cs
    Write_To_Rom(16751692, "3C014120"); //make right leg faster
    Write_To_Rom(16751668, "3C014120"); //make left leg faster

    //remove cs before pushing mikau
    Write_To_Rom(16746330, "4E34");     //remove the text before pushing Mikau
    Write_To_Rom(16746872, "00000000"); //remove the flag that makes mikau in always talking mode

    //shorten NWBN cs
    Write_Cutscene_Rom(39850256, "nwbn");   //shorter cs
    if (Songs_Same_Pool) {
        Write_To_Rom(39852449, Items[NW].Song2_ID); //fix what the player plays, if randomizing the songs that are actually played
    }
    Write_To_Rom(39852473, Items[NW].Text_ID);  //fix the text id for what NW gives

    Write_To_Rom(13421880, "00000000"); //skip SoT cs

    //gfs
    Write_Cutscene_Rom(37958316, "gfs");        //shorten the great fairy sword cs
    Write_To_Rom(37960501, Items[GFS].Text_ID); //fix the shorter gfs text

    //Write_To_Rom(35631340, "0005");                             //skip the tatl text inside the main woodfall room

    //remove the actor that plays the tatl text cs inside the 2nd room in woodfall temple
    Remove_Actor("21FB000", 8);

    Write_Cutscene_Rom(35501876, "woodfall_temple_ye_holds");   //shorten the boss warp cs in woodfall temple
    Write_Cutscene_Rom(45984796, "snowhead_beaten");            //shorten beating snowhead temple cs
    Write_Cutscene_Rom(36492872, "snowhead_temple_ye_holds");   //shorten the boss warp cs in snowhead temple
    //Write_To_Rom(41157064, "0022");                             //remove intro cs for pirate's fortress entrance

    //remove the actor that plays the intro cs in pirate's fortress exterior
    Remove_Actor("02740000", 23);

    //Write_To_Rom(34146119, "00");                               //remove intro cs for pirate's fortress outside

    //remove the intro cs for pirate's fortress from the entrance
    Write_To_Rom(34146526, "FF");

    //Write_To_Rom(37269916, "0022");                             //remove cs of bee flying to it's nest in PF

    //remove the actor that plays the bee entering the beehive cs
    Remove_Actor("238B000", 20);

    //Write_To_Rom(47791535, "00");                               //make the gormon track intro cs 0 frames

    //remove the gorman racetrack intro cs from the entrances
    Write_To_Rom(47792614, "FF");
    Write_To_Rom(47792622, "FF");

    //Write_To_Rom(40443715, "00");                               //make the ranch intro cs 0 frames

    //remove the ranch intro cs from the entrance
    Write_To_Rom(40446482, "FF");

    Write_Cutscene_Rom(40444324, "your_horse");                 //make link go fast in isnt that your horse cs

    //Kart Ride
    Write_To_Rom(16639926, "CE40");             //load gorman track when starting kart ride
    Write_Cutscene_Rom(47789736, "kart_ride");  //remove the text for kart ride to speed it up a little

    //Write_To_Rom(41889291, "00");                       //make Ikana graveyard intro cs 0 frames

    //remove the ikana graveyard intro cs from the entrance
    Write_To_Rom(41889786, "FF");

    //address of start for the iron knuckle dying thing where flats shoes up 1F81190

    //sharp sos cs
    Write_Cutscene_Rom(33777340, "sharp_spawning");     //shorten sharp spawning cs inside the water grave thing
    Write_Cutscene_Rom(33781724, "sharp_sos");          //shorten the sharp sos cutscene phase 1
    Write_To_Rom(33797616, "20A0");                     //skip the music box cs
    Write_To_Rom(33794114, "0000");                     //make the final cs 0 frames

    //gibdo mask
    Write_Cutscene_Rom(45389024, "gibdo");  //shorten the gibdo mask cs
    Write_To_Rom(45391773, Items[Gibdo].Text_ID);   //update the shortened cs's gibdo mask text

    //Write_To_Rom(33796819, "00");   //make ikana canyon intro cs 0 frames

    //remove the ikana canyon intro cs from the entrance
    Write_To_Rom(33797642, "FF");

    //shorten circus leader's mask cs
    Write_To_Rom(34301875, "00");           //make the first 3 cutscenes 0 frames long (it plays the same one three times)
    Write_Cutscene_Rom(34300444, "clm");    //shorten the circus leader's mask cs

    //Write_To_Rom(36495599, "00");           //make snowhead main room cs 0 frames

    //remove the actor that starts the cs in the main room in snowhead temple
    Remove_Actor("230C000", 70);

    Write_Cutscene_Rom(41882900, "cap");    //shorten captain's hat cs

    //shorten the cs before epona archery
    Write_To_Rom(15880699, "A0");       //play the ranch cs instead of the alien, skipping the alien cs
    Write_To_Rom(15880712, "00000000"); //prevent the game from freezing when loading the cs

    Write_Cutscene_Rom(33034640, "flat_spawning");  //shorten flat spawning cs
    Write_To_Rom(15011312, "00000000");             //remove gyorg cs

    //remove the ikana castle cutscene actors
    Remove_Actor("2266000", 45);
    Remove_Actor("2266000", 46);
    Remove_Actor("2266000", 28);    //remove actor that prevented the bg music from playing

    //Shorten Elegy cs
    Write_Cutscene_Rom(45543908, "elegy");          //import shorter cs
    Write_To_Rom(45546381, Items[Elegy].Text_ID);   //update the elegy text
    if (Songs_Same_Pool) {
        Write_To_Rom(45546333, Items[Elegy].Song1_ID);    //fix elegy display song
        Write_To_Rom(45546357, Items[Elegy].Song2_ID);    //fix elegy play song
    }

    Shorten_Igos_CS();  //shorten the Igos cs

    //shorten sakon hideout cutscene
    Write_Cutscene_Rom(44548416, "sakon_hideout");  //shorten the starting cs
    Write_To_Rom(44552390, "003C");                 //shorten the doorway cs
    Write_To_Rom(44553062, "003C");                 //shorten the switch cs
    //Write_To_Rom(33777332, "07");                   //shorten the number of points for kafei leaving
    //Write_To_Rom(33777332, "04");                   //shorten the number of points for kafei leaving
    //Write_To_Rom(33777164, "0909FF601273 09B8FF60113B 09B1FF440FE2 0945FF1F0F01 087FFEF20E54 07D7FEE80DD0 0705FEE50D3E"); //update the points
    //Write_To_Rom(33777164, "0909FF601273 09B1FF440FE2 087FFEF20E54 0705FEE50D3E"); //update the points
    //Write_To_Rom(33777164, "0909FF60127309B1FF440FE2087FFEF20E540705FEE50D3E"); //update the points

    Write_Cutscene_Rom(42835544, "great_bay_temple_ye_holds");      //shorten ye who holds remains cs in gbt
    Write_To_Rom(16165132, "00000000");                             //skip the goht room entrance cs
    Write_Cutscene_Rom(34878820, "stone_tower_temple_ye_holds");    //shorten ye who hold remains cs in stt
    Write_To_Rom(44042848, "9260");                                 //shorten evan hp cs
}

void Write_File_To_Rom(string filename, string rom_offset) {
    int limit = 2;
    char data[limit+1];
    string String_Data = "";
    ifstream file;
    int cur = 0;
    int address = hex_to_decimal(rom_offset);

    file.open(filename, fstream::binary | fstream::out | fstream::in);

    while (file.good()) {
        file.seekg(cur);
        file.read(data, limit);
        String_Data += Char_To_String(data, limit);
        cur += limit;
    }

    file.close();

    Write_To_Rom(address, string_to_hex(String_Data));
}

void Gamecube_Hud() {
    //A Button
    Write_To_Rom(12251938, "64FF"); //red/green
    Write_To_Rom(12251958, "78");   //blue

    //B
    Write_To_Rom(12244714, "00FF"); //red
    Write_To_Rom(12244718, "0064"); //green
    Write_To_Rom(12244706, "0064"); //blue

    //Start
    Write_To_Rom(12245214, "0078"); //red
    Write_To_Rom(12245202, "0078"); //green
    Write_To_Rom(12245206, "0078"); //blue

    //Change Z button in pause screen to L
    Write_File_To_Rom(".\\files\\l.yaz0", "A7B7CC");
}

string Vector_To_String(vector<string> data, string separator) {
    string string_data = "";

    for (int d = 0; d < data.size(); d++) {
        if (d > 0) {
            string_data += separator;
        }

        string_data += data[d];
    }

    return string_data;
}

void Write_Log(string seed) {
    vector<string> items = Get_Keys(Items);
    string item;
    string new_item;

    outFile << "Seed: "<< seed << "\n\n";

    outFile << "Old Item => New Item\n\tItems the logic used to determine that this item is accessible\n\n";

    for (int i = 0; i < items.size(); i++) {
        item = items[i];
        new_item = Items[item].Name;

        outFile << item << " => " << new_item << "\n";

        //if items were needed to obtain this item
        if (Items[item].Items_Needed.size() > 0) {
            outFile << "\t" << Vector_To_String(Items[item].Items_Needed, ", ") << "\n";
        }

        outFile << endl;
    }

    //outFile << Log;
}

void Bingo_Water() {
    //bingo water
    //get item text
    Write_To_Rom(11352237, string_to_hex("Bingo Water") + "002118111F000A5573652004B20020746F20706F7572206974206F6E207768617465766572116D6179206E6565642069742EBF");

    //sign next to water under witch's potion shop
    Write_To_Rom(11376723, string_to_hex("Bingo Water") + "20686F6D65207769746820796F752E11002020");

    //Ikana cave sign
    Write_To_Rom(11379624, string_to_hex("Bingo Water") + "20436176651100456E7472792070726F686962697465642064756520746F2067686F7374117369676874696E677321BF");

    //Sharp talking about flat and the cave?
    Write_To_Rom(11568975, string_to_hex("Bingo Water") + "2063617665002E19BF");

    //pause menu text
    Write_To_Rom(11608680, string_to_hex("Bingo Water") + "1100547279207573696E67206974207769746820B2206F6E207468696E67731174686174206E656564207761746572696E672EBF");

    //write bingo water image for bottom of pause menu
    Write_File_To_Rom(".\\files\\BW.yaz0", "A2A6D4");

    //hot bingo water
    //get item text
    Write_To_Rom(11352345, string_to_hex("Bingo Water") + "002118111F000A55736520697420776974682004B200206265666F726520697420636F6F6C732EBF");

    //goron text next to goron gravewyard
    Write_To_Rom(11507742, string_to_hex("Bingo Water") + "00204920666F756E64207768656E1149207761732064696767696E6720746865206865726F27732067726176652E19BF");

    //pause menu text
    Write_To_Rom(11608760, string_to_hex("Bingo Water") + "1100557365206974207769746820B2206265666F726520697420636F6F6C732EBF");

    //write hot bingo water image for bottom of pause menu
    Write_File_To_Rom(".\\files\\HBW.yaz0", "A2A8A4");
}

string Leading_Zeroes(string hex, int total_length) {
    while (hex.size() < total_length) {
        hex = "0" + hex;
    }

    return hex;
}

void Change_BlastMask(map<string, string> custom_settings) {
    int frames = string_to_dec(custom_settings["BlastMask_Cooldown"]);
    string frames_hex = dec_to_hex(frames);

    frames_hex = Leading_Zeroes(frames_hex, 4);

    Write_To_Rom(13280870, frames_hex);
}

void Fix_Swords() {
    int address = 12228012;
    vector<string> commands = {
        "10800004", //BEQ	A0, R0, FD	Skip saving to b button if FD
        "A518006C", //SH	T8, 0x006C (T0)	Save sword to inv
        "93AE0047", //LBU	T6, 0x0047 (SP)	Load Sword Item ID
        "01007821", //ADDU	T7, T0, R0	Move T0 to T7
        "A1EE004C", //SB	T6, 0x003C (T7)	Save sword to B button
        "00000000", //NOPs
        "00000000", //
        "00000000", //
        "00000000"  //
    };

    for (int c = 0; c < commands.size(); c++) {
        Write_To_Rom(address + (c*4), commands[c]);
    }
}

int main()
{
    err_file.open("Error.txt");

    outFile.open("Spoiler Log.txt");

    //name    item id     get item id   text id     flag   object   get item model  pool = ""   get item locations  item id locations   text id locations
    Items["Adult Wallet"] = Item({Time(1,false,0,3,true,12)}, "Adult Wallet", "5A", "08", "08", "A0", "00A8", "21", "", {"CD688E"}, {}, "1_00010000", {"C5CE6E"});
    Items["All-Night Mask"] = Item({Time(3,true,4,3,false,12,"KillSakon")}, "All-Night Mask", "38", "7E", "7E", "A0", "0265", "11", "", {"CD6B52"}, {"C5CE3D"});
    Items["Big Bomb Bag"] = Item({Time(3,true,4,3,false,12,"KillSakon")}, "Big Bomb Bag", "57", "1C", "1C", "A0", "0098", "19", "", "06", {"CD6906"}, {"C5CE2A"}, "2_00010000", {"C5CE6F"}, "1E", {"C5CE5A"}); //C5CE6F bomb slot
    Items["Biggest Bomb Bag"] = Item({Time(1,false,0,3,true,12)},"Biggest Bomb Bag", "58", "1D", "1D", "A0", "0098", "1A", "", "06", {"CD690C"}, {"C5CE2A"}, "3_00011000", {"C5CE6F"}, "28", {"C5CE5A"}); //C5CE6F bomb slot
    Items["Blast Mask"] = Item({Time(1,false,0,3,true,12,"","KillSakon")},"Blast Mask", "47", "8D", "8D", "A0", "026D", "3B", "", {"CD6BAC"}, {"C5CE3E"});
    Items["Bomb Bag"] = Item({Time(1,false,0,3,true,12)}, "Bomb Bag", "56", "1B", "1B", "A0", "0098", "18", "", "06", {"CD6900"}, {"C5CE2A"}, "1_00001000", {"C5CE6F"}, "14", {"C5CE5A"});
    Items["Bomber's Notebook"] = Item({Time(1,false,0,3,true,12)}, "Bomber's Notebook", "6D", "50", "50", "80", "0253", "0C", "", {"CD6A3E"}, {}, "F_00000100", {"C5CE71"});
    Items["Bow"] = Item({Time(1,false,0,3,true,12)}, "Bow", "01", "22", "22", "A0", "00BF", "2F", "", "01", {"CD692A"}, {"C5CE25"}, "1_00000001", {"C5CE6F"}, "1E", {"C5CE55"});    //C5CE6F = quiver slot
    Items["Bremen Mask"] = Item({Time(1,true,0,1,true,12), Time(2,true,0,2,true,12)}, "Bremen Mask", "46", "8C", "8C", "A0", "025A", "10", "", {"CD6BA6"}, {"C5CE43"});
    Items["Bunny Hood"] = Item({Time(1,false,0,3,true,12)}, "Bunny Hood", "39", "7F", "7F", "A0", "0103", "3F", "", {"CD6B58"}, {"C5CE44"});
    Items["Captain's Hat"] = Item({Time(1,false,0,3,true,12)}, "Captain's Hat", "44", "7C", "7C", "A0", "0102", "3E", "", {"CD6B46"}, {"C5CE51"});
    Items["Circus Leader's Mask"] = Item({Time(1,true,0,2, true,12), Time(2,true,0,2,true,12)}, "Circus Leader's Mask", "3D", "83", "83", "A0", "0259", "0F", "", {"CD6B70"}, {"C5CE49"});
    Items["Couple's Mask"] = Item({Time(3,true,11,3,true,12, "NoKillSakon")}, "Couple's Mask", "3F", "85", "85", "A0", "0282", "04", "", {"CD6B7C"}, {"C5CE4B", "F125C3"}, {"2C7CBD5"});
    Items["Deku Nuts"] = Item({Time(1,false,0,3,true,12)}, "Deku Nuts", "09", "28", "28", "0C", "0094", "EE", "", {"CD694E"}, {"C5CE2D"}, "01", {"C5CE5D"});
    Items["Deku Nuts (10)"] = Item({Time(1,false,0,3,true,12)}, "Deku Nuts (10)", "8E", "2A", "2A", "0C", "0094", "EE", "", "09", {"CD695A"}, {"C5CE2D"}, "0A", {"C5CE5D"});
    Items["Deku Stick"] = Item({Time(1,false,0,3,true,12)}, "Deku Stick", "08", "19", "19", "0D", "009F", "E5", "", "08", {"CD68F4"}, {"C5CE2C"}, "01", {"C5CE5C"});
    Items["Don Gero's Mask"] = Item({Time(1,false,0,3,true,12)}, "Don Gero's Mask", "42", "88", "88", "A0", "0266", "23", "", {"CD6B8E"}, {"C5CE45"});
    Items["Express Letter to Mama"] = Item({Time(3,false,0,3,true,4,"NoKillSakon")}, "Express Letter to Mama", "2E", "A1", "A1", "80", "0245", "37", "", {"CD6C24"}, {"C5CE2F"});
    Items["Fierce Deity Mask"] = Item({Time(3,true,12,3,true,12)},"Fierce Deity Mask", "35", "7B", "7B", "A0", "0242", "76", "", {"CD6B40"}, {"C5CE53"});
    Items["Fire Arrow"] = Item({Time(1,false,0,3,true,12)},"Fire Arrow", "02", "25", "25", "A0", "0121", "48", "", {"CD693C"}, {"C5CE26"});
    Items["Garo Mask"] = Item({Time(1,false,0,1,false,12)},"Garo Mask", "3B", "81", "81", "A0", "0209", "6A", "", {"CD6B64"}, {"C5CE50"});
    Items["Giant Wallet"] = Item({Time(1,false,0,3,true,12)}, "Giant Wallet", "5B", "09", "09", "A0", "00A8", "22", "", {"CD6894"}, {}, "2_00100000", {"C5CE6E"});
    Items["Giant's Mask"] = Item({Time(1,false,0,3,true,12)}, "Giant's Mask", "49", "7D", "7D", "A0", "0226", "73", "", {"CD6B4C"}, {"C5CE52"});
    Items["Gibdo Mask"] = Item({Time(1,false,0,3,true,12)}, "Gibdo Mask", "41", "87", "87", "A0", "020B", "6C", "", {"CD6B88"}, {"C5CE4F", "F12C7F"}, {"2B4A569"});
    Items["Gilded Sword"] = Item({Time(3,false,0,3,false,12)}, "Gilded Sword", "4F", "39", "39", "A0", "01FA", "68", "", {"CD69B4", "CD6C12"}, {"C5CE00"}, "3_00000011", {"C5CE21"}); //C5CE21 Inv sword/shield
    Items["Great Fairy's Mask"] = Item({Time(1,false,0,3,true,12)}, "Great Fairy's Mask", "40", "86", "86", "A0", "020A", "6B", "", {"CD6B82"}, {"C5CE40", "EA3F53", "EA40FB"}, {"243F195", "24415CD"});
    Items["Great Fairy's Sword"] = Item({Time(1,false,0,3,true,12)}, "Great Fairy's Sword", "10", "3B", "3B", "A0", "01FB", "69", "", {"CD69C0", "CD6C00"}, {"C5CE34", "EA3F8B"}, {"243413D"});
    Items["Hero's Shield"] = Item({Time(1,false,0,3,true,12)}, "Hero's Shield", "51", "32", "32", "A0", "00B3", "D8", "", {"CD698A", "CD6C18"}, {}, "1_00010000", {"C5CE21"});
    Items["Hookshot"] = Item({Time(1,false,0,3,true,12)}, "Hookshot", "0F", "41", "41", "A0", "00B4", "29", "", {"CD69E4"}, {"C5CE33"});
    Items["Ice Arrow"] = Item({Time(1,false,0,3,true,12)}, "Ice Arrow", "03", "26", "26", "A0", "0121", "49", "", {"CD6942"}, {"C5CE27"});
    Items["Kafei's Mask"] = Item({Time(1,false,2,1,false,12), Time(2,false,2,2,false,12)}, "Kafei's Mask", "37", "8F", "8F", "A0", "0258", "0E", "", {"CD6BB8"}, {"C5CE4A"});
    Items["Kamaro's Mask"] = Item({Time(1,true,6,1,true,12), Time(2,true,6,2,true,12), Time(3,true,6,3,true,12)}, "Kamaro's Mask", "43", "89", "89", "A0", "027D", "03", "", {"CD6B94"}, {"C5CE4E"});
    Items["Keaton Mask"] = Item({Time(3,false,0,3,true,12)}, "Keaton Mask", "3A", "80", "80", "A0", "0100", "2D", "", {"CD6B5E"}, {"C5CE42"});
    Items["Kokiri Sword"] = Item({Time(1,true,4,1,true,12), Time(2,true,4,2,true,12), Time(3,true,4,3,true,12)}, "Kokiri Sword", "4D", "37", "9C", "A0", "0148", "56", "", {"CD69A8", "CD6C06"}, {"C5CE00", "CBA4AF"}, {"CBA4E3"}, "1_00000001", {"C5CE21"}, 0); //C5CE21 Inv sword/shield
    Items["Land Title Deed"] = Item({Time(1,false,0,3,true,12)}, "Land Title Deed", "29", "97", "97", "80", "01B2", "5B", "", {"CD6BE8"}, {"C5CE29"});
    Items["Large Quiver"] = Item({Time(1,false,0,3,true,12)}, "Large Quiver", "54", "23", "23", "A0", "0097", "16", "", "01", {"CD6930"}, {"C5CE25"}, "2_00000010", {"C5CE6F"}, "28", {"C5CE55"}); //C5CE6F = quiver slot
    Items["Largest Quiver"] = Item({Time(1,false,0,3,true,12)}, "Largest Quiver", "55", "24", "24", "A0", "0097", "17", "", "01", {"CD6936"}, {"C5CE25"}, "3_00000011", {"C5CE6F"}, "32", {"C5CE55"}); //C5CE6F = quiver slot
    Items["Lens of Truth"] = Item({Time(1,false,0,3,true,12)}, "Lens of Truth", "0E", "42", "42", "A0", "00C0", "30", "", {"CD69EA"}, {"C5CE32"});
    Items["Letter to Kafei"] = Item({Time(1,true,6,1,true,12)}, "Letter to Kafei", "2F", "AA", "AA", "80", "0210", "6E", "", {"CD6C5A"}, {"C5CE35"});
    Items["Light Arrow"] = Item({Time(1,false,0,3,true,12)}, "Light Arrow", "04", "27", "27", "A0", "0121", "4A", "", {"CD6948"}, {"C5CE28"});
    Items["Magic Beans"] = Item({Time(1,false,0,3,true,12)}, "Magic Beans", "0A", "35", "35", "80", "00C6", "CB", "", {"CD699C"}, {"C5CE2E"}, "01", {"C5CE5E"});  //get item id of what is given instead is 4F
    Items["Mask of Scents"] = Item({Time(1,false,0,3,true,12)}, "Mask of Scents", "48", "8E", "8E", "A0", "027E", "3D", "", {"CD6BB2"}, {"C5CE46"});
    Items["Mask of Truth"] = Item({Time(1,false,0,1,true,12)}, "Mask of Truth", "36", "8A", "8A", "A0", "0104", "40", "", {"CD6B9A"}, {"C5CE4C"});
    Items["Mirror Shield"] = Item({Time(1,false,0,3,true,12)}, "Mirror Shield", "52", "33", "33", "A0", "00C3", "34", "", {"CD6990"}, {}, "2_00100000", {"C5CE21"});
    Items["Moon's Tear"] = Item({Time(1,false,0,3,true,12)}, "Moon's Tear", "28", "96", "96", "80", "01B1", "5A", "", {"CD6BE2"}, {"C5CE29"});  //CD7646 - another moon's tear for show item
    Items["Mountain Title Deed"] = Item({Time(1,false,0,3,true,12)}, "Mountain Title Deed", "2B", "99", "99", "80", "01B2", "42", "", {"CD6BF4"}, {"C5CE29"});
    Items["Ocean Title Deed"] = Item({Time(1,false,0,3,true,12)}, "Ocean Title Deed", "2C", "9A", "9A", "80", "01B2", "44", "", {"CD6BFA"}, {"C5CE29"});
    Items["Pendant of Memories"] = Item({Time(2,false,10,2,true,4)}, "Pendant of Memories", "30", "AB", "AB", "80", "0215", "6F", "", {"CD6C60"}, {"C5CE35"});  //other show items: CD764B, CD764C, and CD764D
    Items["Pictograph Box"] = Item({Time(1,false,0,3,true,12)}, "Pictograph Box", "0D", "43", "43", "A0", "0228", "75", "", {"CD69F0"}, {"C5CE31"});
    Items["Postman's Hat"] = Item({Time(3,true,0,3,true,12)}, "Postman's Hat", "3E", "84", "84", "A0", "0225", "72", "", {"CD6B76"}, {"C5CE3C"});
    Items["Powder Keg"] = Item({Time(1,false,0,3,true,12)}, "Powder Keg", "0C", "34", "34", "80", "01CA", "5E", "", {"CD6996"}, {"C5CE30"}, "01", {"C5CE60"});
    Items["Razor Sword"] = Item({Time(2,false,0,2,false,12), Time(3,false,0,3,false,12)}, "Razor Sword", "4E", "38", "38", "A0", "01F9", "67", "", {"CD69AE", "CD6C0C"}, {"C5CE00"}, "2_00000010", {"C5CE21"}); //C5CE21 Inv sword/shield
    Items["Romani's Mask"] = Item({Time(2,false,0,2,true,1, "SaveAliens")}, "Romani's Mask", "3C", "82", "82", "A0", "021F", "71", "", {"CD6B6A"}, {"C5CE48"});
    Items["Room Key"] = Item({Time(1,false,6,1,false,8)}, "Room Key", "2D", "A0", "A0", "80", "020F", "6D", "", {"CD6C1E"}, {"C5CE2F"});
    Items["Stone Mask"] = Item({Time(1,false,0,3,true,12)}, "Stone Mask", "45", "8B", "8B", "A0", "0254", "0D", "", {"CD6BA0"}, {"C5CE3F"});
    Items["Swamp Title Deed"] = Item({Time(1,false,0,3,true,12)}, "Swamp Title Deed", "2A", "98", "98", "80", "01B2", "41", "", {"CD6BEE"}, {"C5CE29"});

    Items["Deku Mask"] = Item({Time(1,false,0,3,true,12)}, "Deku Mask", "32", "78", "78", "A0", "01BD", "5C", "", {"CD6B2E"}, {"C5CE41", "F11247"}, {"2CD0FF9"});
    Items["Goron Mask"] = Item({Time(1,false,0,3,true,12)}, "Goron Mask", "33", "79", "79", "A0", "0119", "45", "", {"CD6B34"}, {"C5CE47", "F12A5F"}, {"2A6BE19"});
    Items["Zora Mask"] = Item({Time(1,false,0,3,true,12)}, "Zora Mask", "34", "7A", "7A", "A0", "011A", "46", "", {"CD6B3A"}, {"C5CE4D", "F12B73"}, {"26C128D"});

    Items["Big Poe"] = Item({Time(3,false,0,3,true,12)}, "Big Poe", "1E", "66", "66", "80", "0139", "53", "", {"CD6AC2"}, {"C5CE36", "CD7C53"}, {"CD7C55"});
    Items["Blue Potion"] = Item({Time(1,false,0,3,true,12)}, "Blue Potion", "15", "5D", "5D", "80", "00C1", "33", "", {"CD6A8C"}, {"C5CE36", "CDE5BB"});
    Items["Bugs"] = Item({Time(1,false,0,3,true,12)}, "Bugs", "1B", "63", "63", "80", "0137", "4C", "", {"CD6AB0"}, {"C5CE36", "CD7C17", "CD7C1D"}, {"CD7C19", "CD7C1F"});
    Items["Chateau Romani"] = Item({Time(1,true,4,1,true,12), Time(2,true,4,2,true,12), Time(3,true,4,3,true,12)}, "Chateau Romani", "25", "6F", "6F", "80", "0227", "74", "", {"CD6AF8", "CD6BC4"}, {"C5CE36"});
    Items["Deku Princess"] = Item({Time(1,false,0,3,true,12)}, "Deku Princess", "17", "5F", "5F", "80", "009E", "01", "", {"CD6A98"}, {"C5CE36", "CD7C3B"}, {"CD7C3D"});
    Items["Fairy"] = Item({Time(1,false,0,3,true,12)}, "Fairy", "16", "5E", "5E", "80", "0272", "3C", "", {"CD6A92"}, {"C5CE36", "CD7C0B", "CDE5CF"}, {"CD7C0D"});
    Items["Fish"] = Item({Time(1,false,0,3,true,12)}, "Fish", "1A", "62", "62", "80", "00C7", "36", "", {"CD6AAA"}, {"C5CE36", "CD7C11"}, {"CD7C13"});
    Items["Gold Dust"] = Item({Time(1,false,0,3,true,12)}, "Gold Dust", "22", "6A", "6A", "80", "01E9", "60", "", {"CD6ADA", "CD6BD0"}, {"C5CE36"});
    Items["Green Potion"] = Item({Time(1,false,0,3,true,12)}, "Green Potion", "14", "5C", "5C", "80", "00C1", "31", "", {"CD6A86"}, {"C5CE36", "CDE5A7"});
    Items["Hot Spring Water"] = Item({Time(1,false,0,3,true,12)}, "Hot Spring Water", "20", "68", "68", "00", "0139", "53", "", {"CD6ACE"}, {"C5CE36", "CD7C29", "CD7C2F"}, {"CD7C2B", "CD7C31"});
    Items["Milk"] = Item({Time(1,false,0,3,true,12)}, "Milk", "18", "92", "92", "80", "00B6", "2C", "", {"CD6A9E", "CD6BCA"}, {"C5CE36"});
    Items["Mushroom"] = Item({Time(1,false,0,3,true,12)}, "Mushroom", "23", "6B", "6B", "80", "021D", "70", "", {"CD6AE0"}, {"C5CE36", "CD7C47"}, {"CD7C49"});
    Items["Poe"] = Item({Time(1,false,0,3,true,12)}, "Poe", "1D", "65", "65", "80", "009E", "01", "", {"CD6ABC"}, {"C5CE36", "CD7C4D"}, {"CD7C4F"});
    Items["Red Potion"] = Item({Time(1,false,0,3,true,12)}, "Red Potion", "13", "5B", "5B", "80", "00C1", "32", "", {"CD6A80"}, {"C5CE36", "CDE593"});
    Items["Seahorse"] = Item({Time(1,false,0,3,true,12)}, "Seahorse", "24", "6E", "95", "80", "01E9", "60", "", {"CD6BDC"}, {"C5CE36"});
    Items["Spring Water"] = Item({Time(1,false,0,3,true,12)}, "Spring Water", "1F", "67", "67", "00", "0000", "00", "", {"CD6AC8"}, {"C5CE36", "CD7C23"}, {"CD7C25"});
    Items["Zora Egg"] = Item({Time(1,false,0,3,true,12)}, "Zora Egg", "21", "69", "69", "80", "01AE", "59", "", {"CD6AD4"}, {"C5CE36", "CD7C35"}, {"CD7C37"});

    //Cant start out with a map? - maybe can, when getting a map, it does nothing for some reason
    //change the item ids for the map from 31 to the get item id so that my custom function can determine which map to give
    Items["Clocktown Map"] = Item({Time(1,false,0,3,true,12)}, "Clocktown Map", "B4", "B4", "B4", "A0", "024D", "2E", "", {"CD6C96"}, {}, "M_10110100", {"C1CB13", "C1CB2B"});
    Items["Woodfall Map"] = Item({Time(1,false,0,3,true,12)}, "Woodfall Map", "B5", "B5", "B5", "A0", "024D", "2E", "", {"CD6C9C"}, {}, "M_10110101", {"C1CB13", "C1CB2B"});
    Items["Snowhead Map"] = Item({Time(1,false,0,3,true,12)}, "Snowhead Map", "B6", "B6", "B6", "A0", "024D", "2E", "", {"CD6CA2"}, {}, "M_10110110", {"C1CB13", "C1CB2B"});
    Items["Romani Ranch Map"] = Item({Time(1,false,0,3,true,12)}, "Romani Ranch Map", "B7", "B7", "B7", "A0", "024D", "2E", "", {"CD6CA8"}, {}, "M_10110111", {"C1CB13", "C1CB2B"});
    Items["Great Bay Map"] = Item({Time(1,false,0,3,true,12)}, "Great Bay Map", "B8", "B8", "B8", "A0", "024D", "2E", "", {"CD6CAE"}, {}, "M_10111000", {"C1CB13", "C1CB2B"});
    Items["Stone Tower Map"] = Item({Time(1,false,0,3,true,12)}, "Stone Tower Map", "B9", "B9", "B9", "A0", "024D", "2E", "", {"CD6CB4"}, {}, "M_10111001", {"C1CB13", "C1CB2B"});

    //Items["Green Rupee"] = Item({Time(1,false,0,3,true,12)}, "Green Rupee", "84", "01", "C4", "00", "013F", "B0", "", {"CD6864"}, {"C5CDEF"}, {}, {}, {"C55FE1"}, "01", {"CD6864"}, "84");
    //Items["Blue Rupee"] = Item({Time(1,false,0,3,true,12)}, "Blue Rupee", "85", "02", "02", "01", "013F", "AF", "", {"CD686A"}, {"C5CDEF"}, {}, {}, {"C55FE3"}, "05", {"CD686A"}, "85");
    //Items["Red Rupee"] = Item({Time(1,false,0,3,true,12)}, "Red Rupee", "87", "04", "04", "02", "013F", "AE", "", {"CD6876"}, {"C5CDEF"}, {}, {}, {"C55FE7"}, "14", {"CD6876"}, "87");
    //Items["Purple Rupee"] = Item({Time(1,false,0,3,true,12)}, "Purple Rupee", "88", "05", "05", "14", "013F", "AC", "", {"CD687C"}, {"C5CDEF"}, {}, {}, {"C55FE9"}, "32", {"CD687C"}, "88");
    //Items["Silver Rupee"] = Item({Time(1,false,0,3,true,12)}, "Silver Rupee", "89", "06", "06", "14", "013F", "AB", "", {"CD6882"}, {"C5CDEF"}, {}, {}, {"C55FEB"}, "64", {"CD6882"}, "89");
    //Items["Gold Rupee"] = Item({Time(1,false,0,3,true,12)}, "Gold Rupee", "8A", "07", "07", "13", "013F", "BD", "", {"CD6888"}, {"C5CDEF"}, {}, {}, {"C55FED"}, "C8", {"CD6888"}, "8A");

    Items["Sonata of Awakening"] = Item({Time(1,false,0,3,true,12)}, "Sonata of Awakening", "61", "53", "73", "00", "008F", "08", "", {"CD6A50"}, {"", "F1B4AF"}, {"277A2A5"}, {}, {"C661F9"}, "02", "12", {"277A275"}, {"277A28D"}, "F_01000000", {"C5CE73"});
    Items["Goron Lullaby"] = Item({Time(1,false,0,3,true,12)}, "Goron Lullaby", "62", "54", "74", "00", "008F", "08", "", {"CD6A56"}, {}, {"263BC31"}, {}, {"C661FB"}, "03", "13", {"263B4F9"}, {"263B511"}, "F_10000000", {"C5CE73"});
    Items["New Wave Bossa Nova"] = Item({Time(1,false,0,3,true,12)}, "New Wave Bossa Nova", "63", "71", "75", "00", "008F", "08", "", {"CD6B04"}, {"", "DCCC8F"}, {"2602241"}, {}, {"C661FD"}, "04", "14", {}, {"2602229"}, "F_00000001", {"C5CE72"});
    Items["Elegy of Emptiness"] = Item({Time(1,false,0,3,true,12)}, "Elegy of Emptiness", "64", "72", "76", "00", "008F", "08", "", {"CD6B0A"}, {}, {"2B71639"}, {}, {"C661FF"}, "05", "15", {"2B71609"}, {"2B71621"}, "F_00000010", {"C5CE72"});
    Items["Oath to Order"] = Item({Time(1,false,0,3,true,12)}, "Oath to Order", "65", "73", "77", "00", "008F", "08", "", {"CD6B10"}, {}, {"2D83A0D"}, {}, {"C66201"}, "06", "16", {"2D839DD"}, {"2D839F5"}, "F_00000100", {"C5CE72"});
    //Items["Oath to Order"] = Item({Time(1,false,0,3,true,12)}, "Oath to Order", "65", "73", "77", "00", "008F", "08", "", {"CD6B10"}, {"C5CE72"}, {"2D83A0D"}, {}, {"C66201"}, "06", "16", {"2D83941"}, {"2D83959"});
    //Items["Song of Time"] = Item({Time(1,false,0,3,true,12)}, "Song of Time", "67", "74", "00", "00", "008F", "08", "", {"CD6B16"}, {"C5CE72"});    //song of time minus 61 location C66203
    //Items["Song of Healing"] = Item({Time(1,false,0,3,true,12)}, "Song of Healing", "68", "75", "94", "00", "008F", "08", "", {"CD6B1C"}, {"C5CE72"}, {}, {}, {"C66207"}, "09", "19", {"2CCFB99"}, {"2CCFBB1"});
    Items["Song of Healing"] = Item({Time(1,false,0,3,true,12)}, "Song of Healing", "68", "75", "94", "00", "008F", "08", "", {"CD6B1C"}, {}, {}, {}, {"C66207"}, "09", "19", {"2CCFB99"}, {"2CCFBB1"}, "F_00100000", {"C5CE72"});
    Items["Epona's Song"] = Item({Time(1,false,0,3,true,12)}, "Epona's Song", "69", "76", "70", "00", "008F", "08", "", {"CD6A50"}, {}, {"269141D"}, {}, {"C66209"}, "0A", "1A", {"26913ED"}, {"2691405"}, "F_01000000", {"C5CE72"});
    Items["Song of Soaring"] = Item({Time(1,false,0,3,true,12)}, "Song of Soaring", "6A", "A2", "71", "00", "008F", "08", "", {"CD6C2A"}, {"", "F2FB7B"}, {"2825EAD"}, {}, {"C6620B"}, "0B", "1B", {"2825E7D"}, {"2825E95"}, "F_10000000", {"C5CE72"});
    Items["Song of Storms"] = Item({Time(1,false,0,3,true,12)}, "Song of Storms", "6B", "A3", "72", "00", "008F", "08", "", {"CD6C30"}, {}, {"1F82BC5"}, {}, {"C6620D"}, "0C", "1C", {"1F82BA1"}, {"1F82BAD"}, "F_00000001", {"C5CE71"});

    //vector<string> Song_Names = {   "Song of Time", "Song of Healing", "Song of Soaring", "Epona's Song", "Song of Storms", "Sonata of Awakening", "Goron Lullaby", "New Wave Bossa Nova", "Elegy of Emptiness", "Oath to Order"};
    //vector<string> Song_Bit_Values = {"00010000",      "00100000",         "10000000",       "01000000",     "00000001",         "01000000",         "10000000",        "00000001",             "00000010",          "00000100"};
    //string Songs_Bit_1 = "00000000";   //C5CE71 12963441
    //string Songs_Bit_2 = "00010000";   //C5CE72 12963442
    //string Songs_Bit_3 = "00000000";   //C5CE73 12963443

    Items["Heart Piece"] = Item({Time(1,false,0,3,true,12)}, "Heart Piece", "7B", "0C", "0C", "A0", "0096", "14", "", {"CD68A6"}, {}, "10", {"C5CE70"});
    Items["Heart Container"] = Item({Time(1,false,0,3,true,12)}, "Heart Container", "6F", "0D", "0D", "A0", "0096", "13", "", {"CD68AC"}, {}, "10", {"C5CDE9", "C5CDEB"});

    Items["Bombchu"] = Item({Time(1,false,0,3,true,12)}, "Bombchu", "99", "36", "36", "C0", "00B0", "D9", "", "07", {"CD69A2"}, {"C5CE2B"}, "01", {});
    Items["Bombchus (5)"] = Item({Time(1,false,0,3,true,12)}, "Bombchus (5)", "9A", "3A", "3A", "C0", "00B0", "D9", "", "07", {"CD69BA"}, {"C5CE2B"}, "05", {});
    Items["Bombchus (10)"] = Item({Time(1,false,0,3,true,12)}, "Bombchus (10)", "98", "1A", "1A", "C0", "00B0", "D9", "", "07", {"CD68FA"}, {"C5CE2B"}, "0A", {});
    //Items["Bombchus (20)"] = Item({Time(1,false,0,3,true,12)}, "Bombchus (20)", "97", "2E", "2E", "C0", "00B0", "D9", "", "07", {"CD6972"}, {"C5CE2B"}, "14", {});
    //name    item id     get item id   text id     flag   object   get item model  pool = ""   get item locations  item id locations   item count  item count locations

    //Items["Odalwa's Remains"] = Item({Time(1,false,0,3,true,12)}, "Odalwa's Remains", "5D", "55", "55", "80", "0097", "5D", "", {"CD6A5C"}, {});

    //get the settings from the settings file
    Settings = OpenAsIni("./settings.ini");

    //update the item pools according to the settings
    Update_Pools(Items);

    //cout << "Shuffling items\n";
    //shuffle(Items, Settings["settings"]["Seed"]);

    //make sure able to get all items if using logic
    //if (Settings["settings"]["Logic"] != "" && Settings["settings"]["Logic"] != "None") {
      //  Fix_Shuffled(Items, Settings["settings"]["Seed"], Settings["settings"]["Logic"]);
    //}

    //randomize items according to logic, if any logic was chosen
    cout << "Randomizing Items...\n";
    //Randomize(Items, Settings["settings"]["Seed"], Settings["settings"]["Logic"]);
    Randomize(Items, Settings);

    //write spoiler log
    Write_Log(Settings["settings"]["Seed"]);

    //decompress rom
    cout << "\nDecompressing rom\n";
    if (system(("ndec\\ndec.exe \"" + Settings["settings"]["Rom"] + "\" " + Rom_Location).c_str()) != 0) {
        //if failed to decompress file
        err_file << "Failed to decompress " << Settings["settings"]["Rom"] << " - Might be missing \"ndec\" folder";
        err_file.close();
        exit(0);
    }

    bool Songs_Same_Pool = All_Songs_Same_Pool();

    cout << "\nSongs in Same Pool: " << Songs_Same_Pool << endl;

    cout << "\nPlacing Items\n";

    Place_Items(Items, Songs_Same_Pool);

    cout << "\nChanging Rupees\n";

    //Change max rupee amounts
    Change_Rupees(Settings["wallets"]);

    cout << "\nGiving Starting Items\n";

    //FD anywhere
    Write_To_Rom(12220113, "00");

    //FD can use transformations masks
    Write_To_Rom(12945794, "010101");   //enables deku, goron, and zora

    //Quick Text - softlock at couple's mask - this actually was not responsible for the softlock
    //Write_To_Rom(12523072, "1000");
    //Write_To_Rom(13065591, "30");

    //quick text 2
    Write_To_Rom(12482776, "00000000");
    Write_To_Rom(13065591, "30");

    //Remove check when buying Hero's Shield
    Write_To_Rom(13492260, "10000003");

    cout << "\nRemoving Item Checks\n";

    //Remove Item Checks
    Remove_Item_Checks();

    //change starting scene to clock town
    Write_To_Rom(12433538, "D8");

    //Give Tatl to Link in Clock Town CS, shorten cs to 1 frame, and set entered clocktown for the first time flag
    Write_To_Rom(48476396, "0000000200000001");
    Write_To_Rom(48476404, "0000009A000000010001000100020002");
    Write_To_Rom(48476420, "00000096000000010021000100020002");

    //Fix tear and deeds - dont need this, can change what the showing item shows to be correct for every item
    /*Write_To_Rom(17217455, "40"); //moon's tear
    Write_To_Rom(11857915, "46"); //land title deed
    Write_To_Rom(15994819, "46"); //land title deed (needs to match)
    Write_To_Rom(17112975, "47"); //swamp title deed
    Write_To_Rom(17112983, "48"); //mountain title deed
    Write_To_Rom(17112991, "49"); //ocean title deed

    //Fix Key and Express Mail
    Write_To_Rom(16505383, "4A");
    Write_To_Rom(16019723, "4B");

    //Fix Letter to Kafei and Pendant
    Write_To_Rom(16505708, "4D");
    Write_To_Rom(13454571, "4E");
    Write_To_Rom(11857907, "4E");

    //fix Magic Beans
    Write_To_Rom(14439987, "4F");   //bean man
    Write_To_Rom(45912662, "59FF");    //bean chest
    */

    //make scrub salesman always sell beans
    if (Settings["settings"]["ScrubBeans"] == "True") {
        string Bean_Source = Get_Source("Magic Beans");
        string New_ID = Items[Bean_Source].Get_Item_ID;

        Write_To_Rom(17113087, New_ID);   //scrub salesman
    }

    //fix getting false mask when SoT from tatl text in clocktown
    Write_To_Rom(48611908, "E007");

    //fix turning deku when SoT - stay as link
    Write_To_Rom(47286973, "0A");

    //Make catching bottle contents use the passive get item function instead of whatever it was already using
    //Write_To_Rom(12296876, "0C044BA0");   -   this made infinite bottles and other errors
    //make bottles work with bottle contents and items
    Make_Bottles_Work();

    //input the function to give tingle map branch to it in the get item passive function
    Fix_Tingle_Maps();

    //Give the player the starting items
    Give_Starting_Items();

    //Set the number of frames of the hms cs when you enter clock tower to 0
    //Write_To_Rom(46981670, "0000");

    //remove the actor that plays the hms cs inside the clock tower
    //make hms think you already watched the cs
    Remove_Actor("2CD6000", 11);
    Write_To_Rom(15953540, "24020001");
    Write_To_Rom(15955712, "24020001");

    //fix getting the swords as transformation masks
    Fix_Swords();

    //change the give song passive to work with just raw ids, nevermind, this might effect other things so gonna just write diff values
    //Write_To_Rom(12500416, "00000000");

    //fix the song text ids for when it is loaded and displayed
    Fix_Song_Text();

    Write_To_Rom(13182652, "260DFFFA00000000");  //Fix song looks in inventory
    Write_To_Rom(13184360, "260CFFFA00000000"); //fix playing songs in inventory

    //input the songs text and fix the text offsets
    Songs_Text_Offset();

    //give new wave from the thing with the offset so that it works from song of storms
    //Write_To_Rom(12500392, "00000000");

    //make new wave bossa nova give the song only once
    if (Songs_Same_Pool) {
        string NWBN = Items["New Wave Bossa Nova"].Name;
        Write_To_Rom(12500391, Hex_Minus(Items[NWBN].Item_ID, "61"));
    }

    Fix_Goron_Lullaby();

    Fix_Showing_Items();

    //dont let sonata give item twice
    Write_To_Rom(15840424, "00000000");

    //Use Gamecube HUD
    if (Settings["settings"]["GC_Hud"] == "True") {
        cout << "\nApplying GC Hud\n";
        Gamecube_Hud();
    }

    //remove the cutscenes
    if (Settings["settings"]["Remove_Cutscenes"] == "True") {
        cout << "\nRemoving Cutscenes\n";
        Remove_Cutscenes(Songs_Same_Pool);
    }

    //change tunic colors
    cout << "\nChanging tunic colors\n";
    Change_Colors(Settings["colors"]);

    //making link kafei
    if (Settings["settings"]["Kafei"] == "True") {
        cout << "\nMaking link Kafei @Purpletissuebox\n";
        Change_Link_Kafei();
        Change_Kafei_Color(Settings["settings"]["Tunic"]);
    }

    //change water to bingo water if secret is active
    if (Settings["settings"]["BingoWater"] == "True") {
        Bingo_Water();
    }

    //change blast mask cooldown
    Change_BlastMask(Settings["settings"]);

    //compress rom and create wad
    if (Settings["settings"]["Wad"] == "True") {
        //create rom and wad
        cout << "\nCompressing rom and creating wad\n\n";
        if (system("_create-roms.bat") != 0) {
            //if failed to compress file
            err_file << "Failed to compress rom using \"_create-roms.bat\"\n";
            err_file.close();
            exit(0);
        }
    }
    else {
        //create rom
        cout << "\nCompressing rom\n\n";
        if (system("_create-rom.bat") != 0) {
            //if failed to compress file
            err_file << "Failed to compress rom using \"_create-rom.bat\"\n";
            err_file.close();
            exit(0);
        }
    }

    //rename the rom to include the seed
    /*if (system(("rename \"Legend of Zelda, The - Majoras Mask - Randomizer (U).z64\" \"Legend of Zelda, The - Majoras Mask - Randomizer (U)_" + Settings["settings"]["Seed"] + ".z64\"").c_str()) != 0) {
        //couldn't find the compressed rom
        err_file << "Compressing the rom failed - might be missing \"_create-roms\" folder";
        err_file.close();
        exit(0);
    }*/

    //Delete decompressed rom since it is no longer needed
    //system(("del " + Rom_Location).c_str());

    outFile.close();
    err_file.close();

    return 0;
}

