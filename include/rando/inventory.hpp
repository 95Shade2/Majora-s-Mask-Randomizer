#ifndef RANDO_INVENTORY_HPP
#define RANDO_INVENTORY_HPP

#include "item.hpp"

#include <map>
#include <string>

// bad shade!
using namespace std;

class Inventory
{
  public:
    map<string, bool> Items;

    void Default_Inventory();

    Inventory();

    void Add_Item(const string &Item);

    bool All(const map<string, Item> &List) const;

    void Print_Missing(const map<string, Item> &List) const;

    string Get_Source(const string &item, const map<string, Item> &List) const;

    vector<string> Get_Missing_Items_Source(const vector<string> &Missing_Items,
                                            const map<string, Item> &List) const;

    vector<string> Get_Missing_Items() const;
};

#endif
