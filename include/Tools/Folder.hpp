#pragma once
#include "types.h"
#include "Global.hpp"


class Folder {
public:
    Folder();
    Folder(const char* path);
    Folder(const std::string& path);
    ~Folder(){};

    void Set_Path(const char* path);
    void Set_Path(const std::string& path);

    const std::vector<std::string> Get_ListOfFiles(void) const;
    const std::string Get_FullPath(const std::string& filename) const;

private:
    std::string __path;

    const std::string process_path(const std::string& path) const;

#define SEP_WIN '\\'
#define SEP_UNIX '/'

    inline char SEP(void) const
    {
    #ifdef _WIN32
        return SEP_WIN;
    #else
        return SEP_UNIX;
    #endif
    }
};
