#ifndef DELETER_H
#define DELETER_H
//начинаем добавлять renaming and creating

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <locale>
#include <windows.h>
using namespace std;
using namespace std::filesystem;

class Deleter {
    bool stop = false;

    void del(path path, vector<string>& ext, vector<string>& exeptions);

    bool checker(string name, vector<string>& del_list);

public:
    void ui_asking();
};

#endif //DELETER_H