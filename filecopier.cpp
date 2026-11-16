#include <iostream>
#include "filecopier.hpp"

using std::string;

void FileCopier::OpenFiles()
{
    Src = fopen(SrcName.c_str(), "rb");
    Dst = NULL;
    if (!Src) return;
    Dst = fopen(CopyName.c_str(), "ab");
}

void FileCopier::CloseFiles()
{
    if (Src) { fclose(Src); Src = NULL; }
    if (Dst) { fclose(Dst); Dst = NULL; }
}

string FileCopier::MakeCopyName(const string& Name)
{
    size_t Dot = Name.rfind('.');
    size_t Slash = Name.find_last_of("/\\");
    size_t Start = (Slash == string::npos) ? 0 : Slash + 1;

    if (Dot == string::npos || Dot <= Start)
        return Name + "copy";
    return Name.substr(0, Dot) + "copy" + Name.substr(Dot);
}

FileCopier::FileCopier()
{
    Src = NULL;
    Dst = NULL;
}

FileCopier::FileCopier(const char* SourceName)
{
    SrcName = SourceName;
    CopyName = MakeCopyName(SrcName);
    OpenFiles();
}

FileCopier::FileCopier(const char* SourceName, const char* CopyFileName)
{
    SrcName = SourceName;
    CopyName = CopyFileName;
    OpenFiles();
}

FileCopier::FileCopier(const char* SourceName, const char* CopyFileName, const char* Dir)
{
    SrcName = SourceName;
    CopyName = Dir;
    if (!CopyName.empty() && CopyName.back() != '/' && CopyName.back() != '\\')
        CopyName += '/';
    CopyName += CopyFileName;
    OpenFiles();
}

FileCopier::FileCopier(const FileCopier& Other)
{
    SrcName = Other.SrcName;
    CopyName = Other.CopyName;
    Src = NULL;
    Dst = NULL;
    if (!SrcName.empty()) OpenFiles();
}

FileCopier::~FileCopier()
{
    std::cout << "[~FileCopier] закриваю файли: '" << SrcName << "'\n";
    CloseFiles();
}

bool FileCopier::Copy()
{
    if (!Src || CopyName.empty() || SrcName == CopyName) return false;
    if (Dst) fclose(Dst);
    Dst = fopen(CopyName.c_str(), "wb");
    if (!Dst) return false;

    rewind(Src);
    char Buffer[4096];
    size_t Count;
    while ((Count = fread(Buffer, 1, sizeof(Buffer), Src)) > 0)
    {
        if (fwrite(Buffer, 1, Count, Dst) != Count) return false;
    }
    fflush(Dst);
    return true;
}

long FileCopier::GetSize() const
{
    if (!Src) return -1;
    long Current = ftell(Src);
    fseek(Src, 0, SEEK_END);
    long Size = ftell(Src);
    fseek(Src, Current, SEEK_SET);
    return Size;
}
