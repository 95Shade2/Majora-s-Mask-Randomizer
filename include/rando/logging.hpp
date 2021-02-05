#ifndef RANDO_ERROR_HPP
#define RANDO_ERROR_HPP

#include <string>
#include <fstream>
#include <vector>

// bad shade!
using namespace std;

extern ofstream err_file;

[[noreturn]] void Error(const string &Error_Message);

void log(const vector<string> &stuff);

#endif
