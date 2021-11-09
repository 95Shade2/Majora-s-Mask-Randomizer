#include "rando/color.hpp"
#include "rando/utils.hpp"

#include <cmath>

double round(double num, int places);
int Min(vector<double> numbers);
int Max(vector<double> numbers);

// bad shade!
using namespace std;

std::string Get_Red(const std::string &colors)
{
    std::string R;

    R = colors.substr(IndexOf_S(colors, "R=") + 2);
    R = R.substr(0, IndexOf_S(R, ","));
    R = dec_to_hex(string_to_dec(R));

    return R;
}

int Get_Red_Dec(const std::string &colors)
{
    std::string R;

    R = colors.substr(IndexOf_S(colors, "R=") + 2);
    R = R.substr(0, IndexOf_S(R, ","));

    return string_to_dec(R);
}

std::string Get_Green(const std::string &colors)
{
    std::string G;

    G = colors.substr(IndexOf_S(colors, "G=") + 2);
    G = G.substr(0, IndexOf_S(G, ","));
    G = dec_to_hex(string_to_dec(G));

    return G;
}

int Get_Green_Dec(const std::string &colors)
{
    std::string G;

    G = colors.substr(IndexOf_S(colors, "G=") + 2);
    G = G.substr(0, IndexOf_S(G, ","));

    return string_to_dec(G);
}

std::string Get_Blue(const std::string &colors)
{
    std::string B;

    B = colors.substr(IndexOf_S(colors, "B=") + 2);
    B = B.substr(0, IndexOf_S(B, "]"));
    B = dec_to_hex(string_to_dec(B));

    return B;
}

int Get_Blue_Dec(const std::string &colors)
{
    std::string B;

    B = colors.substr(IndexOf_S(colors, "B=") + 2);
    B = B.substr(0, IndexOf_S(B, "]"));

    return string_to_dec(B);
}

std::string Color_To_Hex(const std::string &colors)
{
    std::string R;
    std::string G;
    std::string B;

    R = Get_Red(colors);
    G = Get_Green(colors);
    B = Get_Blue(colors);

    return R + G + B;
}

map<string, double> RGB_To_HSL(int red, int green, int blue)
{
    map<string, double> HSL;
    vector<double> Prime; // 0 = red, 1 = green, 2 = blue (indexes)
    int C_Max;
    int C_Min;
    double Difference;

    Prime.push_back(red / 255.0);
    Prime.push_back(green / 255.0);
    Prime.push_back(blue / 255.0);

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    Difference = Prime[C_Max] - Prime[C_Min];

    HSL["H"] = Get_Hue(Prime);

    HSL["S"] = Get_Saturation(Prime);

    HSL["L"] = Get_Lightness(Prime);

    return HSL;
}

map<string, double> RGB_To_HSL(string Hex_Colors)
{
    int red, green, blue;

    red = hex_to_decimal(Hex_Colors.substr(0, 2));
    green = hex_to_decimal(Hex_Colors.substr(2, 2));
    blue = hex_to_decimal(Hex_Colors.substr(4, 2));

    return RGB_To_HSL(red, green, blue);
}

// takes hex in form RRGGBB and converts it to rbg5a1
string hex_to_rgb5a1(string hex)
{
    string rbga;
    string binary;

    binary = hex_to_binary(hex);

    rbga = binary.substr(0, 5);   // first 5 red bits
    rbga += binary.substr(8, 5);  // first 5 green bits
    rbga += binary.substr(16, 5); // first 5 blue bits
    rbga += "1";                  // alpha bit

    rbga = binary_to_hex(rbga);

    return rbga;
}

string rgb5a1_to_hex(string rbga)
{
    string hex;
    string binary;

    binary = hex_to_binary(rbga);

    hex = binary.substr(0, 5) + "000";
    hex += binary.substr(5, 5) + "000";
    hex += binary.substr(10, 5) + "000";

    hex = binary_to_hex(hex);

    return hex;
}

double round(double num, int places)
{
    int New_Num;

    if (places == 0)
    {
        New_Num = num;
        return New_Num;
    }
    else
    {
        return round(num * 10, --places) / 10.0;
    }
}

string HSL_To_RGB(double H, double S, double L)
{
    string RGB = "";
    double Prime_R, Prime_G, Prime_B;
    double C, X, m;
    int t;
    double d;
    int Red, Green, Blue;
    int places = 2;

    S /= 100;
    L /= 100;

    S = round(S, places);
    L = round(L, places);

    d = 2 * L - 1;
    d = abs(d);
    d = 1 - d;
    C = d * S;

    d = H / 60;
    d = fmod(d, 2); // d % 2
    d = d - 1;
    d = abs(d);
    d = 1 - d;
    X = C * d;

    m = L - C / 2;

    if (H <= 60)
    { // C, X, 0
        Prime_R = C;
        Prime_G = X;
        Prime_B = 0;
    }
    else if (H <= 120)
    { // X, C, 0
        Prime_R = X;
        Prime_G = C;
        Prime_B = 0;
    }
    else if (H <= 180)
    { // 0, C, X
        Prime_R = 0;
        Prime_G = C;
        Prime_B = X;
    }
    else if (H <= 240)
    { // 0, X, C
        Prime_R = 0;
        Prime_G = X;
        Prime_B = C;
    }
    else if (H <= 300)
    { // X, 0, C
        Prime_R = X;
        Prime_G = 0;
        Prime_B = C;
    }
    else
    { // C, 0, X
        Prime_R = C;
        Prime_G = 0;
        Prime_B = X;
    }

    Red = (Prime_R + m) * 255;
    Green = (Prime_G + m) * 255;
    Blue = (Prime_B + m) * 255;

    RGB = dec_to_hex(Red);
    RGB += dec_to_hex(Green);
    RGB += dec_to_hex(Blue);

    return RGB;
}

double Get_Hue(vector<double> Prime)
{
    double Difference;
    int C_Max;
    int C_Min;
    int Hue;
    double temp;

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    Difference = Prime[C_Max] - Prime[C_Min];

    if (Difference == 0)
    {
        Hue = 0;
    }
    else if (C_Max == 0)
    {
        temp = Prime[1] - Prime[2];
		temp = abs(temp);
        temp = temp / Difference;
        temp = fmod(temp, 6);
        Hue = 60 * temp;
    }
    else if (C_Max == 1)
    {
        temp = Prime[2] - Prime[0];
		temp = abs(temp);
        temp = temp / Difference;
        temp = temp + 2;
        Hue = 60 * temp;
    }
    else
    {
        temp = Prime[0] - Prime[1];
		temp = abs(temp);
        temp = temp / Difference;
        temp = temp + 4;
        Hue = 60 * temp;
    }

    return Hue;
}

double Get_Lightness(vector<double> Prime)
{
    int C_Max;
    int C_Min;

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    return ((Prime[C_Max] + Prime[C_Min]) * 100) / 2;
}

double Get_Saturation(vector<double> Prime)
{
    int C_Max;
    int C_Min;
    double Difference;
    double Light;
    double Sat;

    C_Max = Max(Prime);
    C_Min = Min(Prime);

    Difference = Prime[C_Max] - Prime[C_Min];
    Light = Get_Lightness(Prime) / 100;

    if (Difference == 0)
    {
        Sat = 0;
    }
    else
    {
        Sat = Difference / (1 - abs(2 * Light - 1));
    }

    return Sat * 100;
}

// only positive numbers
// returns the index of the max number
// if there are more than one numbers, then it returns the index of the first one
// returns -1 if vector is empty or no values are greater or equal to 0
int Min(vector<double> numbers)
{
    double Min_Number = -1;
    int Min_Index = -1;

    for (int i = 0; i < numbers.size(); i++)
    {
        if (numbers[i] < Min_Number || Min_Number == -1)
        {
            Min_Number = numbers[i];
            Min_Index = i;
        }
    }

    return Min_Index;
}

// only positive numbers
// returns the index of the max number
// if there are more than one numbers, then it returns the index of the first one
// returns -1 if vector is empty or no values are greater or equal to 0
int Max(vector<double> numbers)
{
    double Max_Number = -1;
    int Max_Index = -1;

    for (int i = 0; i < numbers.size(); i++)
    {
        if (numbers[i] > Max_Number)
        {
            Max_Number = numbers[i];
            Max_Index = i;
        }
    }

    return Max_Index;
}
