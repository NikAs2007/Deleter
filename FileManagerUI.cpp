#include "FileManager.h"


bool FileManagerUI::flags_parser(string all_flags) {
	if (!is_correct_flags_string(all_flags)) {
		cout << "������ ������ ������.\n" << endl;
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
	cout << "����� ������� �����������.\n" << endl;
	return true;
}

void FileManagerUI::ui_asking() {
	while (!stop) {
		cout << "������� ����� [1]\n������������� ����� [2]\n������� ������ ������/����� [3]\n������ ����� [4]\n���������� ���������� � ������ [5]\n������� [6]\n�������� �������: ";
		string com;
		getline(cin, com);
		if (com == "1") {
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
				if (!have_danger_characters(name)) {
					cout << "������� ���������� ����������� ������: ";
					getline(cin, d);
					bool right_count = true;
					for (char x : d) {
						if (!isdigit(x)) right_count = false;
					}
					if (right_count) {
						cre(path, name, stoi(d));
						cout << "�������.\n" << endl;
					}
					else {
						cout << "����������� ������ �����!" << endl;
					}
				}
				else {
					cout << "��� ����� ������������ �������: '\\', '/', ':', '*', '?', '\"', '<', '>', '|'.\n" << endl;
				}
			}
			else {
				cout << "������ ���� �� ����������\n" << endl;
			}
		}
		else if (com == "2") {
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
				if (!have_danger_characters(new_name)) {
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
					cout << "�������������.\n" << endl;
				}
				else {
					cout << "��� ����� ������������ �������: '\\', '/', ':', '*', '?', '\"', '<', '>', '|'.\n" << endl;
				}
			}
			else {
				cout << "������ ���� �� ����������\n" << endl;
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
				cout << "�������.\n" << endl;
			}
			else {
				cout << "������ ���� �� ����������\n" << endl;
			}
		}
		else if (com == "4") {
			string parsing_str;
			cout << "������� �����: ";
			getline(cin, parsing_str);
			flags_parser(parsing_str);
		}
		else if (com == "5") {
			string d;
			cout << "�����:\n���� [1]\n����� [2]\n����: ";
			getline(cin, d);
			if (d == "1") {
				cout << "���������� � ������: \n" << endl;

				cout << "-rec -> �������� ����������� ����� ���� ��������� ����� ��� ������ �������" << endl;
				cout << "-nrec -> ���������" << endl;
				cout << endl;

				cout << "-deld -> ������ ��������� ������ directories" << endl;
				cout << "-delf -> ��������� ������ files" << endl;
				cout << "-delfd -> ��������� directories � files" << endl;
				cout << endl;

				cout << "-cref -> ��������� files" << endl;
				cout << "-cref -> ��������� directories" << endl;
				cout << endl;

				cout << "-rend -> ��������������� ������ directories" << endl;
				cout << "-renf -> ��������������� ������ files" << endl;
				cout << "-renfd -> ��������������� files � directories" << endl;
				cout << endl;

				cout << "-reg -> ����������� ������� ��� ���������" << endl;
				cout << "-nreg -> �� �����������" << endl;
				cout << endl;

				cout << "-root -> ������� ��������������, �������� � �������� �������� � �������� ������" << endl;
				cout << "-nroot -> ������� �������������� � �������� �� ����������� �������� �����, ���� ���� ��� ������ � ������ ��������� ��������, � ������� �������� �� ������� ������ � �������� �����" << endl;
				cout << endl;
			}
			else if (d == "2") {
				cout << "�����: \n" << endl;

				if (recf == recursion_on) cout << "-rec: on" << endl;
				else cout << "-rec: off" << endl;
				if (recf == recursion_off) cout << "-nrec: on" << endl;
				else cout << "-nrec: off\n";
				cout << endl;

				if (delf == del_dir) cout << "-deld: on" << endl;
				else cout << "-deld: off" << endl;
				if (delf == del_files) cout << "-delf: on" << endl;
				else cout << "-delf: off" << endl;
				if (delf == del_dir_files) cout << "-delfd: on" << endl;
				else cout << "-delfd: off\n";
				cout << endl;

				if (cref == cre_files) cout << "-cref: on" << endl;
				else cout << "-cref: off" << endl;
				if (cref == cre_dir) cout << "-cref: on" << endl;
				else cout << "-cred: off\n";
				cout << endl;

				if (renf == ren_dir) cout << "-rend: on" << endl;
				else cout << "-rend: off" << endl;
				if (renf == ren_files) cout << "-renf: on" << endl;
				else cout << "-renf: off" << endl;
				if (renf == ren_dir_files) cout << "-renfd: on" << endl;
				else cout << "-renfd: off\n";
				cout << endl;

				if (regf == reg_on) cout << "-reg: on" << endl;
				else cout << "-reg: off" << endl;
				if (regf == reg_off) cout << "-nreg: on" << endl;
				else cout << "-nreg: off\n";
				cout << endl;

				if (regf == root_on) cout << "-root: on" << endl;
				else cout << "-root: off" << endl;
				if (regf == root_off) cout << "-nroot: on" << endl;
				else cout << "-nroot: off\n";
				cout << endl;
			}
			else cout << "����� ������� ���.\n" << endl;
		}
		else if (com == "6") {
			stop = true;
		}
		else {
			cout << "����� ������� �� ����������!\n" << endl;
		}
	}
	cout << "������ ���������.\n" << endl;
	cin.ignore(); //������� ������
	cin.get(); //��� ������� Enter
}