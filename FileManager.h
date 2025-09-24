//Добавить обработку ошибок при вводе
//Наверно стоит поменять поиск первой точки на поиск последней
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <locale>
#include <windows.h>
#include <thread>
#include <future>
#include <algorithm>
using namespace std;
using namespace std::filesystem;

class FileManager {
    enum Flag {
        recursion_on,
        recursion_off,
        del_dir,
        del_files,
        del_dir_files,
        cre_files,
        cre_dir,
        ren_files,
        ren_dir,
        ren_dir_files
    };

    Flag recf;
    Flag delf;
    Flag cref;
    Flag renf;
    vector<char> danger_chars;

    bool stop;

    bool is_correct_flags_string(string flags_string);

    void flags_parser(string all_flags);

    void del(path path, vector<string>& ext, vector<string>& exeptions);

    void ren(path path, vector<string>& ext, vector<string>& exeptions, string name);

    void cre(path path, string name, int count_f);

    bool checker(string name, vector<string>& del_list);

    bool have_danger_characters(string name);

public:
    
    FileManager();
    void ui_asking();
};

#endif //FILEMANAGER_H