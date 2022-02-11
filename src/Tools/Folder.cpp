#include "Tools/Folder.hpp"
#include <filesystem>

#ifdef __linux__
    #include <dirent.h>
#endif

Folder::Folder()
{
}
Folder::Folder(const char *path)
{
    Folder::Set_Path(path);
}
Folder::Folder(const std::string &path)
{
    Folder::Set_Path(path);
}

void Folder::Set_Path(const char *path) { Folder::__path = process_path(path); }
void Folder::Set_Path(const std::string &path) { Folder::__path = process_path(path); }

const std::vector<std::string> Folder::Get_ListOfFiles(void) const // ftw maybe?
{


    DIR*                        d = nullptr;
    struct dirent*              dir = nullptr;
    std::vector<std::string>    out;



    d = opendir(Folder::__path.c_str());

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG) // DT_UNKNOWN ?
                out.push_back(dir->d_name);
        }
    }

    return out;
}

const std::string Folder::Get_FullPath(const std::string &filename) const { return Folder::__path + filename; }

const std::string Folder::process_path(const std::string &path) const
{
    std::string     temp = path;
    auto            wrong_sep = Folder::SEP() == SEP_UNIX ? SEP_WIN : SEP_UNIX;

    for (auto& ch : temp)
    {
        if (ch == wrong_sep)
            ch = Folder::SEP();
    }

    if (temp.back() != Folder::SEP())
        temp.push_back(Folder::SEP());

    return temp;
}
