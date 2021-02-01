#ifndef RANDO_IO_HPP
#define RANDO_IO_HPP

#include <map>
#include <string>

// bad shade!
using namespace std;

std::map<std::string, std::map<std::string, std::string>>
OpenAsIni(const std::string &filename);

void Open_File(const string &filename, fstream &file);

#endif
