#include "rando/rom.hpp"
#include "rando/utils.hpp"
#include "rando/logging.hpp"

#include <iostream>

void Write_To_Rom(int address, string hex)
{
    try
    {
        if (Contains(hex, ' '))
        {
            hex = RemoveAll(hex, ' ');
        }
        inFile.open(Rom_Location, fstream::binary | fstream::out | fstream::in);
        inFile.seekg(address);
        inFile.write(hex_to_string(hex).c_str(), hex.length() / 2);
        inFile.close();
    }
    catch (exception e)
    {
        err_file << "Error writing to the decompressed rom: " << e.what();
        err_file.close();
        exit(0);
    }
}

