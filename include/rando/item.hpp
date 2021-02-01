#ifndef RANDO_ITEM_HPP
#define RANDO_ITEM_HPP

#include "time.hpp"

#include <map>
#include <string>
#include <vector>

// bad shade!
using namespace std;

// Item class
class Item
{
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
    vector<string>
      Items_Needed; // the items that the logic used to determine the item is obtainable
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

    bool gives_item; // whether or not the item gives something yet (if an item has been
                     // placed here or not)
    bool can_get;    // whether or not another item (or itself) gives this item
    int value; // how valuable the item is, the more other items that can be obtained from
               // using this item, the higher the number

    Item()
    {
    }

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         string item_count,
         vector<string> item_count_locs);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         string inv_id,
         vector<string> add_get_table,
         vector<string> add_item_id,
         string item_count,
         vector<string> item_count_locations);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         string inv_id,
         vector<string> add_get_table,
         vector<string> add_item_id,
         string item_count,
         vector<string> item_count_locations,
         string item_count2,
         vector<string> item_count2_locations);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         vector<string> add_text_id);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         vector<string> add_text_id,
         string item_count,
         vector<string> item_count_locations,
         int just_overlading);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         vector<string> add_text_id,
         vector<string> add_get_item_id);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         vector<string> add_text_id,
         vector<string> add_get_item_id,
         vector<string> Inverted_Song_Locations);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         vector<string> add_text_id,
         vector<string> add_get_item_id,
         vector<string> Inverted_Song_Locations,
         string songid1,
         string songid2,
         vector<string> song1_loc,
         vector<string> song2_loc);

    Item(vector<Time> time_available,
         string nam,
         string id,
         string get_id,
         string text,
         string flg,
         string ob,
         string get_item_model,
         string pol,
         vector<string> add_get_table,
         vector<string> add_item_id,
         vector<string> add_text_id,
         vector<string> add_get_item_id,
         vector<string> Inverted_Song_Locations,
         string songid1,
         string songid2,
         vector<string> song1_loc,
         vector<string> song2_loc,
         string item_count,
         vector<string> item_count_locations);

    /*
    Item(vector<Time> time_available, string nam, string id, string get_id, string text,
    string flg, string ob, string get_item_model, string pol, vector<string>
    add_get_table, vector<string> add_item_id, vector<string> add_text_id, vector<string>
    add_get_item_id, vector<string> Locations, string Locations_Data, vector<string>
    Locations2, string Locations_Data2) { Name = nam;

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

#endif
