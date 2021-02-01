#ifndef RANDO_UTILS_HPP
#define RANDO_UTILS_HPP

#include <string>
#include <vector>
#include <map>

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

string Even_Hex(const string &hex);

string Bits_Or(const string &bits_1, const string &bits_2);

// clears the bit at index bit_index of a byte
string Bit_Clear(const string &byte, int bit_index);

template <typename big, typename small> int IndexOf(big Data, small Text)
{
    for (int i = 0; i < Data.size(); i++)
    {
        if (Data[i] == Text)
        {
            return i;
        }
    }

    return -1;
}

template <typename value>
bool Vector_Has(const std::vector<value> &vect, const value &val)
{
    for (int i = 0; i < vect.size(); i++)
    {
        if (vect[i] == val)
        {
            return true;
        }
    }

    return false;
}

string Hex_Minus(const string &hex, const string &hex_2);

template <typename value>
std::vector<std::string> Get_Keys(std::map<std::string, value> data)
{
    std::vector<string> keys;

    for (const auto &kv : data)
    {
        keys.push_back(kv.first);
    }

    return keys;
}
#endif
