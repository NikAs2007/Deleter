#include "Deleter.h"

void Deleter::del(path path, vector<string>& ext, vector<string>& exeptions) {
	if (exists(path)) {
		if (!checker(path.filename().string(), exeptions) && checker(path.filename().string(), ext)) {
			if (is_directory(path)) remove_all(path);
			else remove(path);
			return;
		}
		if (!checker(path.filename().string(), exeptions)) {
			for (auto& it : directory_iterator(path)) {
				if (is_directory(it.path())) {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						remove_all(it.path());
					}
					else del(it.path(), ext, exeptions);
				}
				else {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						remove(it.path());
					}
				}
			}
		}
	}
}

void Deleter::ren(path path, vector<string>& ext, vector<string>& exeptions, string name = "File") {
	if (exists(path)) {
		if (!checker(path.filename().string(), exeptions) && checker(path.filename().string(), ext)) {
			rename(path, path.parent_path().string() + '\\' + name);
			path = path.parent_path().string() + '\\' + name;
		}
		if (!checker(path.filename().string(), exeptions)) {
			int count_files = 0;
			for (auto& it : directory_iterator(path)) {
				if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
					++count_files;
				}
			}
			int num = 1;
			for (auto& it : directory_iterator(path)) {
				if (count_files <= 1) {
					if (is_directory(it.path())) {
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							rename(it.path(), it.path().parent_path().string() + '\\' + name);
						}
						ren(it.path().parent_path().string() + '\\' + name, ext, exeptions, name);
					}
					else {
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							rename(it.path(), it.path().parent_path().string() + '\\' + name);
						}
					}
				}
				else {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						string new_name;
						bool first_dot;
						do {
							new_name = it.path().parent_path().string() + "\\";
							first_dot = true;
							for (int i = 0; i < name.length(); i++) {
								if (name[i] != '.') {
									new_name += name[i];
								}
								else if (first_dot) {
									new_name += to_string(num++) + name[i];
									first_dot = false;
								}
							}
							if (first_dot) new_name += to_string(num++);
						} while (exists(new_name));
						rename(it.path(), new_name);
						if (is_directory(new_name)) {
							ren(new_name, ext, exeptions, name);
						}
					}
				}
			}
		}
	}
}

void Deleter::cre(path path, string name, int count_f) {
	if (exists(path)) {
		if (count_f < 1) return;
		if (count_f == 1) {
			ofstream{ path.string() + '\\' + name };
		}
		else {
			int num = 1;
			for (int i = 0; i < count_f; i++) {
				while (exists(path.string() + '\\' + name + to_string(num))) ++num;
				ofstream{ path.string() + '\\' + name + to_string(num) };
			}
		}
	}
}

bool Deleter::checker(string name, vector<string>& del_list) {
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
			rs = sub.size();
			while (rs + l < name.length() && name.substr(l, rs) != sub) {
				l++;
			}
			last_rs = rs;
			if (name.substr(l, rs) != sub) {
				right = del_list[i].length();
				flag = false;
			}
			else {
				if (starting && (l != 0) || !starting && (l == 0)) flag_for_starting = true;
				left = right + 1;
				right = left;
				rs = 0;
			}
		}
		if (flag_for_starting && flag && ((ending && ((l + last_rs) < (name.length()))) || (!ending && ((l + last_rs) >= (name.length() - 1))))) return true;
	}
	return false;
}

void Deleter::ui_asking() {
	while (!stop) {
		cout << "Создать файлы [1]\nПереименовать файлы [2]\nУдалить список файлов/папок [3]\nЗакрыть [4]\nВыберите команду: ";
		string com;
		getline(cin, com);
		if (com == "1") {
			//
			string path, d = "", name, repath = "";
			cout << "Введите путь: ";
			getline(cin, path);
			for (int c = 0; c < path.length(); ++c) {
				if (path[c] != '"') repath += path[c];
			}
			path = repath;
			if (exists(path)) {
				cout << "Введите имя создаваемого объекта: ";
				getline(cin, name);
				cout << "Введите количество создаваемых файлов: ";
				getline(cin, d);
				bool right_count = true;
				for (char x : d) {
					if (!isdigit(x)) right_count = false;
				}
				if (right_count) {
					cre(path, name, stoi(d));
				}
				else {
					cout << "Принимаются только числа!" << endl;
				}
			}
			else {
				cout << "Такого пути не существует" << endl;
			}
		}
		else if (com == "2") {
			//
			string path, d = "", repath = "", new_name = "";
			vector<string> ren_vec;
			vector<string> exeptions;
			cout << "Введите путь: ";
			getline(cin, path);
			for (int c = 0; c < path.length(); ++c) {
				if (path[c] != '"') repath += path[c];
			}
			path = repath;
			if (exists(path)) {
				//тут ошибка
				cout << "Введите новое имя объекта/объектов: ";
				getline(cin, new_name);
				cout << "Введите список файлов для переименования (если список закончен, то введите '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						ren_vec.push_back(d);
					}
				} while (d != ".");
				d = "";
				cout << "Введите список ключевых слов для исключений, это файлы, которые не будут удалены (если список закончен, то введите '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						exeptions.push_back(d);
					}
				} while (d != ".");
				ren(path, ren_vec, exeptions, new_name);
			}
			else {
				cout << "Такого пути не существует" << endl;
			}
		}
		else if (com == "3") {
			string path, d = "", repath = "";
			vector<string> del_vec;
			vector<string> exeptions;
			cout << "Введите путь: ";
			getline(cin, path);
			for (int c = 0; c < path.length(); ++c) {
				if (path[c] != '"') repath += path[c];
			}
			path = repath;
			if (exists(path)) {
				//тут ошибка
				cout << "Введите список ключевых слов для удаления (если список закончен, то введите '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						del_vec.push_back(d);
					}
				} while (d != ".");
				d = "";
				cout << "Введите список ключевых слов для исключений, это файлы, которые не будут удалены (если список закончен, то введите '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						exeptions.push_back(d);
					}
				} while (d != ".");
				del(path, del_vec, exeptions);
			}
			else {
				cout << "Такого пути не существует" << endl;
			}
		}
		else if (com == "4") {
			stop = true;
		}
		else {
			cout << "Такой команды не существует!" << endl;
		}
	}
	//cout << "Работа завершена." << endl;
	cin.ignore(); //очистка буфера
	cin.get(); //ждёт нажатия Enter
}