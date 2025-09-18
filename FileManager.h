//Добавить обработку ошибок при вводе
//Добавить флаги: -rec/-recn - рекурсивный обход, -deld/-delf/-delfd - флаги для удаления, -cref/-cred - флаги для создания, -renf/-rend/renfd - флаги для переименования
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

    bool stop;

    bool is_correct_flags_string(string flags_string);

    void flags_parser(string all_flags);

    void del(path path, vector<string>& ext, vector<string>& exeptions);

    void ren(path path, vector<string>& ext, vector<string>& exeptions, string name);

    void cre(path path, string name, int count_f);

    bool checker(string name, vector<string>& del_list);

public:
    FileManager();
    void ui_asking();
};

#endif //FILEMANAGER_H