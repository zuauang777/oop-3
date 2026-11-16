#ifndef FILECOPIER_HPP
#define FILECOPIER_HPP

#include <cstdio>
#include <string>

class FileCopier {
    std::string SrcName;
    std::string CopyName;
    FILE* Src;
    FILE* Dst;

    void OpenFiles();
    void CloseFiles();
    static std::string MakeCopyName(const std::string& Name);

public:
    FileCopier();
    explicit FileCopier(const char* SourceName);
    FileCopier(const char* SourceName, const char* CopyFileName);
    FileCopier(const char* SourceName, const char* CopyFileName, const char* Dir);
    FileCopier(const FileCopier& Other);
    ~FileCopier();

    bool Copy();
    long GetSize() const;

    bool IsOpen() const { return Src != NULL; }
    const char* GetSourceName() const { return SrcName.c_str(); }
    const char* GetCopyName() const { return CopyName.c_str(); }
};

#endif
