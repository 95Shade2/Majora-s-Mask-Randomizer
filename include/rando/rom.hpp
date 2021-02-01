#ifndef RANDO_ROM_HPP
#define RANDO_ROM_HPP

#include <map>
#include <string>
#include <vector>

#include <fstream>

// bad shade!
using namespace std;

void Write_To_Rom(int address, string hex);

extern fstream inFile;
extern string Rom_Location;

#endif
