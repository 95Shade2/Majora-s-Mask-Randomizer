#include "rando/error.hpp"

#include <iostream>

void Error(const string& Error_Message) {
    cout << Error_Message;
    err_file << Error_Message;
    err_file.close();
    exit(0);
}
