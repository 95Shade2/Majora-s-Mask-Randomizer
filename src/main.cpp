#include "rando/logging.hpp"
#include "rando/kafei.hpp"
#include "rando/inventory.hpp"
#include "rando/item.hpp"
#include "rando/utils.hpp"
#include "rando/color.hpp"
#include "rando/io.hpp"
#include "rando/rom.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <vector>
#include <filesystem>

// bad shade!
using namespace std;

ofstream err_file;
ofstream log_file;

bool DEBUG = true;

fstream inFile;
ofstream outFile;
int Max_Percentage;
map<string, Item> Items = {};
vector<string> Item_Names;
vector<string> Item_Keys;
vector<string> Bottle_Names;
vector<string> Bottle_Keys;
vector<string> TMask_Names;
vector<string> TMask_Keys;
map<string, int> Points;
map<string, int> Logic_Item_Sets;	//which item sets that the logic used to determine the items were obtainable
string Rom_Location = "mm2.z64"; // location of the decompressed rom
map<string, map<string, string>> Settings;
map < string,	//location
	vector< //item set
	vector<	//item index in set + 1 (0 = when item at location is given)
	map < string,	//day or night
	vector<bool>>>>>	//day or night 1, 2, and 3
	Day_Data;

int Random(int min, int max)
{
    int number = rand();

    if (min == max)
    {
        return max;
    }

    number = number % max + min;

    return number;
}

string Item_Get(Item it)
{
    string data = "";

    data = it.Item_ID + it.Flag + it.Get_Item_Model + it.Text_ID + it.Obj;

    return data;
}

string Get_Source(string item, int index = 0)
{
	vector<string> keys;

	keys = Get_Keys(Items);

	for (int i = 0; i < keys.size(); i++)
	{
		if (Items[keys[i]].Name == item)
		{
			if (index == 0) {
				return keys[i];
			}
			else {
				index--;
			}
		}
	}

	return "";
}

//multiple items have the possibility of giving the same source, this returns a vector of the items that all give "item"
vector<string> Get_Sources(string item) {
	vector<string> sources;
	string source = Get_Source(item);
	int s = 0;

	while (source != "") {
		sources.push_back(source);

		//gets the next source (if there are more)
		s++;
		source = Get_Source(item, s);
	}

	return sources;
}

void Place_Items(map<string, Item> &Items, bool Songs_Same_Pool)
{
    string Old_Item = "";
    string New_Item = "";

    for (const auto &kv : Items)
    {
        Old_Item = kv.first;
        New_Item = Items[Old_Item].Name;

        // replace each item id entry - starts at 1 because 0 is where the item is in the
        // inventory screen (used for replacing deku mask)
        if (Items[Old_Item].Address_Item_ID.size() > 1)
        {
            for (int i = 1; i < Items[Old_Item].Address_Item_ID.size(); i++)
            {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Address_Item_ID[i]),
                             Items[New_Item].Item_ID);
            }
        }

        // replace each get item entry
        if (Items[Old_Item].Address_Get.size() > 0)
        {
			//only change the get item data if the current item is not vanilla
			if (Old_Item != New_Item) {
				for (int i = 0; i < Items[Old_Item].Address_Get.size(); i++)
				{
					Write_To_Rom(hex_to_decimal(Items[Old_Item].Address_Get[i]),
						Item_Get(Items[New_Item]));
				}
			}
        }

        // replace each text id location
        if (Items[Old_Item].Address_Text_ID.size() > 0)
        {
            for (int i = 0; i < Items[Old_Item].Address_Text_ID.size(); i++)
            {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Address_Text_ID[i]),
                             Items[New_Item].Text_ID);
            }
        }

        // place the other locations - don't think this is ever used anymore, need to check before removing
        if (Items[Old_Item].Other_Locations.size() > 0)
        {
            for (int i = 0; i < Items[Old_Item].Other_Locations.size(); i++)
            {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Other_Locations[i]),
                             Items[New_Item].Other_locations_Data);
            }
        }

        // making the get item give the rupee value of the correct rupee - don't think this is ever used anymore, need to check before removing
        if (Items[Old_Item].Other_Locations2.size() > 0)
        {
            for (int i = 0; i < Items[Old_Item].Other_Locations2.size(); i++)
            {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Other_Locations2[i]),
                             Items[Old_Item].Other_locations_Data2);
            }
        }

        // replace the song staffs with the new song if a song is replacing a song AND if
        // all songs are songs to prevent getting locked out of an item
        if (Items[Old_Item].Song2_Locatinos.size() > 0 &&
            Items[New_Item].Song2_Locatinos.size() > 0 && Songs_Same_Pool)
        {
            for (int i = 0; i < Items[Old_Item].Song1_Locatinos.size(); i++)
            {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Song1_Locatinos[i]),
                             Items[New_Item].Song1_ID);
            }
            for (int i = 0; i < Items[Old_Item].Song2_Locatinos.size(); i++)
            {
                Write_To_Rom(hex_to_decimal(Items[Old_Item].Song2_Locatinos[i]),
                             Items[New_Item].Song2_ID);
            }
        }

        // place the song ids minus 61 where they go
        else if (Items[Old_Item].ID_Minus_61_Locations.size() > 0)
        {
            for (int i = 0; i < Items[Old_Item].ID_Minus_61_Locations.size(); i++)
            {
                // hex minus will make it wrap back around when 61 is added to it
                Write_To_Rom(hex_to_decimal(Items[Old_Item].ID_Minus_61_Locations[i]),
                             Hex_Minus(Items[New_Item].Item_ID, "61"));
            }
        }
    }
}

void Update_Wallet(int location, string wallet_amount)
{
    int wallet_int = string_to_dec(wallet_amount);
    string wallet_hexL = dec_to_hex(wallet_int >> 8); // left byte
    string wallet_hexR = dec_to_hex(wallet_int & 0xFF);      // right byte
    string wallet_size = wallet_hexL + wallet_hexR;

    Write_To_Rom(location, wallet_size);
}

void Change_Rupees(map<string, string> wallet_amounts)
{
    int small_digits = wallet_amounts["Small"].size();
    int med_digits = wallet_amounts["Medium"].size();
    int large_digits = wallet_amounts["Large"].size();

    if (small_digits > 5)
    {
        small_digits = 5;
    }
    if (med_digits > 5)
    {
        med_digits = 5;
    }
    if (large_digits > 5)
    {
        large_digits = 5;
    }

    // small
    // write padding for small wallet
    Write_To_Rom(12935772, "000" + dec_to_string(5 - small_digits));

    // write number of digits for small wallet
    Write_To_Rom(12935780, "000" + dec_to_string(small_digits));

    // write the small wallet amount
    Update_Wallet(12944236, wallet_amounts["Small"]);

    // adult
    // write padding for adult wallet
    Write_To_Rom(12935774, "000" + dec_to_string(5 - med_digits));

    // write number of digits for adult wallet
    Write_To_Rom(12935782, "000" + dec_to_string(med_digits));

    // write the adult wallet amount
    Update_Wallet(12944238, wallet_amounts["Medium"]);

    // update the text for adult wallet
    string plurarl = "s";
    if (string_to_dec(wallet_amounts["Medium"]) == 1)
    {
        plurarl = "";
    }
    Write_To_Rom(11342319,
                 string_to_hex("You can now carry ") + "06" +
                   string_to_hex(wallet_amounts["Medium"]) + "00" +
                   string_to_hex(" Rupee" + plurarl) + "002EBF");

    // giant
    // write padding for giant wallet
    Write_To_Rom(12935776, "000" + dec_to_string(5 - large_digits));

    // write number of digits for giant wallet
    Write_To_Rom(12935784, "000" + dec_to_string(large_digits));

    // write the giant wallet amount
    Update_Wallet(12944240, wallet_amounts["Large"]);

    // update the text for giant wallet
    plurarl = "s";
    if (string_to_dec(wallet_amounts["Large"]) == 1)
    {
        plurarl = "";
    }
    Write_To_Rom(11342482,
                 "01" + string_to_hex(wallet_amounts["Large"] + " Rupee" + plurarl) +
                   "002EBF");

    // fix max wallet not being green with bigger wallet
    Write_To_Rom(12281248, "9462003A");

    // fix from going over max capacity when capacity is really high
    Write_To_Rom(12286660, "9503003A");

    // write jimmie's 5 digit function :)
    Write_To_Rom(
      12280768,
      "3C02801F9442F6AA0002702534180006340F000A29C10001142000080000000001CF001A0000000000"
      "0070120000181003B8C821A32302B71000FFF52318FFFF0000702503B8C821A32E02B82B0100011020"
      "FFFA000000000000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000003C19801F3C0E801C8DCE1DD0"
      "8F39F7283C0F801C91EF1E08032EC0248623027201F81006000210403C08801C3C0C801C0102402101"
      "826021286100B58508FD1C14200002858CFD24240300B41980009200004825306B00FF8E0502A03C0E"
      "E7002406000824B90008AE1902A0ACA00004ACAE00008E0502A03C0FFA002519000124B80008AE1802"
      "A0ACAB0004ACAF000003B9702191CE02B8");
}

///returns a pointer to an array of the wallet sizes
vector<int> Get_Wallet_Sizes() {
	vector<int> sizes = { 0, 0, 0 };

	sizes[0] = string_to_dec(Settings["wallets"]["Small"]);
	sizes[1] = string_to_dec(Settings["wallets"]["Medium"]);
	sizes[2] = string_to_dec(Settings["wallets"]["Large"]);

	return sizes;
}

///returns which wallet the players starts with, 0 for child, 1 for adult, and 2 for giant
int Get_Starting_Wallet() {

	//if the player starts with giant wallet
	if (Items["Song of Healing"].Name == "Giant Wallet" || Items["Deku Mask"].Name == "Giant Wallet") {
		return 2;
	}
	//if the player starts with adult wallet
	if (Items["Song of Healing"].Name == "Adult Wallet" || Items["Deku Mask"].Name == "Adult Wallet") {
		return 1;
	}
	//the player starts out with child wallet
	else {
		return 0;
	}
}

///returns Giant Wallet, Adult Wallet, or Child Wallet
string Get_Wallet(map<string, int> Items_Gotten) {
	string wallet;

	if (Items_Gotten["Giant Wallet"] >= 0) {
		wallet = "Giant Wallet";
	}
	else if (Items_Gotten["Adult Wallet"] >= 0) {
		wallet = "Adult Wallet";
	}
	else {
		wallet = "Child Wallet";
	}

	return wallet;
}

///Returns the max amount of rupees the player can get according to the current wallet
int Get_Wallet_Max(map<string, int> Items_Gotten = {}) {
	vector<int> wallets = Get_Wallet_Sizes();
	int Wal_Index;
	int max;
	string wallet;

	//get the wallet
	wallet = Get_Wallet(Items_Gotten);
	
	//get the wallet index	
	if (wallet == "Giant Wallet") {
		Wal_Index = 2;
	}
	else if (wallet == "Adult Wallet") {
		Wal_Index = 1;
	}
	else {
		Wal_Index = 0;
	}

	//get the wallet's max rupee count
	max = wallets[Wal_Index];

	return max;
}

void Give_Starting_Items()
{
    vector<string> Start_Sources = {"Deku Mask", "Song of Healing"};
    string hex;
    string location;
    string Count_Location;
    string Start_Item;
    vector<string> Song_Names = {"Song of Time",
                                 "Song of Healing",
                                 "Song of Soaring",
                                 "Epona's Song",
                                 "Song of Storms",
                                 "Sonata of Awakening",
                                 "Goron Lullaby",
                                 "New Wave Bossa Nova",
                                 "Elegy of Emptiness",
                                 "Oath to Order"};
    vector<string> Song_Bit_Values = {"00010000",
                                      "00100000",
                                      "10000000",
                                      "01000000",
                                      "00000001",
                                      "01000000",
                                      "10000000",
                                      "00000001",
                                      "00000010",
                                      "00000100"};
    // string Songs_Bit_1 = "00000000";   //C5CE71 12963441
    // string Songs_Bit_2 = "00010000";   //C5CE72 12963442    always start out with Song
    // of Time string Songs_Bit_3 = "00000000";   //C5CE73 12963443
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

    // give ocarina
    Write_To_Rom(12963364, "00");

    Item_Flags[songs_1] = "00000000";
    Item_Flags[songs_2] = "00010000"; // start out with Song of Time
    Item_Flags[songs_3] = "00000000";
    Item_Flags[wallet] = "00000000";       // start out with small wallet
    Item_Flags[sword_shield] = "00010001"; // start out with sword and shield
    Item_F_Locations.push_back(songs_1);
    Item_F_Locations.push_back(songs_2);
    Item_F_Locations.push_back(songs_3);
    Item_F_Locations.push_back(wallet);
    Item_F_Locations.push_back(sword_shield);

    // start out with 3 hearts
    Item_Counts[HC] = hex_to_decimal("30");
    Item_Counts[Health] = hex_to_decimal("30");
    Item_C_Locations.push_back(HC);
    Item_C_Locations.push_back(Health);

    // replace each starting item with the new item
    for (int i = 0; i < Start_Sources.size(); i++)
    {
        Start_Item = Start_Sources[i];

        hex = Items[Items[Start_Item].Name].Item_ID2;
        hex = Even_Hex(hex);

        // if there is any locations for the item ids
        if (Items[Items[Start_Item].Name].Address_Item_ID.size() > 0)
        {
            location = Items[Items[Start_Item].Name].Address_Item_ID[0];

            // if there is a location in the inventory to place the item id
            if (location.size() > 0)
            {
                Write_To_Rom(hex_to_decimal(location), hex);
            }
        }

        // if there is a location in the inventory to place the item count, then add to it
        // the count of the current item
        if (Items[Items[Start_Item].Name].Item_Count_Locations.size() > 0)
        {
            for (int l = 0; l < Items[Items[Start_Item].Name].Item_Count_Locations.size();
                 l++)
            {
                Count_Location = Items[Items[Start_Item].Name].Item_Count_Locations[l];

                string key = Count_Location;
                string count_str = Items[Items[Start_Item].Name].Item_Count;
                int Count;

                if (count_str[1] == '_')
                {
                    // this is a flag, not an item count. Or the item is a tier 3 item
                    if (count_str[0] == 'F' || count_str[0] == '3')
                    {
                        count_str = count_str.substr(2);

                        // or the bits in the same byte
                        if (Vector_Has(Item_F_Locations, key))
                        {
                            Item_Flags[key] = Bits_Or(Item_Flags[key], count_str);
                        }
                        else
                        {
                            Item_Flags[key] = count_str;
                            Item_F_Locations.push_back(Count_Location);
                        }
                    }
                    // this is a set value, the byte can be from one item or another, is
                    // not ored together or added. one or the other
                    else if (count_str[0] == 'S')
                    {
                        count_str = count_str.substr(2);

                        if (!Vector_Has(Item_F_Locations, key))
                        {
                            Item_F_Locations.push_back(Count_Location);
                        }
                        Item_Flags[key] = count_str;
                    }
                    else if (count_str[0] == '1')
                    {
                        count_str = count_str.substr(2);

                        if (Vector_Has(Item_F_Locations, key))
                        {
                            int ignore_bit = IndexOf_S(count_str, "1") -
                                             1; // gets the bit that is for tier 2
                            count_str = Bits_Or(count_str, Item_Flags[key]);
                            count_str =
                              Bit_Clear(count_str, ignore_bit); // clears the tier 2 bit
                        }
                        else
                        {
                            Item_F_Locations.push_back(Count_Location);
                        }

                        Item_Flags[key] = count_str;
                    }
                    // tier two upgrade item
                    else if (count_str[0] == '2')
                    {
                        count_str = count_str.substr(2);

                        if (Vector_Has(Item_F_Locations, key))
                        {
                            int ignore_bit = IndexOf_S(count_str, "1") +
                                             1; // gets the bit that is for tier 1 and 3
                                                // to make sure it's not set
                            count_str = Bits_Or(count_str, Item_Flags[key]);
                            count_str = Bit_Clear(
                              count_str, ignore_bit); // clears the lower/upper tier bit
                        }
                        else
                        {
                            Item_F_Locations.push_back(Count_Location);
                        }

                        Item_Flags[key] = count_str;
                    }
                    // map
                    else if (count_str[0] == 'M')
                    {
                        count_str = count_str.substr(2);
                        Count_Location =
                          Items[Items[Start_Item].Name].Item_Count_Locations[i];
                        key = Count_Location;

                        Item_Flags[key] = count_str;
                        Item_F_Locations.push_back(Count_Location);

                        l++;
                    }
                    else
                    {
                        count_str = count_str.substr(2);

                        Count = hex_to_decimal(count_str);

                        if (Vector_Has(Item_C_Locations, key))
                        {
                            Item_Counts[key] += Count;
                        }
                        else
                        {
                            Item_Counts[key] = Count;
                            Item_C_Locations.push_back(Count_Location);
                        }
                    }
                }
                else
                {
                    Count = hex_to_decimal(count_str);

                    if (Vector_Has(Item_C_Locations, key))
                    {
                        Item_Counts[key] += Count;
                    }
                    else
                    {
                        Item_Counts[key] = Count;
                        Item_C_Locations.push_back(Count_Location);
                    }
                }
            }
        }

        // these counts dont get added together
        if (Items[Items[Start_Item].Name].Item_Count_Locations2.size() > 0)
        {
            location = Items[Items[Start_Item].Name].Item_Count_Locations2[0];
            string key = location;

            if (location.size() > 0)
            {
                int Count = hex_to_decimal(Items[Items[Start_Item].Name].Item_Count2);

                if (!Vector_Has(Item_C_Locations, key))
                {
                    Item_C_Locations.push_back(location);
                }
                Item_Counts[key] = Count;
            }
        }
    }

    // write the counts from each item to the locations
    for (int c = 0; c < Item_C_Locations.size(); c++)
    {
        string key = Item_C_Locations[c];
        string location = Item_C_Locations[c];
        int Count = Item_Counts[key];

	//if at least one rupee is a starting item
	if (location == "C5CDEF") {
		int Max_Rupees = Get_Wallet_Max();

		//Don't go over the max number of rupees
		if (Count > Max_Rupees) {
			Count = Max_Rupees;
		}
	}

	//if the count is more than one byte
	if (Count > 255) {
		int High_Count = Count > 4;	//right shift count to only have the left byte
		string High_C_Loc = Hex_Minus(key, "01");	//make a new location right before the current byte to hold the high byte

		Item_Counts[High_C_Loc] = High_Count;
		Item_C_Locations.push_back(High_C_Loc);

		Count = Count & 0xFF;	//make Count the low byte only
	}

        Write_To_Rom(hex_to_decimal(location), dec_to_hex(Count));
    }

    // write the flag bits from each item to the locations
    for (int c = 0; c < Item_F_Locations.size(); c++)
    {
        string key = Item_F_Locations[c];
        string location = Item_F_Locations[c];
        string Flag = Item_Flags[key];

        Write_To_Rom(hex_to_decimal(location), binary_to_hex(Flag));
    }
}

void Remove_Item_Checks()
{
    Write_To_Rom(14439692, "9318360301CF182300031C003301004014010004");
    Write_To_Rom(13334652, "00000000");
    Write_To_Rom(13334692, "10000016");
    Write_To_Rom(15512024, "1000000F");
    Write_To_Rom(15511648,
                 "00000000"); // this makes it where tingle doesnt write the map data
    Write_To_Rom(17168980, "10000007");
    Write_To_Rom(12962936, "00003000");
    Write_To_Rom(16968828, "10000009");
    Write_To_Rom(14442228, "10000009");
    Write_To_Rom(17054436, "00000000");
    Write_To_Rom(16958052, "10000006");
    Write_To_Rom(14902472, "10000068");
    Write_To_Rom(14900732, "00000000");
    Write_To_Rom(16876332, "1000000E");
    Write_To_Rom(16365624, "10000005");
    Write_To_Rom(16019572, "10000010");
    Write_To_Rom(15445332, "000D0000"); // always give pictograph box
    Write_To_Rom(16124449, "003E0000");
    Write_To_Rom(17059293, "39080000");
    Write_To_Rom(16833748, "10000007");
    Write_To_Rom(16449840, "1000000C");
    Write_To_Rom(14085672, "10000006");
    Write_To_Rom(16726828, "1000000E");
    Write_To_Rom(16732804, "1000000A");
    Write_To_Rom(17113652, "10000003");
    Write_To_Rom(17113676, "10000004");
    Write_To_Rom(15521640, "00000000"); // remove banker check for already gotten wallet
    Write_To_Rom(15204407, "000C0000");
    Write_To_Rom(15204429, "000C0000");
    Write_To_Rom(16479968, "906E3F79");
    Write_To_Rom(16479980, "31CF0004");
    Write_To_Rom(12962878, "000C");
    Write_To_Rom(16566324, "10000004");
    Write_To_Rom(15573992, "10000004");
    Write_To_Rom(16567248, "10000006");

    Write_To_Rom(14914328, "24060023"); // make clock town archery give large quiver
    Write_To_Rom(14914340, "00000000"); // make clock town archery guy ignore if you've already gotten the quiver from him
	Write_To_Rom(14914316, "00000000");	// make town archery ignore which quiver the player has
	Write_To_Rom(14914408, "24060023");	// make town archery gold rupee give quiver (gold rupee is what you get after getting hp)

    Write_To_Rom(14913640, "24060024"); // make swamp archery give largest quiver
    Write_To_Rom(14913688, "24060024"); // make swamp archery guy gives quiver instead of 20 rupees (red rupee is what you get after getting the quiver)
	Write_To_Rom(14913736, "24060024");	// make swamp archery give quiver instead of purple rupee (purple rupee is what you get after getting the hp)
	Write_To_Rom(14913628, "00000000");	// make swamp archery ignore which quiver the player has

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
    Write_To_Rom(12228768, "0000000000000000");

    Write_To_Rom(16479984, "10000003");
    Write_To_Rom(
      11835848,
      "8EE6038884C700002408000E1107000A00000000240900331127001500000000240A00661147001200"
      "000000240200FF03E000080000000084CC001C318D00FF2408000C110D000A000000002409000D112D"
      "000700000000240A0017114D000400000000240200FF03E00008000000003C05801F24080008110400"
      "0D00000000240900091124000D00000000240A008D1144000A00000000240B008E1164000700000000"
      "240200FF03E000080000000090A2F6E803E000080000000090A2F6E903E0000800000000");
    Write_To_Rom(15678584, "2406005B");
    Write_To_Rom(12962948, "00000CF0");
    Write_To_Rom(16134584, "91AD36023C01422031AE0004");
    Write_To_Rom(16134504, "9108360231090004");
    Write_To_Rom(16134260, "91CE36023C0B80B03C0880B031CF0004");
    Write_To_Rom(13298264, "00000000");
    Write_To_Rom(15953724, "00000000"); // Make HMS always give you "deku mask"

    Write_To_Rom(14536572, "10000002"); // make kid ignore if you completed the trials
    Write_To_Rom(14537748, "240F00FF"); // make kid ignore if you have FD mask

    // Remove buying bomb bag checks
    Write_To_Rom(13492708, "15000003");
    Write_To_Rom(13492732, "15000003");

    Write_To_Rom(13513644, "10000004"); // make giant's mask chest not check for giant's
                                        // mask so that it always gives you the item
    Write_To_Rom(15521648, "24180000"); // make banker never check which wallet you have
    Write_To_Rom(16565908, "24060009"); // make giant wallet ignore what wallet you have

    Write_To_Rom(13492684,
                 "24020000"); // make bomb shop guy let you always buy big bomb bag
    Write_To_Rom(13492812, "24020000");

    Write_To_Rom(13492580, "24020000"); // make curiosity shop guy not check for bomb bag
                                        // size when buying big bomb bag

    Write_To_Rom(12234412, "240200FF"); // make town shop guy give you what 10 deku nuts
                                        // give instead of nuts if nuts in inventory
    Write_To_Rom(13492116,
                 "240C0000"); // make town shop guy not check if you have max nuts

    Write_To_Rom(12233552,
                 "00000000"); // make bow do the get item animation if had a larger quiver
    Write_To_Rom(
      12233656,
      "00000000"); // make bomb bag play the get item animation when already having a bag

    Write_To_Rom(12274656,
                 "24060020"); // make hot spring water give hot spring water when cools
    Write_To_Rom(
      12274668,
      "00000000"); // skip jumping to the textbox function for Hot Spring Water cooling

    Write_To_Rom(13513592, "240F0000"); // make captain's hat chest ignore if you have it

    Write_To_Rom(16000100,
                 "10000003"); // make clock town scrub ignore if you've traded in swamp

	Write_To_Rom(16000388, "00000000");	//make the clock town scrub always spawn (skip the cs of him coming in)

    Write_To_Rom(12233316,
                 "240400FF"); // Make the game always give GFS from a randomized item
    Write_To_Rom(15923616, "24020000"); // always give song of soaring
    Write_To_Rom(12509920, "24020000"); // always give Epona's Song

    // elder goron ignores if you have lullaby and intro
    Write_To_Rom(16622184, "24030000");
    Write_To_Rom(16619604, "24030000");

    // Day 3 Gormon Brothers always give you Garo Mask
    Write_To_Rom(14282268, "2419FFFF");
    Write_To_Rom(14087392, "240DFFFF");

    Write_To_Rom(14536768, "00000000"); // fix dimming transformation masks during majora
                                        // fight when receiving what FD gives
    Write_To_Rom(17167004, "240F0043"); // make healing kamaro give the notebook ribbon
    Write_To_Rom(13845660, "10000003"); // always play oath cs when picking up remains
    Write_To_Rom(13838032, "24040000"); // always spawn odolwa's remains
    Write_To_Rom(13838084, "24040000"); // always spawn goht's remains
    Write_To_Rom(13838124, "24040000"); // always spawn gyorh's remains
    Write_To_Rom(13838164, "24040000"); // always spawn twinmold's remains
    Write_To_Rom(15349096, "240F00FF"); // make gf always give gfm item after already
                                        // getting magic (and have deku mask of course)
    Write_To_Rom(13492260, "10000003");	// Remove check when buying Hero's Shield

}

void Print_Map(ostream &out, map<string, map<string, string>> data)
{
vector<string> keys;
vector<string> keys2;

for (const auto &kv : data)
{
	keys.push_back(kv.first);
}

for (int i = 0; i < keys.size(); i++)
{
	cout << keys[i] << endl;

	for (const auto &kv : data[keys[i]])
	{
		keys2.push_back(kv.first);
	}

	for (int ii = 0; ii < keys2.size(); ii++)
	{
		cout << "\t" << keys2[ii] << endl;
		cout << "\t\t" << data[keys[i]][keys2[ii]] << endl;
	}
}
}

void Update_Pools(map<string, Item> &Items)
{
	vector<string> keys;
	vector<string> values;

	for (const auto &kv : Settings["items"])
	{
		keys.push_back(kv.first);
		values.push_back(kv.second);
	}

	for (int i = 0; i < keys.size(); i++)
	{
		Items[keys[i]].Pool = values[i];
	}
}

vector<vector<string>> Get_Items_Needed(ifstream &Logic_File,
	map<string, vector<string>> *Invalid_Items,
	string Item_Name)
{
	int Set_Index = -1;
	int Line_Index = -1;
	string line = "";
	string item = "";
	string Day_String;
	vector<vector<string>> Items_Needed;
	vector<string> List;
	vector<string> words;
	vector<string> Cur_Invalid_Items;
	vector<string> Day_Array;
	vector<map<string, vector<bool>>> Set_Day_Data;
	vector<bool> Day_Night;
	bool comment = false;

	while (!Contains(line, '}'))
	{
		getline(Logic_File, line);
		Line_Index++;
		comment = false;

		// clear the list
		List.clear();
		Cur_Invalid_Items.clear();

		// remove comments on line if any
		if (IndexOf_S(line, "//") != -1)
		{
			line = line.substr(0, IndexOf_S(line, "//"));
			comment = true;
		}

		//remove tabs to read the line easier
		line = RemoveAll(line, '\t');

		// list of invalid items to place here
		if (line[0] == '#')
		{
			line = line.substr(1); // remove #

			if (Contains(line, ','))
			{
				Cur_Invalid_Items = Split(line, ", ");
			}
			else if (line != "")
			{
				Cur_Invalid_Items.push_back(line);
			}
		}
		//day data for the previous item set
		else if (line[0] == '@') {
			Day_String = line.substr(1);	//remove @ from the line
			Day_Array = Split(Day_String, " ");

			//there were no needed items for this location
			if (Set_Index == -1) {
				Set_Index = 0;
			}

			//index 0 is for when the item is given, the rest is each item in the set
			for (int d = 0; d < Day_Array.size(); d++) {
				Day_String = Day_Array[d];
				//only continue if this is binary
				if (isBinary(Day_String)) {
					Day_Night = Binary_Str_To_Vec(Day_String);
					
					//make sure the binary string is long enough to get each day data
					if (Day_Night.size() >= 3) {
						Day_Data[Item_Name][Set_Index][d]["day"] = Sub_Vec(Day_Night, 0, 3);	//gets the day data
						if (Day_Night.size() >= 6) {
							Day_Data[Item_Name][Set_Index][d]["night"] = Sub_Vec(Day_Night, 3, 3);	//gets the night data
							if (Day_Night.size() == 7) {
								Day_Data[Item_Name][Set_Index][d]["moon"] = Sub_Vec(Day_Night, 6);	//gets the moon data
							}
						}
					}
				}
			}
		}
		// an item set
		else if (!comment)
		{
			Set_Index++;

			if (Contains(line, ','))
			{
				List = Split(line, ", ");
			}
			else if (line != "")
			{
				List.push_back(line);
			}
			//the location doesnt need any items
			else {
				Day_Data[Item_Name].push_back({
{
	{"day", { true, true, true }},
	{"night", { true, true, true }},
					{"moon", {false}}
},
{
	{"day", { true, true, true }},
	{"night", { true, true, true }},
					{"moon", {false}}
}
					});
			}
		}

		if (List.size() > 0)
		{
			//if this is not the closing bracket
			if (!Contains(List[0], '}'))
			{
				//if this set has items
				if (List[0] != "")
				{
					//add the item set
					Items_Needed.push_back(List);

					//make the default time for when the item at the location is given for the current item set
					while (Set_Index >= Day_Data[Item_Name].size()) {
						//set the default day data for when the item is given
						Day_Data[Item_Name].push_back({
		{
			{"day", { true, true, true }},
			{"night", { true, true, true }},
					{"moon", {false}}
		}
							});
					}


					//setup default day data for the items in the set
					for (int i = 0; i < List.size(); i++) {
						Day_Data[Item_Name][Set_Index].push_back({ { "day", { true, true, true } }, { "night", { true, true, true } }, {"moon", { false } } });
					}
				}
        }
		}

        if (Cur_Invalid_Items.size() > 0)
        {
            (*Invalid_Items)[Item_Name] = Cur_Invalid_Items;
        }
    }

    return Items_Needed;
}

void Print_Logic(map<string, vector<vector<string>>> Logic)
{
    vector<string> keys;

    for (const auto &kv : Logic)
    {
        keys.push_back(kv.first);
    }

    for (int i = 0; i < keys.size(); i++)
    {
        cout << keys[i] << endl;
        for (int a = 0; a < Logic[keys[i]].size(); a++)
        {
            for (int b = 0; b < Logic[keys[i]][a].size(); b++)
            {
                if (b == 0)
                {
                    cout << "\t";
                }
                else
                {
                    cout << ", ";
                }

                cout << Logic[keys[i]][a][b];
            }
            cout << endl;
        }
        cout << endl;
    }
}

map<string, vector<vector<string>>> Get_Logic(string Logic_Location,
                                              map<string, vector<string>> *Invalid_Items)
{
    ifstream Logic_File;
    string line = "";
    map<string, vector<vector<string>>> Logic;
    string Item = "";

    Logic_Location = "./logic/" + Logic_Location + ".txt";

    // Logic_File.open("./logic/Glitched.txt");
    Logic_File.open(Logic_Location.c_str());

    if (!Logic_File)
    {
        Error("Could not find logic file: \"" + Logic_Location + "\"");
    }

    //getline(Logic_File, line);

    while (getline(Logic_File, line))
    {
        // remove comments on line if any
        if (IndexOf_S(line, "//") != -1)
        {
            line = line.substr(0, IndexOf_S(line, "//"));
        }

        if (line != "")
        {
            if (Contains(line, '{'))
            {
                Item = line.substr(0, IndexOf(line, '{') - 1);
				Logic[Item] = Get_Items_Needed(Logic_File, Invalid_Items, Item);
            }
        }
    }

    Logic_File.close();

    return Logic;
}

bool Locked_Trade_Item(map<string, Item> Items)
{
    vector<string> Deeds = {"Moon's Tear",
                            "Land Title Deed",
                            "Swamp Title Deed",
                            "Mountain Title Deed",
                            "Ocean Title Deed"};
    vector<string> Key = {"Room Key", "Letter to Kafei"};
    vector<string> Pendant = {"Pendant of Memories", "Express Letter to Mama"};

    if (IndexOf(Deeds, Items["Keaton Mask"].Name) != -1 &&
        IndexOf(Deeds, Items["Express Letter to Mama"].Name) != -1)
    {
        return true;
    }
    else if (IndexOf(Key, Items["Keaton Mask"].Name) != -1 &&
             IndexOf(Key, Items["Express Letter to Mama"].Name) != -1)
    {
        return true;
    }
    else if (IndexOf(Pendant, Items["Keaton Mask"].Name) != -1 &&
             IndexOf(Pendant, Items["Express Letter to Mama"].Name) != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// get the key for the first highest value in a map (positive numbers only)
string Get_Key_Max(map<string, int> data, int index)
{
    string key = "";
    int highest = -1;
    vector<string> keys;

    keys = Get_Keys(data);

    for (int i = 0; i < keys.size(); i++)
    {
        if (data[keys[i]] > highest)
        {
            highest = data[keys[i]];
            key = keys[i];
        }
    }

    if (index > 0)
    {
        data.erase(key); // remove the 1st one to get to the next one until it reaches the
                         // wanted index
        key = Get_Key_Max(data, --index);
    }

    return key;
}

string Get_Pool(string item)
{
    return Items[item].Pool;
}

///Returns a vector of strings of all the pool names
vector<string> Get_All_Pools(map<string, Item> Items)
{
    vector<string> Pools;
    vector<string> Item_Names;
    string pool;
    string item;

    Item_Names = Get_Keys(Items);

    for (int i = 0; i < Item_Names.size(); i++)
    {
        item = Item_Names[i];
        pool = Items[item].Pool;

        // if haven't added this pool yet
        if (IndexOf(Pools, pool) == -1)
        {
            // if is a pool, doesnt give an item and is randomized
            if (pool[0] != '#' && pool != "")
            {
                Pools.push_back(pool);
            }
        }
    }

    return Pools;
}

/// Gets the wallet size (nothing, adult, or giant) needed to have the 'rupees_needed'
/// amount of rupees
string Get_Wallet_Needed(int rupees_needed, map<string, string> wallet_sizes)
{
    vector<string> wallet_keys = {"Small", "Medium", "Large"};
    vector<string> wallets = {"", "Adult Wallet", "Giant Wallet"};

    for (int w = 0; w < wallets.size(); w++)
    {
        string key = wallet_keys[w];
        string Size = wallets[w];
        string rupees_str = wallet_sizes[key];
        int max_rupees = string_to_dec(rupees_str);

        // this is the wallet that is needed
        if (max_rupees >= rupees_needed)
        {
            return Size;
        }
    }

    // the player cannot buy the item ever
    return "Error";
}

//looks at each string in "sources" and determines if that source gives an item, and returns a vector with only those that do (whether or not an item has been placed here)
vector<string> Remove_Not_Giving(vector<string> sources) {
	vector<string> removed;
	string location;

	for (int s = 0; s < sources.size(); s++) {
		location = sources[s];

		//add it to the list if this location gives an item
		if (Items[location].gives_item) {
			removed.push_back(location);
		}
	}

	return removed;
}

//remove the item from the list if the item's flag isnt checked for being obtainable
vector<string> Remove_Not_Obtainable(vector<string> sources) {
	vector<string> locations;
	string location;

	for (int s = 0; s < sources.size(); s++) {
		location = sources[s];

		//add the location to the list if the player got an item from there
		if (Items[location].Obtainable) {
			locations.push_back(location);
		}
	}

	return locations;
}

//gets the total count from each location in "locations" per cycle, 0 Means infinite
int Get_Total_Count(vector<string> locations) {
	int total = 0;
	int count;
	string location;

	for (int l = 0; l < locations.size(); l++) {
		location = locations[l];
		count = Items[location].Count;

		//add count of this item to the total
		if (count > 0) {
			total += count;
		}
		//infinite source, so make total 0 (for infinite) and leave loop
		else {
			total = 0;
			l = locations.size();
		}
	}

	return total;
}

map<string, vector<vector<bool>>> Setup_Items_Setup(vector<string> items, map<string, vector<vector<string>>> Items_Needed) {
	map<string, vector<vector<bool>>> Items_Setup;
	string location;

	//cout << "items.size(): " << items.size() << endl;

	//for each item location
	for (int l = 0; l < items.size(); l++) {
		location = items[l];
		
		//for each item set
		for (int s = 0; s < Items_Needed[location].size(); s++) {
			//create the first (or the none item)
			Items_Setup[location].push_back({ false });

			//for each item in the set
			for (int i = 1; i < Items_Needed[location][s].size(); i++) {
				Items_Setup[location][s].push_back(false);	//set each item to false by default
			}
		}
	}

	//cout << "Items_Setup.size(): " << Items_Setup.size() << endl;

	return Items_Setup;
}

bool All_True(vector<bool> booleans) {
	for (int b = 0; b < booleans.size(); b++) {
		//found a false
		if (!booleans[b]) {
			return false;
		}
	}

	//all are true
	return true;
}

bool All_False(vector<bool> booleans) {
	for (int b = 0; b < booleans.size(); b++) {
		//found a false
		if (booleans[b]) {
			return false;
		}
	}

	//all are true
	return true;
}

//whether or not this location gives any items in the given vector
bool Location_Gives_Items(string location, vector<string> items) {
	bool gives = true;



	return gives;
}

map<string, int> Set_Counts(vector<string> items, int count) {
	map<string, int> counts;
	string item;

	for (int i = 0; i < items.size(); i++) {
		item = items[i];
		counts[item] = count;
	}

	return counts;
}

bool Resettable_Item(string item) {
	vector<string> Reset_Sot = {	//items that go away on song of time
	"Moon's Tear",
	"Land Title Deed",
	"Swamp Title Deed",
	"Mountain Title Deed",
	"Ocean Title Deed",
	"Room Key",
	"Pendant of Memories",
	"Letter to Kafei",
	"Express Letter to Mama",
	"Red Potion",
	"Blue Potion",
	"Green Potion",
	"Bugs",
	"Fish",
	"Milk",
	"Poe",
	"Big Poe",
	"Spring Water",
	"Hot Spring Water",
	"Fairy",
	"Deku Princess",
	"Zora Egg",
	"Gold Dust",
	"Mushroom",
	"Seahorse",
	"Chateau Romani"
	};
	//items that matter with an item count
	vector<string> Countable_Items = {
		"Bombchus",
		"Bombchus (5)",
		"Bombchus (10)",
		"Deku Nuts",
		"Deku Nuts (10)",
		"Deku Sticks",
		"Magic Beans",
		"Powder Keg"
	};

	//return true if the item is an item that goes away on SoT or an item that has counts
	if (IndexOf(Reset_Sot, item) != -1 || IndexOf(Countable_Items, item) != -1) {
		return true;
	}
	//return false if a generic item like All-Night Mask
	else {
		return false;
	}
}

int Get_Count(string location, string Item_Given) {
	int count = 0;
	int mult = 1;
	int index = Item_Given.find('(');

	//if this item has (X) (like deku nuts (10))
	if (index != -1) {
		mult = string_to_dec(Item_Given.substr(Item_Given.find('(') + 1, Item_Given.find(')') - Item_Given.find('(') - 1));
	}

	count = Items[location].Count;

	return count * mult;
}

void Give_Item(string location, map<string, int> &Cycle_Items, map<string, int> &Item_Counts, vector<string> &Items_Aval, vector<string> &Remove_Locations, map<string, int> &Items_Gotten, map<string, bool> &Resettable_Items, bool &Got_Item, bool &Got_Cycle_Item, map<string, bool> &Locations_Checked) {
	string Item_Given = Items[location].Name;
	int count;
	int index;
	int mult;
	string Shorter_Item;

	Logger("Give_Item(" + location + ")");

	//add the item to the list of locations available if it's not in the list
	if (IndexOf(Items_Aval, location) == -1) {
		Items_Aval.push_back(location);
		Logger("Adding " + location + " to the list of items available");
	}

	//add the location to the list of locations that are now able to be obtainable
	Remove_Locations.push_back(location);

	//if the location gives an item (an item has been randomized here or manually placed here)
	if (Items[location].gives_item) {
		Logger("Giving Item at " + location + " (" + Item_Given + ")");

		//gotten what this location gives this cycle
		Locations_Checked[location] = true;

		//if the item gotten from this location is not already obtained in cycle items
		if (Cycle_Items[Item_Given] == -1) {
			//add the item to the list of items gotten if it's not already in it
			//-1 means it's not in the list, 0 means the item can be gotten infinitely per cycle
			if (Items_Gotten[Item_Given] == -1) {
				Items_Gotten[Item_Given] = Get_Count(location, Item_Given);
				Got_Item = true;	//got a new item which might open new paths
				Got_Cycle_Item = true;
			}

			//get item in this cycle
			Cycle_Items[Item_Given] = Get_Count(location, Item_Given);

			//add stuff like deku nuts (10) to deku nuts
			index = Item_Given.find('(');
			if (index != -1) {
				Shorter_Item = Item_Given.substr(0, index - 1);	//gets Deku Nuts from Deku Nuts (10)
				mult = string_to_dec(Item_Given.substr(index + 1, Item_Given.find(')') - index - 1));	//get 10 from Deku Nuts (10)
				count = Items[location].Count;
				
				//the player can get infinite this cycle
				if (count == 0) {
					Cycle_Items[Shorter_Item] = 0;
				}
				//if the player hasn't gotten deku nuts
				else if (Cycle_Items[Shorter_Item] == -1) {
					//set it to whatever this gives per cycle times 10
					Cycle_Items[Shorter_Item] = count * mult;
				}
				//if the player already has a count of this item (neither of them gives infnite)
				else if (Cycle_Items[Shorter_Item] > 0) {
					//add count * mult to whatever the count of the item already is
					Cycle_Items[Shorter_Item] += count * mult;
				}
			}
		}
	}
}

///returns a vector of strings of all the items that go away or go to zero on SoT
vector<string> Get_Resettable_Items() {
	vector<string> Reset_Items = {
			"Moon's Tear",
			"Land Title Deed",
			"Swamp Title Deed",
			"Mountain Title Deed",
			"Ocean Title Deed",
			"Room Key",
			"Pendant of Memories",
			"Letter to Kafei",
			"Express Letter to Mama",
			"Red Potion",
			"Blue Potion",
			"Green Potion",
			"Bugs",
			"Fish",
			"Milk",
			"Poe",
			"Big Poe",
			"Spring Water",
			"Hot Spring Water",
			"Fairy",
			"Deku Princess",
			"Zora Egg",
			"Gold Dust",
			"Mushroom",
			"Seahorse",
			"Chateau Romani",
			"Bombchus",
			"Bombchus (5)",
			"Bombchus (10)",
			"Deku Nuts",
			"Deku Nuts (10)",
			"Deku Stick",
			"Magic Beans",
			"Powder Keg"
	};

	return Reset_Items;
}

bool Player_Has_Item(map<string, int> &Items_Gotten, map<string, int> &Cycle_Items, map<string, int> &Item_Counts, string &Item_Needed, int &Count_Needed) {
	int Wallet_Size;
	int count;

	//Logger("Player_Has_Item()");

	//if the player needs a certain rupee count
	if (Item_Needed == "Rupees") {
		//Logger("Player needs rupees");
		Wallet_Size = Get_Wallet_Max(Items_Gotten);
		//if the wallet can hold enough rupees
		if (Wallet_Size >= Count_Needed) {
			//Logger("Player has enough rupees");
			return true;
		}
	}
	//does the player have this item?
	else if (Cycle_Items[Item_Needed] != -1) {
		count = Cycle_Items[Item_Needed];	//gets the number of times the needed item can be gotten per cycle
		//Logger("Count: " + to_string(count) + " / " + to_string(Count_Needed));

		//if the player has the count needed, or if the item can be gotten infinitely
		if (count >= Count_Needed || count == 0) {
			//Logger("Player has " + to_string(Count_Needed) + " " + Item_Needed);
			return true;
		}
		else {
			//Logger("Player does not have " + to_string(Count_Needed) + " " + Item_Needed);
		}
	}
	else {
		//Logger("Player does not have " + Item_Needed);
	}

	//Logger("Returning from Player_Has_Item()");

	return false;
}

///returns a copy of Items_Gotten where each item that resests on song of time is set back to -1
void Reset_SoT_Items(map<string, int> &Cycle_Items) {
	vector<string> Reset_SoT = Get_Resettable_Items();

	for (int i = 0; i < Reset_SoT.size(); i++) {
		Cycle_Items[Reset_SoT[i]] = -1;
	}
}

//Returns a vector of strings of the items that are left in a given pool
vector<string> Get_Items_Left_Pool(string pool)
{
	vector<string> Items_Pool;
	vector<string> items;

	items = Get_Keys(Items);

	for (int i = 0; i < items.size(); i++)
	{
		string item = items[i];

		if (Items[item].Pool == pool)
		{
			if (!Items[item].can_get)
			{
				Items_Pool.push_back(item);
			}
		}
	}

	return Items_Pool;
}

//Returns a vector of strings of the items that the player can get with the current equipment according to the logic
void Get_Items_Aval(map<string, Item> &Items,
                              map<string, vector<vector<string>>> Items_Needed,
                              map<string, int> &Items_Gotten,
                              map<string, string> wallets,
							map<string, int> &Locations_Item_Set, //which item set for each location that the logic thinks the player can use to reach the location
	vector<string> &Locations,	//locations left that haven't been obtainable
	vector<string> &Locations_Reset,	//locations that give items that reset on song of time (bow if bow gives a deed, for example)
	vector<string> &Items_Aval,	//locations that are available
	map<string, int> &Item_Counts,
	map<string, bool> &Resettable_Items,
	map<string, int> &Cycle_Items)
{
	vector<string> Locations_Ready = {};	//vector of locations where they are ready to give their items when the day arrives
	vector<int> Locations_Ready_Set = {};	//vector of which item set the logic thinks the player can use to get the item
	vector<string> Cur_List;
	//map<string, int> Cycle_Items = Copy(Items_Gotten);
	int day;
	int count;
	int Count_Needed;
	int Last_Runthrough;
	int Item_Index;
	int Set_Index;
	int Last_Cycle;
	int Location_Set;
	int Wallet_Size;
	string time;
	string Item_Needed;
	string location;
	string Item_Given;
	string Item_Location;
	string Location_Ready;
	vector<string> Item_Vec;
	map<string,	//location
		vector	//item sets
			<vector	//item index
				<bool>>> Items_Setup;
	map<string,	//location
		map<string,	//day or night
			vector<	// day 1, 2, or 3 (0, 1, or 2 index)
				bool>>> Days_Given;	//days an obtainable item is given
	//map<string, int> Item_Counts;	//the count of each item
	vector<string> Remove_Locations;	//locations that the logic determines is obtainable, so will remove it from the list of locations to check in the future
	bool Got_Item;
	bool Got_Cycle_Item;
	map<string, bool> Locations_Checked;

	int cycle = 1;

	//for each cycle
	do {
		Last_Cycle = Items_Aval.size();
		Got_Cycle_Item = false;

		Logger("Cycle " + to_string(cycle));

		//clear the previous cycle's setups for the items that were ready to get that could never be gotten
		Locations_Ready.clear();
		Locations_Ready_Set.clear();

		//reset the items that reset on song of time
		Reset_SoT_Items(Cycle_Items);

		//clear the flags for each cycle of setting up to get items
		Items_Setup = Setup_Items_Setup(Item_Keys, Items_Needed);

		Locations_Checked = Set_Map(Item_Keys, false);
	
		//for each day/night in this cycle
		for (int t = 0; t < 6; t++) {
			day = t / 2;	//0, 1, or 2

			if (t % 2 == 0) {
				time = "day";
			}
			else {
				time = "night";
			}

			Logger("Dawn of " + time + " " + to_string(day + 1));

			//if there are items ready to be given
			if (Locations_Ready.size() > 0) {
				for (int r = 0; r < Locations_Ready.size(); r++) {
					Location_Ready = Locations_Ready[r];
					Location_Set = Locations_Ready_Set[r];

					//if the item can be gotten today, then give it to the player
					if (Days_Given[Location_Ready][time][day]) {
						//get the item/add it to the list of items available
						Give_Item(Location_Ready, Cycle_Items, Item_Counts, Items_Aval, Remove_Locations, Items_Gotten, Resettable_Items, Got_Item, Got_Cycle_Item, Locations_Checked);

						//Get the item set index the logic used to determine the item was obtainable
						Locations_Item_Set[Location_Ready] = Location_Set;

						Locations_Ready = Remove_Index(Locations_Ready, r);
						Locations_Ready_Set = Remove_Index(Locations_Ready_Set, r);
						r--;	//because the total size is now one less
					}
				}
			}

			do {
				Got_Item = false;
				Last_Runthrough = Cycle_Items.size();

				//for each resettable item location
				for (int l = 0; l < Locations_Reset.size(); l++) {
					location = Locations_Reset[l];

					Logger("Checking " + location);

					//if already got the item in this cycle, then there is no need to check again, skip to the next one
					if (Locations_Checked[location]) {
						Logger(location + " already checked");
						continue;
					}

					//if this location needs no items, then check to see what day/night can be used to setup getting the item
					if (Items_Needed[location].size() == 0 || Items_Needed[location][0].size() == 0) {
						Logger(location + " doesn't need any items");
						//if "nothing" can be used on this day
						if (Day_Data[location].size() == 0 || Day_Data[location][0][1][time][day]) {
							Set_Index = 0;
							//if the item at the location can be gotten on the same day
							if (Day_Data[location].size() == 0 || Day_Data[location][0][0][time][day]) {
								//get the item/add it to the list of items available
								Give_Item(location, Cycle_Items, Item_Counts, Items_Aval, Remove_Locations, Items_Gotten, Resettable_Items, Got_Item, Got_Cycle_Item, Locations_Checked);

								//Get the item set index the logic used to determine the item was obtainable
								Locations_Item_Set[location] = Set_Index;
							}
							//store the location for future days
							else {
								Locations_Ready.push_back(location);
								Locations_Ready_Set.push_back(Set_Index);
							}
						}
					}

					//for each item set in the logic for this item
					for (int s = 0; s < Items_Needed[location].size(); s++) {
						Cur_List = Items_Needed[location][s];

						//for each item in this set
						for (int i = 0; i < Cur_List.size(); i++) {
							Item_Index = i;
							Item_Needed = Cur_List[Item_Index];

							Item_Vec = Split(Item_Needed, " ");

							//if the first word is a number, then the player needs this amount per cycle
							if (isNumber(Item_Vec[0])) {
								Count_Needed = string_to_dec(Item_Vec[0]);
								Item_Needed = Item_Needed.substr(Item_Needed.find(' ') + 1);	//removes the count from the beginning to just get the item name
							}
							//the player only needs 1 per cycle, or just to have the item
							else {
								Count_Needed = 1;
							}

							//does the player have the item and the count needed?
							if (Player_Has_Item(Items_Gotten, Cycle_Items, Item_Counts, Item_Needed, Count_Needed)) {
								//can the item that is needed be used on this day?
								//+1 because the day day index 0 is for when the item at the location is given, 1 is the actual first item in the list
								if (Day_Data[location][s][Item_Index + 1][time][day]) {
									Items_Setup[location][s][Item_Index] = true;	//"use" the item
								}
							}
						}

						//check if all the items have been used for this item set
						if (All_True(Items_Setup[location][s])) {
							Set_Index = s;

							Days_Given[location] = Day_Data[location][Set_Index][0];	//get the day data for when this location gives it's item

							//if this item has already been determined that it was obtainble before, then do not change the item set
							if (IndexOf(Get_Keys(Locations_Item_Set), location) == -1) {
								Set_Index = Locations_Item_Set[location];
							}

							//if the day data includes this day
							if (Days_Given[location][time][day]) {
								//get the item/add it to the list of items available
								Give_Item(location, Cycle_Items, Item_Counts, Items_Aval, Remove_Locations, Items_Gotten, Resettable_Items, Got_Item, Got_Cycle_Item, Locations_Checked);

								//Get the item set index the logic used to determine the item was obtainable
								Locations_Item_Set[location] = Set_Index;

								//skip to the next location
								s = Day_Data[location].size();
							}
							//the item at the location cannot be gotten today, need to check back on a future day
							else {
								Locations_Ready.push_back(location);
								Locations_Ready_Set.push_back(Set_Index);
							}
						}
					}
				}

				//for each item location that the logic hasn't determined the player can get or an item that gives a resettable item (land deed, deku sticks, etc.)
				for (int l = 0; l < Locations.size(); l++) {
					location = Locations[l];

					Logger("Checking " + location);

					//Logger("Can the player get " + location + "?");

					//if this location needs no items, then check to see what day/night can be used to setup getting the item
					if (Items_Needed[location].size() == 0 || Items_Needed[location][0].size() == 0) {
						Logger(location + " doesn't need any items");
						//if "nothing" can be used on this day
						if (Day_Data[location].size() == 0 || Day_Data[location][0][1][time][day]) {
							Set_Index = 0;
							//if the item at the location can be gotten on the same day
							if (Day_Data[location].size() == 0 || Day_Data[location][0][0][time][day]) {
								//get the item/add it to the list of items available
								Give_Item(location, Cycle_Items, Item_Counts, Items_Aval, Remove_Locations, Items_Gotten, Resettable_Items, Got_Item, Got_Cycle_Item, Locations_Checked);

								//Get the item set index the logic used to determine the item was obtainable
								Locations_Item_Set[location] = Set_Index;
							}
							//store the location for future days
							else {
								Locations_Ready.push_back(location);
								Locations_Ready_Set.push_back(Set_Index);
							}
						}
					}

					//for each item set in the logic for this item
					for (int s = 0; s < Items_Needed[location].size(); s++) {
						Cur_List = Items_Needed[location][s];

						//for each item in this set
						for (int i = 0; i < Cur_List.size(); i++) {
							Item_Index = i;
							Item_Needed = Cur_List[Item_Index];

							Item_Vec = Split(Item_Needed, " ");

							//if the first word is a number, then the player needs this amount per cycle
							if (isNumber(Item_Vec[0])) {
								Count_Needed = string_to_dec(Item_Vec[0]);
								Item_Needed = Item_Needed.substr(Item_Needed.find(' ') + 1);	//removes the count from the beginning to just get the item name
							}
							//the player only needs 1 per cycle, or just to have the item
							else {
								Count_Needed = 1;
							}

							//does the player have the item and the count needed?
							if (Player_Has_Item(Items_Gotten, Cycle_Items, Item_Counts, Item_Needed, Count_Needed)) {
								//can the item that is needed be used on this day?
								//+1 because the day day index 0 is for when the item at the location is given, 1 is the actual first item in the list
								if (Day_Data[location][s][Item_Index + 1][time][day]) {
									Items_Setup[location][s][Item_Index] = true;	//"use" the item
								}
							}
						}

						//check if all the items have been used for this item set
						if (All_True(Items_Setup[location][s])) {
							Set_Index = s;

							Days_Given[location] = Day_Data[location][Set_Index][0];	//get the day data for when this location gives it's item
							
							//if this item has already been determined that it was obtainble before, then do not change the item set
							if (IndexOf(Get_Keys(Locations_Item_Set), location) == -1) {
								Set_Index = Locations_Item_Set[location];
							}

							//if the day data includes this day
							if (Days_Given[location][time][day]) {
								//get the item/add it to the list of items available
								Give_Item(location, Cycle_Items, Item_Counts, Items_Aval, Remove_Locations, Items_Gotten, Resettable_Items, Got_Item, Got_Cycle_Item, Locations_Checked);

								//Get the item set index the logic used to determine the item was obtainable
								Locations_Item_Set[location] = Set_Index;

								//skip to the next location
								s = Day_Data[location].size();
							}
							//the item at the location cannot be gotten today, need to check back on a future day
							else {
								Locations_Ready.push_back(location);
								Locations_Ready_Set.push_back(Set_Index);
							}
						}
					}
				}

			} while (Got_Item);	//repeat checking if got new items in the last runthrough, something might have unlocked that is on the same day

			//remove the obtainable items from the list of locations so we don't have to check them again
			//this also makes it where the logic doesnt have to check the same items again on a different day if it already knows those items are good
			Locations = Remove_Values(Locations, Remove_Locations);
			Remove_Locations.clear();
		}

		//check items that can only be used on the moon
		for (int l = 0; l < Locations.size(); l++) {
			location = Locations[l];
			for (int s = 0; s < Items_Needed[location].size(); s++) {
				Cur_List = Items_Needed[location][s];
				for (int i = 0; i < Cur_List.size(); i++) {
					Item_Index = i;
					Item_Needed = Cur_List[Item_Index];

					Item_Vec = Split(Item_Needed, " ");

					//if the first word is a number, then the player needs this amount per cycle
					if (isNumber(Item_Vec[0])) {
						Count_Needed = string_to_dec(Item_Vec[0]);
						Item_Needed = Item_Needed.substr(Item_Needed.find(' ') + 1);	//removes the count from the beginning to just get the item name
					}
					//the player only needs 1 per cycle, or just to have the item
					else {
						Count_Needed = 1;
					}

					//does the player have the item and the count needed?
					if (Player_Has_Item(Items_Gotten, Cycle_Items, Item_Counts, Item_Needed, Count_Needed)) {
						//can the item that is needed be used on this day?
						//+1 because the day day index 0 is for when the item at the location is given, 1 is the actual first item in the list
						if (Day_Data[location][s][Item_Index + 1]["moon"][0]) {
							Items_Setup[location][s][Item_Index] = true;	//"use" the item
						}
					}
				}

				//check if all the items have been used for this item set
				if (All_True(Items_Setup[location][s])) {
					Set_Index = s;

					if (Day_Data[location].size() == 0) {
						Days_Given[location] = {};
					}
					else {
						Days_Given[location] = Day_Data[location][Set_Index][0];	//get the day data for when this location gives it's item
					}

					//if this item has already been determined that it was obtainble before, then do not change the item set
					if (IndexOf(Get_Keys(Locations_Item_Set), location) == -1) {
						Set_Index = Locations_Item_Set[location];
					}

					//if the day data includes the moon
					if (Day_Data[location].size() == 0 || Days_Given[location]["moon"][0]) {
						//get the item/add it to the list of items available
						Give_Item(location, Cycle_Items, Item_Counts, Items_Aval, Remove_Locations, Items_Gotten, Resettable_Items, Got_Item, Got_Cycle_Item, Locations_Checked);

						//Get the item set index the logic used to determine the item was obtainable
						Locations_Item_Set[location] = Set_Index;

						//skip to the next location
						s = Day_Data[location].size();
					}
					//the item at the location cannot be gotten today - item can never be gotten so far (another item set might make it obtainable)
					else {

					}
				}
			}
		}

		//remove the obtainable items from the list of locations so we don't have to check them again
		//this also makes it where the logic doesnt have to check the same items again on a different day if it already knows those items are good
		Locations = Remove_Values(Locations, Remove_Locations);
		Remove_Locations.clear();

		cycle++;
	}
	while (Got_Cycle_Item);	//repeat because could have gotten a deed that lead to an item or gotten an new item later on that leads to a new location earlier in the cycle

	Logger("Got Items Available - total count available: " + to_string(Items_Aval.size()));

	Logger("Total Items Gotten: " + to_string(Items_Gotten.size()));

	Logger("Locations size: " + to_string(Locations.size()));

	Logger("-Locations Left-");
	Log_Vector(Locations);
	Logger("---------------");

	Logger("-Items left in Pool Everything-");
	Log_Vector(Get_Items_Left_Pool("Everything"));
	Logger("------------------------------");
}

//Returns a vector of strings of items the player has that was needed for an item
vector<string> Get_First_Items_List(string item,
                                    map<string, vector<vector<string>>> Items_Needed,
                                    vector<string> &Items_Gotten)
{
    vector<string> items;
    vector<string> empty_vector;
    bool yes = true;

    for (int i = 0; i < Items_Needed[item].size(); i++)
    {
        bool yes = true;
        items.clear();
        for (int j = 0; j < Items_Needed[item][i].size(); j++)
        {
            string Item_Needed = Items_Needed[item][i][j];

            items.push_back(Item_Needed);

	    //if have not obtained this item
            if (IndexOf(Items_Gotten, Item_Needed) == -1)
            {
                yes = false;
            }
        }

        if (yes)
        {
            return items;
        }
    }

    return empty_vector;
}

vector<string> Sort_Value(vector<string> Items_Pool)
{
    vector<string> Sorted_List;
    map<string, int> Item_Values;
    string item;
    int value;

	//return an empty list if the vector given was empty
	if (Items_Pool.size() == 0) {
		return Sorted_List;
	}

    for (int i = 0; i < Items_Pool.size(); i++)
    {
        item = Items_Pool[i];
        value = Items[item].value;

        Item_Values[item] = value;
    }

    while (Items_Pool.size() > 0)
    {
        int highest = -1;
        int Highest_Index = -1;
        string Highest_Item = "";

        for (int ip = 0; ip < Items_Pool.size(); ip++)
        {
            item = Items_Pool[ip];

            if (Item_Values[item] > highest)
            {
                Highest_Item = item;
                highest = Item_Values[item];
                Highest_Index = ip;
            }
        }

        Sorted_List.push_back(Highest_Item); // store the current highest value

		//only if there is a highest
		if (Highest_Index != -1) {
			Items_Pool.erase(Items_Pool.begin() +
				Highest_Index); // remove the current highest value from pool
		}
		else {
			//remove the first element
			Items_Pool.erase(Items_Pool.begin());
		}
    }

    return Sorted_List;
}

///Returns a string of item locations not yet checked
string Get_Missing_Items(vector<string> &Items_Gotten)
{
    string miss_items = "\t";
    vector<string> items;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++)
    {
        if (IndexOf(Items_Gotten, items[i]) == -1)
        {
            if (miss_items == "\t")
            {
                miss_items += items[i];
            }
            else
            {
                miss_items += "\n\t" + items[i];
            }
        }
    }

    return miss_items;
}

///Returns a string of item locations that cannot be checked yet
string Get_Missing_Locations(map<string, vector<vector<string>>> &Items_Needed,
                             vector<string> &Items_Gotten)
{
    string miss_loc = "";
    vector<string> items;
    bool Can_Get = true;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++)
    {
        string item = items[i];
        Can_Get = true;

        vector<vector<string>> needs = Items_Needed[item];
        for (int n = 0; n < needs.size(); n++)
        {
            vector<string> Item_List = needs[n];

            for (int il = 0; il < Item_List.size(); il++)
            {
                string Item_Needed = Item_List[il];
                if (IndexOf(Items_Gotten, Item_Needed) == -1)
                {
                    Can_Get = false;
                    il = Item_List.size();
                }
            }

            if (Can_Get)
            {
                n = needs.size();
            }
        }

        if (!Can_Get)
        {
            miss_loc += "\n\t" + item;
        }
    }

    return miss_loc;
}

///Checks the item locations to prevent an item overwritting an item before the item can be used (like keaton mask and express letter to mama)
bool Check_Curiosity_Items(string Cur_Item)
{
	map<string, int> item_slot_numbers = {
		{"Moon's Tear", 1},
		{"Land Title Deed", 1},
		{"Swamp Title Deed", 1},
		{"Mountain Title Deed", 1},
		{"Ocean Title Deed", 1},
		{"Room Key", 2},
		{"Express Letter to Mama", 2},
		{"Letter to Kafei", 3},
		{"Pendant of Memories", 3}
	};

	vector<string> Item_Locations = {
		"Keaton Mask",
		"Express Letter to Mama",
		"Odolwa's Remains",
		"Goht's Remains",
		"Gyorg's Remains",
		"Twinmold's Remains",
		"Oath to Order"
	};

	map<string, vector<string>> Item_Neighbor = {
		{"Keaton Mask",
			{"Express Letter to Mama"}},
		{"Express Letter to Mama",
			{"Keaton Mask"}},
		{"Odolwa's Remains",
			{"Oath to Order"}},
		{"Goht's Remains",
			{"Oath to Order"}},
		{"Gyorg's Remains",
			{"Oath to Order"}},
		{"Twinmold's Remains",
			{"Oath to Order"}},
		{"Oath to Order",
			{"Odolwa's Remains",
			"Goht's Remains",
			"Gyorg's Remains",
			"Twinmold's Remains"}}
	};

	//if the item location is one of the items
	if (IndexOf(Item_Locations, Cur_Item) != -1) {
		vector<string> neighbors = Item_Neighbor[Cur_Item];

		for (int n = 0; n < neighbors.size(); n++) {
			string neighbor = neighbors[n];

			//if the neighbor gives an item, then we need to check to see if one will overwrite the other
			if (Items[neighbor].gives_item) {
				//if the items overwrite eachother, then one of them can never be used
				if (item_slot_numbers[Items[neighbor].Name] != NULL && item_slot_numbers[Items[neighbor].Name] == item_slot_numbers[Items[Cur_Item].Name]) {
					return false;
				}
			}
		}
	}

	return true;
}

vector<string> Sort_Points(vector<string> Items_To_Sort) {
	vector<string> Sorted_Items;
	string item;
	string sorted;
	bool Item_Sorted;

	for (int i = 0; i < Items_To_Sort.size(); i++) {
		item = Items_To_Sort[i];
		Item_Sorted = false;

		if (Sorted_Items.size() == 0) {
			Sorted_Items.push_back(item);
		}
		else {
			for (int s = 0; s < Sorted_Items.size(); s++) {
				sorted = Sorted_Items[s];

				if (Points[sorted] >= Points[item]) {
					Sorted_Items.insert(Sorted_Items.begin() + s, item);
					Item_Sorted = true;
					break; //found where to place the item
				}
			}

			//add the item to the end of the list because it's the one with the most points so far
			if (!Item_Sorted) {
				Sorted_Items.push_back(item);
			}
		}
	}

	return Sorted_Items;
}

void Print_Placed_Items(string sep) {
	string location;
	
	for (int l = 0; l < Item_Keys.size(); l++) {
		location = Item_Keys[l];
		if (Items[location].gives_item) {
			cout << location << " > " << Items[location].Name;

			if (l < Item_Keys.size() - 1) {
				cout << sep;
			}
		}
	}
}

///get a vector of the items that have the gives_item as false meaning these are locations that don't give an item yet
vector<string> Get_Items_Give_Nothing() {
	vector<string> nothing;
	string location;

	for (int l = 0; l < Item_Keys.size(); l++) {
		location = Item_Keys[l];

		if (!Items[location].gives_item) {
			nothing.push_back(location);
		}
	}

	return nothing;
}

map<string, bool> Reset_Item_Flags() {
	map<string, bool> Item_Resets;
	vector<string> Reset_Sot = Get_Resettable_Items();
	string item;

	for (int i = 0; i < Item_Keys.size(); i++) {
		item = Item_Keys[i];
		if (IndexOf(Reset_Sot, item) != -1) {
			Item_Resets[item] = true;	//resets or losses it on SoT
		}
		else {
			Item_Resets[item] = false;	//keeps it through SoT
		}
	}

	return Item_Resets;
}

vector<vector<string>> Setup_Starting_Items_Available() {
	vector<vector<string>> items;

	for (int i = 0; i < Item_Keys.size(); i++) {
		items.push_back({});
	}

	return items;
}

//Randomize the items
bool Randomize(string Log,
               map<string, Item> &Items,
               string &Seed,
               map<string, vector<vector<string>>> &Items_Needed,
               map<string, int> Items_Gotten,
               map<string, int> Cycle_Items,
               map<string, string> wallets, double long &seed,
               map<string, vector<string>> Invalid_Items,
		map<string, bool> &Resettable_Items,
	map<string, int> &Location_Sets,
			   vector<string> Items_Last = {},
	vector<string> Items_Aval = {}, // item locations that the player is able to get to currently according to logic
	vector<string> Locations = {},	//locations that the player cannot get to (or havent checked yet)
	vector<string> Locations_Reset = {},	//the locations that gives resettable items
	map<string, int> Item_Counts = {},
	int Placed_Items = 0)
{
    vector<string> Items_This;
    string Invalid_Item;
    bool Has_All = true;
    bool Placed_All = true;
	int index;
	int points;

	int Cur_Perc = Percentage(Item_Keys.size() - Locations.size(), Item_Keys.size());

	if (Items_Gotten.size() == 0) {
		Locations = Item_Keys;
		Item_Counts = Set_Counts(Item_Keys, 0);
		Resettable_Items = Reset_Item_Flags();
		Items_Gotten = Set_Counts(Item_Keys, -1);
		Cycle_Items = Copy(Items_Gotten);
		cout << "0%";
	}
	else if (Cur_Perc > Max_Percentage) {
		Max_Percentage = Cur_Perc;
		cout << "\r" << Cur_Perc << "%";
		
		if (Max_Percentage == 100) {
			cout << endl;
		}
	}

	//only set this to no item sets if it's the first call of this function
	if (Location_Sets.size() == 0) {
		Location_Sets = Set_Map(Item_Keys, -1);	//set the default item set index to -1 for each item
	}

    Get_Items_Aval(Items, Items_Needed, Items_Gotten, wallets, Location_Sets, Locations, Locations_Reset, Items_Aval, Item_Counts, Resettable_Items, Cycle_Items);
	
    // place an item at each available spot
    for (int ia = 0; ia < Items_Aval.size(); ia++)
    {
        string Cur_Item = Items_Aval[ia];
        int New_Item_Index;
        string New_Item;
		points = -1;

		//Logger("Checking item " + Cur_Item);

        // if this item doesn't give anything yet, then make it give an item
        if (!Items[Cur_Item].gives_item)
        {
			Logger(Cur_Item + " doesn't give an item");
            string pool = Items[Cur_Item].Pool;	//gets the item's pool
            vector<string> Items_Pool = Get_Items_Left_Pool(pool);	//get the items in the same pool that haven't been placed

            //random_shuffle(Items_Pool.begin(), Items_Pool.end());
			shuffle(Items_Pool.begin(), Items_Pool.end(), std::default_random_engine(seed));

			//get the items in the pool from lowest worth to highest
			//Items_Pool = Sort_Points(Items_Pool);

            // if ran out of items in the pool
            if (Items_Pool.size() == 0)
            {
                // if not using logic, then it doesn't matter
                if (Items_Needed.size() == 0)
                {
                    // make it where the item gives itself
                    Items[Cur_Item].gives_item = true;
                    Items[Cur_Item].can_get = true;
                    // go to start of the for loop
                    continue;
                }
                else
                {
                    Error(Cur_Item + " could not be placed - pool was empty - probably "
                                     "because of some manual placements?");
                }
            }

			//get a random index to use to start checking
			//index = Random(0, Items_Pool.size() - 1);
			index = 0;

			//for each item in the pool
            for (int ip = 0; ip < Items_Pool.size(); ip++)
            {
                string New_Log = Log;
                bool invalid = false;
				//start at a random index, and wrap around until it comes to the starting item it tried
				int p = (index + ip) % Items_Pool.size();

                New_Item = Items_Pool[p];

				//if the first item didn't work, and the current item doesn't help open any more locations, then just skip it.
				if (ip > 0 && Points[New_Item] == 0) {
					continue;
				}

                // check whether or not this item is in the invalid list
                if (Invalid_Items[Cur_Item].size() > 0)
                {
                    // check each item in the invalid list if it is the item that was just
                    // placed here
                    for (int ii = 0; ii < Invalid_Items[Cur_Item].size(); ii++)
                    {
                        Invalid_Item = Invalid_Items[Cur_Item][ii];
                        // this item cannot be placed here
                        if (Invalid_Item == New_Item)
                        {
                            invalid = true;

                            break; // no need to check rest of loop
                        }
                    }

					//if this item is in the invalid list, then it cannot be placed here
                    if (invalid)
                    {
                        continue; // try the next item in the list
                    }
                }

				Items[Cur_Item].Name = New_Item;
				Items[Cur_Item].gives_item = true;
				Items[New_Item].can_get = true;

				// make the player acquire the item that is now placed here
				Items_Gotten[New_Item] = Items[Cur_Item].Count;
				Cycle_Items[New_Item] = Items[Cur_Item].Count;
				Items[Cur_Item].Obtainable = true;

				//if the item being placed here resets on sot
				if (Resettable_Items[New_Item]) {
					Locations_Reset.push_back(Cur_Item);	//add the location to the list of locations that reset
				}

				//only check curiosity if there are needed items for this item
                if (Items_Needed.size() > 0)
                {
					//check if a item overwrites an item in curiosity shop, and check/randomize the rest of the items
                    if (Check_Curiosity_Items(Cur_Item) && Randomize(New_Log,
                                                                     Items,
                                                                     Seed,
                                                                     Items_Needed,
                                                                     Items_Gotten,
						Cycle_Items,
                                                                     wallets, seed,
                                                                     Invalid_Items,
						Resettable_Items,
						Location_Sets,
                                                                     Items_This,
						Items_Aval, Locations, Locations_Reset, Item_Counts, Placed_Items + 1))
                    {
                        return true;
                    }
					//this item would not work, so remove it and try the next one in the pool list
                    else
                    {
						Logger(Cur_Item + " giving " + New_Item + " did not work, removing it and trying a different item");
						Items_Gotten[New_Item] = -1;
						Cycle_Items[New_Item] = -1;
						Items[Cur_Item].Obtainable = false;
                        Items[Cur_Item].Name = Cur_Item;
                        Items[Cur_Item].gives_item = false;
                        Items[New_Item].can_get = false;
						// if the item being placed here resets on sot
						if (Resettable_Items[New_Item]) {
							Locations_Reset.pop_back();	//remove location for reset list
						}
                    }
                }
				//no logic
                else
                {
					Logger("No Logic");
                    return Randomize(New_Log,
                                     Items,
                                     Seed,
                                     Items_Needed,
                                     Items_Gotten,
						Cycle_Items,
                                     wallets, seed,
                                     Invalid_Items,
						Resettable_Items,
						Location_Sets,
                                     Items_This,
						Items_Aval, Locations, Locations_Reset, Item_Counts, Placed_Items + 1);
                }
            }

            // if an item wasn't placed, then have to back track because there are no
            // valid items left that can go here
            if (!Items[Cur_Item].gives_item)
            {
                return false;
            }
        }
		else {

		}

        // if this item hasn't been gotten yet, and it's been placed
        if (Items[Cur_Item].gives_item && Items_Gotten[Items[Cur_Item].Name] == -1)
        {
			Logger(Cur_Item + " has an item placed here, but the player hasn't gotten it yet (" + Items[Cur_Item].Name + ")");
			New_Item = Items[Cur_Item].Name;

			//if this is not a resettable item
			if (!Resettable_Items[New_Item]) {
				//remove the item from the list of items being checked if they are obtainable (it already is)
				Locations = Remove_Values(Locations, { Cur_Item });
			}

			//update the items gotten
			Items_Gotten[New_Item] = Items[Cur_Item].Count;
			Cycle_Items[New_Item] = Items[Cur_Item].Count;
        }
		else {
			//Logger(Cur_Item + " has an item, and the player already has it");
		}
    }

	Logger("Checking if all items have been gotten and placed");
    for (int i = 0; i < Item_Keys.size(); i++)
    {
        // check if every item has been gotten
        if (Items_Gotten[Item_Keys[i]] == -1)
        {
			//Logger(items[i] + " hasn't been placed");
            Has_All = false;
        }

        // check if all items have been placed
        if (!Items[Item_Keys[i]].gives_item)
        {
			//Logger(items[i] + " doesn't give an item");
            Placed_All = false;
        }
    }

	// keep going if haven't placed all items, and using no logic
	if (Items_Needed.size() == 0) {
		if (Has_All) {
			return Randomize(Log,
				Items,
				Seed,
				Items_Needed,
				Items_Gotten,
				Cycle_Items,
				wallets, seed,
				Invalid_Items,
				Resettable_Items,
				Location_Sets,
				Items_This,  Items_Aval, Locations, Locations_Reset, Item_Counts, Placed_Items + 1);
		}
	}
	else {
		//all items have been obtained
		if (Has_All) {
			//all items have been placed
			if (Placed_All) {
				Logic_Item_Sets = Copy(Location_Sets);
				Logger("Everything has been placed, returning true");
				return true;
			}
		}
	}

	Logger("Got to end of the randomize function, returning false");
	return false;
}

///setup default points of the items to "def"
map<string, int> Default_Points(int def) {
	map<string, int> points;

	for (int l = 0; l < Item_Keys.size(); l++) {
		points[Item_Keys[l]] = def;
	}

	return points;
}

void Setup_Points(map<string, vector<vector<string>>> &Items_Needed) {
	string location;
	string item;
	vector<string> Item_Vec;

	Logger("Setup_Points() - Start");

	Points = Default_Points(0);

	for (int l = 0; l < Item_Keys.size(); l++) {
		location = Item_Keys[l];

		for (int s = 0; s < Items_Needed[location].size(); s++) {
			for (int i = 0; i < Items_Needed[location][s].size(); i++) {
				item = Items_Needed[location][s][i];

				Item_Vec = Split(item, " ");
				//if the first word is a number
				if (isNumber(Item_Vec[0])) {
					item = item.substr(item.find(' ') + 1);	//removes the count from the beginning to just get the item name
				}

				//increment the usefullness of this item
				Points[item] += 1;
			}
		}
	}

	Logger("Setup_Points() - End");
}

///Sets up the non randomized items
void Setup_NonRandom_Items(map<string, Item> &Items, string *Log)
{
    vector<string> items;

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++)
    {
        string item = items[i];

        // if item gives itself, then set the gives_item and can_get flags
        if (Items[item].Pool == "")
        {
            Items[item].gives_item = true;
            Items[item].can_get = true;
        }
        // if an item gives a specific item, then update the item's name to what it gives
        else if (Items[item].Pool[0] == '#')
        {
            string other_item = "";

            other_item = Items[item].Pool.substr(1); // remove # from start
            Items[item].Name = other_item;
            Items[item].gives_item = true;
            Items[other_item].can_get = true; // the other item has been placed

            *Log += item + " => " + other_item + "\n\n";
        }
    }

    return;
}

///Sets up how useful an item is according to how many other item checks it could open up
void Setup_Item_Values(map<string, vector<vector<string>>> Items_Needed)
{
    vector<string> items;
    string item;
    int item_count = 0; // how many times the item is needed for another item

    items = Get_Keys(Items);

    for (int i = 0; i < items.size(); i++)
    {
        item = items[i];
        item_count = 0;

        for (int oi = 0; oi < items.size(); oi++)
        {
            string Other_Item = items[oi];

            for (int in = 0; in < Items_Needed[Other_Item].size(); in++)
            {
                // if the current item is needed for this list of the other item, then
                // increment item_count
                if (IndexOf(Items_Needed[Other_Item][in], item) != -1)
                {
                    item_count++;
                }
            }
        }

        Items[item].value = item_count;
    }
}

///Sets things up before randomizing items
bool Randomize_Setup(map<string, Item> &Items,
               map<string, map<string, string>> *Custom_Settings, map<string, vector<vector<string>>> &Items_Needed, map<string, int> &Location_Sets)
{
    map<string, vector<string>>
      Invalid_Items; // items that cannot be placed in a certain spot, ex: "Fierce Deity
                     // Mask" => {"Red Potion", "Green Potion", etc}
    map<string, int> Items_Gotten;
    map<string, int> Cycle_Items;
    string Log = "";
    string &Seed = (*Custom_Settings)["settings"]["Seed"];
    string Logic_File = (*Custom_Settings)["settings"]["Logic"];
	bool randomized;
	map<string, bool> Resettable_Items;
	double long Seed_Num;

	Logger("Randomize_Setup() - Start");

    if (Logic_File != "" && Logic_File != "None")
    {
        Items_Needed = Get_Logic(Logic_File, &Invalid_Items);
        Setup_Item_Values(Items_Needed);
    }
	//no logic
	else {
		Items_Needed = Set_Map(Get_Keys(Items), { {} });
	}

    // sets up vanilla and placed items
    Setup_NonRandom_Items(Items, &Log);

    // make a random seed if a seed isn't given
    if (Seed == "")
    {
        Seed = dec_to_string(time(0));
        (*Custom_Settings)["settings"]["Seed"] = Seed;
		Seed_Num = string_to_dec(Seed);
    }
	else if (isNumber(Seed)) {
		Seed_Num = string_to_dec(Seed);
	}
	else {
		Seed_Num = to_ascii(Seed);
	}
    srand(Seed_Num);

	//setup the "points" that determine how good items are
	Setup_Points(Items_Needed);

	randomized = Randomize(Log,
		Items,
		Seed,
		Items_Needed,
		Items_Gotten,
		Cycle_Items,
		(*Custom_Settings)["wallets"], Seed_Num,
		Invalid_Items, Resettable_Items, Location_Sets);

	Logger("Randomize_Setup() - End");

	return randomized;
}

///Change FD tunic color
void Change_FD(string Tunic_Color)
{
    string Original_FD =
      "6D6B9D6B756B7D6BAD6B956BA56B8D6B856BB56B656BBD6BBDABCDEFBDAB9DAB";
    string Green_FD = "0A831B470A830AC323891B4723891305130523C90A4324492449250924491BC7";
    vector<string> FD = String_Split(Green_FD, 4);
    string FD_Location = "01155128";
    int red, green, blue;
    map<string, double> HSL;
    vector<double> H = {}; // no change in hue, keeps same hue throughout
    vector<double> S = {22.7273,
                        3.40909,
                        22.7273,
                        24.2424,
                        -3.53535,
                        3.40909,
                        -3.53535,
                        12.3377,
                        12.3377,
                        -1.19617,
                        20.9091,
                        2.81385,
                        2.81385,
                        7.57576,
                        2.81385,
                        7.57576};
    vector<double> L = {-8.62745,
                        -0.784314,
                        -8.62745,
                        -7.05882,
                        2.35294,
                        -0.784314,
                        2.35294,
                        -3.92157,
                        -3.92157,
                        3.92157,
                        -10.1961,
                        7.05882,
                        7.05882,
                        11.7647,
                        7.05882,
                        2.35294};
    double Hue, Sat, Light;
    string Data = "";

    // convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    // if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue)
    {
        for (int i = 0; i < S.size(); i++)
        {
            S[i] = 0;
        }
    }

    HSL = RGB_To_HSL(red, green, blue);

    for (int i = 0; i < FD.size(); i++)
    {
        // Hue = 120;
        // Sat += 50;
        // Light -= 35;

        Hue = HSL["H"];
        // Sat = RGB_To_HSL(rgb5a1_to_hex(FD[i]))["S"] + S[i];
        // Light = RGB_To_HSL(rgb5a1_to_hex(FD[i]))["L"] + L[i];
        Sat = HSL["S"] + S[i];
        Light = HSL["L"] + L[i];

        if (Sat > 100)
        {
            Sat = 100;
        }
        else if (Sat < 0)
        {
            Sat = 0;
        }

        if (Light > 100)
        {
            Light = 100;
        }
        else if (Light < 0)
        {
            Light = 0;
        }

        Data += hex_to_rgb5a1(HSL_To_RGB(Hue, Sat, Light));
    }

    Write_To_Rom(hex_to_decimal(FD_Location), Data);
}

///Change Zora Tunic Color
void Change_Zora(string Tunic_Color)
{
    // tunic and back of head - 1, 2, 3
    string Original_Zora_1 =
      "E7F9D7B529CB431100C10081CFB50347024504490307020504CB03C902C7054D048B0389028715910D"
      "4F25950185561F6EA53DDB6E655E6386EB5317ADED7EEB76A98EEFCFB9A73397310107D7BBAF77CEB5"
      "B5EFBE31A56B94E78CA584637C21";
    // zora fins
    string Original_Zora_2 =
      "C7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7"
      "BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BD"
      "C7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7"
      "BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BD"
      "C7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDC7BDBF7BBF7BBF7BBF7BBF7BBF7BBF"
      "7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BBF7BAF77AF77AF77AF77AF77AF77AF77AF77AF77AF77AF77"
      "AF77AF77AF77AF77AF77A733A733A733A733A733A733A733A733A733A733A733A733A733A733A733A7"
      "3397319731973197319731973197319731973197319731973197319731973197318EEF8EEF8EEF8EEF"
      "8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF8EEF86EB86EB86EB7EEB86EB7EEB86EB7EEB86"
      "EB86EB7EEB7EEB7EEB86EB86EB86EB76A976A976A976A976A976A976A976A976A976A976A976A976A9"
      "76A976A976A96EA56E656EA56E656EA56EA56EA56E656EA56E656EA56EA56E656EA56EA56EA55E635E"
      "635E635E635E635E635E635E635E635E635E635E635E635E635E635E63561F561F561F561F561F561F"
      "561F561F561F561F561F561F561F561F561F561F3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3D"
      "DB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB3DDB"
      "3DDB259525952595259525952595259525952595259525952595259525952595259525952595259525"
      "9525952595259525952595259525952595259525952595259515911591159115911591159115911591"
      "159115911591159115911591159115910D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D4F0D"
      "4F0D4F0D4F0D4F054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D"
      "054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D054D05"
      "4D054D054D054D054D054D054D054D054D054D054D04CB04CB04CB04CB04CB04CB04CB04CB04CB04CB"
      "04CB04CB04CB04CB04CB04CB0449044904490449044904490449044904490449044904490449044904"
      "49044903C903C903C903C903C903C903C903C903C903C903C903C903C903C903C903C9034703470347"
      "034703470347034703470347034703470347034703470347034702C702C702C702C702C702C702C702"
      "C702C702C702C702C702C702C702C702C7024502450245024502450245024502450245024502450245"
      "02450245024502450245024502450245024502450245024502450245024502450245024502450245";
    // next set of tunics - 4, 5, 6 (Same as other one? so gonna write to here as well
    // instead of doing it a third time)
    string Original_Zora_3 =
      "E7F9D7B529CB431100C10081CFB50347024504490307020504CB03C902C7054D048B0389028715910D"
      "4F25950185561F6EA53DDB6E655E6386EB5317ADED7EEB76A98EEFCFB9A73397310107D7BBAF77CEB5"
      "B5EFBE31A56B94E78CA584637C21";
    string Zora_Location_1 = "01197120";
    string Zora_Location_2 = "011A2228";
    string Zora_Location_3 = "0119E698";
    string Zora_Location_4 = "010FB0B0"; // zora boomerang
    int red, green, blue;
    vector<string> Zora_1 = String_Split(Original_Zora_1, 4);
    vector<string> Zora_2 = String_Split(Original_Zora_2, 4);
    vector<double> H = {3,  3,  3,  3,  3,  3,  15, 16, 16, 17, 18, 18, 18, 18, 19, 20,
                        19, 20, 20, 20, 20, 23, 23, 24, 26, 25, 27, 28, 30, 33, 33, 33,
                        33, 39, 39, 40, 39, 48, 48, 48, 63, 63, 63, 63, 63, 63, 63, 63};
    vector<double> S = {
      4.06699,  -7.47801, -42.4242, -39.0909, 40.9091,  40.9091,  -1.94805, 40.9091,
      40.9091,  40.9091,  40.9091,  40.9091,  40.9091,  40.9091,  40.9091,  40.9091,
      40.9091,  40.9091,  40.9091,  24.2424,  31.8182,  10.1399,  40.9091,  -12.0321,
      -6.56566, -5.75758, -12.489,  -8.64046, -6.07886, -50,      -48.9643, -3.9185,
      -8.56459, -8.458,   -1.94805, -8.29726, -2.75288, 40.9091,  -7.47801, -0.909091,
      -51.2478, -53.7576, -53.1208, -54.6953, -55.3526, -55.6126, -55.8389, -55.8651};
    vector<double> L = {
      66.6667,  61.9608,  -7.05882, 5.4902,   -21.1765, -22.7451, 60.3922,  -5.4902,
      -11.7647, 0.784314, -7.05882, -13.3333, 3.92157,  -2.35294, -8.62745, 7.05882,
      2.35294,  -3.92157, -10.1961, 11.7647,  8.62745,  14.902,   -16.4706, 27.451,
      35.2941,  21.1765,  33.7255,  30.5882,  41.5686,  8.62745,  43.1373,  40,
      36.8627,  43.1373,  60.3922,  49.4118,  46.2745,  -19.6078, 61.9608,  52.549,
      54.1176,  44.7059,  47.8431,  38.4314,  32.1569,  29.0196,  25.8824,  22.7451};
    map<string, double> HSL;

    // convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    // if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue)
    {
        for (int i = 0; i < S.size(); i++)
        {
            S[i] = 0;
        }
    }

    HSL = RGB_To_HSL(red, green, blue);

    // tunic and back of the head
    for (int i = 0; i < Zora_1.size(); i++)
    {
        H[i] += HSL["H"] - 20;
        if (H[i] > 360)
        {
            H[i] -= 360;
        }
        else if (H[i] < 0)
        {
            H[i] = 360 - H[i];
        }

        S[i] += HSL["S"];
        if (S[i] > 100)
        {
            S[i] = 100;
        }
        else if (S[i] < 0)
        {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100)
        {
            L[i] = 100;
        }
        else if (L[i] < 0)
        {
            L[i] = 0;
        }

        Write_To_Rom(hex_to_decimal(Zora_Location_1) + (i * 2),
                     hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i]))); // tunic 1, 2, 3
        Write_To_Rom(hex_to_decimal(Zora_Location_3) + (i * 2),
                     hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i]))); // tunic 4, 5, 6
    }

    // update the H, S, and L for zora fins
    H = {63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
         63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
         63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
         63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
         63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
         63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 48, 48, 48, 48, 48, 48, 48, 48,
         48, 48, 48, 48, 48, 48, 48, 48, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
         40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
         39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 30, 30, 30, 33,
         30, 33, 30, 33, 30, 30, 33, 33, 33, 30, 30, 30, 33, 33, 33, 33, 33, 33, 33, 33,
         33, 33, 33, 33, 33, 33, 33, 33, 26, 27, 26, 27, 26, 26, 26, 27, 26, 27, 26, 26,
         27, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25,
         25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
         25, 25, 25, 25, 25, 25, 25, 25, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
         23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
         20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
         20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
         20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
         20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
         18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 17, 17,
         17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18,
         18, 18, 18, 18, 18, 18, 18, 18, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
         16, 16, 16, 16, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
         16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
         16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
    S = {2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   2.44755,
         2.44755,   2.44755,   2.44755,   2.44755,   2.44755,   -8.02708,  -8.02708,
         -8.02708,  -8.02708,  -8.02708,  -8.02708,  -8.02708,  -8.02708,  -8.02708,
         -8.02708,  -8.02708,  -8.02708,  -8.02708,  -8.02708,  -8.02708,  -8.02708,
         -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091,
         -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091, -0.909091,
         -0.909091, -0.909091, -8.29726,  -8.29726,  -8.29726,  -8.29726,  -8.29726,
         -8.29726,  -8.29726,  -8.29726,  -8.29726,  -8.29726,  -8.29726,  -8.29726,
         -8.29726,  -8.29726,  -8.29726,  -8.29726,  -2.75288,  -2.75288,  -2.75288,
         -2.75288,  -2.75288,  -2.75288,  -2.75288,  -2.75288,  -2.75288,  -2.75288,
         -2.75288,  -2.75288,  -2.75288,  -2.75288,  -2.75288,  -2.75288,  -8.458,
         -8.458,    -8.458,    -8.458,    -8.458,    -8.458,    -8.458,    -8.458,
         -8.458,    -8.458,    -8.458,    -8.458,    -8.458,    -8.458,    -8.458,
         -8.458,    -6.07886,  -6.07886,  -6.07886,  -3.9185,   -6.07886,  -3.9185,
         -6.07886,  -3.9185,   -6.07886,  -6.07886,  -3.9185,   -3.9185,   -3.9185,
         -6.07886,  -6.07886,  -6.07886,  -8.56459,  -8.56459,  -8.56459,  -8.56459,
         -8.56459,  -8.56459,  -8.56459,  -8.56459,  -8.56459,  -8.56459,  -8.56459,
         -8.56459,  -8.56459,  -8.56459,  -8.56459,  -8.56459,  -6.56566,  -12.489,
         -6.56566,  -12.489,   -6.56566,  -6.56566,  -6.56566,  -12.489,   -6.56566,
         -12.489,   -6.56566,  -6.56566,  -12.489,   -6.56566,  -6.56566,  -6.56566,
         -8.64046,  -8.64046,  -8.64046,  -8.64046,  -8.64046,  -8.64046,  -8.64046,
         -8.64046,  -8.64046,  -8.64046,  -8.64046,  -8.64046,  -8.64046,  -8.64046,
         -8.64046,  -8.64046,  -12.0321,  -12.0321,  -12.0321,  -12.0321,  -12.0321,
         -12.0321,  -12.0321,  -12.0321,  -12.0321,  -12.0321,  -12.0321,  -12.0321,
         -12.0321,  -12.0321,  -12.0321,  -12.0321,  -5.75758,  -5.75758,  -5.75758,
         -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,
         -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,
         -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,
         -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,  -5.75758,
         -5.75758,  10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,
         10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,
         10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,
         10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   10.1399,
         10.1399,   10.1399,   10.1399,   10.1399,   10.1399,   24.2424,   24.2424,
         24.2424,   24.2424,   24.2424,   24.2424,   24.2424,   24.2424,   24.2424,
         24.2424,   24.2424,   24.2424,   24.2424,   24.2424,   24.2424,   24.2424,
         31.8182,   31.8182,   31.8182,   31.8182,   31.8182,   31.8182,   31.8182,
         31.8182,   31.8182,   31.8182,   31.8182,   31.8182,   31.8182,   31.8182,
         31.8182,   31.8182,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,   40.9091,
         40.9091};
    L = {58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,  58.8235,
         55.6863,  55.6863,  55.6863,  55.6863,  55.6863,  55.6863,  55.6863,  55.6863,
         55.6863,  55.6863,  55.6863,  55.6863,  55.6863,  55.6863,  55.6863,  55.6863,
         52.549,   52.549,   52.549,   52.549,   52.549,   52.549,   52.549,   52.549,
         52.549,   52.549,   52.549,   52.549,   52.549,   52.549,   52.549,   52.549,
         49.4118,  49.4118,  49.4118,  49.4118,  49.4118,  49.4118,  49.4118,  49.4118,
         49.4118,  49.4118,  49.4118,  49.4118,  49.4118,  49.4118,  49.4118,  49.4118,
         46.2745,  46.2745,  46.2745,  46.2745,  46.2745,  46.2745,  46.2745,  46.2745,
         46.2745,  46.2745,  46.2745,  46.2745,  46.2745,  46.2745,  46.2745,  46.2745,
         43.1373,  43.1373,  43.1373,  43.1373,  43.1373,  43.1373,  43.1373,  43.1373,
         43.1373,  43.1373,  43.1373,  43.1373,  43.1373,  43.1373,  43.1373,  43.1373,
         41.5686,  41.5686,  41.5686,  40,       41.5686,  40,       41.5686,  40,
         41.5686,  41.5686,  40,       40,       40,       41.5686,  41.5686,  41.5686,
         36.8627,  36.8627,  36.8627,  36.8627,  36.8627,  36.8627,  36.8627,  36.8627,
         36.8627,  36.8627,  36.8627,  36.8627,  36.8627,  36.8627,  36.8627,  36.8627,
         35.2941,  33.7255,  35.2941,  33.7255,  35.2941,  35.2941,  35.2941,  33.7255,
         35.2941,  33.7255,  35.2941,  35.2941,  33.7255,  35.2941,  35.2941,  35.2941,
         30.5882,  30.5882,  30.5882,  30.5882,  30.5882,  30.5882,  30.5882,  30.5882,
         30.5882,  30.5882,  30.5882,  30.5882,  30.5882,  30.5882,  30.5882,  30.5882,
         27.451,   27.451,   27.451,   27.451,   27.451,   27.451,   27.451,   27.451,
         27.451,   27.451,   27.451,   27.451,   27.451,   27.451,   27.451,   27.451,
         21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,
         21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,
         21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,
         21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,  21.1765,
         14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,
         14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,
         14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,
         14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,   14.902,
         11.7647,  11.7647,  11.7647,  11.7647,  11.7647,  11.7647,  11.7647,  11.7647,
         11.7647,  11.7647,  11.7647,  11.7647,  11.7647,  11.7647,  11.7647,  11.7647,
         8.62745,  8.62745,  8.62745,  8.62745,  8.62745,  8.62745,  8.62745,  8.62745,
         8.62745,  8.62745,  8.62745,  8.62745,  8.62745,  8.62745,  8.62745,  8.62745,
         7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,
         7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,
         7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,
         7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,
         7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,
         7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,  7.05882,
         3.92157,  3.92157,  3.92157,  3.92157,  3.92157,  3.92157,  3.92157,  3.92157,
         3.92157,  3.92157,  3.92157,  3.92157,  3.92157,  3.92157,  3.92157,  3.92157,
         0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314,
         0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314, 0.784314,
         -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294,
         -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294, -2.35294,
         -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,
         -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,  -5.4902,
         -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745,
         -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745, -8.62745,
         -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647,
         -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647,
         -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647,
         -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647, -11.7647};

    for (int i = 0; i < Zora_2.size(); i++)
    {
        H[i] += HSL["H"] - 20;
        if (H[i] > 360)
        {
            H[i] -= 360;
        }
        else if (H[i] < 0)
        {
            H[i] = 360 - H[i];
        }

        S[i] += HSL["S"];
        if (S[i] > 100)
        {
            S[i] = 100;
        }
        else if (S[i] < 0)
        {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100)
        {
            L[i] = 100;
        }
        else if (L[i] < 0)
        {
            L[i] = 0;
        }

        Write_To_Rom(hex_to_decimal(Zora_Location_2) + (i * 2),
                     hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
        Write_To_Rom(hex_to_decimal(Zora_Location_4) + (i * 2),
                     hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
    }
}

///Change Goron Tunic Color
void Change_Goron(string Tunic_Color)
{
    map<string, double> HSL;
    int red, green, blue;
    string Goron_Location_1 = "0117C780";
    string Goron_Location_2 = "01186EB8"; // when curled
    string Original_Goron_1 =
      "0185018501850185018501850185018501830183018301830185018501C501C501C501C50245020501"
      "C501C50245024501C501C301C501830205028702C7024501430143014301C3028503C903C903070143"
      "014302850347044B050B044B038903470389040904CB054D0409038903CB054D054D04CD048B040903"
      "070347040904490449034702C5024502050307044B01C301C301830143014302850389034701C501C5"
      "01830245030704090409038902470287034903C9048B050B050B044B054D054D048B04090347028503"
      "87050B01C301430143018301C50287040B04CB020502C5034703C90409048B048B048B04CB048B050B"
      "054D054D054D048D04CB";
    vector<string> Goron_1 = String_Split(Original_Goron_1, 4);
    vector<double> H = {23, 23, 23, 23, 23, 23, 23, 23, 12, 12, 12, 12, 23, 23, 20, 20,
                        20, 20, 16, 18, 20, 20, 16, 16, 20, 11, 20, 12, 18, 20, 19, 16,
                        15, 15, 15, 11, 15, 18, 18, 18, 15, 15, 15, 16, 20, 18, 20, 20,
                        16, 20, 18, 18, 20, 18, 20, 23, 20, 20, 21, 19, 18, 18, 16, 18,
                        17, 17, 16, 13, 16, 18, 18, 20, 11, 11, 12, 15, 15, 15, 20, 16,
                        20, 20, 12, 16, 18, 18, 18, 20, 23, 20, 21, 18, 19, 18, 18, 20,
                        20, 20, 19, 18, 16, 15, 15, 18, 11, 15, 15, 12, 20, 20, 21, 18,
                        18, 13, 16, 18, 18, 19, 19, 19, 18, 19, 18, 20, 20, 20, 23, 18};
    vector<double> S = {
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091, 40.9091,
      40.9091, 40.9091};
    vector<double> L = {
      -16.4706,  -16.4706,  -16.4706,  -16.4706, -16.4706,  -16.4706,  -16.4706,
      -16.4706,  -16.4706,  -16.4706,  -16.4706, -16.4706,  -16.4706,  -16.4706,
      -14.902,   -14.902,   -14.902,   -14.902,  -11.7647,  -13.3333,  -14.902,
      -14.902,   -11.7647,  -11.7647,  -14.902,  -14.902,   -14.902,   -16.4706,
      -13.3333,  -10.1961,  -8.62745,  -11.7647, -18.0392,  -18.0392,  -18.0392,
      -14.902,   -10.1961,  -2.35294,  -2.35294, -7.05882,  -18.0392,  -18.0392,
      -10.1961,  -5.4902,   0.784314,  5.4902,   0.784314,  -3.92157,  -5.4902,
      -3.92157,  -0.784314, 3.92157,   7.05882,  -0.784314, -3.92157,  -2.35294,
      7.05882,   7.05882,   3.92157,   2.35294,  -0.784314, -7.05882,  -5.4902,
      -0.784314, 0.784314,  0.784314,  -5.4902,  -8.62745,  -11.7647,  -13.3333,
      -7.05882,  0.784314,  -14.902,   -14.902,  -16.4706,  -18.0392,  -18.0392,
      -10.1961,  -3.92157,  -5.4902,   -14.902,  -14.902,   -16.4706,  -11.7647,
      -7.05882,  -0.784314, -0.784314, -3.92157, -11.7647,  -10.1961,  -5.4902,
      -2.35294,  2.35294,   5.4902,    5.4902,   0.784314,  7.05882,   7.05882,
      2.35294,   -0.784314, -5.4902,   -10.1961, -3.92157,  5.4902,    -14.902,
      -18.0392,  -18.0392,  -16.4706,  -14.902,  -10.1961,  -0.784314, 3.92157,
      -13.3333,  -8.62745,  -5.4902,   -2.35294, -0.784314, 2.35294,   2.35294,
      2.35294,   3.92157,   2.35294,   5.4902,   7.05882,   7.05882,   7.05882,
      2.35294,   3.92157};

    // convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    // if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue)
    {
        for (int i = 0; i < S.size(); i++)
        {
            S[i] = 0;
        }
    }

    HSL = RGB_To_HSL(red, green, blue);

    for (int i = 0; i < Goron_1.size(); i++)
    {
        H[i] += HSL["H"] - 5;
        if (H[i] > 360)
        {
            H[i] -= 360;
        }
        else if (H[i] < 0)
        {
            H[i] = 360 - H[i];
        }

        S[i] += HSL["S"];
        if (S[i] > 100)
        {
            S[i] = 100;
        }
        else if (S[i] < 0)
        {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100)
        {
            L[i] = 100;
        }
        else if (L[i] < 0)
        {
            L[i] = 0;
        }

        Write_To_Rom(hex_to_decimal(Goron_Location_1) + (i * 2),
                     hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
        Write_To_Rom(hex_to_decimal(Goron_Location_2) + (i * 2),
                     hex_to_rgb5a1(HSL_To_RGB(H[i], S[i], L[i])));
    }
}

///Change Deku Tunic Color
void Change_Deku(string Tunic_Color)
{
    vector<string> Deku_Locations = {"011A9092",
                                     "011A9094",
                                     "011A9096",
                                     "011A9098",
                                     "011A909A",
                                     "011A909C",
                                     "011A909E",
                                     "011A90A0",
                                     "011A90A2",
                                     "011A90A4",
                                     "011A90A6",
                                     "011A90A8",
                                     "011A90AA",
                                     "011A90AC",
                                     "011A90AE",
                                     "011A90B0"};
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

    // convert tunic color to HSL
    red = Get_Red_Dec(Tunic_Color);
    green = Get_Green_Dec(Tunic_Color);
    blue = Get_Blue_Dec(Tunic_Color);

    // if black or gray or white, then ignore the saturation changes
    if (red == green && red == blue)
    {
        S = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    HSL = RGB_To_HSL(red, green, blue);

    // apply base to each offset
    for (int i = 0; i < H.size(); i++)
    {
        // update the hues
        H[i] += HSL["H"] - 10;
        H[i] %= 360;
        if (H[i] < 0)
        { // if hue goes negative, then wrap it around
            H[i] = 360 - H[i];
        }

        // update the Saturations
        S[i] += HSL["S"];
        if (S[i] > 100)
        {
            S[i] = 100;
        }
        else if (S[i] < 0)
        {
            S[i] = 0;
        }

        L[i] += HSL["L"];
        if (L[i] > 100)
        {
            L[i] = 100;
        }
        else if (L[i] < 0)
        {
            L[i] = 0;
        }
    }

    // convert HSL back to hex
    for (int i = 0; i < H.size(); i++)
    {
        RGB.push_back(HSL_To_RGB(H[i], S[i], L[i]));
    }

    // convert new hex to rgb5a1
    for (int i = 0; i < RGB.size(); i++)
    {
        RBG5A1.push_back(hex_to_rgb5a1(RGB[i]));
    }

    // write values to rom
    for (int i = 0; i < RBG5A1.size(); i++)
    {
        Write_To_Rom(hex_to_decimal(Deku_Locations[i]), RBG5A1[i]);
    }
}

///Change Link's Tunic Color
void Change_Link_Color(string color)
{
    vector<string> Link_Locations = {"0116639C",
                                     "011668C4",
                                     "01166DCC",
                                     "01166FA4",
                                     "01167064",
                                     "0116766C",
                                     "01167AE4",
                                     "01167D1C",
                                     "011681EC"};
    string RGB;

    // change kokiri tunic color
    RGB = Color_To_Hex(color);
    for (int i = 0; i < Link_Locations.size(); i++)
    {
        Write_To_Rom(hex_to_decimal(Link_Locations[i]), RGB);
    }
}

///Change the item subscreen color
void Change_Item_Screen(string color)
{
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

///Change the map subscreen color
void Change_Map_Screen(string color)
{
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

///Change the status subscreen color
void Change_Status_Screen(string color)
{
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

///Change the mask subscreen color
void Change_Mask_Screen(string color)
{
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

///Change the nameplate, L, and R in the pause menu
void Change_Nameplate(string color)
{
    string RGB;
    string RG;
    string B;

    RGB = Color_To_Hex(color);
    RG = RGB.substr(0, 4);
    B = RGB.substr(4, 2);
    Write_To_Rom(13230330, RG); // nameplate
    Write_To_Rom(13230466, RG); // Highlighted Z
    Write_To_Rom(13230578, RG); // Highlighted R
    Write_To_Rom(13230334, B);  // nameplate
    Write_To_Rom(13230486, B);  // Highlighted Z
    Write_To_Rom(13230582, B);  // Highlighted R
}

///Change the colors
void Change_Colors(map<string, string> colors)
{
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

///Make the bottles able to catch non-bottle items, also fixes new bottles from never being obtained after 6 bottles
void Make_Bottles_Work()
{
    string Jump_Function = "0C061912";
    vector<string> New_Function = {
      "27BDFFE8", // ADDIU	SP, SP, 0xFFE8  Get enough free memory to store stuff
      "AFBF0014", // RA	0x0014 (SP)     Store the return value
      "28A60027", // SLTI	A2, A1, 0x0027  A2 = (Item < 27) ? 1 : 0;
      "10060007", // BEQ	R0, A1          Branch if ID > 27 (A2 = 1, is Item)
      "28A60012", // SLTI	A2, A1, 0x0012  A2 = (Item < 12) ? 1 : 0;
      "14C00005", // BNEZ	A2              Branch if ID < 12 (A2 = 0, Is Item)
      "00000000", // NOP                   Previous command runs nothing here
      "0C0453F4", // JAL	giv bottl item  Jump to the normal bottle function (only gets
                  // here if bottled item)
      "91C60146", // LBU	A2, 0x0146 (T6) Load something into A2 before jump, the
                  // original jump did this
      "10000003", // BEQ	R0, R0          Jump to the end to skip the item function
      "00000000", // NOP                   Don't wanna run anymore commands for bottle
      "0C044BA0", // JAL	giv item pass   Jump to the give item passive function (only
                  // runs if not bottle content)
      "91C60146", // LBU	A2, 0x0146 (T6) Load something into A2
      "8FBF0014", // LW	RA, 0x0014 (SP) Load the return Address
      "27BD0018", // ADDIU	SP, SP, 0x0018  Put the SP back to where it was
      "03E00008"  // JR	RA              Return
    };
    vector<string> Max_Bottles = {
	    "27BDFFE0",
	    "AFBF0000",
	    "AFA40004",
	    "AFA20008",
	    "AFB9000C",
	    "AFA50010",
	    "AFA60014",
	    "34050000",
	    "34060012",
	    "00821821",
	    "90790070",
	    "240200FF",
	    "54D90004",
	    "24A50001",
	    "10000007",
	    "00000000",
	    "24A50001",
	    "30A500FF",
	    "28A10006",
	    "1420FFF5",
	    "00A01025",
	    "34830000",
	    "8FBF0000",
	    "8FA40004",
	    "8FA20008",
	    "8FB9000C",
	    "8FA50010",
	    "8FA60014",
	    "27FFFFDC",
	    "03E00008",
	    "27BD0020"
    };

    Write_To_Rom(12296876, Jump_Function);
    for (int i = 0; i < New_Function.size(); i++)
    {
        Write_To_Rom(12700040 + (i * 4), New_Function[i]);
    }


    Write_To_Rom(12231952, "0C02CCB330020000");
    for (int i = 0; i < Max_Bottles.size(); i++)
    {
	    Write_To_Rom(11835404 + (i * 4), Max_Bottles[i]);
    }
}

///Make tingle maps able to be gotten with a get item id value
void Fix_Tingle_Maps()
{
    int Pass = 12227676;
    int Maps = 12342348;
    string starting_maps_add = "C1CAD4";

    vector<string> Pass_New = {"00000000", "2401006D", "0C04BBC3"};
    vector<string> Maps_Fun = {
      "27BDFFE8", // ADDIU	SP, SP, 0xFFE8  Get enough free memory to store stuff
      "AFA50010", // SW	A1, 0x0010 (SP)	Store A1
      "AFA6000C", // SW	A2, 0x000C (SP)	Store A2
      "AFA70008", // SW	A3, 0x0008 (SP)	Store A3
      "AFBF0004", // SW	RA, 0x0004 (SP)	Store RA
      "28E600B4", // SLTI	A2, A3, 0x00B4  A2 = (Item < B4) ? 1 : 0;
      "14C0001B", // BNEZ	A2, 		Branch if < B4 (A2 = 1)
      "00000000", // NOP
      "28E600B5", // SLTI	A2, A3, 0x00B5  A2 = (Item < B5) ? 1 : 0;
      "14C0000E", // BNEZ	A2, A		Branch if B4 (A2 = 1)
      "24060003", // ADDIU	A2, R0, 0x0003
      "28E600B6", // SLTI	A2, A3, 0x00B6  A2 = (Item < B6) ? 1 : 0;
      "14C0000B", // BNEZ	A2, A		Branch if B5 (A2 = 1)
      "2406001C", // ADDIU	A2, R0, 0x001C
      "28E600B7", // SLTI	A2, A3, 0x00B7  A2 = (Item < B7) ? 1 : 0;
      "14C00008", // BNEZ	A2, A		Branch if B6 (A2 = 1)
      "240600E0", // ADDIU	A2, R0, 0x00E0
      "28E600B8", // SLTI	A2, A3, 0x00B8  A2 = (Item < B8) ? 1 : 0;
      "14C00005", // BNEZ	A2, A		Branch if B7 (A2 = 1)
      "24060100", // ADDIU	A2, R0, 0x0100
      "28E600B9", // SLTI	A2, A3, 0x00B9  A2 = (Item < B9) ? 1 : 0;
      "14C00002", // BNEZ	A2, A		Branch if B8 (A2 = 1)
      "24061E00", // ADDIU	A2, R0, 0x1E00
      "24066000", // ADDIU	A2, R0, 0x6000
      "3C05801F", // LUI	A1, 0x801F              label: A
      "24A505D0", // ADDIU	A1, A1, 0x05D0	A1 = 801F05D0
      "8CA70000", // LW	A3, A1		A3 = Current map values
      "00C73825", // OR	A3, A2, A3
      "ACA70000", // SW	A3, A1
      "27BD0018", // ADDIU	SP, SP, 0x0018  Put the SP back to where it was
      "8FBF0014", // LW	RA, 0x0014 (SP) Load previous RA
      "27BD0040", // ADDIU	SP, SP, 0x0040
      "03E00008", // JR	RA	Return
      "00000000", // NOP
      "8FA50010", // LW	A1, 0x0010 (SP) Load A1     (not a map jumps to here)
      "8FA6000C", // LW	A2, 0x000C (SP) Load A2
      "8FA70008", // LW	A3, 0x0008 (SP) Load A3
      "8FBF0004", // LW	RA, 0x0004 (SP) Load RA
      "27BD0018", // ADDIU	SP, SP, 0x0018  Put the SP back to where it was
      "03E00008", // JR	RA	Return
      "00000000"  // NOP       Make sure not to do a random command
    };

    Maps_Fun = {
      "27BDFFD8", "AFA50010", "AFA60014", "AFA70018", "AFBF001C", "AFA40020", "28E600B4",
      "14C00067", "00000000", "28E600B5", "1006000D", "24060003", "AFA60024", "3C061000",
      "AFA60000", "24062000", "AFA60004", "3C060008", "24C60009", "AFA60008", "3C060002",
      "24C6F000", "AFA6000C", "10000043", "28E600B6", "1006000C", "2406001C", "AFA60024",
      "24060001", "AFA60000", "3C064000", "24C60880", "AFA60004", "24060060", "AFA60008",
      "24060000", "AFA6000C", "10000035", "28E600B7", "1006000C", "240600E0", "AFA60024",
      "24060000", "AFA60000", "3C060004", "AFA60004", "3C067C01", "24C62100", "AFA60008",
      "24060800", "AFA6000C", "10000027", "28E600B8", "1006000C", "24060100", "AFA60024",
      "3C060001", "AFA60000", "3C060020", "24C60004", "AFA60004", "24060006", "AFA60008",
      "24060400", "AFA6000C", "10000019", "28E600B9", "1006000C", "24061E00", "AFA60024",
      "3C060010", "AFA60000", "3C060988", "24C60020", "AFA60004", "24060400", "AFA60008",
      "24060000", "AFA6000C", "1000000B", "24066000", "AFA60024", "3C062008", "AFA60000",
      "24060000", "AFA60004", "3C060300", "24C60800", "AFA60008", "24060000", "AFA6000C",
      "3C05801F", "24A505D0", "8CA70000", "8FA60024", "00C73825", "ACA70000", "24A5FF50",
      "27A4000C", "8C860000", "8CA70000", "00C73825", "ACA70000", "24A5FFFC", "149DFFFA",
      "2484FFFC", "27BD0028", "8FBF0014", "27BD0040", "03E00008", "00000000", "8FA50010",
      "8FA60014", "8FA70018", "8FBF001C", "8FA40020", "27BD0028", "03E00008", "00000000"};

    vector<string> starting_maps_func = {
      "27BDFFC0", "AFA70018", "AFBF0014", "0C051224", "00000000", "0C061972", "27BDFFC0",
      "0C061978", "27BDFFC0", "8FA70018", "8FBF0014", "03E00008", "27BD0040",

      "AFBF0014", "0C04BBC3",
      "24070000", // deku mask map id
      "8FBF0014", "03E00008", "27BD0040", "AFBF0014", "0C04BBC3",
      "24070000", // song of healing map id
      "8FBF0014", "03E00008", "27BD0040"};

    for (int i = 0; i < Pass_New.size(); i++)
    {
        Write_To_Rom(Pass + (i * 4), Pass_New[i]);
    }
    for (int i = 0; i < Maps_Fun.size(); i++)
    {
        Write_To_Rom(Maps + (i * 4), Maps_Fun[i]);
    }

    Write_To_Rom(hex_to_decimal("BDD008"),
                 "0C061965"); // jump to starting tingle maps custom function in the
                              // creation of a new file
    for (int i = 0; i < starting_maps_func.size(); i++)
    { // custom starting tingle maps
        Write_To_Rom(hex_to_decimal(starting_maps_add) + (i * 4), starting_maps_func[i]);
    }
}

///Change the song text ids so that they only need one byte
void Fix_Song_Text()
{
    // Sonata of Awakening first byte of the text id
    Write_To_Rom(41394852, "00");

    // Goron Lullaby first byte of the text id
    Write_To_Rom(40090672, "00");

    // New Wave Bossa Nova first byte of the text id
    Write_To_Rom(39854656, "00");

    // Elegy of Emptiness first byte of the text id
    Write_To_Rom(45553208, "00");

    // oath to order first byte of the text id
    Write_To_Rom(47725068, "00");

    // epona song first byte of the text id
    Write_To_Rom(40440860, "00");

    // song of soaring first byte of the text id
    Write_To_Rom(42098348, "00");

    // song of storms first byte of the text id
    Write_To_Rom(33041348, "00");
}

///Makes a get song text
string Get_Song_Text(string song)
{
    string text = "";

    text += "0200FEFFFFFFFFFFFFFFFF";      // header
    text += string_to_hex("You learned "); // pre-white text
    text += "01";                          // change color to red
    text += string_to_hex(song);           // get song text in red
    text += "00";                          // change color back to white
    text += string_to_hex("!");            // end with an exclamation mark
    text += "BF";                          // footer

    return text;
}

///Writes the new songs text to free places in the text area
void Songs_Text_Offset()
{
    vector<string> Songs_Text = {
      "Epona's Song",        // 70
      "Song of Soaring",     // 71
      "Song of Storms",      // 72
      "Sonata of Awakening", // 73
      "Goron Lullaby",       // 74
      "New Wave Bossa Nova", // 75
      "Elegy of Emptiness",  // 76
      "Oath to Order"        // 77
    };
    string Text_Hex = "";

    for (int i = 0; i < Songs_Text.size(); i++)
    {
        Text_Hex += Get_Song_Text(Songs_Text[i]);
    }

    // the new song text overlaps deku mask and goron mask text, so making new short ones
    Text_Hex += "020178FFFFFFFFFFFFFFFF596F7520676F7420746865200144656B75204D61736B0021B"
                "F"; // you got the deku mask text
    Text_Hex += "020179FFFFFFFFFFFFFFFF596F7520676F74207468652001476F726F6E204D61736B0021"
                "BF"; // you got the goron mask text

    Write_To_Rom(11352914, Text_Hex);

    // create song of healing text
    Text_Hex = Get_Song_Text("Song of Healing"); // 94
    Text_Hex +=
      "020095FFFFFFFFFFFFFFFF596F7520676F7420612001736561686F7273650021BF"; // you got a
                                                                            // seahorse
                                                                            // text, 95

    Write_To_Rom(11357024, Text_Hex);

    // update the text offsets in the text table
    Write_To_Rom(12964854, "2B52"); // 70
    Write_To_Rom(12964862, "2B79"); // 71
    Write_To_Rom(12964870, "2BA3"); // 72
    Write_To_Rom(12964878, "2BCC"); // 73
    Write_To_Rom(12964886, "2BFA"); // 74
    Write_To_Rom(12964894, "2C22"); // 75
    Write_To_Rom(12964902, "2C50"); // 76
    Write_To_Rom(12964910, "2C7D"); // 77
    Write_To_Rom(12964918, "2CA5"); // 78    deku mask
    Write_To_Rom(12964926, "2CC9"); // 79    goron mask
    Write_To_Rom(12965142, "3B8A"); // 95    (94 is same offset as before)
}

///Make zfg check if intro is in inventory, and play the cutscene if it is
void Fix_Goron_Lullaby()
{
    vector<string> Code = {
      "3C01801F", // AT = 801F0000
      "8425F72C", // A1 = X1XX OR XXXX (0x801EF72C)
      "30A20100", // V0 =A1 or AT = 0100 (Intro Bit)
      "10400008", // Branch to 70 if no intro
      "00000000", // Replace the test of the normal instructions with nothing
      "00000000", //
      "00000000", //
      "00000000"  //
    };
    int Location = 16475616;

    for (int i = 0; i < Code.size(); i++)
    {
        Write_To_Rom(Location + (i * 4), Code[i]);
    }
}

///Make it where link holds up the correct item when showing an item to someone
void Fix_Showing_Items()
{
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

    for (int it = 0; it < Item_Names.size(); it++)
    {
        item = Item_Names[it];
        source = Get_Source(item);
        ind = Index[item];
        address = hex_to_decimal(base) + hex_to_decimal(ind); // base + offset
        id = Items[source].Get_Item_ID;

        Write_To_Rom(address, id);
    }
}

///Check if all the songs are in the same pool
bool All_Songs_Same_Pool()
{
    vector<string> Songs = {"Song of Healing",
                            "Song of Soaring",
                            "Epona's Song",
                            "Song of Storms",
                            "Sonata of Awakening",
                            "Goron Lullaby",
                            "New Wave Bossa Nova",
                            "Elegy of Emptiness",
                            "Oath to Order"};
    string song;
    string Song_Gives;
    string pool = "";

    for (int s = 0; s < Songs.size(); s++)
    {
        song = Songs[s];
        Song_Gives = Items[song].Name;

        if (IndexOf(Songs, Song_Gives) == -1)
        {
            return false;
        }
    }

    // all songs gives songs
    return true;
}

///Writes a cutscene from a file to the rom
void Write_Cutscene_Rom(int address, string filename)
{
    constexpr int limit = 2;
    char data[limit + 1];
    string String_Data = "";
    ifstream file;
    int cur = 0;

    filename = ".\\cutscenes\\" + filename + ".zcutscene";

    file.open(filename, fstream::binary | fstream::out | fstream::in);

    while (file.good())
    {
        file.seekg(cur);
        file.read(data, limit);
        String_Data += Char_To_String(data, limit);
        cur += limit;
    }

    file.close();

    Write_To_Rom(address, string_to_hex(String_Data));
}

///Makes the giant cutscene (the cs atre oath) instant
void Write_Short_Giant()
{
    vector<string> instructions = {"27BDFFD8",
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
                                   "24C66178"};
    vector<string> in_2 = {"ACA6F380",
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
                           "27BD0028"};

    string data = "";

    for (int i = 0; i < instructions.size(); i++)
    {
        data += instructions[i];
    };

    Write_To_Rom(15923716, data);

    data = "";
    for (int i = 0; i < in_2.size(); i++)
    {
        data += in_2[i];
    };

    Write_To_Rom(15923780, data);
}

///Gets data from the rom
string Open_From_Rom(int address, int size)
{
    // int address = string_to_dec(start);
    char *data = new char[size];

    try
    {
        inFile.open(Rom_Location, fstream::binary | fstream::out | fstream::in);
        inFile.seekg(address);
        inFile.read(data, size);
        inFile.close();
    }
    catch (exception e)
    {
        err_file << "Error opening the decompressed rom";
        err_file.close();
        exit(0);
    }

    return string_to_hex(Char_To_String(data, size));
}

///Romeves an actor from a room
void Remove_Actor(string room_offset, int actor_index)
{
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

    while (header_code != "14" && header_code != "01")
    {
        offset += 8;
        header = Open_From_Rom(offset, 8);
        header_code = header.substr(0, 2);
    }

    // found the header command for the actors
    if (header_code == "01")
    {
        num_actors = hex_to_decimal(header.substr(2, 2));
        actor_num_offset = offset + 1;

        actors_offset = hex_to_decimal(header.substr(10, 6)) + room_offset_int;

        for (int a = 0; a < num_actors; a++)
        {
            // if this is not the actor we're removing
            if (a != actor_index)
            {
                actors += Open_From_Rom(actors_offset + (a * 16), 16);
            }
        }
        actors += "00000000000000000000000000000000";

        Write_To_Rom(actors_offset, actors);

        // take 1 away from the number of actors in the file
        num_actors--;
        Write_To_Rom(actor_num_offset, dec_to_hex(num_actors));
    }
    // there are no actors
    else
    {
    }
}

///Makes the frames shorter for the Igos cutscene
void Shorten_Igos_CS()
{
    Write_To_Rom(14857239, "0A"); // Frames of first camera angle
    Write_To_Rom(14857759, "30"); // Frames of showing the curtains
    Write_To_Rom(14857835, "00"); // Frames of curtains with a different link animation?
    Write_To_Rom(14859019, "20"); // Frames of showing one guy walk in
    Write_To_Rom(14859655, "20"); // Frames of showing the other guy walk in
    Write_To_Rom(14861167, "00"); // Frames of closeups

    Write_To_Rom(14857500, "00000000"); // remove first textbox
    Write_To_Rom(14858408, "00000000"); // remove second textbox
    Write_To_Rom(14859140, "00000000"); // remove third textbox
    Write_To_Rom(14860016, "00000000"); // remove fourth textbox

    Write_To_Rom(14858572, "1000000C"); // remove Igos mouth from moving in first angle
    Write_To_Rom(14859252, "1000000C"); // remove Igos mouth from moving in second angle
    Write_To_Rom(14860252, "10000006"); // remove Igos mouth from moving in third angle
}

void Remove_Clock_Tower_CS() {
	vector<string> function = {
		"27BDFFF0",
		"AFA50004",
		"AFA60008",
		"AFBF000C",
		"3C05801F",
		"2406FFFC",
		"ACA63320",
		"8FA50004",
		"8FA60008",
		"8FBF000C",
		"03E00008",
		"27BD0010"
	};

	Write_Function(11835528, function);

	//write the jump instruction to the function
	Write_To_Rom(15387068, "0C02CCD2");

	//make the game save link's cordinates to the correct place in memory to load it later
	Write_To_Rom(15386940, "24050000");
}

///whether or not a setting is true
bool Setting_True(string section, string setting) {
	map<string, string> Section;

	//if the section exist
	if (Settings.find(section) != Settings.end()) {
		Section = Settings[section];

		//if the setting exist
		if (Section.find(setting) != Section.end()) {
			//if the setting is true
			if (Section[setting] == "True") {
				return true;	//return true
			}
		}
	}

	//could not find the setting, or the setting was false
	return false;
}

///Shortens and remove cutscenes
void Remove_Cutscenes(bool Songs_Same_Pool)
{
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

    // remove the east clock town into cs from the entrances
	if (Setting_True("cutscenes", "ClockTown_EastClockTownIntro")) {
		Write_To_Rom(47996414, "FF");
		Write_To_Rom(47996422, "FF");
		Write_To_Rom(47996430, "FF");
	}

    // remove the north clock town into cs from the entrances
	if (Setting_True("cutscenes", "ClockTown_NorthClockTownIntro")) {
		Write_To_Rom(48354914, "FF");
		Write_To_Rom(48354922, "FF");
	}

    // remove the west clock town into cs from the entrances
	if (Setting_True("cutscenes", "ClockTown_WestClockTownIntro")) {
		Write_To_Rom(48224334, "FF");
		Write_To_Rom(48224342, "FF");
	}

    // remove the termina field intro cs from the entrances
	if (Setting_True("cutscenes", "Termina_TerminaFieldIntro")) {
		Write_To_Rom(39390886, "FF");
		Write_To_Rom(39390894, "FF");
		Write_To_Rom(39390902, "FF");
		Write_To_Rom(39390910, "FF");
	}

    // remove actor that spawns the skull kid cs
	if (Setting_True("cutscenes", "Termina_SkullKidTree")) {
		Remove_Actor("025C5000", 103);
	}

    // remove southern swamp intro cs from the entrance
	if (Setting_True("cutscenes", "Swamp_SouthernSwampIntro")) {
		Write_To_Rom(42098938, "FF");
	}

    // remove the daku palace intro cs from the entrance
	if (Setting_True("cutscenes", "Swamp_DekuPalaceIntro")) {
		Write_To_Rom(39012318, "FF");
	}

    // sonata
	if (Setting_True("cutscenes", "Swamp_SonataOfAwakening")) {
		Write_To_Rom(41386310, "0000");         // make the first part of sonata cs 0 frames
		Write_Cutscene_Rom(41389520, "sonata"); // shorten sonata cutscene
		Write_To_Rom(41393633, Items[Sonata].Text_ID); // fix sonata text
		if (Songs_Same_Pool)
		{
			Write_To_Rom(
				41393585,
				Items[Sonata].Song1_ID); // fix monkey singing right song, if randomizing the
										 // songs that are actually played
			Write_To_Rom(41393609,
				Items[Sonata].Song2_ID); // fix what the player plays, if randomizing
										 // the songs that are actually played
		}
	}

    // hms deku mask cs
	if (Setting_True("cutscenes", "ClockTown_DekuMask")) {
		Write_Cutscene_Rom(46985680, "hms_1"); // shorten first part of the happy mask
											   // cutscene
		Write_Cutscene_Rom(46988304,
			"hms_2");    // shorten second part of the happy mask cutscene
		Write_To_Rom(47001596, "C002"); // make first cs jump to 2nd cs
		Write_To_Rom(47001604, "FFFF"); // make 2nd cs not jump to another
		Write_To_Rom(46991505, Items[Deku_Mask].Text_ID); // fix deku mask text
	}

    // remove woodfall intro cs from the entrance
	if (Setting_True("cutscenes", "Swamp_WoodfallIntro")) {
		Write_To_Rom(42440514, "FF");
	}

	// shorten the woodfall rising cs
	if (Setting_True("cutscenes", "Swamp_WoodfallTempleRising")) {
		Write_Cutscene_Rom(42439444, "woodfall_rising"); 
	}

    // remove the woodfall temple intro cs
	if (Setting_True("cutscenes", "Swamp_WoodfallTempleIntro")) {
		Write_To_Rom(35504954, "FF");
	}

    // remove the woodfall intro cs from back room
	if (Setting_True("cutscenes", "Swamp_WoodfallTempleBackIntro")) {
		Write_To_Rom(35504946, "FF");
	}

    // moon's tear
	if (Setting_True("cutscenes", "Termina_MoonsTear")) {
		Write_Cutscene_Rom(39375352, "tear_falling"); // shorten the tear falling cs
		Write_Cutscene_Rom(39381640,
			"tear_falling"); // shorten the tear falling cs for 3rd day
		Write_Cutscene_Rom(39383720,
			"tear_falling"); // shorten the tear falling for final hours
	}

    // prevent the skull kid cs from playing on the clock tower roof
	if (Setting_True("cutscenes", "ClockTown_SkullKidIntro")) {
		Write_To_Rom(15758392, "00000000"); // removes the cs
		Write_To_Rom(35246095, "38");       // fixes the background music
		Write_To_Rom(35356789, "95");       // fixes skull kid's y position
	}

    // moon
	if (Setting_True("cutscenes", "ClockTown_GiantsStoppingMoon")) {
		Write_Short_Giant(); // remove the giants moon cs

		// remove the cucco shack 1AB actor to prevent a crash that occurs because of the
		// giant's cs skip
		Remove_Actor("27EB000", 20);
	}

	// make owl's textbox for SoS short
	if (Setting_True("cutscenes", "Swamp_SongOfSoaringOwl")) {
		Write_To_Rom(11478999,
			"4E6F7420796F7520616761696E21BF"); 
	}

    // oath
	if (Setting_True("cutscenes", "Termina_OathToOrder")) {
		Write_Cutscene_Rom(47718668, "oath_cs");     // make the oath cs shorter
		Write_To_Rom(47724913, Items[Oath].Text_ID); // fix oath's text id
		if (Songs_Same_Pool)
		{
			Write_To_Rom(47724865,
				Items[Oath].Song1_ID); // fix oath right song, if randomizing the
									   // songs that are actually played
			Write_To_Rom(47724889,
				Items[Oath].Song2_ID); // fix what the player plays, if randomizing
									   // the songs that are actually played
		}
	}

	// remove the deku princess cs after beating woodfall
	if (Setting_True("cutscenes", "Swamp_FreeingDekuPrincess")) {
		Write_To_Rom(35492834, "0000"); 
	}

	// shorten the woodfall cleaning cs after beating woodfall
	if (Setting_True("cutscenes", "Swamp_SwampClearing")) {
		Write_Cutscene_Rom(
			42455024,
			"woodfall_cleaning"); 
	}

    // couples mask
	if (Setting_True("cutscenes", "ClockTown_CouplesMask")) {
		Write_Cutscene_Rom(46642260, "couples");        // shorten the couple's mask cs
		Write_To_Rom(46644305, Items[Couples].Text_ID); // fix the text for the shorter
														// couples
	}

    // Returning Deku Princess
	if (Setting_True("cutscenes", "Swamp_ReturningTheDekuPrincess")) {
		Write_To_Rom(
			41399030,
			"0000"); // make the cs terminator at 0 frames in returning the deku princess cs
		Write_To_Rom(41399254, "0000"); // make the 2nd phase of the princess cs 0 frames
	}

    // Lullaby Intro
	if (Setting_True("cutscenes", "Mountain_LullabyIntro")) {
		Write_Cutscene_Rom(44660088, "lullaby_intro");  // day 3
		Write_Cutscene_Rom(46240568, "lullaby_intro");  // other days
		Write_Cutscene_Rom(44664824, "lullaby_intro2"); // day 3 opposite angle
		Write_Cutscene_Rom(46245304, "lullaby_intro2"); // other days opposite angle
	}

    // Great Fairy in Clock Town
	if (Setting_True("cutscenes", "ClockTown_GreatFairy")) {
		Write_Cutscene_Rom(38001628,
			"great_fairy_magic_mask"); // Shorten Great Fairy Mask + Magic CS
		Write_To_Rom(38005181, Items[GFM].Text_ID);   // fix the text id
		Write_Cutscene_Rom(38011356,
			"great_fairy_mask");     // shorten Great Fairy Mask on same cycle
		Write_To_Rom(38014181, Items[GFM].Text_ID); // fix the text id
		Write_Cutscene_Rom(37946092, "great_fairy_magic_1"); // shorten Magic CS
		Write_Cutscene_Rom(38007244, "great_fairy_magic_2"); // shorten phase 2 of the magic
															 // cs
	}

	// remove the tatl text trigger in the fountain
	if (Setting_True("cutscenes", "ClockTown_GreatFairyTatlTrigger")) {
		Remove_Actor("2454000", 4); 
	}

    // remove the mountain village intro cs from the entrance
	if (Setting_True("cutscenes", "Mountain_MountainVillageIntro")) {
		Write_To_Rom(44669566, "FF");
	}

    // remove the goron shrine into cs from the entrance
	if (Setting_True("cutscenes", "Mountain_GoronShrineIntro")) {
		Write_To_Rom(40091518, "FF");
	}

    // remove the snowhead intro cs from the entrance
	if (Setting_True("cutscenes", "Mountain_SnowheadIntro")) {
		Write_To_Rom(46178166, "FF");
	}

    // remove the snowhead temple intro cs from the entrance
	if (Setting_True("cutscenes", "Mountain_SnowheadTempleIntro")) {
		Write_To_Rom(36496622, "FF");
	}

	// make the giant goron in snowhead sleep faster
	if (Setting_True("cutscenes", "Mountain_GiantGoronFallingAsleep")) {
		Write_Cutscene_Rom(46175216,
			"goron_sleep"); 
	}

    // remove the great bay coast into cs from the entrance
	if (Setting_True("cutscenes", "GreatBay_GreatBayCoastIntro")) {
		Write_To_Rom(40639162, "FF");
	}

    // remove the zora hall intro cs from the entrances
	if (Setting_True("cutscenes", "GreatBay_ZoraHallIntro")) {
		Write_To_Rom(40232106, "FF");
		Write_To_Rom(40232114, "FF");
	}

	// shorten the turtle rising cs
	if (Setting_True("cutscenes", "GreatBay_TurtleRising")) {
		Write_Cutscene_Rom(40879224, "turtle"); 
	}

	// skip pirate cs altogether when entering gbt
	if (Setting_True("cutscenes", "GreatBay_PiratesWhenEnteringGBT")) {
		Write_To_Rom(40892756, "8C10");
	}
    
    // remove the great bay temple intro cs from the entrances
	if (Setting_True("cutscenes", "GreatBay_GreatBayTempleIntro")) {
		Write_To_Rom(42838550, "FF");
		Write_To_Rom(42838542, "FF");
	}

	//shorten entering great bay temple
	if (Setting_True("cutscenes", "GreatBay_EnteringGBT")) {
		Write_To_Rom(40887446, "0001"); // make riding the turtle intro gbt 1 frame
		Write_To_Rom(40887983, "01");   // shorten the 2nd time entering gbt with turtle
	}

    // Goron Mask
	if (Setting_True("cutscenes", "Mountain_GoronMask")) {
		Write_Cutscene_Rom(44479080, "goron_mask");   // shorten the goron mask cs
		Write_To_Rom(44480645, Items[Goron].Text_ID); // fix the goron mask text
	}

    // Zora Mask
	if (Setting_True("cutscenes", "GreatBay_ZoraMask")) {
		Write_Cutscene_Rom(40631980, "zora_mask");   // shorten zora mask cs
		Write_To_Rom(40633689, Items[Zora].Text_ID); // Fix zora mask text
	}

    // Goron Lullaby
	if (Setting_True("cutscenes", "Mountain_GoronLullaby")) {
		Write_Cutscene_Rom(40087880, "goron_lullaby");  // shorten goron lullaby cs
		Write_To_Rom(40091500, "5E20");                 // skip phase 2 of the cs
		Write_Cutscene_Rom(40088936, "goron_lullaby2"); // shorten goron lullaby cs phase 3
		Write_To_Rom(16480271,
			"68"); // make zfg stay asleep on frame 0x68 in the phase 3 cutscene
		Write_To_Rom(40090225, Items[Lullaby].Text_ID); // fix lullaby text
		if (Songs_Same_Pool)
		{
			Write_To_Rom(40088625, Items[Lullaby].Song1_ID); // fix lullaby display song
			Write_To_Rom(40088649, Items[Lullaby].Song2_ID); // fix lullaby play song
		}
	}

    // shorten Mikau walking cs
	if (Setting_True("cutscenes", "GreatBay_MikauWalking")) {
		Write_To_Rom(16751692, "3C014120"); // make right leg faster
		Write_To_Rom(16751668, "3C014120"); // make left leg faster
	}

    // remove cs before pushing mikau
	if (Setting_True("cutscenes", "GreatBay_MikauTextInWater")) {
		Write_To_Rom(16746330, "4E34"); // remove the text before pushing Mikau
		Write_To_Rom(16746872,
			"00000000"); // remove the flag that makes mikau in always talking mode
	}

    // shorten NWBN cs
	if (Setting_True("cutscenes", "GreatBay_NewWaveBossaNova")) {
		Write_Cutscene_Rom(39850256, "nwbn"); // shorter cs
		if (Songs_Same_Pool)
		{
			Write_To_Rom(39852449,
				Items[NW].Song2_ID); // fix what the player plays, if randomizing the
									 // songs that are actually played
		}
		Write_To_Rom(39852473, Items[NW].Text_ID); // fix the text id for what NW gives
	}

	// skip SoT cs
	if (Setting_True("cutscenes", "Termina_PlayingSongOfTime")) {
		Write_To_Rom(13421880, "00000000"); 
	}

    // gfs
	if (Setting_True("cutscenes", "Ikana_GreatFairySword")) {
		Write_Cutscene_Rom(37958316, "gfs");        // shorten the great fairy sword cs
		Write_To_Rom(37960501, Items[GFS].Text_ID); // fix the shorter gfs text
	}

    // remove the actor that plays the tatl text cs inside the 2nd room in woodfall temple
    //Remove_Actor("21FB000", 8);

	// shorten the boss warp cs in woodfall temple
	if (Setting_True("cutscenes", "Swamp_WoodfallBossWarpPad")) {
		Write_Cutscene_Rom(
			35501876,
			"woodfall_temple_ye_holds"); 
	}

	// shorten beating snowhead temple cs
	if (Setting_True("cutscenes", "Mountain_SnowMelting")) {
		Write_Cutscene_Rom(45984796, "snowhead_beaten");
	}

	// shorten the boss warp cs in snowhead temple
	if (Setting_True("cutscenes", "Mountain_SnowheadBossWarpPad")) {
		Write_Cutscene_Rom(
			36492872,
			"snowhead_temple_ye_holds");
	}
 
    // remove the actor that plays the intro cs in pirate's fortress exterior
	if (Setting_True("cutscenes", "GreatBay_PiratesFortressExteriorIntro")) {
		Remove_Actor("02740000", 23);
	}

    // remove the intro cs for pirate's fortress from the entrance
	if (Setting_True("cutscenes", "GreatBay_PiratesFortressIntro")) {
		Write_To_Rom(34146526, "FF");
	}

    // remove the actor that plays the bee entering the beehive cs
	if (Setting_True("cutscenes", "GreatBay_BeeEnteringBeehive")) {
		Remove_Actor("238B000", 20);
	}

    // remove the gorman racetrack intro cs from the entrances
	if (Setting_True("cutscenes", "Termina_GormanRacetrackIntro")) {
		Write_To_Rom(47792614, "FF");
		Write_To_Rom(47792622, "FF");
	}

    // remove the ranch intro cs from the entrance
	if (Setting_True("cutscenes", "Termina_RomaniRanchIntro")) {
		Write_To_Rom(40446482, "FF");
	}

    // Kart Ride
	if (Setting_True("cutscenes", "Termina_KartRide")) {
		Write_To_Rom(16639926, "CE40"); // load gorman track when starting kart ride
		Write_Cutscene_Rom(
			47789736, "kart_ride"); // remove the text for kart ride to speed it up a little
	}

    // remove the ikana graveyard intro cs from the entrance
	if (Setting_True("cutscenes", "Ikana_IkanaGraveyardIntro")) {
		Write_To_Rom(41889786, "FF");
	}

    // sharp sos cs
	if (Setting_True("cutscenes", "Ikana_Sharp")) {
		Write_Cutscene_Rom(
			33777340,
			"sharp_spawning"); // shorten sharp spawning cs inside the water grave thing
		Write_Cutscene_Rom(33781724, "sharp_sos"); // shorten the sharp sos cutscene phase 1
		Write_To_Rom(33797616, "20A0");            // skip the music box cs
		Write_To_Rom(33794114, "0000");            // make the final cs 0 frames
	}

    // gibdo mask
	if (Setting_True("cutscenes", "Ikana_GibdoMask")) {
		Write_Cutscene_Rom(45389024, "gibdo"); // shorten the gibdo mask cs
		Write_To_Rom(45391773,
			Items[Gibdo].Text_ID); // update the shortened cs's gibdo mask text
	}

    // remove the ikana canyon intro cs from the entrance
	if (Setting_True("cutscenes", "Ikana_IkanaCanyonIntro")) {
		Write_To_Rom(33797642, "FF");
	}

    // shorten circus leader's mask cs
	if (Setting_True("cutscenes", "ClockTown_CircusLeadersMask")) {
		Write_To_Rom(34301875, "00"); // make the first 3 cutscenes 0 frames long (it plays
									  // the same one three times)
		Write_Cutscene_Rom(34300444, "clm"); // shorten the circus leader's mask cs
	}

	// remove the actor that starts the cs in the main room in snowhead temple
	if (Setting_True("cutscenes", "Mountain_SnowheadTempleMainRoom")) {
		Remove_Actor("230C000", 70);
	}

	// shorten captain's hat cs
	if (Setting_True("cutscenes", "Ikana_CaptainsHat")) {
		Write_Cutscene_Rom(41882900, "cap"); 
	}

    // shorten the cs before epona archery
	if (Setting_True("cutscenes", "Termina_EponaSong")) {
		Write_To_Rom(15880699,
			"A0"); // play the ranch cs instead of the alien, skipping the alien cs
		Write_To_Rom(15880712,
			"00000000"); // prevent the game from freezing when loading the cs
	}

	// shorten flat spawning cs
	if (Setting_True("cutscenes", "Ikana_Flat")) {
		Write_Cutscene_Rom(33034640, "flat_spawning"); 
	}

	// remove gyorg cs
	if (Setting_True("cutscenes", "GreatBay_GyorgIntro")) {
		Write_To_Rom(15011312, "00000000");
	}

    // remove the ikana castle cutscene actors
	if (Setting_True("cutscenes", "Ikana_IkanaCastleIntro")) {
		Remove_Actor("2266000", 45);
		Remove_Actor("2266000", 46);
		Remove_Actor("2266000", 28); // remove actor that prevented the bg music from playing
	}

    // Shorten Elegy cs
	if (Setting_True("cutscenes", "Ikana_ElegyOfEmptiness")) {
		Write_Cutscene_Rom(45543908, "elegy");        // import shorter cs
		Write_To_Rom(45546381, Items[Elegy].Text_ID); // update the elegy text
		if (Songs_Same_Pool)
		{
			Write_To_Rom(45546333, Items[Elegy].Song1_ID); // fix elegy display song
			Write_To_Rom(45546357, Items[Elegy].Song2_ID); // fix elegy play song
		}
	}

	// shorten the Igos cs
	if (Setting_True("cutscenes", "Ikana_Igos")) {
		Shorten_Igos_CS(); 
	}

    // shorten sakon hideout cutscene
	if (Setting_True("cutscenes", "Ikana_SakonsHideout")) {
		Write_Cutscene_Rom(44548416, "sakon_hideout"); // shorten the starting cs
		Write_To_Rom(44552390, "003C");                // shorten the doorway cs
		Write_To_Rom(44553062, "003C");                // shorten the switch cs
		Write_To_Rom(12482972, "24011471");			   // make the notebook kafei entry look for the 2nd text after saving sun mask
		Write_To_Rom(15374732, "1000001C24020002");	   //remove kafei running cs after saving sun's mask
	}

	// shorten ye who holds remains cs in gbt
	if (Setting_True("cutscenes", "GreatBay_GreatBayBossWarpPad")) {
		Write_Cutscene_Rom(
			42835544, "great_bay_temple_ye_holds"); 
	}

	// skip the goht room entrance cs
	if (Setting_True("cutscenes", "Mountain_GohtIntro")) {
		Write_To_Rom(16165132, "00000000");
	}

	// shorten ye who hold remains cs in stt
	if (Setting_True("cutscenes", "Ikana_StoneTowerBossWarpPad")) {
		Write_Cutscene_Rom(
			34878820, "stone_tower_temple_ye_holds");
	}

	// shorten evan hp cs
	if (Setting_True("cutscenes", "GreatBay_EvanHP")) {
		Write_To_Rom(44042848, "9260");
	}

	// remove stt intro cs
	if (Setting_True("cutscenes", "Ikana_StoneTowerTempleIntro")) {
		Write_To_Rom(34403946, "FF");
	}

	// remove istt intro cs
	if (Setting_True("cutscenes", "Ikana_InvertedSTTIntro")) {
		Write_To_Rom(34881666, "FF");
	}

	// remove ist intro cs
	if (Setting_True("cutscenes", "Ikana_InvertedStoneTowerIntro")) {
		Write_To_Rom(45831950, "FF");
	}

    // remove the actor that plays the hms cs inside the clock tower
    // make hms think you already watched the cs
	if (Setting_True("cutscenes", "ClockTown_HappyMaskSalesman")) {
		Remove_Actor("2CD6000", 11);
		Write_To_Rom(15953540, "24020001");
		Write_To_Rom(15955712, "24020001");
	}

	//remove clock tower opening cs
	if (Setting_True("cutscenes", "ClockTown_ClockTowerOpening")) {
		Remove_Clock_Tower_CS();
	}

	//remove waterfall rapids intro cs
	if (Setting_True("cutscenes", "GreatBay_WaterfallRapidsIntro")) {
		Write_To_Rom(43430098, "FF");
	}

	//remove Stone Tower intro cs
	if (Setting_True("cutscenes", "Ikana_StoneTowerIntro")) {
		Write_To_Rom(45696914, "FF");
	}

	// make link go fast in isnt that your horse cs
	/*if (Setting_True("cutscenes", "Termina_IsntThatYourHorse")) {
		Write_Cutscene_Rom(40444324,
			"your_horse");
	}*/

	//remove isn't that your horse cutscene
	if (Setting_True("cutscenes", "Termina_IsntThatYourHorse")) {
		Remove_Actor("26AD000", 1);
	}
}

///Writes data from a file to the rom
void Write_File_To_Rom(string filename, string rom_offset)
{
	constexpr int limit = 2;
	char data[limit + 1];
	string String_Data = "";
	ifstream file;
	int cur = 0;
	int address = hex_to_decimal(rom_offset);

	file.open(filename, fstream::binary | fstream::out | fstream::in);

	while (file.good())
	{
		file.seekg(cur);
		file.read(data, limit);
		String_Data += Char_To_String(data, limit);
		cur += limit;
	}

	file.close();

	Write_To_Rom(address, string_to_hex(String_Data));
}

///Activate the GC HUD
void Gamecube_Hud()
{
	// A Button
	Write_To_Rom(12251938, "64FF"); // red/green
	Write_To_Rom(12251958, "78");   // blue

	// B
	Write_To_Rom(12244714, "00FF"); // red
	Write_To_Rom(12244718, "0064"); // green
	Write_To_Rom(12244706, "0064"); // blue

	// Start
	Write_To_Rom(12245214, "0078"); // red
	Write_To_Rom(12245202, "0078"); // green
	Write_To_Rom(12245206, "0078"); // blue

	// Change Z button in pause screen to L
	Write_File_To_Rom(".\\files\\l.yaz0", "A7B7CC");
}

string Get_Log_Start(string seed) {
	ifstream log_start;
	string log_text = "";
	string line = "";

	log_start.open("./files/LogStart.html");
	while (log_start >> line) {
		log_text += line + "\n";
	}
	log_start.close();

	log_text += seed;

	log_start.open("./files/LogStart2.html");
	while (log_start >> line) {
		log_text += line + "\n";
	}
	log_start.close();

	return log_text;
}

string Get_Log_End() {
	ifstream log_end;
	string log_text = "";
	string line = "";

	log_end.open("./files/LogEnd.html");

	while (log_end >> line) {
		log_text += line + "\n";
	}

	log_end.close();

	return log_text;
}

//writes the spoiler log as html file
void Write_HTML_Log(string seed, map<string, vector<vector<string>>> &Items_Needed, map<string, int> &Location_Sets) {
	vector<string> items = Get_Keys(Items);
	string item;
	string new_item;
	string html = "";
	vector<string> Needed_Items;
	ofstream html_log;
	vector<string> classes = { "even", "odd" };
	int class_index;
	string row_class;

	html += Get_Log_Start(seed);

	for (int i = 0; i < items.size(); i++)
	{
		item = items[i];
		new_item = Items[item].Name;
		class_index = i % 2;
		row_class = classes[class_index];

		if (Items_Needed[item].size() > 0) {
			Needed_Items = Items_Needed[item][Location_Sets[item]];
		}
		else {
			Needed_Items = {""};
		}

		html += "		<div class = \"item " + row_class + "\" id = \"item_";
		html += to_string(i);
		html += "\">\n";
		html += "			<div class = \"item_source\" id = \"item_source_";
		html += to_string(i);
		html += "\">\n";
		html += "					" + item + "\n";
		html += "			</div>\n";
		html += "			<div class = \"item_gives\">\n";
		html += "				<div>\n";
		html += "					<div class=\"items_gives_header\">\n";
		html += "						<div class=\"gives_text\">\n";
		html += "							Item Gives\n";
		html += "						</div>\n";
		html += "						<div class=\"down_arrow item_button\" onclick = \"Cover(";
		html += to_string(i);
		html += "); \">\n";
		html += "							^\n";
		html += "						</div>\n";
		html += "					</div>\n";
		html += "				</div>\n";
		html += "				<div class = \"item_give cover\" id = \"item_give_";
		html += to_string(i);
		html += "\">\n";
		html += "					" + new_item + "\n";
		html += "				</div>\n";
		html += "			</div>\n";
		html += "			<div class = \"items_needed\" id = \"items_needed_";
		html += to_string(i);
		html += "\">\n";
		html += "				<div class=\"items_needed_header\">";
		html += "					Items Needed\n";
		html += "				</div>\n";
		html += "				<div class = \"items_needed_items\">\n";
		html += "				" + Vector_To_String(Needed_Items, ", "); +"\n";
		html += "				</div>\n";
		html += "			</div>\n";
		html += "		</div>\n";
	}

	html += "<script type = \"text/javascript\">\n";
	html += "		Total_Items = " + to_string(items.size());
	html += ";\n";
	html += "</script>\n";

	html += Get_Log_End();

	html_log.open("Spoiler Log.html");

	html_log << html;

	html_log.close();

	system("copy \"files\\Log.js\" \"javascript.js\"");
	system("copy \"files\\Log.css\" \"style.css\"");
}

//Writes the spoiler log as txt file
void Write_Log(string seed, map<string, vector<vector<string>>> &Items_Needed, map<string, int> &Location_Sets)
{
    vector<string> items = Get_Keys(Items);
    string item;
    string new_item;
	vector<string> Item_Set;
	int Set_Index;

	Logger("Write_Log() - start");

    outFile << "Seed: " << seed << "\n\n";

    outFile << "Old Item => New Item\n\tItems the logic used to determine that this item "
               "is accessible\n\n";

    for (int i = 0; i < items.size(); i++)
    {
        item = items[i];
        new_item = Items[item].Name;

        outFile << item << " => " << new_item << "\n";
		Logger(item + " gives " + new_item);

		Set_Index = Location_Sets[item];
		Logger("Set index = " + dec_to_string(Set_Index));
        // if items were needed to obtain this item
        if (Set_Index >= 0)
        {
			if (Items_Needed[item].size() > 0) {
				Item_Set = Items_Needed[item][Set_Index];
				Logger("Item Set: " + Vector_To_String(Item_Set, ", "));
				outFile << "\t" << Vector_To_String(Item_Set, ", ") << "\n";
			}
			else {
				Logger("No items needed to check " + item);
			}
        }

        outFile << endl;
    }

	Write_HTML_Log(seed, Items_Needed, Location_Sets);

	Logger("Write_Log() - end");
}

///Change all the references of spring water to bingo water
void Bingo_Water()
{
    // bingo water
    // get item text
    Write_To_Rom(11352237,
                 string_to_hex("Bingo Water") +
                   "002118111F000A5573652004B20020746F20706F7572206974206F6E2077686174657"
                   "66572116D6179206E6565642069742EBF");

    // sign next to water under witch's potion shop
    Write_To_Rom(11376723,
                 string_to_hex("Bingo Water") + "20686F6D65207769746820796F752E11002020");

    // Ikana cave sign
    Write_To_Rom(11379624,
                 string_to_hex("Bingo Water") +
                   "20436176651100456E7472792070726F686962697465642064756520746F2067686F7"
                   "374117369676874696E677321BF");

    // Sharp talking about flat and the cave?
    Write_To_Rom(11568975, string_to_hex("Bingo Water") + "2063617665002E19BF");

    // pause menu text
    Write_To_Rom(11608680,
                 string_to_hex("Bingo Water") +
                   "1100547279207573696E67206974207769746820B2206F6E207468696E67731174686"
                   "174206E656564207761746572696E672EBF");

    // write bingo water image for bottom of pause menu
    Write_File_To_Rom(".\\files\\BW.yaz0", "A2A6D4");

    // hot bingo water
    // get item text
    Write_To_Rom(11352345,
                 string_to_hex("Bingo Water") +
                   "002118111F000A55736520697420776974682004B200206265666F726520697420636"
                   "F6F6C732EBF");

    // goron text next to goron gravewyard
    Write_To_Rom(11507742,
                 string_to_hex("Bingo Water") +
                   "00204920666F756E64207768656E1149207761732064696767696E672074686520686"
                   "5726F27732067726176652E19BF");

    // pause menu text
    Write_To_Rom(11608760,
                 string_to_hex("Bingo Water") +
                   "1100557365206974207769746820B2206265666F726520697420636F6F6C732EBF");

    // write hot bingo water image for bottom of pause menu
    Write_File_To_Rom(".\\files\\HBW.yaz0", "A2A8A4");
}

///Changes the blast mask cooldown
void Change_BlastMask(map<string, string> custom_settings)
{
    int frames = string_to_dec(custom_settings["BlastMask_Cooldown"]);
    string frames_hex = dec_to_hex(frames);

    frames_hex = Leading_Zeroes(frames_hex, 4);

    Write_To_Rom(13280870, frames_hex);
}

///Fixes swords from getting on transformations B buttons
void Fix_Swords()
{
    int address = 12228012;
    vector<string> commands = {
      "10800004", // BEQ	A0, R0, FD	Skip saving to b button if FD
      "A518006C", // SH	T8, 0x006C (T0)	Save sword to inv
      "93AE0047", // LBU	T6, 0x0047 (SP)	Load Sword Item ID
      "01007821", // ADDU	T7, T0, R0	Move T0 to T7
      "A1EE004C", // SB	T6, 0x003C (T7)	Save sword to B button
      "00000000", // NOPs
      "00000000", //
      "00000000", //
      "00000000"  //
    };

    for (int c = 0; c < commands.size(); c++)
    {
        Write_To_Rom(address + (c * 4), commands[c]);
    }
}

/// Makes it where you can get each HP once per cycle
void RespawnHPs()
{
    Write_To_Rom(12962814, "00"); // remove SoT flag mask for hp in south clock town 1
    Write_To_Rom(12962063,
                 "00"); // remove SoT flag mask for hp in the tree in road to swamp  2
    Write_To_Rom(12962140, "00"); // remove SoT flag mask for swamp scrub hp   3
    Write_To_Rom(12961340, "00"); // remove SoT flag mask for ikana scrub hp   4
    Write_To_Rom(12961148,
                 "00000000"); // remove SoT flag mask for Peahat Grotto HP (0x80), Dodongo
                              // Grotto HP (0x40), and zora grotto hp (0x4)    5, 6, 7
    Write_To_Rom(
      12963012,
      "00000000"); // make business scrub and gossip stones hp flag unset on SoT 8, 9
    Write_To_Rom(12961724, "00");       // remove SoT flag mask for deku palace hp   10
    Write_To_Rom(12961228, "00");       // remove SoT flag mask for grave night 2 hp 11
    Write_To_Rom(12961711, "00");       // remove SoT flag mask for deku trial hp    12
    Write_To_Rom(12962047, "00");       // remove SoT flag for goron trial hp    13
    Write_To_Rom(12962175, "00");       // remove SoT flag for zora trial hp 14
    Write_To_Rom(12962671, "00");       // remove SoT flag for human trial hp  15
    Write_To_Rom(12962932, "00000020"); // remove grandma sot flags  16, 17
    Write_To_Rom(15521792, "00000000"); // make banker ignore if you've gotten the hp 18
    Write_To_Rom(12962798, "00"); // remove SoT flag for North Clock Town Tree HP  19
    Write_To_Rom(12961392,
                 "00000000"); // remove SoT flag for treasure chest minigame hp    20
    Write_To_Rom(
      14508924,
      "00000000"); // Make honey and darling ignore if you've already gotten the hp   21
    Write_To_Rom(17100479, "08"); // make mayor give hp once per cycle   22
    // make town archery hp once per cycle   23
    Write_To_Rom(14914415, "08");
    Write_To_Rom(14914227, "08");
    Write_To_Rom(14914235, "08");
    // make swamp archery hp once per cycle  24
    Write_To_Rom(14913743, "08");
    Write_To_Rom(14913543, "08");
    Write_To_Rom(14913559, "08");
    // make witch swamp archery gives hp once per cycle  25
    Write_To_Rom(15445676, "80");
    Write_To_Rom(15445619, "80");
    // make jump minigame hp gives once per cycle    26
    Write_To_Rom(17274148, "33190020");
    Write_To_Rom(17274104, "344F0020");
    Write_To_Rom(12961919, "02"); // remove SoT flag for great bay coast wall hp (not sure
                                  // what the 2 here is for, so I am leaving it) 27
    Write_To_Rom(12962882, "0000"); // make beaver hp once per cycle 28
    Write_To_Rom(12962252, "00");   // remove SoT flag for zora hall scrub hp    29
    Write_To_Rom(12962268, "00");   // remove SoT flag for goron village scrub hp    30
    Write_To_Rom(12962494, "00");   // remove SoT flag for path to snowhead hp   31
    Write_To_Rom(12962940, "00000000"); // pow hut hp gives once per cycle   32
    Write_To_Rom(17182338,
                 "0C"); // make ??? give hp instead of blue rupee after flag is set    33
    Write_To_Rom(17175348, "10000005"); // rosa sisters ignores hp flag  34
    Write_To_Rom(12961502, "00");       // remove SoT flag for ikana castle hp   35
    Write_To_Rom(12962527,
                 "00"); // remove SoT flag for chest hp in path to goron village 36
    Write_To_Rom(12962987, "00"); // remove SoT flag for postman minigame hp   37
    Write_To_Rom(12962995, "00"); // remove SoT flag for postbox hp    38
    Write_To_Rom(12961676, "00"); // remove SoT flag for oceanside spider house hp 39
    Write_To_Rom(12962158, "00"); // remove SoT flag for woodfall chest hp 40
    Write_To_Rom(12962945, "00"); // remove SoT flag for marine fish lab hp    41
    Write_To_Rom(16519940, "10000004"); // make pinnacle rock seahorse always give hp 42
    Write_To_Rom(12961598, "00"); // remove SoT flag for pirate's fortress sewer hp    43
    Write_To_Rom(12962908, "0C000000"); // remove SoT flag for Evan hp   44
    // Make frog hp give once per cycle   45
    Write_To_Rom(13757063, "40");
    Write_To_Rom(13757111, "40");
    // Make keaton mask give hp once per cycle   46
    Write_To_Rom(17229367, "40");
    Write_To_Rom(17229335, "40");
    // Dog race hp gives once per cycle    47
    Write_To_Rom(14893171, "10");
    Write_To_Rom(14893179, "10");
    Write_To_Rom(14893227, "10");
    // Swordsman school gives once per cycle   48
    Write_To_Rom(16304667, "10");
    Write_To_Rom(16304839, "10");
    Write_To_Rom(16058812, "00000000"); // Deku Playground hp gives once per cycle   49
    // Picto hp gives once per cycle   50
    Write_To_Rom(16040736, "02");
    Write_To_Rom(16040751, "02");
    Write_To_Rom(12961935, "00"); // remove SoT flag for Zora Cape likelike hp 51
    Write_To_Rom(12962574, "00"); // remove SoT flag for secret shrine hp  52
}

void Setup_Items() {

	// number of times it can be gotten per cycle	name    item id     get item id   text id     flag   object   get item model  pool
	// = ""   get item locations  item id locations   text id locations
	Items["Adult Wallet"] = Item(
		1,
		"Adult Wallet",
		"5A",
		"08",
		"08",
		"A0",
		"00A8",
		"21",
		"",
		{ "CD688E" },
		{},
		"1_00010000",
		{ "C5CE6E" });
	Items["All-Night Mask"] = Item(
		1,
		"All-Night Mask",
		"38",
		"7E",
		"7E",
		"A0",
		"0265",
		"11",
		"",
		{ "CD6B52" },
		{ "C5CE3D" });
	Items["Big Bomb Bag"] = Item(
		0,
		"Big Bomb Bag",
		"57",
		"1C",
		"1C",
		"A0",
		"0098",
		"19",
		"",
		"06",
		{ "CD6906" },
		{ "C5CE2A" },
		"2_00010000",
		{ "C5CE6F" },
		"1E",
		{ "C5CE5A" }); // C5CE6F bomb slot
	Items["Biggest Bomb Bag"] = Item(
		0,
		"Biggest Bomb Bag",
		"58",
		"1D",
		"1D",
		"A0",
		"0098",
		"1A",
		"",
		"06",
		{ "CD690C" },
		{ "C5CE2A" },
		"3_00011000",
		{ "C5CE6F" },
		"28",
		{ "C5CE5A" }); // C5CE6F bomb slot
	Items["Blast Mask"] = Item(
		1,
		"Blast Mask",
		"47",
		"8D",
		"8D",
		"A0",
		"026D",
		"3B",
		"",
		{ "CD6BAC" },
		{ "C5CE3E" });
	Items["Bomb Bag"] = Item(
		0,
		"Bomb Bag",
		"56",
		"1B",
		"1B",
		"A0",
		"0098",
		"18",
		"",
		"06",
		{ "CD6900" },
		{ "C5CE2A" },
		"1_00001000",
		{ "C5CE6F" },
		"14",
		{ "C5CE5A" });
	Items["Bomber's Notebook"] = Item(
		0,
		"Bomber's Notebook",
		"6D",
		"50",
		"50",
		"80",
		"0253",
		"0C",
		"",
		{ "CD6A3E" },
		{},
		"F_00000100",
		{ "C5CE71" });
	Items["Bow"] = Item(
		1,
		"Bow",
		"01",
		"22",
		"22",
		"A0",
		"00BF",
		"2F",
		"",
		"01",
		{ "CD692A" },
		{ "C5CE25" },
		"1_00000001",
		{ "C5CE6F" },
		"1E",
		{ "C5CE55" }); // C5CE6F = quiver slot
	Items["Bremen Mask"] =
		Item(
			1,
			"Bremen Mask",
			"46",
			"8C",
			"8C",
			"A0",
			"025A",
			"10",
			"",
			{ "CD6BA6" },
			{ "C5CE43" });
	Items["Bunny Hood"] = Item(
		1,
		"Bunny Hood",
		"39",
		"7F",
		"7F",
		"A0",
		"0103",
		"3F",
		"",
		{ "CD6B58" },
		{ "C5CE44" });
	Items["Captain's Hat"] = Item(
		1,
		"Captain's Hat",
		"44",
		"7C",
		"7C",
		"A0",
		"0102",
		"3E",
		"",
		{ "CD6B46" },
		{ "C5CE51" });
	Items["Circus Leader's Mask"] =
		Item(
			1,
			"Circus Leader's Mask",
			"3D",
			"83",
			"83",
			"A0",
			"0259",
			"0F",
			"",
			{ "CD6B70" },
			{ "C5CE49" });
	Items["Couple's Mask"] = Item(
		1,
		"Couple's Mask",
		"3F",
		"85",
		"85",
		"A0",
		"0282",
		"04",
		"",
		{ "CD6B7C" },
		{ "C5CE4B", "F125C3" },
		{ "2C7CBD5" });
	Items["Deku Nuts"] = Item(
		0,
		"Deku Nuts",
		"09",
		"28",
		"28",
		"0C",
		"0094",
		"EE",
		"",
		{ "CD694E" },
		{ "C5CE2D" },
		"01",
		{ "C5CE5D" });
	Items["Deku Nuts (10)"] = Item(
		0,
		"Deku Nuts (10)",
		"8E",
		"2A",
		"2A",
		"0C",
		"0094",
		"EE",
		"",
		"09",
		{ "CD695A" },
		{ "C5CE2D" },
		"0A",
		{ "C5CE5D" });
	Items["Deku Stick"] = Item(
		0,
		"Deku Stick",
		"08",
		"19",
		"19",
		"0D",
		"009F",
		"E5",
		"",
		"08",
		{ "CD68F4" },
		{ "C5CE2C" },
		"01",
		{ "C5CE5C" });
	Items["Don Gero's Mask"] = Item(
		1,
		"Don Gero's Mask",
		"42",
		"88",
		"88",
		"A0",
		"0266",
		"23",
		"",
		{ "CD6B8E" },
		{ "C5CE45" });
	Items["Express Letter to Mama"] = Item(
		1,
		"Express Letter to Mama",
		"2E",
		"A1",
		"A1",
		"80",
		"0245",
		"37",
		"",
		{ "CD6C24" },
		{ "C5CE2F" });
	Items["Fierce Deity Mask"] = Item(
		1,
		"Fierce Deity Mask",
		"35",
		"7B",
		"7B",
		"A0",
		"0242",
		"76",
		"",
		{ "CD6B40" },
		{ "C5CE53" });
	Items["Fire Arrow"] = Item(
		1,
		"Fire Arrow",
		"02",
		"25",
		"25",
		"A0",
		"0121",
		"48",
		"",
		{ "CD693C" },
		{ "C5CE26" });
	Items["Garo Mask"] = Item(
		1,
		"Garo Mask",
		"3B",
		"81",
		"81",
		"A0",
		"0209",
		"6A",
		"",
		{ "CD6B64" },
		{ "C5CE50" });
	Items["Giant Wallet"] = Item(
		1,
		"Giant Wallet",
		"5B",
		"09",
		"09",
		"A0",
		"00A8",
		"22",
		"",
		{ "CD6894" },
		{},
		"2_00100000",
		{ "C5CE6E" });
	Items["Giant's Mask"] = Item(
		1,
		"Giant's Mask",
		"49",
		"7D",
		"7D",
		"A0",
		"0226",
		"73",
		"",
		{ "CD6B4C" },
		{ "C5CE52" });
	Items["Gibdo Mask"] = Item(
		1,
		"Gibdo Mask",
		"41",
		"87",
		"87",
		"A0",
		"020B",
		"6C",
		"",
		{ "CD6B88" },
		{ "C5CE4F", "F12C7F" },
		{ "2B4A569" });
	Items["Gilded Sword"] = Item(
		2,
		"Gilded Sword",
		"4F",
		"39",
		"39",
		"A0",
		"01FA",
		"68",
		"",
		{ "CD69B4", "CD6C12" },
		{ "C5CE00" },
		"3_00000011",
		{ "C5CE21" }); // C5CE21 Inv sword/shield
	Items["Great Fairy's Mask"] = Item(
		1,
		"Great Fairy's Mask",
		"40",
		"86",
		"86",
		"A0",
		"020A",
		"6B",
		"",
		{ "CD6B82" },
		{ "C5CE40", "EA3F53", "EA40FB" },
		{ "243F195", "24415CD" });
	Items["Great Fairy's Sword"] = Item(
		2,
		"Great Fairy's Sword",
		"10",
		"3B",
		"3B",
		"A0",
		"01FB",
		"69",
		"",
		{ "CD69C0", "CD6C00" },
		{ "C5CE34", "EA3F8B" },
		{ "243413D" });
	Items["Hero's Shield"] = Item(
		0,
		"Hero's Shield",
		"51",
		"32",
		"32",
		"A0",
		"00B3",
		"D8",
		"",
		{ "CD698A", "CD6C18" },
		{},
		"1_00010000",
		{ "C5CE21" });
	Items["Hookshot"] = Item(
		1,
		"Hookshot",
		"0F",
		"41",
		"41",
		"A0",
		"00B4",
		"29",
		"",
		{ "CD69E4" },
		{ "C5CE33" });
	Items["Ice Arrow"] = Item(
		1,
		"Ice Arrow",
		"03",
		"26",
		"26",
		"A0",
		"0121",
		"49",
		"",
		{ "CD6942" },
		{ "C5CE27" });
	Items["Kafei's Mask"] =
		Item(
			0,
			"Kafei's Mask",
			"37",
			"8F",
			"8F",
			"A0",
			"0258",
			"0E",
			"",
			{ "CD6BB8" },
			{ "C5CE4A" });
	Items["Kamaro's Mask"] = Item(
		1,
		"Kamaro's Mask",
		"43",
		"89",
		"89",
		"A0",
		"027D",
		"03",
		"",
		{ "CD6B94" },
		{ "C5CE4E" });
	Items["Keaton Mask"] = Item(
		1,
		"Keaton Mask",
		"3A",
		"80",
		"80",
		"A0",
		"0100",
		"2D",
		"",
		{ "CD6B5E" },
		{ "C5CE42" });
	Items["Kokiri Sword"] = Item(
		2,
		"Kokiri Sword",
		"4D",
		"37",
		"9C",
		"A0",
		"0148",
		"56",
		"",
		{ "CD69A8", "CD6C06" },
		{ "C5CE00", "CBA4AF" },
		{ "CBA4E3" },
		"1_00000001",
		{ "C5CE21" },
		0); // C5CE21 Inv sword/shield
	Items["Land Title Deed"] = Item(
		1,
		"Land Title Deed",
		"29",
		"97",
		"97",
		"80",
		"01B2",
		"5B",
		"",
		{ "CD6BE8" },
		{ "C5CE29" });
	Items["Large Quiver"] = Item(
		1,
		"Large Quiver",
		"54",
		"23",
		"23",
		"A0",
		"0097",
		"16",
		"",
		"01",
		{ "CD6930" },
		{ "C5CE25" },
		"2_00000010",
		{ "C5CE6F" },
		"28",
		{ "C5CE55" }); // C5CE6F = quiver slot
	Items["Largest Quiver"] = Item(
		1,
		"Largest Quiver",
		"55",
		"24",
		"24",
		"A0",
		"0097",
		"17",
		"",
		"01",
		{ "CD6936" },
		{ "C5CE25" },
		"3_00000011",
		{ "C5CE6F" },
		"32",
		{ "C5CE55" }); // C5CE6F = quiver slot
	Items["Lens of Truth"] = Item(
		1,
		"Lens of Truth",
		"0E",
		"42",
		"42",
		"A0",
		"00C0",
		"30",
		"",
		{ "CD69EA" },
		{ "C5CE32" });
	Items["Letter to Kafei"] = Item(
		1,
		"Letter to Kafei",
		"2F",
		"AA",
		"AA",
		"80",
		"0210",
		"6E",
		"",
		{ "CD6C5A" },
		{ "C5CE35" });
	Items["Light Arrow"] = Item(
		1,
		"Light Arrow",
		"04",
		"27",
		"27",
		"A0",
		"0121",
		"4A",
		"",
		{ "CD6948" },
		{ "C5CE28" });
	Items["Magic Beans"] = Item(
		0,
		"Magic Beans",
		"0A",
		"35",
		"35",
		"80",
		"00C6",
		"CB",
		"",
		{ "CD699C" },
		{ "C5CE2E" },
		"01",
		{ "C5CE5E" }); // get item id of what is given instead is 4F
	Items["Mask of Scents"] = Item(
		1,
		"Mask of Scents",
		"48",
		"8E",
		"8E",
		"A0",
		"027E",
		"3D",
		"",
		{ "CD6BB2" },
		{ "C5CE46" });
	Items["Mask of Truth"] = Item(
		1,
		"Mask of Truth",
		"36",
		"8A",
		"8A",
		"A0",
		"0104",
		"40",
		"",
		{ "CD6B9A" },
		{ "C5CE4C" });
	Items["Mirror Shield"] = Item(
		1,
		"Mirror Shield",
		"52",
		"33",
		"33",
		"A0",
		"00C3",
		"34",
		"",
		{ "CD6990" },
		{},
		"2_00100000",
		{ "C5CE21" });
	Items["Moon's Tear"] = Item(
		1,
		"Moon's Tear",
		"28",
		"96",
		"96",
		"80",
		"01B1",
		"5A",
		"",
		{ "CD6BE2" },
		{ "C5CE29" }); // CD7646 - another moon's tear for show item
	Items["Mountain Title Deed"] = Item(
		1,
		"Mountain Title Deed",
		"2B",
		"99",
		"99",
		"80",
		"01B2",
		"42",
		"",
		{ "CD6BF4" },
		{ "C5CE29" });
	Items["Ocean Title Deed"] = Item(
		1,
		"Ocean Title Deed",
		"2C",
		"9A",
		"9A",
		"80",
		"01B2",
		"44",
		"",
		{ "CD6BFA" },
		{ "C5CE29" });
	Items["Pendant of Memories"] =
		Item(
			1,
			"Pendant of Memories",
			"30",
			"AB",
			"AB",
			"80",
			"0215",
			"6F",
			"",
			{ "CD6C60" },
			{ "C5CE35" }); // other show items: CD764B, CD764C, and CD764D
	Items["Pictograph Box"] = Item(
		1,
		"Pictograph Box",
		"0D",
		"43",
		"43",
		"A0",
		"0228",
		"75",
		"",
		{ "CD69F0" },
		{ "C5CE31" });
	Items["Postman's Hat"] = Item(
		1,
		"Postman's Hat",
		"3E",
		"84",
		"84",
		"A0",
		"0225",
		"72",
		"",
		{ "CD6B76" },
		{ "C5CE3C" });
	Items["Powder Keg"] = Item(
		0,
		"Powder Keg",
		"0C",
		"34",
		"34",
		"80",
		"01CA",
		"5E",
		"",
		{ "CD6996" },
		{ "C5CE30" },
		"01",
		{ "C5CE60" });
	Items["Razor Sword"] =
		Item(
			2,
			"Razor Sword",
			"4E",
			"38",
			"38",
			"A0",
			"01F9",
			"67",
			"",
			{ "CD69AE", "CD6C0C" },
			{ "C5CE00" },
			"2_00000010",
			{ "C5CE21" }); // C5CE21 Inv sword/shield
	Items["Romani's Mask"] = Item(
		1,
		"Romani's Mask",
		"3C",
		"82",
		"82",
		"A0",
		"021F",
		"71",
		"",
		{ "CD6B6A" },
		{ "C5CE48" });
	Items["Room Key"] = Item(
		1,
		"Room Key",
		"2D",
		"A0",
		"A0",
		"80",
		"020F",
		"6D",
		"",
		{ "CD6C1E" },
		{ "C5CE2F" });
	Items["Stone Mask"] = Item(
		1,
		"Stone Mask",
		"45",
		"8B",
		"8B",
		"A0",
		"0254",
		"0D",
		"",
		{ "CD6BA0" },
		{ "C5CE3F" });
	Items["Swamp Title Deed"] = Item(
		1,
		"Swamp Title Deed",
		"2A",
		"98",
		"98",
		"80",
		"01B2",
		"41",
		"",
		{ "CD6BEE" },
		{ "C5CE29" });

	Items["Deku Mask"] = Item(
		0,
		"Deku Mask",
		"32",
		"78",
		"78",
		"A0",
		"01BD",
		"5C",
		"",
		{ "CD6B2E" },
		{ "C5CE41", "F11247" },
		{ "2CD0FF9" });
	Items["Goron Mask"] = Item(
		0,
		"Goron Mask",
		"33",
		"79",
		"79",
		"A0",
		"0119",
		"45",
		"",
		{ "CD6B34" },
		{ "C5CE47", "F12A5F" },
		{ "2A6BE19" });
	Items["Zora Mask"] = Item(
		0,
		"Zora Mask",
		"34",
		"7A",
		"7A",
		"A0",
		"011A",
		"46",
		"",
		{ "CD6B3A" },
		{ "C5CE4D", "F12B73" },
		{ "26C128D" });

	Items["Big Poe"] = Item(
		2,
		"Big Poe",
		"1E",
		"66",
		"66",
		"80",
		"0139",
		"53",
		"",
		{ "CD6AC2" },
		{ "C5CE36", "CD7C53" },
		{ "CD7C55" });
	Items["Blue Potion"] = Item(
		0,
		"Blue Potion",
		"15",
		"5D",
		"5D",
		"80",
		"00C1",
		"33",
		"",
		{ "CD6A8C" },
		{ "C5CE36", "CDE5BB" });
	Items["Bugs"] = Item(
		0,
		"Bugs",
		"1B",
		"63",
		"63",
		"80",
		"0137",
		"4C",
		"",
		{ "CD6AB0" },
		{ "C5CE36", "CD7C17", "CD7C1D" },
		{ "CD7C19", "CD7C1F" });
	Items["Chateau Romani"] = Item(
		3,
		"Chateau Romani",
		"25",
		"6F",
		"6F",
		"80",
		"0227",
		"74",
		"",
		{ "CD6AF8", "CD6BC4" },
		{ "C5CE36" });
	Items["Deku Princess"] = Item(
		1,
		"Deku Princess",
		"17",
		"5F",
		"5F",
		"80",
		"009E",
		"01",
		"",
		{ "CD6A98" },
		{ "C5CE36", "CD7C3B" },
		{ "CD7C3D" });
	Items["Fairy"] = Item(
		0,
		"Fairy",
		"16",
		"5E",
		"5E",
		"80",
		"0272",
		"3C",
		"",
		{ "CD6A92" },
		{ "C5CE36", "CD7C0B", "CDE5CF" },
		{ "CD7C0D" });
	Items["Fish"] = Item(
		0,
		"Fish",
		"1A",
		"62",
		"62",
		"80",
		"00C7",
		"36",
		"",
		{ "CD6AAA" },
		{ "C5CE36", "CD7C11" },
		{ "CD7C13" });
	Items["Gold Dust"] = Item(
		0,
		"Gold Dust",
		"22",
		"6A",
		"6A",
		"80",
		"01E9",
		"60",
		"",
		{ "CD6ADA", "CD6BD0" },
		{ "C5CE36" });
	Items["Green Potion"] = Item(
		0,
		"Green Potion",
		"14",
		"5C",
		"5C",
		"80",
		"00C1",
		"31",
		"",
		{ "CD6A86" },
		{ "C5CE36", "CDE5A7" });
	Items["Hot Spring Water"] = Item(
		0,
		"Hot Spring Water",
		"20",
		"68",
		"68",
		"00",
		"0139",
		"53",
		"",
		{ "CD6ACE" },
		{ "C5CE36", "CD7C29", "CD7C2F" },
		{ "CD7C2B", "CD7C31" });
	Items["Milk"] = Item(
		0,
		"Milk",
		"18",
		"92",
		"92",
		"80",
		"00B6",
		"2C",
		"",
		{ "CD6A9E", "CD6BCA" },
		{ "C5CE36" });
	Items["Mushroom"] = Item(
		0,
		"Mushroom",
		"23",
		"6B",
		"6B",
		"80",
		"021D",
		"70",
		"",
		{ "CD6AE0" },
		{ "C5CE36", "CD7C47" },
		{ "CD7C49" });
	Items["Poe"] = Item(
		0,
		"Poe",
		"1D",
		"65",
		"65",
		"80",
		"009E",
		"01",
		"",
		{ "CD6ABC" },
		{ "C5CE36", "CD7C4D" },
		{ "CD7C4F" });
	Items["Red Potion"] = Item(
		0,
		"Red Potion",
		"13",
		"5B",
		"5B",
		"80",
		"00C1",
		"32",
		"",
		{ "CD6A80" },
		{ "C5CE36", "CDE593" });
	Items["Seahorse"] = Item(
		1,
		"Seahorse",
		"24",
		"95",
		"95",
		"80",
		"01F0",
		"63",
		"",
		{ "CD6BDC" },
		{ "C5CE36" });
	Items["Spring Water"] = Item(
		0,
		"Spring Water",
		"1F",
		"67",
		"67",
		"00",
		"0000",
		"00",
		"",
		{ "CD6AC8" },
		{ "C5CE36", "CD7C23" },
		{ "CD7C25" });
	Items["Zora Egg"] = Item(
		7,
		"Zora Egg",
		"21",
		"69",
		"69",
		"80",
		"01AE",
		"59",
		"",
		{ "CD6AD4" },
		{ "C5CE36", "CD7C35" },
		{ "CD7C37" });
	Items["Clocktown Map"] = Item(
		0,
		"Clocktown Map",
		"B4",
		"B4",
		"B4",
		"A0",
		"024D",
		"2E",
		"",
		{ "CD6C96" },
		{},
		"M_10110100",
		{ "C1CB13", "C1CB2B" });
	Items["Woodfall Map"] = Item(
		0,
		"Woodfall Map",
		"B5",
		"B5",
		"B5",
		"A0",
		"024D",
		"2E",
		"",
		{ "CD6C9C" },
		{},
		"M_10110101",
		{ "C1CB13", "C1CB2B" });
	Items["Snowhead Map"] = Item(
		0,
		"Snowhead Map",
		"B6",
		"B6",
		"B6",
		"A0",
		"024D",
		"2E",
		"",
		{ "CD6CA2" },
		{},
		"M_10110110",
		{ "C1CB13", "C1CB2B" });
	Items["Romani Ranch Map"] = Item(
		0,
		"Romani Ranch Map",
		"B7",
		"B7",
		"B7",
		"A0",
		"024D",
		"2E",
		"",
		{ "CD6CA8" },
		{},
		"M_10110111",
		{ "C1CB13", "C1CB2B" });
	Items["Great Bay Map"] = Item(
		0,
		"Great Bay Map",
		"B8",
		"B8",
		"B8",
		"A0",
		"024D",
		"2E",
		"",
		{ "CD6CAE" },
		{},
		"M_10111000",
		{ "C1CB13", "C1CB2B" });
	Items["Stone Tower Map"] = Item(
		0,
		"Stone Tower Map",
		"B9",
		"B9",
		"B9",
		"A0",
		"024D",
		"2E",
		"",
		{ "CD6CB4" },
		{},
		"M_10111001",
		{ "C1CB13", "C1CB2B" });
	Items["Sonata of Awakening"] = Item(
		1,
		"Sonata of Awakening",
		"61",
		"53",
		"73",
		"00",
		"008F",
		"08",
		"",
		{ "CD6A50" },
		{ "", "F1B4AF" },
		{ "277A2A5" },
		{},
		{ "C661F9" },
		"02",
		"12",
		{ "277A275" },
		{ "277A28D" },
		"F_01000000",
		{ "C5CE73" });
	Items["Goron Lullaby"] = Item(
		1,
		"Goron Lullaby",
		"62",
		"54",
		"74",
		"00",
		"008F",
		"08",
		"",
		{ "CD6A56" },
		{},
		{ "263BC31" },
		{},
		{ "C661FB" },
		"03",
		"13",
		{ "263B4F9" },
		{ "263B511" },
		"F_10000000",
		{ "C5CE73" });
	Items["New Wave Bossa Nova"] = Item(
		1,
		"New Wave Bossa Nova",
		"63",
		"71",
		"75",
		"00",
		"008F",
		"08",
		"",
		{ "CD6B04" },
		{ "", "DCCC8F" },
		{ "2602241" },
		{},
		{ "C661FD" },
		"04",
		"14",
		{},
		{ "2602229" },
		"F_00000001",
		{ "C5CE72" });
	Items["Elegy of Emptiness"] = Item(
		1,
		"Elegy of Emptiness",
		"64",
		"72",
		"76",
		"00",
		"008F",
		"08",
		"",
		{ "CD6B0A" },
		{},
		{ "2B71639" },
		{},
		{ "C661FF" },
		"05",
		"15",
		{ "2B71609" },
		{ "2B71621" },
		"F_00000010",
		{ "C5CE72" });
	Items["Oath to Order"] = Item(
		0,
		"Oath to Order",
		"65",
		"73",
		"77",
		"00",
		"008F",
		"08",
		"",
		{ "CD6B10" },
		{},
		{ "2D83A0D" },
		{},
		{ "C66201" },
		"06",
		"16",
		{ "2D839DD" },
		{ "2D839F5" },
		"F_00000100",
		{ "C5CE72" });
	Items["Song of Healing"] = Item(
		0,
		"Song of Healing",
		"68",
		"75",
		"94",
		"00",
		"008F",
		"08",
		"",
		{ "CD6B1C" },
		{},
		{},
		{},
		{ "C66207" },
		"09",
		"19",
		{ "2CCFB99" },
		{ "2CCFBB1" },
		"F_00100000",
		{ "C5CE72" });
	Items["Epona's Song"] = Item(
		0,
		"Epona's Song",
		"69",
		"76",
		"70",
		"00",
		"008F",
		"08",
		"",
		{ "CD6A50" },
		{},
		{ "269141D" },
		{},
		{ "C66209" },
		"0A",
		"1A",
		{ "26913ED" },
		{ "2691405" },
		"F_01000000",
		{ "C5CE72" });
	Items["Song of Soaring"] = Item(
		0,
		"Song of Soaring",
		"6A",
		"A2",
		"71",
		"00",
		"008F",
		"08",
		"",
		{ "CD6C2A" },
		{ "", "F2FB7B" },
		{ "2825EAD" },
		{},
		{ "C6620B" },
		"0B",
		"1B",
		{ "2825E7D" },
		{ "2825E95" },
		"F_10000000",
		{ "C5CE72" });
	Items["Song of Storms"] = Item(
		0,
		"Song of Storms",
		"6B",
		"A3",
		"72",
		"00",
		"008F",
		"08",
		"",
		{ "CD6C30" },
		{},
		{ "1F82BC5" },
		{},
		{ "C6620D" },
		"0C",
		"1C",
		{ "1F82BA1" },
		{ "1F82BAD" },
		"F_00000001",
		{ "C5CE71" });
	Items["Heart Piece"] = Item(
		52,
		"Heart Piece",
		"7B",
		"0C",
		"0C",
		"A0",
		"0096",
		"14",
		"",
		{ "CD68A6" },
		{},
		"10",
		{ "C5CE70" });
	Items["Heart Container"] = Item(
		4,
		"Heart Container",
		"6F",
		"0D",
		"0D",
		"A0",
		"0096",
		"13",
		"",
		{ "CD68AC" },
		{},
		"10",
		{ "C5CDE9", "C5CDEB" });

	Items["Bombchu"] = Item(
		1,
		"Bombchu",
		"99",
		"36",
		"36",
		"C0",
		"00B0",
		"D9",
		"",
		"07",
		{ "CD69A2" },
		{ "C5CE2B" },
		"01",
		{});
	Items["Bombchus (5)"] = Item(
		1,
		"Bombchus (5)",
		"9A",
		"3A",
		"3A",
		"C0",
		"00B0",
		"D9",
		"",
		"07",
		{ "CD69BA" },
		{ "C5CE2B" },
		"05",
		{});
	Items["Bombchus (10)"] = Item(
		0,
		"Bombchus (10)",
		"98",
		"1A",
		"1A",
		"C0",
		"00B0",
		"D9",
		"",
		"07",
		{ "CD68FA" },
		{ "C5CE2B" },
		"0A",
		{});

	//Rupees
	Items["Green Rupee"] = Item(
		1,
		"Green Rupee",
		"84",
		"01",
		"C4",
		"00",
		"013F",
		"B0",
		"",
		{ "CD6864" },
		{},
		"01",
		{ "C5CDEF" });
	Items["Blue Rupee"] = Item(
		0,
		"Blue Rupee",
		"85",
		"02",
		"02",
		"01",
		"013F",
		"AF",
		"",
		{ "CD686A" },
		{},
		"05",
		{ "C5CDEF" });
	Items["Red Rupee"] = Item(
		0,
		"Red Rupee",
		"87",
		"04",
		"04",
		"02",
		"013F",
		"AE",
		"",
		{ "CD6876" },
		{},
		"14",
		{ "C5CDEF" });
	Items["Purple Rupee"] = Item(
		0,
		"Purple Rupee",
		"88",
		"05",
		"05",
		"14",
		"013F",
		"AC",
		"",
		{ "CD687C" },
		{},
		"32",
		{ "C5CDEF" });
	Items["Silver Rupee"] = Item(
		9,
		"Silver Rupee",
		"89",
		"06",
		"06",
		"14",
		"013F",
		"AB",
		"",
		{ "CD6882" },
		{},
		"64",
		{ "C5CDEF" });
	Items["Gold Rupee"] = Item(
		0,
		"Gold Rupee",
		"8A",
		"07",
		"07",
		"13",
		"013F",
		"BD",
		"",
		{ "CD6888" },
		{},
		"C8",
		{ "C5CDEF" });

	//Remains
	//Odolwa's remains GI ID: 5D, Obj ID: 01CC - Need to figure out how to make this work
	Items["Odolwa's Remains"] = Item(
		0,
		"Odolwa's Remains",
		"5D",
		"55",
		"55",
		"80",
		"0000",
		"00",	//5D => 00 because it won't work
		"",
		{ "CD6A5C" },
		{},
		"F_00000001",
		{ "C5CE73" });
	Items["Goht's Remains"] = Item(
		0,
		"Goht's Remains",
		"5E",
		"56",
		"56",
		"80",
		"0000",
		"00",	//64 => 00
		"",
		{ "CD6A62" },
		{},
		"F_00000010",
		{ "C5CE73" });
	Items["Gyorg's Remains"] = Item(
		0,
		"Gyorg's Remains",
		"5F",
		"57",
		"57",
		"80",
		"0000",
		"00",	//65 => 00
		"",
		{ "CD6A68" },
		{},
		"F_00000100",
		{ "C5CE73" });
	Items["Twinmold's Remains"] = Item(
		0,
		"Twinmold's Remains",
		"60",
		"58",
		"58",
		"80",
		"0000",
		"00",	//66 => 0
		"",
		{ "CD6A6E" },
		{},
		"F_00001000",
		{ "C5CE73" });

}

void RespawnHCs() {
	Write_To_Rom(13857024, "2405001E");	//change saving bit flag
	Write_To_Rom(13856764, "2405001E");	//change checking bit flag
}

void Apply_Settings(bool Songs_Same_Pool) {

	// Change max rupee amounts
	Change_Rupees(Settings["wallets"]);

	// make scrub salesman always sell beans
	if (Settings["settings"]["ScrubBeans"] == "True")
	{
		string Bean_Source = Get_Source("Magic Beans");
		string New_ID = Items[Bean_Source].Get_Item_ID;

		Write_To_Rom(17113087, New_ID); // scrub salesman
	}

	// Use Gamecube HUD
	if (Settings["settings"]["GC_Hud"] == "True")
	{
		Gamecube_Hud();
	}

	// remove the cutscenes
	Remove_Cutscenes(Songs_Same_Pool);

	// change tunic colors
	Change_Colors(Settings["colors"]);

	// making link kafei
	if (Settings["settings"]["Kafei"] == "True")
	{
		Change_Link_Kafei();
		Change_Kafei_Color(Settings["colors"]["Link"]);
	}

	// change water to bingo water if secret is active
	if (Settings["settings"]["BingoWater"] == "True")
	{
		Bingo_Water();
	}

	// change blast mask cooldown
	Change_BlastMask(Settings["settings"]);

	// respawn hps once per cycle
	if (Settings["settings"]["RespawnHPs"] == "True")
	{
		RespawnHPs();
	}

	//Make likelikes able to eat mirror shields (but they still spit out hero's shield, this is only here to make the no shield goal in bingo easier)
	if (Settings["settings"]["LikeLikeMirror"] == "True") {
		Write_To_Rom(14108508, "13200008");
	}

	//keep razor sword on song of time
	if (Settings["settings"]["KeepRazor"] == "True") {
		Write_To_Rom(12428980, "29A10002");
	}

	//make ocean spider house any day by making the guy think it's always day one
	if (Settings["settings"]["OceanAnyDay"] == "True") {
		Write_To_Rom(16565980, "24080001");
	}

	//respawn hcs once per cycle
	if (Settings["settings"]["RespawnHCs"] == "True") {
		RespawnHCs();
	}

	//change default targeting to hold
	if (Settings["settings"]["Targeting"] == "Hold") {
		Write_To_Rom(12438076, "A20D3F45");
	}

	//make scrub salesman not show up after trading
	if (Settings["settings"]["TradeQuest"] == "True") {
		Write_To_Rom(48611460, "0007"); // remove woodfall scrub from clocktown
		Write_To_Rom(42168432, "0007"); // remove snowhead scrub from swamp
		Write_To_Rom(32628840, "0007"); // remove snowhead scrub from cleared swamp
		Write_To_Rom(42725596, "0007"); // remove ocean scrub from spring snowhead
		Write_To_Rom(44384392, "0007"); // remove ocean scrub from snowhead
		Write_To_Rom(44212312, "0007"); // remove ikana scrub from ocean
	}
}

void Fix_Things(bool Songs_Same_Pool) {

	// FD anywhere
	Write_To_Rom(12220113, "00");

	// FD can use transformations masks
	Write_To_Rom(12945794, "010101"); // enables deku, goron, and zora

	// quick text
	Write_To_Rom(12482776, "00000000");
	Write_To_Rom(13065591, "30");

	// change starting scene to clock town
	Write_To_Rom(12433538, "D8");

	// Give Tatl to Link in Clock Town CS, shorten cs to 1 frame, and set entered
	// clocktown for the first time flag
	Write_To_Rom(48476396, "0000000200000001");
	Write_To_Rom(48476404, "0000009A000000010001000100020002");
	Write_To_Rom(48476420, "00000096000000010021000100020002");

	// fix getting false mask when SoT from tatl text in clocktown
	Write_To_Rom(48611908, "E007");

	// fix turning deku when SoT - stay as link
	Write_To_Rom(47286973, "0A");

	// Make it where if a bottle catch is not a bottle item, then it runs the normal item give function. Otherwise it runs the normal bottle catch function
	Make_Bottles_Work();

	// input the function to give tingle map branch to it in the get item passive function
	Fix_Tingle_Maps();

	// fix getting the swords as transformation masks
	Fix_Swords();

	// fix the song text ids for when it is loaded and displayed
	Fix_Song_Text();

	Write_To_Rom(13182652, "260DFFFA00000000"); // Fix song looks in inventory
	Write_To_Rom(13184360, "260CFFFA00000000"); // fix playing songs in inventory

	// input the songs text and fix the text offsets
	Songs_Text_Offset();

	// make new wave bossa nova give the song only once
	if (Songs_Same_Pool)
	{
		string NWBN = Items["New Wave Bossa Nova"].Name;
		Write_To_Rom(12500391, Hex_Minus(Items[NWBN].Item_ID, "61"));
	}

	Fix_Goron_Lullaby();

	Fix_Showing_Items();

	// dont let sonata give item twice
	Write_To_Rom(15840424, "00000000");

	//don't let song of soaring give item twice
	Write_To_Rom(15924108, "00000000");

	//Make couple's mask only give once in the normal cutscene if the cutscene is not shorter
	if (!Setting_True("cutscenes", "ClockTown_CouplesMask")) {
		Write_Cutscene_Rom(46642260, "couples_long");
	}

	//Don't get more than one item from a boss remains
	Write_To_Rom(13844604, "00000000");

	//fix FD softlock for bomber's
	Write_To_Rom(17205600, "00000000");

	//make clock town guard treat fd like link
	Write_To_Rom(16044964, "00000000");

	//write the randomizer text on the title screen
	Write_File_To_Rom("./files/Randomizer", "014E7F40");

	//move the randomizer text on the title screen
	Write_To_Rom(14552815, "95");
}

int main()
{
	string error_message;
	string file_ext;
	bool Songs_Same_Pool;
	map<string, vector<vector<string>>> Items_Needed;
	map<string, int> Location_Sets;

	Max_Percentage = 0;

    err_file.open("Error.txt");
	if (DEBUG) {
		log_file.open("Log.txt");
	}

	// get the settings from the settings file
	Settings = OpenAsIni("./settings.ini");

	error_message = Valid_Rom(Settings["settings"]["Rom"], &file_ext);
	if (error_message != "") {
		Error(error_message);
	}

	//convert other formats to z64 in a new file
	Settings["settings"]["Rom"] = Convert_Z64(Settings["settings"]["Rom"], file_ext);

    outFile.open("Spoiler Log.txt");

	//setup the item objects
	Setup_Items();

	//get the list of items
	Item_Keys = Get_Keys(Items);

    // update the item pools according to the settings
    Update_Pools(Items);

    // randomize items according to logic, if any logic was chosen
    cout << "Randomizing Items...\n";
	if (!Randomize_Setup(Items, &Settings, Items_Needed, Location_Sets)) {
		//if the items were not able to be randomized according to the logic
		Error("Could not randomize the items with the logic selected");
	}

	Logger("Randomized Items");

    // write spoiler log
    Write_Log(Settings["settings"]["Seed"], Items_Needed, Location_Sets);

	Songs_Same_Pool = All_Songs_Same_Pool();

    // decompress rom
    cout << "\nDecompressing rom\n";
    if (system(("ndec.exe \"" + Settings["settings"]["Rom"] + "\" " + Rom_Location)
                 .c_str()) != 0)
    {
        // if failed to decompress file
        err_file << "Failed to decompress " << Settings["settings"]["Rom"]
                 << " - Might be missing \"ndec.exe\"";
        err_file.close();
        exit(0);
    }

    Place_Items(Items, Songs_Same_Pool);

	//Apply custom settings from the user
	Apply_Settings(Songs_Same_Pool);

	//fix some item things and do some setups
	Fix_Things(Songs_Same_Pool);

    // Remove Item Checks
    Remove_Item_Checks();

    // Give the player the starting items
    Give_Starting_Items();

    // compress rom and create wad
    if (Settings["settings"]["Wad"] == "True")
    {
        // create rom and wad
		Logger("Creating Rom and Wad");
        cout << "\nCompressing rom and creating wad\n\n";
        if (system("_create-roms.bat") != 0)
        {
            // if failed to compress file
            err_file << "Failed to compress rom using \"_create-roms.bat\"\n";
            err_file.close();
            exit(0);
        }
    }
    else
    {
        // create rom
        cout << "\nCompressing rom\n\n";
        if (system("_create-rom.bat") != 0)
        {
            // if failed to compress file
            err_file << "Failed to compress rom using \"_create-rom.bat\"\n";
            err_file.close();
            exit(0);
        }
    }

    // rename the rom to include the seed
    /*if (system(("rename \"Legend of Zelda, The - Majora's Mask - Randomizer (U).z64\" \"Legend of Zelda, The - Majora's Mask - Randomizer (U)_"
	    + Settings["settings"]["Seed"] + ".z64\"").c_str()) != 0) {
        //couldn't find the compressed rom
        err_file << "Renaming the rom failed";
	err_file.close(); exit(0);
    }*/

    // Delete decompressed rom since it is no longer needed
    // system(("del " + Rom_Location).c_str());

	Success();

    outFile.close();
    err_file.close();
	if (DEBUG) {
		log_file.close();
	}

    return 0;
}
