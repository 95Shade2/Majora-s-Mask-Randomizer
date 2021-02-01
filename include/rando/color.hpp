#ifndef RANDO_COLOR_HPP
#define RANDO_COLOR_HPP

#include <string>
#include <vector>
#include <map>

std::string Get_Red(const std::string &colors);

int Get_Red_Dec(const std::string &colors);

std::string Get_Green(const std::string &colors);

int Get_Green_Dec(const std::string &colors);

std::string Get_Blue(const std::string &colors);

int Get_Blue_Dec(const std::string &colors);

std::string Color_To_Hex(const std::string &colors);

std::map<std::string, double> RGB_To_HSL(int red, int green, int blue);

std::map<std::string, double> RGB_To_HSL(std::string Hex_Colors);

// takes hex in form RRGGBB and converts it to rbg5a1
std::string hex_to_rgb5a1(std::string hex);

std::string rgb5a1_to_hex(std::string rbga);

std::string HSL_To_RGB(double H, double S, double L);

double Get_Hue(std::vector<double> Prime);

double Get_Lightness(std::vector<double> Prime);

double Get_Saturation(std::vector<double> Prime);

#endif
