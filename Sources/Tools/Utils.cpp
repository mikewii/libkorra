#include "Tools/Utils.hpp"
#include <string.h>
#include <algorithm>

#ifdef __linux__
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <string.h>
#else
//
#endif


namespace Utils {

char File::CWD[NAME_MAX]{0};
bool File::isCWD = false;


    // Alignment safe copy
extern void* copybytes(void* _dest, const void* _src, size_t _size)
{
    int isSafe = reinterpret_cast<u64>(_dest) % sizeof(_dest) ;

    if ( isSafe == 0 )
        return memcpy(_dest, _src, _size);

    return memmove(_dest, _src, _size);
}

void    File::SetCWD(void)
{
#ifdef __linux__
    if (!isCWD) { //get cwd once
        isCWD = true;
        getcwd(CWD, sizeof(CWD));
    }
#endif
}

int     File::makedir(const char* dir)
{
    int res = 0;

#ifdef __linux__
#define SEPARATOR '/'

    char tmp[NAME_MAX];
    char *p = NULL;
    int perm = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s", dir);
    len = strlen(tmp);
    if(tmp[len - 1] == SEPARATOR)
            tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
            if(*p == SEPARATOR) {
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

std::string     File::makepath(const char* _path)
{
    std::string res;
    int len;

    res.append(CWD);

    len = strlen(_path);
    if (len) {
        if (_path[0] != '/') res.append("/");
        res.append(_path);
    }

    return res;
}

std::string     File::makepath(std::string& _path)
{
    std::string res;
    res.append(CWD);

    if(_path.at(0) != '/')
        res.append("/");

    res.append(_path);

    return res;
}

std::string     File::extractName(std::string _path){
    return _path.substr(_path.find_last_of('/') + 1);
}

std::string     File::extractName(const char* _path)
{
    std::string res(_path);
    return extractName(res);
}

std::string     File::cutName(std::string& _path)
{
    std::string res = _path;
    return res.substr(0, res.find_last_of("/"));
}

void         File::PairToFiles(Pair& _pair, std::string& _fname)
{
    std::string root = makepath(_fname);
    root.append("_out");

    makedir(root.c_str());



}

void File::PairsVectorToFiles(std::vector<Pair>& _list, std::string& _fname, std::string _firstPath)
{
    std::string first;
    std::string last;
    std::string full;
    std::string path;

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

    for(auto& pair : _list)
    {
        last    = pair.Filename;

        last.append(".");
        last.append(extractExt(pair.cc));

        FixPathSeparators(last);

        full    = first + last;
        path    = cutName(full);

        makedir(path.c_str());
        pair.cc.writeToFile(full.c_str());


        printf("%s\n", path.c_str());
        printf("%s\n", full.c_str());
    }
}

bool FileToCC(const char* fname, CContainer* cc)
{
    FILE* f = nullptr;
    u32 fsize = 0;
    u32 rsize = 0;

#ifdef __linux__

    if ( access(fname, F_OK | R_OK ) == 0 )
    {
        f = fopen(fname, "rb");

        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);

        cc->allocate(fsize);

        rsize = fread(cc->data(), sizeof(u8), fsize, f);
        fclose(f);
    }

#else
    //
#endif

    if (rsize > 0) return true;
    return false;
}

bool CCtoFile(const char* fname, CContainer* cc, bool makedir)
{
    FILE* f = nullptr;
    u32 wsize = 0;

    if (makedir)
    {
        std::string path = fname;
        path = Utils::File::cutName(path);
        Utils::File::makedir(path.c_str());
    }

    f = fopen(fname, "wb");

    if (f)
    {
        wsize = fwrite(cc->data(), sizeof(u8), cc->size(), f);
        fclose(f);
    }

    if (wsize > 0) return true;
    return false;
}

u32 CalculateChecksum(CContainer& data)
{
    u32 i = 0, checksum = 0;

    while (i < data.size()) {
        checksum += data.as_u8(i) & 0xFF; i++;
    };

    return checksum;
}

void File::FixPathSeparators(char* _str, bool revert)
{
    char    find;
    char    replace;
    int     len = 0;

    find = '\\';
    replace = '/';
    len = strlen(_str);

    if (len > 0)
    {
        if (revert) {
            find = '/';
            replace = '\\';
        }

        for ( int i = 0; i < len; i++ )
            if (_str[i] == find) _str[i] = replace;
    }
}

void     File::FixPathSeparators(std::string& _str, bool revert)
{
    char    find    = '\\';
    char    replace = '/';

    if (revert) {
        find    = '/';
        replace = '\\';
    }

    std::replace( _str.begin(), _str.end(), find, replace);
}

std::string File::extractExt(CContainer& _cc, bool BE)
{
    std::string out;
    char ext[5]{0};

    std::copy(_cc.data(), _cc.data() + 4, ext);


    for(u32 i = 0; i < 4; i++)
    {
        if(isalpha(ext[i]))
            out.push_back(tolower( ext[i] ));
        else
        {
            if(ext[i] > 0)
                out.append(std::to_string(ext[i]));
        }

    }

    if(BE) std::reverse(out.begin(), out.end());
    return out;
}

} // Utils
