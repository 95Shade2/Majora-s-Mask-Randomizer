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

void Write_Function(int address, vector<string> function) {
	string command;
	
	for (int c = 0; c < function.size(); c++) {
		command = function[c];
		Write_To_Rom(address + (c * 4), command);
	}
}

long GetFileSize(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

//reads from the rom and returns in hex
string Read_From_Rom(string rom_path, int offset, int length_in_bytes) {
	ifstream rom(rom_path);
	string data = "";
	char data_c[2] = { 0, '\0' };

	if (rom.is_open()) {
		rom.seekg(offset);
		for (int b = 0; b < length_in_bytes; b++) {
			rom.read(data_c, 1);
			data += data_c[0];
		}
	}

	return data;
}

string Valid_Rom(string rom_path) {
	string error_message = "Could not find file\n" + rom_path;
	string file_ext = "";
	string file_header = "";
	string header_text = "ZELDA MAJORA'S MASK";
	ifstream rom(rom_path.c_str());
	int index = -1;
	long file_size = 0;
	long rom_size = 33554432;

	//if the file exist
	if (rom.is_open()) {
		rom.close();
		error_message = "Could not determine the file type\n" + rom_path;

		//if the file has an extension
		index = IndexOf_Last(rom_path, '.');
		if (index != -1) {
			error_message = "File specified for rom has incorrect file type\n" + rom_path + "\nValid type is z64";

			//if the file has the correct extension
			file_ext = rom_path.substr(index + 1);
			if (file_ext == "z64") {
				error_message = "Incorrect file size detected for\n" + rom_path;

				//if the file is the correct size
				file_size = GetFileSize(rom_path);
				if (file_size == rom_size) {
					error_message = "Incorrect rom, not a Majora's Mask rom\n" + rom_path;

					file_header = Read_From_Rom(rom_path, 32, 19);
					if (file_header == header_text) {
						return "";
					}
				}
			}
		}
	}

	return error_message;
}
