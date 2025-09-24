#include "FileManager.h"

int main()
{
    //SetConsoleOutputCP(65001);
    //SetConsoleCP(65001);
    setlocale(LC_ALL, "Ru");

    FileManager filemanager;
    filemanager.ui_asking();
    return 0;
}