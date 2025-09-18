#include "FileManager.h"

FileManager::FileManager() {
	stop = false;
	recf = recursion_on;
	delf = del_dir_files;
	cref = cre_files;
	renf = ren_dir_files;
}

bool FileManager::is_correct_flags_string(string flags_string) {
	string flag;
	int i = 0;
	vector<string> avaliable_flags = { "-rec", "-recn", "-deld", "-delf", "-delfd", "-cref", "-cred", "-renf", "-rend", "-renfd" };
	while (i < flags_string.length()) {
		while (i < flags_string.length() && flags_string[i] != '-') {
			if (flags_string[i] != ' ') return false;
			i++;
		}
		flag = "";
		while (i < flags_string.length() && flags_string[i] != ' ') {
			flag += flags_string[i++];
		}
		if (find(avaliable_flags.begin(), avaliable_flags.end(), flag) == avaliable_flags.end() && flag != "") return false;
	}
	return true;
}

void FileManager::flags_parser(string all_flags) {
	if (!is_correct_flags_string(all_flags)) {
		cout << "Ошибка чтения флагов." << endl;
		return;
	}
	string flag;
	int i = 0;
	while (i < all_flags.length()) {
		while (i < all_flags.length() && all_flags[i] != '-') {
			i++;
		}
		flag = "";
		while (i < all_flags.length() && all_flags[i] != ' ') {
			flag += all_flags[i++];
		}
		if (flag == "-rec") recf = recursion_on;
		else if (flag == "-recn") recf = recursion_off;
		else if (flag == "-deld") delf = del_dir;
		else if (flag == "-delf") delf = del_files;
		else if (flag == "-delfd") delf = del_dir_files;
		else if (flag == "-cref") cref = cre_files;
		else if (flag == "-cred") cref = cre_dir;
		else if (flag == "-renf") renf = ren_files;
		else if (flag == "-rend") renf = ren_dir;
		else if (flag == "-renfd") renf = ren_dir_files;
	}
	cout << "Флаги успешно установлены." << endl;
}

void FileManager::del(path path, vector<string>& ext, vector<string>& exeptions) {
	if (exists(path)) {
		if (!checker(path.filename().string(), exeptions) && checker(path.filename().string(), ext)) {
			if ((delf == del_dir || delf == del_dir_files) && is_directory(path)) remove_all(path);
			else if ((delf == del_files || delf == del_dir_files) && !is_directory(path)) remove(path);
			return;
		}
		if (!checker(path.filename().string(), exeptions)) {
			for (auto& it : directory_iterator(path)) {
				if (is_directory(it.path())) {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						if (delf == del_dir || delf == del_dir_files) remove_all(it.path());
					}
					else if (recf == recursion_on) del(it.path(), ext, exeptions);
				}
				else {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						if (delf == del_dir_files || delf == del_files) remove(it.path());
					}
				}
			}
		}
	}
}

void FileManager::ren(path path, vector<string>& ext, vector<string>& exeptions, string name = "File") {
	if (exists(path)) {
		if (!checker(path.filename().string(), exeptions) && checker(path.filename().string(), ext)) {
			if ((is_directory(path) && (renf == ren_dir || renf == ren_dir_files)) || (!is_directory(path) && (renf == ren_files || renf == ren_dir_files))) rename(path, path.parent_path().string() + '\\' + name); 
			path = path.parent_path().string() + '\\' + name;
		}
		if (!checker(path.filename().string(), exeptions)) {
			int count_files = 0;
			int count_folders = 0;
			for (auto& it : directory_iterator(path)) {
				if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
					if ((renf == ren_files) || (renf == ren_dir_files) && !is_directory(it.path())) ++count_files;
					else if ((renf == ren_dir) || (renf == ren_dir_files) && is_directory(it.path())) ++count_folders;
				}
			}
			int num = 1;
			for (auto& it : directory_iterator(path)) {
				if (count_files + count_folders <= 1) {
					if (is_directory(it.path())) {
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							if (renf == ren_dir || renf == ren_dir_files) rename(it.path(), it.path().parent_path().string() + '\\' + name);
						}
						if (recf == recursion_on) ren(it.path().parent_path().string() + '\\' + name, ext, exeptions, name);
					}
					else {
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							if (renf == ren_dir_files || renf == ren_files) rename(it.path(), it.path().parent_path().string() + '\\' + name);
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
						if (is_directory(it.path()) && (renf == ren_dir || renf == ren_dir_files) || !is_directory(it.path()) && (renf == ren_files || renf == ren_dir_files)) rename(it.path(), new_name);
						if (is_directory(new_name)) {
							if (recf == recursion_on) ren(new_name, ext, exeptions, name);
						}
					}
				}
			}
		}
	}
}

//добавить рекурсию
void FileManager::cre(path path, string name, int count_f) {
	if (exists(path)) {
		if (count_f < 1) return;
		if (count_f == 1) {
			if (cref == cre_files) ofstream{ path.string() + '\\' + name };
			else if (cref == cre_dir) create_directory(path.string() + '\\' + name);
		}
		else {
			int num = 1;
			for (int i = 0; i < count_f; i++) {
				string new_name;
				bool first_dot;
				do {
					new_name = path.string() + "\\";
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
				if (cref == cre_files) ofstream{ new_name };
				else if (cref == cre_dir) create_directory(new_name);
			}
		}
	}
}

//есть ошибка в чекере: исключение Hah1 это еще и Hah10 и др.
bool FileManager::checker(string name, vector<string>& del_list) {
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

void FileManager::ui_asking() {
	while (!stop) {
		cout << "Создать файлы [1]\nПереименовать файлы [2]\nУдалить список файлов/папок [3]\nЗадать флаги [4]\nЗакрыть [5]\nВыберите команду: ";
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
		else if(com == "4") {
			string parsing_str;
			cout << "Введите флаги: ";
			getline(cin, parsing_str);
			flags_parser(parsing_str);
		}
		else if (com == "5") {
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