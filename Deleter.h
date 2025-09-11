#ifndef DELETER_H
#define DELETER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
using namespace std;
using namespace std::filesystem;

class Deleter {
    bool stop = false;

    void del(path path, vector<string>& ext);

    bool checker(string name, vector<string>& del_list);

public:
    void ui_asking();
};

#endif //DELETER_H