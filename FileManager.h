//ƒобавить обработку ошибок при вводе
//Ќаверно стоит помен€ть поиск первой точки на поиск последней
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

    void flags_parser(string all_flags);

    virtual void del(path path, vector<string>& ext, vector<string>& exeptions);

    virtual void ren(path path, vector<string>& ext, vector<string>& exeptions, string name);

    virtual void cre(path path, string name, int count_f);

    bool checker(string name, vector<string>& del_list);

    bool have_danger_characters(string name);

public:
    
    FileManager();
    //void ui_asking();
};

class FileManagerUI : public FileManager {
public:
    void ui_asking();
     //повысили уровень доступа до public

};

class FileManagerBK : public FileManager {
public:

};

#endif //FILEMANAGER_H