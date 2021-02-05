#ifndef RANDO_IO_HPP
#define RANDO_IO_HPP

#include <map>
#include <string>
#include <vector>

// bad shade!
using namespace std;

std::map<std::string, std::map<std::string, std::string>>
OpenAsIni(const std::string &filename);

void Open_File(const string &filename, fstream &file);

void Print_Vector(const std::vector<string> &Items,
                  const std::string &separator = "",
                  const std::string &preString = "");

#endif
