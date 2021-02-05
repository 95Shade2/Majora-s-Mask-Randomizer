#include "rando/logging.hpp"

#include <iostream>

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
