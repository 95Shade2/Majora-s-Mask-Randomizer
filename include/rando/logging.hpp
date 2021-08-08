#ifndef RANDO_ERROR_HPP
#define RANDO_ERROR_HPP

#include <string>
#include <fstream>
#include <vector>
#include <chrono>

// bad shade!
using namespace std;

extern ofstream err_file;
extern ofstream log_file;
extern bool DEBUG;

[[noreturn]] void Error(const string &Error_Message);

void log(const vector<string> &stuff);

void Success();

void Logger(string Log_Message);

void Log_Vector(vector<string> Log_Vec);

vector<string> And(vector<string> vec1, vector<string> vec2);

#endif
