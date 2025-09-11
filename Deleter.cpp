#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

using namespace std;
using namespace std::filesystem;

class Deleter {
    bool stop = false;

    void del(path path, vector<string>& ext) {
        if (exists(path)) {
            if (checker(path.filename().string(), ext)) {
                if (is_directory(path)) remove_all(path);
                else remove(path);
                return;
            }
            for (auto& it : directory_iterator(path)) {
                if (is_directory(it.path())) {
                    if (checker(it.path().filename().string(), ext)) {
                        remove_all(it.path());
                    }
                    else del(it.path(), ext);
                }
                else {
                    if (checker(it.path().filename().string(), ext)) {
                        remove(it.path());
                    }
                }
            }
        }
        else {
            return;
        }
    }

    bool checker(string name, vector<string>& del_list) {
        for (int i = 0; i < del_list.size(); i++) {
            int left = 0, right = 0;
            int l = 0, rs = 0;
            bool flag = true;
            bool ending = false;
            int last_rs = 0;
            bool starting = false;
            bool flag_for_starting = false;
            while (right < del_list[i].length()) {
                while (right < del_list[i].length() && del_list[i][right] != '*') {
                    right++;
                }
                if (right == 0) starting = true;
                if (right == 0) l++;
                if (right == del_list[i].length() - 1) ending = true;
                string sub = del_list[i].substr(left, right - left);
                //cout << sub << endl;
                rs = sub.size();
                while (rs + l < name.length() && name.substr(l, rs) != sub) {
                    //cout << name.substr(l, rs) << endl;
                    l++;
                }
                //cout << name.substr(l, rs) << endl;
                last_rs = rs;
                if (name.substr(l, rs) != sub) {
                    right = del_list[i].length();
                    flag = false;
                }
                else {
                    if (starting && (l != 0) || !starting && (l == 0)) flag_for_starting = true;
                    left = right + 1;
                    right = left;
                    //l++;
                    rs = 0;
                }
            }
            if (flag_for_starting && flag && ((ending && ((l + last_rs) < (name.length() - 1))) || (!ending && ((l + last_rs) >= (name.length() - 1))))) return true;
        }
        return false;
    }

public:
    void ui_asking() {
        setlocale(LC_ALL, "Ru");
        while (!stop) {
            cout << "Удалить список файлов/папок [1]\nЗакрыть [2]\nВыберите команду: ";
            string com;
            getline(cin, com);
            if (com == "1") {
                string path, d;
                vector<string> del_vec;
                cout << "Введите путь: ";
                getline(cin, path);
                if (exists(path)) {
                    cout << "Введите список ключевых слов для удаления (если список закончен, то введите '.'): " << endl;
                    do {
                        getline(cin, d);
                        del_vec.push_back(d);
                    } while (d != ".");
                    del(path, del_vec);
                }
                else {
                    cout << "Такого пути не существует" << endl;
                }
            }
            else if (com == "2") {
                stop = true;
            }
            else {
                cout << "Такой команды не существует!" << endl;
            }
        }
        cout << "Работа завершена." << endl;
        cin.ignore(); //очистка буфера
        cin.get(); //ждёт нажатия Enter
    }
};

int main()
{
    Deleter del;
    del.ui_asking();

    return 0;

}