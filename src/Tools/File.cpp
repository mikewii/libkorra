#include "Tools/File.hpp"
//#include <cstdlib>
#include <string.h>
#include <algorithm>


#ifdef __linux__
    //#include <stdio.h>
    #include <unistd.h>
    #include <sys/stat.h>
#else
// TODO
#endif

// error prints
#ifdef WIN
void PrintLastError(void)
{
    //Get the error message ID, if any.
    auto errorMessageID = ::GetLastError();

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    printf("Error:: %s\n", messageBuffer);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);
}
#endif

char File::CWD[PATH_MAX]{0};


static bool isCWD = false;
void File::SetCWD(void)
{
    if (!isCWD) //get cwd once
    {
        isCWD = true;
#ifdef __linux__
        getcwd(CWD, sizeof(CWD));
#elif WIN
        auto res = GetCurrentDirectory(sizeof(CWD), CWD);
        if (!res) PrintLastError();
#endif
    }
}

int File::makedir(const char* dir)
{
    int res = 0;

#ifdef __linux__

    char tmp[NAME_MAX];
    char *p = NULL;
    int perm = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s", dir);
    len = strlen(tmp);
    if(tmp[len - 1] == SEPARATOR)
            tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
            if(*p == SEPARATOR)
            {
                *p = 0;
                res = mkdir(tmp, perm);
                *p = SEPARATOR;
            }
    res = mkdir(tmp, perm);
   #else
       res = _mkdir(dir);
   #endif

    return res;
}

std::string File::makepath(const char* _path)
{
    std::string     out;
    int             len;


    out.append(CWD);

    len = strlen(_path);
    if (len) {
        if (_path[0] != SEPARATOR) out += SEPARATOR;
        out.append(_path);
    }

    return out;
}

std::string File::makepath(std::string& _path)
{
    std::string     out;

    out.append(CWD);

    if(_path.at(0) != SEPARATOR)
        out += SEPARATOR;

    return out += _path;
}

std::string File::extractName(std::string& _path)
{
    int pos = _path.find_last_of(SEPARATOR);
    if (pos == -1) pos = _path.find_last_of(SEPARATOR_WIN);

    return std::string( _path.substr(pos + 1) );
}

std::string File::extractName(const char* _path) {
    std::string res(_path);
    return extractName(res);
}

std::string File::popName(std::string& _path) {
    return std::string(_path.substr(0, _path.find_last_of(SEPARATOR)));
}

void File::PairToFile(Pair& _pair, std::string& _fname, std::string _firstPath)
{
    std::string     first;
    std::string     last;
    std::string     full;
    std::string     path;


}

void File::PairVectorToFiles(std::vector<Pair>& _vec, std::string& _fname, std::string _firstPath)
{
    std::string     first;
    std::string     last;
    std::string     full;
    std::string     path;

    if (!_firstPath.empty())
    {
        first = _firstPath;
        first.append(_fname);
    }
    else
    {
        first = makepath(_fname);
    }

    first.append("_out/");
    makedir(first.c_str());

    for(auto& pair : _vec)
    {
        last    = pair.info.Filename;

        last.append(".");
        last.append(extractExt(pair.cc));

        FixPathSeparators(last);

        full    = first + last;
        path    = popName(full);

        makedir(path.c_str());
        pair.cc.writeToFile(full.c_str());
    }
}

bool File::FileToCC(std::string& _fpath, CContainer* _cc) {
    return File::FileToCC(_fpath.c_str(), _cc);
}

bool File::FileToCC(const char* _fpath, CContainer* _cc)
{
    FILE*   f = nullptr;
    u32     fsize = 0;
    u32     rsize = 0;

#ifdef __linux__

    if (access(_fpath, F_OK | R_OK) == 0)
    {
        f = fopen(_fpath, "rb");

        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);

        _cc->resize(fsize);

        rsize = fread(_cc->data(), sizeof(u8), fsize, f);
        fclose(f);
    }
    else NotifyError("FileToCC: File doenst exist or cant read!");

#else
    //
#endif

    if (rsize > 0) return true;
    return false;
}

bool File::CCtoFile(std::string& _fpath, CContainer* _cc, bool _makedir) {
    return File::CCtoFile(_fpath.c_str(), _cc, _makedir);
}

bool File::CCtoFile(const char* _fpath, CContainer* _cc, bool _makedir)
{
    FILE*   f = nullptr;
    u32     wsize = 0;

    if (_cc->size() == 0) {
        NotifyError("CCtoFile: CContainer is empty!");
        return false;
    }


    if (_makedir)
    {
        // TODO
    }

    f = fopen(_fpath, "wb");

    if (f)
    {
        wsize = fwrite(_cc->data(), sizeof(u8), _cc->size(), f);
        fclose(f);
    }

    if (wsize > 0) return true;
    return false;
}

void File::FixPathSeparators(char* _str, bool revert)
{
    char    find;
    char    replace;
    int     len = 0;

    find = SEPARATOR_WIN;
    replace = SEPARATOR;
    len = strlen(_str);

    if (len > 0)
    {
        if (revert) {
            find = SEPARATOR;
            replace = SEPARATOR_WIN;
        }

        for (int i = 0; i < len; i++)
            if (_str[i] == find) _str[i] = replace;
    }
}

void File::FixPathSeparators(std::string& _str, bool revert)
{
    char    find    = SEPARATOR_WIN;
    char    replace = SEPARATOR;

    if (revert) {
        find    = SEPARATOR;
        replace = SEPARATOR_WIN;
    }

    std::replace(_str.begin(), _str.end(), find, replace);
}

std::string File::extractExt(CContainer& _cc, bool BE)
{
    std::string out;
    char ext[5]{0};

    std::copy(_cc.data(), _cc.data() + 4, ext);


    for (u32 i = 0; i < 4; i++)
    {
        if (isalpha(ext[i]))
            out.push_back(tolower( ext[i] ));
        else
        {
            if (ext[i] > 0)
                out.append(std::to_string(ext[i]));
        }

    }

    if (BE) std::reverse(out.begin(), out.end());
    return out;
}
