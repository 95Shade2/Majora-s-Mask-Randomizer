#include "rando/utils.hpp"

#include <algorithm>
#include <cmath>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <iomanip>

string hex_to_binary(const string &hex)
{
    std::stringstream binary;

    for (int i = 0; i < hex.size(); i++)
    {
        char solo_hex = hex[i];

        switch (solo_hex)
        {
        case '0':
            binary << "0000";
            break;
        case '1':
            binary << "0001";
            break;
        case '2':
            binary << "0010";
            break;
        case '3':
            binary << "0011";
            break;
        case '4':
            binary << "0100";
            break;
        case '5':
            binary << "0101";
            break;
        case '6':
            binary << "0110";
            break;
        case '7':
            binary << "0111";
            break;
        case '8':
            binary << "1000";
            break;
        case '9':
            binary << "1001";
            break;
        case 'A':
        case 'a':
            binary << "1010";
            break;
        case 'B':
        case 'b':
            binary << "1011";
            break;
        case 'C':
        case 'c':
            binary << "1100";
            break;
        case 'D':
        case 'd':
            binary << "1101";
            break;
        case 'E':
        case 'e':
            binary << "1110";
            break;
        case 'F':
        case 'f':
            binary << "1111";
            break;
        default:
            throw std::runtime_error("unexpected");
        }
    }
    return binary.str();
}

string dec_to_hex(unsigned number)
{
    std::stringstream hex;
    hex << std::hex << std::setfill('0'); 
    if (number <= 0xff)
    {
        hex << std::setw(2);
    }
    else if (number <= 0xffff)
    {
        hex << std::setw(4);
    }
    else if (number <= 0xffffff)
    {
        hex << std::setw(6);
    }
    else
    {
        hex << std::setw(8);
    }
    hex << number;
    return hex.str();
}

std::string string_to_hex(const std::string &input)
{
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string hex_to_string(const std::string &input)
{
    size_t len = input.length();
    if (len & 1)
        throw std::invalid_argument("odd length");

    string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char val = atoi(input.substr(i, 2).c_str());
        output.push_back(val);
    }
    return output;
}

// converts a hexadecimal string to a decimal integer
int hex_to_decimal(const string &hex)
{
    if (hex.empty())
    {
        return 0;
    }
    return std::stoi(hex, nullptr, 16);
}

int string_to_dec(const string &text)
{
    return atoi(text.c_str());
}

string dec_to_string(int dec)
{
    std::stringstream res;
    res << dec;
    return res.str();
}

string binary_to_hex(const std::string &binary)
{
    std::stringstream hex;

    if (binary.size() % 4 != 0)
    {
        throw std::runtime_error("input binary string not a multiple of 4");
    }

    hex << std::hex;

    for (int i = 0; i < binary.size(); i += 4)
    {
        hex << std::stoi(binary.substr(i, 4), nullptr, 2);
    }

    return hex.str();
}

bool Contains(const string &text, char chr)
{
    return std::find(text.begin(), text.end(), chr) != text.end();
}

bool isNumber(const string &text)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (!std::isdigit(text[i]))
        {
            return false;
        }
    }

    return true;
}

string RemoveAll(const string &text, char chr)
{
    string newText = text;
    newText.erase(std::remove(newText.begin(), newText.end(), chr), newText.end());

    return newText;
}

string invert_binary(const string &binary)
{
    string inverted = binary;

    for (auto &c : inverted)
    {
        c = ((c == '0') ? '1' : '0');
    }

    return inverted;
}

int IndexOf_S(const string &Data, const string &Text)
{
    size_t pos = Data.find(Text);
    if (pos == std::string::npos)
    {
        return -1;
    }
    return pos;
}

string Even_Hex(const string &hex)
{
    if (hex.size() / 2 == 0)
    {
        return "0" + hex;
    }
    else
    {
        return hex;
    }
}

string Bits_Or(const string &bits_1, const string &bits_2)
{
    if (bits_1.length() == 0 && bits_2.length() == 0)
    {
        return "";
    }
    else if (bits_1.length() == 0)
    {
        return bits_2;
    }
    else if (bits_2.length() == 0)
    {
        return bits_1;
    }
    else
    {
        if (bits_1.length() > bits_2.length())
        {
            return bits_1[0] + Bits_Or(bits_1.substr(1), bits_2);
        }
        else if (bits_1.length() < bits_2.length())
        {
            return bits_2[0] + Bits_Or(bits_1, bits_2.substr(1));
        }
        else
        {
            if (bits_1[0] == '1' || bits_2[0] == '1')
            {
                return "1" + Bits_Or(bits_1.substr(1), bits_2.substr(1));
            }
            else
            {
                return "0" + Bits_Or(bits_1.substr(1), bits_2.substr(1));
            }
        }
    }
}

string Bit_Clear(const string &byte, int bit_index)
{
    string new_byte = "";

    for (int b = 0; b < byte.size(); b++)
    {
        if (b != bit_index)
        {
            new_byte += byte[b];
        }
        else
        {
            new_byte += "0";
        }
    }

    return new_byte;
}

string Hex_Minus(const string &hex, const string &hex_2)
{
    string New_Hex;
    int Hex_Dec = hex_to_decimal(hex);
    int Hex_2_Dec = hex_to_decimal(hex_2);
    int New_Hex_Dec = Hex_Dec - Hex_2_Dec;

    // if it's negative then get FFFF - the value
    if (New_Hex_Dec < 0)
    {
        New_Hex_Dec *= -1; // make it positive
        New_Hex_Dec -=
          1; // if it goes over 100, then the game ignores the 1 and only looks at the 00,
             // but it's 1 too high, so doing this fixes it to give the correct item
        New_Hex = dec_to_hex(New_Hex_Dec); // convert back to hex
        New_Hex = hex_to_binary(New_Hex);  // convert the new hex to binary
        New_Hex = invert_binary(New_Hex);  // invert the binary
        New_Hex = binary_to_hex(New_Hex);  // convert the inverted binary to hex
    }
    else
    {
        New_Hex = dec_to_hex(New_Hex_Dec); // convert from positive number back to hex
    }

    return New_Hex;
}

/// Gets the word at index 'word_index' from 'text' (0 based)
string Get_Word(string text, int word_index)
{
    vector<string> words = Split(text, " ");

    if (words.size() > word_index)
    {
        return words[word_index];
    }
    else
    {
        return "";
    }
}

vector<string> Split(string line, string Splitter)
{
    vector<string> List;

    // while the line contains the splitter
    while (IndexOf_S(line, Splitter) != -1)
    {
        List.push_back(line.substr(0, IndexOf_S(line, Splitter)));
        line = line.substr(IndexOf_S(line, Splitter) + Splitter.size());
    }
    List.push_back(line);

    return List;
}

string Remove_Whitespace(string text)
{
    text = RemoveAll(text, ' ');
    text = RemoveAll(text, '\t');

    return text;
}

vector<string> String_Split(string text, int split)
{
    vector<string> Data;

    for (int i = 0; i < text.size(); i += split)
    {
        Data.push_back(text.substr(i, split));
    }

    return Data;
}

string Leading_Zeroes(string hex, int total_length)
{
    while (hex.size() < total_length)
    {
        hex = "0" + hex;
    }

    return hex;
}
