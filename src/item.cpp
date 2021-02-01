#include "rando/item.hpp"

Item::Item(vector<Time> time_available,
           string nam,
           string id,
           string get_id,
           string text,
           string flg,
           string ob,
           string get_item_model,
           string pol,
           vector<string> add_get_table,
           vector<string> add_item_id)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> item_count_locs)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> item_count_locations)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> item_count2_locations)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> add_text_id)
{
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

Item::Item(vector<Time> time_available,
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
           int just_overlading)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> add_get_item_id)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> Inverted_Song_Locations)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> song2_loc)
{
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

Item::Item(vector<Time> time_available,
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
           vector<string> item_count_locations)
{
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
