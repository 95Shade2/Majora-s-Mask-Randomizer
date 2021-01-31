#ifndef RANDO_ERROR_HPP
#define RANDO_ERROR_HPP

#include <string>
#include <fstream>

// bad shade!
using namespace std;

extern ofstream err_file;

void Error(const string& Error_Message);

#endif
