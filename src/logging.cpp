#include "rando/logging.hpp"

#include <iostream>
#include <chrono>
#include <ctime>

[[noreturn]] void Error(const string &Error_Message)
{
    cout << Error_Message;
    err_file << Error_Message;
    err_file.close();
    exit(0);
}

void log(const vector<string> &stuff)
{
    for (int i = 0; i < stuff.size(); i++)
    {
        cout << stuff[i] << endl;
    }

    cout << endl;
}

void Success() {
	err_file << "Success";
}

void Logger(string Log_Message) {
	if (DEBUG) {
		log_file << Log_Message << endl;
		log_file.flush();	//make sure to write all the current data to the file
	}
}

void Log_Vector(vector<string> Log_Vec) {
	for (int v = 0; v < Log_Vec.size(); v++) {
		Logger(Log_Vec[v]);
	}
}

///returns a list of items that are in both vectors
vector<string> And(vector<string> vec1, vector<string> vec2) {
	vector<string> both;

	for (int v1 = 0; v1 < vec1.size(); v1++) {
		for (int v2 = 0; v2 < vec2.size(); v2++) {
			if (vec1[v1] == vec2[v2]) {
				both.push_back(vec1[v1]);
			}
		}
	}

	return both;
}