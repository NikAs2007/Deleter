
//Наверно стоит поменять поиск первой точки на поиск последней
//переопределить flags_parser для BK
//добавить флаг чтобы функции не трогали корневую папку
//АХХАХХА многопоточка херня - переделывай
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
protected:
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
        ren_dir_files,
        reg_on,
        reg_off
    };

    Flag recf;
    Flag delf;
    Flag cref;
    Flag renf;
    Flag regf;
    vector<char> danger_chars;
    vector<string> avaliable_flags;

    bool stop;

    bool is_correct_flags_string(string flags_string);

    virtual bool flags_parser(string all_flags);

    bool del(path path, vector<string>& ext, vector<string>& exeptions);

    bool ren(path path, vector<string>& ext, vector<string>& exeptions, string name);

    bool cre(path path, string name, int count_f);

    bool checker(string name, vector<string>& del_list);

    bool have_danger_characters(string name);

public:

    FileManager();

};

class FileManagerUI : public FileManager {
protected:
    bool flags_parser(string all_flags) override;
public:
    void ui_asking();
};

class FileManagerBK : public FileManager {
public:
    using FileManager::checker; //повысили уровень доступа до public
    using FileManager::ren;
    using FileManager::del;
    using FileManager::cre;
    using FileManager::have_danger_characters;
    using FileManager::is_correct_flags_string;
    using FileManager::flags_parser;
};

#endif //FILEMANAGER_H