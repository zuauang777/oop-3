#include <iostream>
#include <cstdio>
#include <filesystem>
#include "filecopier.hpp"
using namespace std;

bool MakeTestFile(const char* Name, const char* Text)
{
    FILE* F = fopen(Name, "wb");
    if (!F) return false;
    fputs(Text, F);
    fclose(F);
    return true;
}

void Report(const char* Title, const FileCopier& C)
{
    cout << Title << "\n  оригінал: " << C.GetSourceName()
         << "\n  копія   : " << C.GetCopyName()
         << "\n  відкрито: " << (C.IsOpen() ? "так" : "ні")
         << "\n  розмір  : " << C.GetSize() << " байт\n\n";
}

void CheckCopy(const FileCopier& C)
{
    error_code Ec;
    auto Size = filesystem::file_size(C.GetCopyName(), Ec);
    if (Ec) cout << "  файл копії не знайдено\n\n";
    else cout << "  розмір копії на диску: " << Size << " байт\n\n";
}

int main()
{
    if (!MakeTestFile("myfile.txt", "Hello, OOP! This is a test file for lab 3.\n"))
    {
        cout << "Не вдалося створити тестовий файл\n";
        return 1;
    }
    filesystem::create_directory("copies");

    FileCopier A;
    Report("A: за замовчуванням", A);
    cout << "  A.Copy() -> " << (A.Copy() ? "ok" : "відмова (файл не відкрито)") << "\n\n";

    FileCopier B("myfile.txt");
    Report("B: один аргумент", B);
    cout << "  B.Copy() -> " << (B.Copy() ? "ok" : "помилка") << "\n";
    CheckCopy(B);

    FileCopier C("myfile.txt", "backup.txt");
    Report("C: два аргументи", C);
    cout << "  C.Copy() -> " << (C.Copy() ? "ok" : "помилка") << "\n";
    CheckCopy(C);

    FileCopier D("myfile.txt", "backup.txt", "copies");
    Report("D: три аргументи", D);
    cout << "  D.Copy() -> " << (D.Copy() ? "ok" : "помилка") << "\n";
    CheckCopy(D);

    FileCopier E(B);
    Report("E: копія об'єкта B", E);
    cout << "  E.Copy() -> " << (E.Copy() ? "ok" : "помилка") << "\n";
    CheckCopy(E);

    FileCopier F("no_such_file.txt");
    Report("F: неіснуючий файл", F);

    FileCopier G("myfile.txt", "myfile.txt");
    cout << "G: копія з тією ж назвою, G.Copy() -> "
         << (G.Copy() ? "ok" : "відмова (захист оригіналу)") << "\n\n";

    cout << "Вхід у вкладений блок\n";
    {
        FileCopier T("myfile.txt", "temp_copy.txt");
        cout << "  T створено, відкрито: " << (T.IsOpen() ? "так" : "ні") << "\n";
        cout << "Вихід з блоку (деструктор T):\n";
    }
    cout << "Блок завершено\n\n";

    cout << "Перевірте на диску: myfilecopy.txt, backup.txt, copies/backup.txt\n";
    return 0;
}
