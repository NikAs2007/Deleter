#include "FileManager.h"

FileManager::FileManager() {
	stop = false;
	recf = recursion_on;
	delf = del_dir_files;
	cref = cre_files;
	renf = ren_dir_files;
	regf = reg_off;
	rootf = root_on;
	danger_chars = { '\\','/',':','*','?','"','<','>','|' };
	avaliable_flags = { "-rec", "-nrec", "-deld", "-delf", "-delfd", "-cref", "-cred", "-renf", "-rend", "-renfd", "-reg", "-nreg", "-root", "-nroot" };
}

bool FileManager::is_correct_flags_string(string flags_string) {
	string flag;
	int i = 0;
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

bool FileManager::flags_parser(string all_flags) {
	if (!is_correct_flags_string(all_flags)) {
		//cout << "Ошибка чтения флагов.\n" << endl;
		return false;
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
		else if (flag == "-nrec") recf = recursion_off;
		else if (flag == "-deld") delf = del_dir;
		else if (flag == "-delf") delf = del_files;
		else if (flag == "-delfd") delf = del_dir_files;
		else if (flag == "-cref") cref = cre_files;
		else if (flag == "-cred") cref = cre_dir;
		else if (flag == "-renf") renf = ren_files;
		else if (flag == "-rend") renf = ren_dir;
		else if (flag == "-renfd") renf = ren_dir_files;
		else if (flag == "-reg") regf = reg_on;
		else if (flag == "-nreg") regf = reg_off;
		else if (flag == "-root") rootf = root_on;
		else if (flag == "-nroot") rootf = root_off;
	}
	return true;
	//cout << "Флаги успешно установлены.\n" << endl;
}

bool FileManager::del(path path, vector<string>& ext, vector<string>& exeptions, bool first_call) {
	if (exists(path)) {
		if (!checker(path.filename().string(), exeptions) && checker(path.filename().string(), ext)) {
			if (rootf == root_on && first_call && (delf == del_dir || delf == del_dir_files) && is_directory(path)) {
				remove_all(path);
				return true;
			}
			else if ((delf == del_files || delf == del_dir_files) && !is_directory(path)) {
				remove(path);
				return true;
			}
		}
		first_call = false;
		if (!checker(path.filename().string(), exeptions)) {
			for (auto& it : directory_iterator(path)) {
				if (is_directory(it.path())) {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						if (delf == del_dir || delf == del_dir_files) remove_all(it.path());
					}
					else if (recf == recursion_on) {
						del(it.path(), ext, exeptions, first_call);
						//thread th([&](){ del(it.path(), ext, exeptions); });
						//th.detach();
					}
				}
				else {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						if (delf == del_dir_files || delf == del_files) remove(it.path());
					}
				}
			}
		}
		return true;
	}
	return false;
}


//тут баг - при переименовании файлов в тоже имя происходит конфликт между ними и num получает лишнии номера
//еще баг - нет рекурсивного обхода
bool FileManager::ren(path path, vector<string>& ext, vector<string>& exeptions, string name, bool first_call) {
	if (exists(path)) {
		if (!checker(path.filename().string(), exeptions) && checker(path.filename().string(), ext)) {
			if ((rootf == root_on || (rootf == root_off && !first_call)) && (is_directory(path) && (renf == ren_dir || renf == ren_dir_files)) || (!is_directory(path) && (renf == ren_files || renf == ren_dir_files))) {
				//тут
				//if (rootf == root_on || (rootf == root_off && !first_call)) rename(path, path.parent_path().string() + '\\' + name);
				rename(path, path.parent_path().string() + '\\' + name);
				path = path.parent_path().string() + '\\' + name;
			}
		}
		first_call = false;
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
						string new_name = it.path().parent_path().string();
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							new_name += '\\' + name;
						}
						else {
							new_name = it.path().string();
						}
						rename(it.path(), new_name);
						if (recf == recursion_on && (rootf == root_on || (rootf == root_off && !first_call))) {
							ren(new_name, ext, exeptions, name, first_call);
							//thread th([&]() { ren(new_name, ext, exeptions, name); });
							//th.detach();
						}
					}
					else {
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							if (renf == ren_dir_files || renf == ren_files) rename(it.path(), it.path().parent_path().string() + '\\' + name);
						}
					}
				}
				else {
					string new_name = it.path().string();
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
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
					}
					if (is_directory(new_name) && !checker(it.path().filename().string(), exeptions)) {
						if (recf == recursion_on && (rootf == root_on || (rootf == root_off && !first_call))) {
							ren(new_name, ext, exeptions, name, first_call);
							//thread th([&]() { ren(new_name, ext, exeptions, name); });
							//th.detach();
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool FileManager::cre(path path, string name, int count_f, bool first_call) {
	if (exists(path)) {
		if (count_f < 1) return false;
		bool this_first_call = first_call;
		first_call = false;
		if (recf == recursion_on) {
			for (auto& it : directory_iterator(path)) {
				if (is_directory(it.path())) {
					cre(it.path(), name, count_f, first_call);
					//thread th([&]() { cre(it.path(), name, count_f); });
					//th.detach();
				}
			}
		}
		if (rootf == root_on || (rootf == root_off && !this_first_call)) {
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
		return true;
	}
	return false;
}

bool FileManager::checker(string name, vector<string>& str_list) {
	if (regf == reg_off) {
		transform(name.begin(), name.end(), name.begin(), [](char c) { return tolower((int)c); });
		for (int i = 0; i < str_list.size(); i++) {
			transform(str_list[i].begin(), str_list[i].end(), str_list[i].begin(), [](char c) { return tolower((int)c); });
		}
	}
	for (int i = 0; i < str_list.size(); i++) {
		int left = 0, right = 0;
		int l = 0, rs = 0;
		bool flag = true;
		bool ending = false;
		int last_rs = 0;
		bool starting = false;
		bool flag_for_starting = false;
		bool flag_for_ending = true;
		while (right < str_list[i].length()) {
			while (right < str_list[i].length() && str_list[i][right] != '*') {
				right++;
			}
			if (right == 0) starting = true;
			if (right == 0) l++;
			if (right == str_list[i].length() - 1) ending = true;
			string sub = str_list[i].substr(left, right - left);
			rs = sub.size();
			while (rs + l < name.length() && name.substr(l, rs) != sub) {
				l++;
			}
			last_rs = rs;
			if (name.substr(l, rs) != sub) {
				right = str_list[i].length();
				flag = false;
			}
			else {
				if (starting && (l != 0) || !starting && (l == 0)) flag_for_starting = true;
				//тут добавили флаг
				if (right >= str_list[i].length() - 1 && !ending && ((l + rs - 1) < name.length() - 1)) flag_for_ending = false;
				left = right + 1;
				right = left;
				rs = 0;
			}
		}
		if (flag_for_ending && flag_for_starting && flag && ((ending && ((l + last_rs) < (name.length()))) || (!ending && ((l + last_rs) >= (name.length() - 1))))) return true;
	}
	return false;
}

bool FileManager::have_danger_characters(string name) {
	for (char ch : danger_chars) {
		if (find(name.begin(), name.end(), ch) != name.end()) return true;
	}
	return false;
}