#include "rando/color.hpp"
#include "rando/utils.hpp"

std::string Get_Red(const std::string& colors)
{
    std::string R;

    R = colors.substr(IndexOf_S(colors, "R=") + 2);
    R = R.substr(0, IndexOf_S(R, ","));
    R = dec_to_hex(string_to_dec(R));

    return R;
}

int Get_Red_Dec(const std::string& colors)
{
    std::string R;

    R = colors.substr(IndexOf_S(colors, "R=") + 2);
    R = R.substr(0, IndexOf_S(R, ","));

    return string_to_dec(R);
}

std::string Get_Green(const std::string& colors)
{
    std::string G;

    G = colors.substr(IndexOf_S(colors, "G=") + 2);
    G = G.substr(0, IndexOf_S(G, ","));
    G = dec_to_hex(string_to_dec(G));

    return G;
}

int Get_Green_Dec(const std::string& colors)
{
    std::string G;

    G = colors.substr(IndexOf_S(colors, "G=") + 2);
    G = G.substr(0, IndexOf_S(G, ","));

    return string_to_dec(G);
}

std::string Get_Blue(const std::string& colors)
{
    std::string B;

    B = colors.substr(IndexOf_S(colors, "B=") + 2);
    B = B.substr(0, IndexOf_S(B, "]"));
    B = dec_to_hex(string_to_dec(B));

    return B;
}

int Get_Blue_Dec(const std::string& colors)
{
    std::string B;

    B = colors.substr(IndexOf_S(colors, "B=") + 2);
    B = B.substr(0, IndexOf_S(B, "]"));

    return string_to_dec(B);
}

std::string Color_To_Hex(const std::string& colors)
{
    std::string R;
    std::string G;
    std::string B;

    R = Get_Red(colors);
    G = Get_Green(colors);
    B = Get_Blue(colors);

    return R + G + B;
}
