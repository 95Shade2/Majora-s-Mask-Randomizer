#ifndef RANDO_UTILS_HPP
#define RANDO_UTILS_HPP

#include <string>

// bad shade!
using namespace std;

// number stuff

string hex_to_binary(const string &hex);

string dec_to_hex(int number);

std::string string_to_hex(const std::string &input);

std::string hex_to_string(const std::string &input);

int hex_to_decimal(const string &hex);

int string_to_dec(const string &text);

string dec_to_string(int dec);

string binary_to_hex(const string &binary);

string decimal_to_hex(int dec);

// replace each 1 with a 0, and each 0 with a 1
string invert_binary(const string &binary);

bool isNumber(const string &text);

// string stuff

bool Contains(const string &text, char chr);

string RemoveAll(const string &text, char chr);

int IndexOf_S(const string &Data, const string &Text);

#endif
