#include "rando/io.hpp"
#include "rando/logging.hpp"
#include "rando/utils.hpp"

#include <fstream>
#include <iostream>

std::map<std::string, std::map<std::string, std::string>>
OpenAsIni(const std::string &filename)
{
    ifstream file;
    string line = "";
    string key = "";
    string value = "";
    string section = "";
    map<string, map<string, string>> File_Contents;

    file.open(filename.c_str());

    if (!file)
    {
        err_file << "Error opening " << filename.substr(2);
        err_file.close();
        exit(0);
    }

    // get each item
    while (getline(file, line) && line != "")
    {
        if (section == "" || line[0] == '[')
        {
            section = line.substr(1, IndexOf(line, ']') - 1);
        }
        else
        {
            key = line.substr(0, IndexOf(line, '='));
            value = line.substr(IndexOf(line, '=') + 1);

            File_Contents[section][key] = value;
        }
    }

    file.close();

    return File_Contents;
}

void Open_File(const string &filename, fstream &file)
{
    file.open(filename.c_str(), fstream::binary | fstream::out | fstream::in);

    if (!file)
    {
        std::cout << "\nError - opening\t" + filename;
        exit(0);
    }
}
