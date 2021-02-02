#include "rando/utils.hpp"

#include <algorithm>
#include <cmath>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <iomanip>

string Single_Hex(int number)
{
    if (number == 0)
    {
        return "0";
    }
    else if (number == 1)
    {
        return "1";
    }
    else if (number == 2)
    {
        return "2";
    }
    else if (number == 3)
    {
        return "3";
    }
    else if (number == 4)
    {
        return "4";
    }
    else if (number == 5)
    {
        return "5";
    }
    else if (number == 6)
    {
        return "6";
    }
    else if (number == 7)
    {
        return "7";
    }
    else if (number == 8)
    {
        return "8";
    }
    else if (number == 9)
    {
        return "9";
    }
    else if (number == 10)
    {
        return "A";
    }
    else if (number == 11)
    {
        return "B";
    }
    else if (number == 12)
    {
        return "C";
    }
    else if (number == 13)
    {
        return "D";
    }
    else if (number == 14)
    {
        return "E";
    }
    else if (number == 15)
    {
        return "F";
    }
    else
    {
        return "";
    }
}

string hex_to_binary(string hex)
{
    string binary = "";
    string solo_hex;

    for (int i = 0; i < hex.size(); i++)
    {
        solo_hex = hex[i];

        if (solo_hex == "0")
        {
            binary += "0000";
        }
        else if (solo_hex == "1")
        {
            binary += "0001";
        }
        else if (solo_hex == "2")
        {
            binary += "0010";
        }
        else if (solo_hex == "3")
        {
            binary += "0011";
        }
        else if (solo_hex == "4")
        {
            binary += "0100";
        }
        else if (solo_hex == "5")
        {
            binary += "0101";
        }
        else if (solo_hex == "6")
        {
            binary += "0110";
        }
        else if (solo_hex == "7")
        {
            binary += "0111";
        }
        else if (solo_hex == "8")
        {
            binary += "1000";
        }
        else if (solo_hex == "9")
        {
            binary += "1001";
        }
        else if (solo_hex == "A")
        {
            binary += "1010";
        }
        else if (solo_hex == "B")
        {
            binary += "1011";
        }
        else if (solo_hex == "C")
        {
            binary += "1100";
        }
        else if (solo_hex == "D")
        {
            binary += "1101";
        }
        else if (solo_hex == "E")
        {
            binary += "1110";
        }
        else if (solo_hex == "F")
        {
            binary += "1111";
        }
    }

    return binary;
}

string dec_to_hex(int number)
{
    string hex = "";
    string binary = "";
    int power;
    char bin;
    int value = 0;

    number %= 256;

    // get the binary form of the numbers
    for (int i = 8; i > 0; i--)
    {
        power = pow(2, i) / 2;

        if (number >= power)
        {
            number -= power;
            binary += "1";
        }
        else
        {
            binary += "0";
        }
    }

    // convert the binary form to the hex form
    for (int i = 0; i < binary.size() / 2; i++)
    {
        bin = binary[i];
        power = pow(2, 4 - i) / 2;

        if (bin == '1')
        {
            value += power;
        }
    }

    hex = Single_Hex(value);
    value = 0;

    // convert the last four binary
    for (int i = 0; i < binary.size() / 2; i++)
    {
        bin = binary[i + 4];
        power = pow(2, 4 - i) / 2;

        if (bin == '1')
        {
            value += power;
        }
    }

    hex += Single_Hex(value);

    return hex;
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
int hex_to_decimal(string hex)
{
    int number = 0;

    for (int i = 0; i < hex.size(); i++)
    {
        number *= 16;
        char h = hex[i];

        if (h == 'A')
        {
            number += 10;
        }
        else if (h == 'B')
        {
            number += 11;
        }
        else if (h == 'C')
        {
            number += 12;
        }
        else if (h == 'D')
        {
            number += 13;
        }
        else if (h == 'E')
        {
            number += 14;
        }
        else if (h == 'F')
        {
            number += 15;
        }
        else
        {
            number += (h - '0');
        }
    }

    return number;
}

int string_to_dec(string text)
{
    int number = 0;

    for (int i = 0; i < text.size(); i++)
    {
        number *= 10;
        number += (text[i] - '0');
    }

    return number;
}

string dec_to_string(int dec, int depth)
{
    char digit;

    if (dec == 0)
    {
        if (depth == 0)
        {
            return "0";
        }
        else
        {
            return "";
        }
    }
    else
    {
        digit = (dec % 10) + '0';
        return dec_to_string(dec / 10, ++depth) + digit;
    }
}

bool isNumber(string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        if ((text[i] - '0') > 9)
        {
            return false;
        }
    }

    return true;
}

string binary_to_hex(string binary)
{
    string hex = "";
    string single;

    for (int i = 0; i < binary.size(); i += 4)
    {
        single = binary.substr(i, 4);

        if (single == "0000")
        {
            hex += "0";
        }
        else if (single == "0001")
        {
            hex += "1";
        }
        else if (single == "0010")
        {
            hex += "2";
        }
        else if (single == "0011")
        {
            hex += "3";
        }
        else if (single == "0100")
        {
            hex += "4";
        }
        else if (single == "0101")
        {
            hex += "5";
        }
        else if (single == "0110")
        {
            hex += "6";
        }
        else if (single == "0111")
        {
            hex += "7";
        }
        else if (single == "1000")
        {
            hex += "8";
        }
        else if (single == "1001")
        {
            hex += "9";
        }
        else if (single == "1010")
        {
            hex += "A";
        }
        else if (single == "1011")
        {
            hex += "B";
        }
        else if (single == "1100")
        {
            hex += "C";
        }
        else if (single == "1101")
        {
            hex += "D";
        }
        else if (single == "1110")
        {
            hex += "E";
        }
        else if (single == "1111")
        {
            hex += "F";
        }
        else
        {
            hex += "";
        }
    }

    return hex;
}

string decimal_to_hex(int dec)
{
    string hex = "";
    int d = 0;

    if (dec == 0)
    {
        return "";
    }
    else
    {
        d = (dec % 16);

        if (d >= 10)
        {
            hex = (d / 10) + '0';
            hex += (d % 10) + '0';
        }
        else
        {
            hex = d + '0';
        }

        if (hex == "10")
        {
            hex = "A";
        }
        else if (hex == "11")
        {
            hex = "B";
        }
        else if (hex == "12")
        {
            hex = "C";
        }
        else if (hex == "13")
        {
            hex = "D";
        }
        else if (hex == "14")
        {
            hex = "E";
        }
        else if (hex == "15")
        {
            hex = "F";
        }

        return decimal_to_hex(dec / 16) + hex;
    }
}


// replace each 1 with a 0, and each 0 with a 1
string invert_binary(string binary)
{
    string inverted = "";

    for (int i = 0; i < binary.size(); i++)
    {
        if (binary[i] == '0')
        {
            inverted += "1";
        }
        else
        {
            inverted += "0";
        }
    }

    return inverted;
}

string Hex_Minus(string hex, string hex_2)
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


string Even_Hex(string hex)
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

string Bits_Or(string bits_1, string bits_2)
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

// clears the bit at index bit_index of a byte
string Bit_Clear(string byte, int bit_index)
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

string Replace(string bigger_text, string smaller_text, string new_smaller_text)
{
    string new_text = "";
    for (int a = 0; a < bigger_text.size(); a++)
    {
        int c = a;
        int b = 0;

        for (b = 0; b < smaller_text.size(); b++)
        {
            // not equal
            if (bigger_text[c] != smaller_text[b])
            {
                b = smaller_text.size();
            }
            c++;
        }
        // found a match
        if (b == smaller_text.size())
        {
            new_text += new_smaller_text; // get the replacement
            a += smaller_text.size() -
                 1; // skip passed the old text (-1 cause for loop will increase it by 1)
        }
        else
        {
            new_text += bigger_text[a];
        }
    }

    return new_text;
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

bool Contains(string text, char chr)
{
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == chr)
        {
            return true;
        }
    }

    return false;
}

string Remove_Whitespace(string text)
{
    text = RemoveAll(text, ' ');
    text = RemoveAll(text, '\t');

    return text;
}


string RemoveAll(string text, char chr)
{
    string newText = "";

    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] != chr)
        {
            newText += text[i];
        }
    }

    return newText;
}

int IndexOf_S(string Data, string Text)
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