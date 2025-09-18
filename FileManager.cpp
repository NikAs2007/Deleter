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
		cout << "������ ������ ������." << endl;
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

}

void FileManager::del(path path, vector<string>& ext, vector<string>& exeptions) {
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

void FileManager::ren(path path, vector<string>& ext, vector<string>& exeptions, string name = "File") {
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

void FileManager::cre(path path, string name, int count_f) {
	if (exists(path)) {
		if (count_f < 1) return;
		if (count_f == 1) {
			ofstream{ path.string() + '\\' + name };
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
				ofstream{ new_name };
			}
		}
	}
}

//���� ������ � ������: ���������� Hah1 ��� ��� � Hah10 � ��.
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
		cout << "������� ����� [1]\n������������� ����� [2]\n������� ������ ������/����� [3]\n������ ����� [4]\n������� [5]\n�������� �������: ";
		string com;
		getline(cin, com);
		if (com == "1") {
			//
			string path, d = "", name, repath = "";
			cout << "������� ����: ";
			getline(cin, path);
			for (int c = 0; c < path.length(); ++c) {
				if (path[c] != '"') repath += path[c];
			}
			path = repath;
			if (exists(path)) {
				cout << "������� ��� ������������ �������: ";
				getline(cin, name);
				cout << "������� ���������� ����������� ������: ";
				getline(cin, d);
				bool right_count = true;
				for (char x : d) {
					if (!isdigit(x)) right_count = false;
				}
				if (right_count) {
					cre(path, name, stoi(d));
				}
				else {
					cout << "����������� ������ �����!" << endl;
				}
			}
			else {
				cout << "������ ���� �� ����������" << endl;
			}
		}
		else if (com == "2") {
			//
			string path, d = "", repath = "", new_name = "";
			vector<string> ren_vec;
			vector<string> exeptions;
			cout << "������� ����: ";
			getline(cin, path);
			for (int c = 0; c < path.length(); ++c) {
				if (path[c] != '"') repath += path[c];
			}
			path = repath;
			if (exists(path)) {
				//��� ������
				cout << "������� ����� ��� �������/��������: ";
				getline(cin, new_name);
				cout << "������� ������ ������ ��� �������������� (���� ������ ��������, �� ������� '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						ren_vec.push_back(d);
					}
				} while (d != ".");
				d = "";
				cout << "������� ������ �������� ���� ��� ����������, ��� �����, ������� �� ����� ������� (���� ������ ��������, �� ������� '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						exeptions.push_back(d);
					}
				} while (d != ".");
				ren(path, ren_vec, exeptions, new_name);
			}
			else {
				cout << "������ ���� �� ����������" << endl;
			}
		}
		else if (com == "3") {
			string path, d = "", repath = "";
			vector<string> del_vec;
			vector<string> exeptions;
			cout << "������� ����: ";
			getline(cin, path);
			for (int c = 0; c < path.length(); ++c) {
				if (path[c] != '"') repath += path[c];
			}
			path = repath;
			if (exists(path)) {
				//��� ������
				cout << "������� ������ �������� ���� ��� �������� (���� ������ ��������, �� ������� '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						del_vec.push_back(d);
					}
				} while (d != ".");
				d = "";
				cout << "������� ������ �������� ���� ��� ����������, ��� �����, ������� �� ����� ������� (���� ������ ��������, �� ������� '.'): " << endl;
				do {
					if (d != ".") {
						getline(cin, d);
						exeptions.push_back(d);
					}
				} while (d != ".");
				del(path, del_vec, exeptions);
			}
			else {
				cout << "������ ���� �� ����������" << endl;
			}
		}
		else if(com == "4") {
			string parsing_str;
			cout << "������� �����: ";
			getline(cin, parsing_str);
			flags_parser(parsing_str);
		}
		else if (com == "5") {
			stop = true;
		}
		else {
			cout << "����� ������� �� ����������!" << endl;
		}
	}
	cout << "������ ���������." << endl;
	cin.ignore(); //������� ������
	cin.get(); //��� ������� Enter
}