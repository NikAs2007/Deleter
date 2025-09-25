#include "FileManager.h"


bool FileManagerUI::flags_parser(string all_flags) {
	if (!is_correct_flags_string(all_flags)) {
		cout << "Ошибка чтения флагов.\n" << endl;
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
	cout << "Флаги успешно установлены.\n" << endl;
	return true;
}

void FileManagerUI::ui_asking() {
	while (!stop) {
		cout << "Создать файлы [1]\nПереименовать файлы [2]\nУдалить список файлов/папок [3]\nЗадать флаги [4]\nПосмотреть информацию о флагах [5]\nЗакрыть [6]\nВыберите команду: ";
		string com;
		getline(cin, com);
		if (com == "1") {
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
				if (!have_danger_characters(name)) {
					cout << "Введите количество создаваемых файлов: ";
					getline(cin, d);
					bool right_count = true;
					for (char x : d) {
						if (!isdigit(x)) right_count = false;
					}
					if (right_count) {
						cre(path, name, stoi(d));
						cout << "Создано.\n" << endl;
					}
					else {
						cout << "Принимаются только числа!" << endl;
					}
				}
				else {
					cout << "Имя имеет недопустимые символы: '\\', '/', ':', '*', '?', '\"', '<', '>', '|'.\n" << endl;
				}
			}
			else {
				cout << "Такого пути не существует\n" << endl;
			}
		}
		else if (com == "2") {
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
				if (!have_danger_characters(new_name)) {
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
					cout << "Переименовано.\n" << endl;
				}
				else {
					cout << "Имя имеет недопустимые символы: '\\', '/', ':', '*', '?', '\"', '<', '>', '|'.\n" << endl;
				}
			}
			else {
				cout << "Такого пути не существует\n" << endl;
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
				cout << "Удалено.\n" << endl;
			}
			else {
				cout << "Такого пути не существует\n" << endl;
			}
		}
		else if (com == "4") {
			string parsing_str;
			cout << "Введите флаги: ";
			getline(cin, parsing_str);
			flags_parser(parsing_str);
		}
		else if (com == "5") {
			string d;
			cout << "Выбор:\nИнфо [1]\nФлаги [2]\nВвод: ";
			getline(cin, d);
			if (d == "1") {
				cout << "Информация о флагах: \n" << endl;

				cout << "-rec -> включает рекусривный обход всех вложенных папок для каждой функции" << endl;
				cout << "-nrec -> выключает" << endl;
				cout << endl;

				cout << "-deld -> теперь удаляются только directories" << endl;
				cout << "-delf -> удаляются только files" << endl;
				cout << "-delfd -> удаляются directories и files" << endl;
				cout << endl;

				cout << "-cref -> создаются files" << endl;
				cout << "-cref -> создаются directories" << endl;
				cout << endl;

				cout << "-rend -> переименовывает только directories" << endl;
				cout << "-renf -> переименовывает только files" << endl;
				cout << "-renfd -> переименовывает files и directories" << endl;
				cout << endl;

				cout << "-reg -> учитывается регистр при проверках" << endl;
				cout << "-nreg -> не учитывается" << endl;
				cout << endl;

				cout << "-root -> функции переименования, создания и удаления работают с корневой папкой" << endl;
				cout << "-nroot -> функции переименования и удаления не затрагивают корневую папку, даже если она попала в список выбранных шаблонов, а функция создания не создает ничего в корневой папке" << endl;
				cout << endl;
			}
			else if (d == "2") {
				cout << "Флаги: \n" << endl;

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
			else cout << "Такой команды нет.\n" << endl;
		}
		else if (com == "6") {
			stop = true;
		}
		else {
			cout << "Такой команды не существует!\n" << endl;
		}
	}
	cout << "Работа завершена.\n" << endl;
	cin.ignore(); //очистка буфера
	cin.get(); //ждёт нажатия Enter
}