#include "rando/rom.hpp"
#include "rando/utils.hpp"
#include "rando/logging.hpp"

#include <iostream>
#include <thread>

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

void Write_To_Rom(string file_path, int address, string hex)
{
	try
	{
		if (Contains(hex, ' '))
		{
			hex = RemoveAll(hex, ' ');
		}
		inFile.open(file_path, fstream::binary | fstream::out | fstream::in);
		inFile.seekg(address);
		inFile.write(hex_to_string(hex).c_str(), hex.length() / 2);
		inFile.close();
	}
	catch (exception e)
	{
		err_file << "Error writing to the rom: " << e.what();
		err_file.close();
		exit(0);
	}
}

void Write_To_Rom_Raw(string file_path, int address, string raw_data)
{
	try
	{

		inFile.open(file_path, fstream::binary | fstream::out | fstream::in);
		inFile.seekg(address);
		inFile.write(raw_data.c_str(), raw_data.length() / 2);
		inFile.close();
	}
	catch (exception e)
	{
		err_file << "Error writing to the rom: " << e.what();
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
	ifstream rom(rom_path, ios_base::binary);
	string data = "";
	char data_c[2] = { 0, '\0' };

	if (rom.is_open()) {
		rom.seekg(offset);
		for (int b = 0; b < length_in_bytes; b++) {
			rom.read(data_c, 1);
			data += data_c[0];
		}
		rom.close();
	}

	return data;
}

void Copy_Rom(string old_path, string new_path) {
	string instruction = "copy \"" + old_path + "\" \"" + new_path + "\"";
	system(instruction.c_str());
}

void Swap(char *one, char *two) {
	char temp = *one;
	*one = *two;
	*two = temp;
}

void Swap_Bytes(string file_path, int groups) {
	long rom_size = 33554432 * 2;
	string data;
	int length = 2592;

	//get the rom data
	data = Read_From_Rom(file_path, 0, rom_size);

	//byteswap the data
	for (long g = 0; g < rom_size; g += groups) {
		//n64
		if (groups == 4) {
			Swap(&data[g], &data[g + 3]);
			Swap(&data[g + 1], &data[g + 2]);
		}
		//v64
		else if (groups == 2) {
			Swap(&data[g], &data[g + 1]);
		}
	}

	//write the byteswapped data to the new z64 file
	Write_To_Rom_Raw(file_path, 0, data);
}

//converts a rom to z64 if it's not already that format
string Convert_Z64(string file_path, string file_type) {
	string z64_path = "./mmz.z64";
	map<string, int> type_bytes = {
		{"n64", 4},
		{"v64", 2}
	};

	if (file_type == "z64") {
		return file_path;
	}
	else {
		cout << "Converting rom from " << file_type << " to z64\n";

		Copy_Rom(file_path, z64_path);
		Swap_Bytes(z64_path, type_bytes[file_type]);

		cout << "Converted rom\n";

		return z64_path;
	}
}

string Valid_Rom(string rom_path, string *file_ext) {
	string error_message = "Could not find file\n" + rom_path;
	string file_header = "";
	ifstream rom(rom_path.c_str());
	int index = -1;
	long file_size = 0;
	long rom_size = 33554432;
	map<string, bool> Valid_Types = {
		{"z64", true},
		{"n64", true},
		{"v64", true}
	};
	map<string, string> Headers = {
		{"z64", "ZELDA MAJORA'S MASK "},
		{"n64", "DLEZAM AAROJM S' KSA"},
		{"v64", "EZDL AAMOJARS'M SA K"}
	};

	//if the file exist
	if (rom.is_open()) {
		rom.close();
		error_message = "Could not determine the file type\n" + rom_path;

		//if the file has an extension
		index = IndexOf_Last(rom_path, '.');
		if (index != -1) {
			error_message = "File specified for rom has incorrect file type\n" + rom_path + "\nValid types are z64, n64, and v64";

			//if the file has the correct extension
			*file_ext = rom_path.substr(index + 1);
			if (Valid_Types[*file_ext]) {
				error_message = "Incorrect file size detected for\n" + rom_path;

				//if the file is the correct size
				file_size = GetFileSize(rom_path);
				if (file_size == rom_size) {
					error_message = "Incorrect rom, not a Majora's Mask rom\n" + rom_path;

					//if the file headers match
					file_header = Read_From_Rom(rom_path, 32, 20);
					if (file_header == Headers[*file_ext]) {
						return "";
					}
				}
			}
		}
	}

	return error_message;
}
