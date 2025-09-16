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
						ren(it.path(), ext, exeptions, name);
					}
					else {
						if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
							rename(it.path(), it.path().parent_path().string() + '\\' + name);
						}
					}
				}
				//�� ������ ��� � ����� ��� ����� ���� ����� ����� name11.txt ��������
				else {
					if (!checker(it.path().filename().string(), exeptions) && checker(it.path().filename().string(), ext)) {
						string new_name = it.path().parent_path().string() + "\\";
						bool first_dot = true;
						for (int i = 0; i < name.length(); i++) {
							if (name[i] != '.') {
								new_name += name[i];
							}
							else if (first_dot){
								new_name += to_string(num++) + name[i];
								first_dot = false;
							}
						}
						if (first_dot) new_name += to_string(num++);
						rename(it.path(), new_name);
						if (is_directory(it.path())) {
							ren(it.path(), ext, exeptions, name);
						}
					}
				}
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
		cout << "������� ����� [1]\n������������� ����� [2]\n������� ������ ������/����� [3]\n������� [4]\n�������� �������: ";
		string com;
		getline(cin, com);
		if (com == "1") {
			//
			stop = false;
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
		else if (com == "4") {
			stop = true;
		}
		else {
			cout << "����� ������� �� ����������!" << endl;
		}
	}
	//cout << "������ ���������." << endl;
	cin.ignore(); //������� ������
	cin.get(); //��� ������� Enter
}