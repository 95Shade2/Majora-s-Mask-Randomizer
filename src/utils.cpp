#include "rando/utils.hpp"

#include <algorithm>
#include <cmath>
#include <ios>
#include <sstream>
#include <stdexcept>

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
            binary << "1010";
            break;
        case 'B':
            binary << "1011";
            break;
        case 'C':
            binary << "1100";
            break;
        case 'D':
            binary << "1101";
            break;
        case 'E':
            binary << "1110";
            break;
        case 'F':
            binary << "1111";
            break;
        }
    }
    return binary.str();
}

string dec_to_hex(int number)
{
    std::stringstream hex;
    hex << std::hex << number;
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
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1)
        throw std::invalid_argument("odd length");

    string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char *p = std::lower_bound(lut, lut + 16, a);
        if (*p != a)
            throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char *q = std::lower_bound(lut, lut + 16, b);
        if (*q != b)
            throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

// converts a hexadecimal string to a decimal integer
int hex_to_decimal(const string &hex)
{
    return std::stoi(hex, 0, 16);
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

    for (int i = 0; i < binary.size(); i += 4)
    {
        hex << std::stoi(binary.substr(i, 4));
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
    for (int i = 0; i < Data.size(); i++)
    {
        int a = i;
        bool same = true;

        for (int b = 0; b < Text.size(); b++)
        {
            if (Data[a] != Text[b])
            {
                same = false;
            }

            a++;
        }

        if (same)
        {
            return i;
        }
    }

    return -1;
}
