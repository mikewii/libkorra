#include "Tools/Utils.hpp"
#include <string.h>
#include <algorithm>
#include <cstdlib>

#ifdef __linux__
    //#include <stdio.h>
    #include <unistd.h>
    #include <sys/stat.h>
#else
// TODO
#endif


void NotifyError(std::string _str){
    fprintf(stderr, "\n%s\n", _str.c_str());
}


namespace Utils {

char File::CWD[NAME_MAX]{0};


    // Alignment safe copy
extern void*    copybytes( void* _dest, const void* _src, size_t _size )
{
    int isSafe = reinterpret_cast<u64>(_dest) % sizeof(_dest) ;

    if ( isSafe == 0 )
        return memcpy(_dest, _src, _size);

    return memmove(_dest, _src, _size);
}

static bool isCWD = false;
void            File::SetCWD( void )
{
#ifdef __linux__
    if (!isCWD) { //get cwd once
        isCWD = true;
        getcwd(CWD, sizeof(CWD));
    }
#endif
}

int             File::makedir( const char* dir )
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

    if ( access(_fpath, F_OK | R_OK ) == 0 )
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

u32 CalculateChecksum(CContainer& _data)
{
    u32 i = 0, checksum = 0;

    while (i < _data.size()) {
        checksum += _data.as<u8>(i) & 0xFF; i++;
    };

    return checksum;
}

u32 CalculateChecksum(u8* _data, u32 _size)
{
    u32 i = 0, checksum = 0;

    while (i < _size) {
        checksum += _data[i] & 0xFF; i++;
    }

    return checksum;
}


std::pair<u8*, u8*> FindDiff(u8* _data0, u8* _data1, u32 _size)
{
    u32 i = 0, sum0 = 0, sum1 = 0;
    u8* p0 = nullptr;
    u8* p1 = nullptr;

    while (i < _size)
    {
        sum0 += _data0[i] & 0xFF;
        sum1 += _data1[i] & 0xFF;

        if (sum0 != sum1)
        {
            p0 = &_data0[i];
            p1 = &_data1[i];

            break;
        }

        i++;
    }

    return {p0, p1};
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

std::string GetUserHome(void)
{
#ifdef __linux__
    return std::getenv("HOME");
#else
    // TODO
    return "";
#endif
}

u32 GetHeaderRelativePos(const void* const header, const void* const member)
{
    return reinterpret_cast<u64>(member) - reinterpret_cast<u64>(header);
}

} // Utils
