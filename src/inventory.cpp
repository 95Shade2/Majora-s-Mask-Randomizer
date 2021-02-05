#include "rando/inventory.hpp"
#include "rando/logging.hpp"

void Inventory::Default_Inventory()
{
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

Inventory::Inventory()
{
    Default_Inventory();
}

void Inventory::Add_Item(const string &Item)
{
    if (Item == "Bow" || Item == "Large Quiver" || Item == "Largest Quiver")
    {
        Items["Quiver"] = true;
    }
    else if (Item == "Bomb Bag" || Item == "Big Bomb Bag" || Item == "Biggest Bomb Bag")
    {
        Items["Bombs"] = true;
    }
    else if (Item == "Deku Nuts" || Item == "Deku Nuts (10)")
    {
        Items["Nuts"] = true;
    }

    Items[Item] = true;
}

bool Inventory::All(const map<string, Item> &List) const
{
    vector<string> keys;

    // get the keys for each item
    for (const auto &kv : Items)
    {
        keys.push_back(kv.first);
    }

    // check to see if each item was obtained
    for (int i = 0; i < keys.size(); i++)
    {
        if (!Items.at(keys[i]))
        {
            // cout << Get_Source(keys[i], List) << " => " << keys[i] << endl;
            return false;
        }
    }

    return true;
}

void Inventory::Print_Missing(const map<string, Item> &List) const
{
    vector<string> Missing_Items;
    vector<string> Missing_Items_Source;

    Missing_Items = Get_Missing_Items();
    Missing_Items_Source = Get_Missing_Items_Source(Missing_Items, List);

    for (int i = 0; i < Missing_Items_Source.size(); i++)
    {
        // cout << Missing_Items_Source[i] << " => " << Missing_Items[i] << endl;
    }
}

string Inventory::Get_Source(const string &item, const map<string, Item> &List) const
{
    vector<string> keys;

    // get the keys for each item
    for (const auto &kv : List)
    {
        keys.push_back(kv.first);
    }

    for (int i = 0; i < keys.size(); i++)
    {
        if (List.at(keys[i]).Name == item)
        {
            return List.at(keys[i]).Name;
        }
    }

    Error("No items give " + item);
}

vector<string> Inventory::Get_Missing_Items_Source(const vector<string> &Missing_Items,
                                                   const map<string, Item> &List) const
{
    vector<string> keys;
    vector<string> Missing_Items_Source;

    // get the keys for each item
    for (const auto &kv : Items)
    {
        keys.push_back(kv.first);
    }

    // get what gives current item
    for (int i = 0; i < Missing_Items.size(); i++)
    {
        for (int j = 0; j < keys.size(); j++)
        {
            if (List.at(keys[j]).Name == Missing_Items[i])
            {
                // cout << keys[j] << "\t" << List[keys[j]].Name << "\t" <<
                // Missing_Items[i] << endl;
                Missing_Items_Source.push_back(keys[j]);
                j = keys.size();
            }
        }
    }

    return Missing_Items_Source;
}

vector<string> Inventory::Get_Missing_Items() const
{
    vector<string> keys;
    vector<string> missing;

    // get the keys for each item
    for (const auto &kv : Items)
    {
        keys.push_back(kv.first);
    }

    // check to see if each item was obtained
    for (int i = 0; i < keys.size(); i++)
    {
        if (!Items.at(keys[i]))
        {
            missing.push_back(keys[i]);
        }
    }

    return missing;
}
