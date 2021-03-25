#ifndef RANDO_ROM_HPP
#define RANDO_ROM_HPP

#include <map>
#include <string>
#include <vector>

#include <fstream>

// bad shade!
using namespace std;

void Write_To_Rom(int address, string hex);
void Write_Function(int address, vector<string> function);
long GetFileSize(std::string filename);
string Read_From_Rom(string rom_path, int offset, int length_in_bytes);
void Copy_Rom(string old_path, string new_path);
void Swap(char *one, char *two);
void Swap_Bytes(string file_path, int groups);
string Convert_Z64(string file_path, string file_type);
string Valid_Rom(string rom_path, string *);

extern fstream inFile;
extern string Rom_Location;

#endif
