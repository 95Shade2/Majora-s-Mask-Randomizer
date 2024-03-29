#ifndef RANDO_UTILS_HPP
#define RANDO_UTILS_HPP

#include "rando/logging.hpp"

#include <map>
#include <string>
#include <vector>
#include <list>

extern void Logger(string);

// bad shade!
using namespace std;

// number stuff

string hex_to_binary(const string& hex);

string dec_to_hex(int number);

string dec_to_hex_work(int number);

std::string string_to_hex(const std::string &input);

std::string hex_to_string(const std::string &input);

int hex_to_decimal(const std::string& hex);

int string_to_dec( string text);

string dec_to_string(int dec, int depth = 0);

string double_to_string(double dec, int depth = 0);

string binary_to_hex(string binary);

string decimal_to_hex(int dec);

// replace each 1 with a 0, and each 0 with a 1
string invert_binary(string binary);

bool isNumber(string text);

// string stuff

bool Contains(string text, char chr);

string RemoveAll(string text, char chr);

int IndexOf_S(string Data, string Text);

string Even_Hex(string hex);

string Bits_Or(string bits_1, string bits_2);

// clears the bit at index bit_index of a byte
string Bit_Clear(string byte, int bit_index);

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

string Hex_Minus(string hex, string hex_2);

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

template <typename vec_type, typename var_type, typename Func>
void forEach(vector<vec_type> vec, Func f, var_type var1)
{
    for (int i = 0; i < vec.size(); i++)
    {
        f(vec[i], var1);
    }
}

string Get_Word(string text, int word_index);

vector<string> Split(string line, string Splitter);

string Remove_Whitespace(string text);

vector<string> String_Split(string text, int split);

string Leading_Zeroes(string hex, int total_length);

string Replace(string bigger_text, string smaller_text, string new_smaller_text);

///Converts a char array to a string
string Char_To_String(char chr[], int size);

///Converts a vector to a string
string Vector_To_String(vector<string> data, string separator);

int IndexOf_Last(string text, char chr);

vector<bool> Binary_Str_To_Vec(string Binary_String);

template<typename type>
vector<type> Sub_Vec(vector<type> vec, int start, int length = -1) {
	vector<type> sub;
	int end;
	//if no length is given, then get from start to the end
	if (length == -1) {
		end = vec.size();
	}
	else {
		end = start + length;
		//don't go outside the bounds of the vector
		if (end > vec.size()) {
			end = vec.size();
		}
	}

	for (int v = start; v < end; v++) {
		sub.push_back(vec[v]);
	}

	return sub;
}

vector<string> Remove_Index(vector<string> data, int index);

vector<string> Remove_Values(vector<string>, vector<string>);

vector<int> Remove_Index(vector<int> data, int index);

vector<int> Remove_Values(vector<int>, vector<int>);

vector<string> Copy(vector<string> source);

vector<vector<string>> Copy(vector<vector<string>> source);

map<string, int> Set_Map(vector<string> keys, int value);

map<string, bool> Set_Map(vector<string> keys, bool value);

map<string, vector<vector<string>>> Set_Map(vector<string> keys, vector<vector<string>> value);

vector<string> Append(vector<string> data, vector<string> More_Data);

int Percentage(int, int);

bool isBinary(string binary_string);

map<string, int> Copy(map<string, int> source);

double long to_ascii(string text);

double average(vector<double> numbers);

#endif
