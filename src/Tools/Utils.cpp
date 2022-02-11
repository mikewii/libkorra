#include "Tools/Utils.hpp"
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <ostream>
#include <fstream>

#include "MHXX/Quest/Common.hpp"

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

    // Alignment safe copy
extern void* copybytes(void* _dest, const void* _src, size_t _size)
{
    int isSafe = reinterpret_cast<u64>(_dest) % sizeof(_dest);

    if (isSafe == 0)
        return memcpy(_dest, _src, _size);

    return memmove(_dest, _src, _size);
}

extern void* Copy_UTF16_String(void* dest, const std::u16string& str)
{
    return copybytes(dest, reinterpret_cast<const void*>(str.data()), str.size() * sizeof(u16));
}

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

void Collector::Add(const Collector::Info& in)
{
    if (!Collector::IsActive()) return;

    switch(Collector::_operation){
    default:break;
    case Collector::Op::Equal:          { if (in.Value == Collector::_value) Collector::_vec.push_back(in); break; }
    case Collector::Op::NotEqual:       { if (in.Value != Collector::_value) Collector::_vec.push_back(in); break; }
    case Collector::Op::Less:           { if (in.Value <  Collector::_value) Collector::_vec.push_back(in); break; }
    case Collector::Op::Greater:        { if (in.Value >  Collector::_value) Collector::_vec.push_back(in); break; }
    case Collector::Op::LessEqual:      { if (in.Value <= Collector::_value) Collector::_vec.push_back(in); break; }
    case Collector::Op::GreaterEqual:   { if (in.Value >= Collector::_value) Collector::_vec.push_back(in); break; }
    case Collector::Op::Unique:
    {
        if (!Collector::_vec.empty())
            for (const auto& item : Collector::_vec)
                if (in.Value == item.Value) return;

        Collector::_vec.push_back(in);
        break;
    }
    case Collector::Op::Collect:
    {
        Collector::_vec.push_back(in);

        if (!Collector::_vec_unique_ids.empty())
            for (const auto& value : Collector::_vec_unique_ids)
                if (in.Value == value) return;

        Collector::_vec_unique_ids.push_back(in.Value);
    }
    }
}

void Collector::Show(const bool sorted)
{
    if (!Collector::IsActive()) return;

    if (Collector::_operation == Collector::Op::Collect)
    {
        if (Collector::_path.empty())
        {
            printf("Collector:: Path is not set! Aborting\n");
            return;
        }
        else
        {
            if (Collector::Flush())
                printf("Collector:: data saved as %s at path %s\n", Collector::_name.c_str(), Collector::_path.c_str());
            else printf("Collector:: Error happened trying to write file at path %s\n", Collector::_path.c_str());
            return;
        }
    }

    if (sorted)
        std::sort
        (
            Collector::_vec.begin(),
            Collector::_vec.end(),
            [](const Collector::Info& a, const Collector::Info& b){ return a.Value < b.Value; }
        );

    printf("\n");
    for (const auto& info : Collector::_vec)
        printf("q%07d %-30s %-3s | dec: %d - hex: %X\n",
               info.QuestID, info.Name.c_str(), MHXX::QuestLv::GetStr(info.QuestLevel), info.Value, info.Value);
}

bool Collector::Flush(void)
{
    if (!Collector::IsActive()) return false;

    char            buffer[0x100];
    std::fstream    fout(Collector::_path + Collector::_name, std::ios::out);


    std::sort
    (
        Collector::_vec.begin(),
        Collector::_vec.end(),
        [](const Collector::Info& a, const Collector::Info& b){ return a.QuestID < b.QuestID; }
    );

    std::sort
    (
        Collector::_vec_unique_ids.begin(),
        Collector::_vec_unique_ids.end(),
        [](const s32& a, const s32& b){ return a < b; }
    );

    if (!fout.is_open()) return false;

    for (u32 i = 0; i < Collector::_vec_unique_ids.size(); i++)
    {
        const auto& token = Collector::_vec_unique_ids.at(i);

        for (const auto& info : Collector::_vec)
            if (info.Value == token) // format and write to file
            {
                const auto& size = sprintf(buffer, "q%07d %-30s %-3s | dec: %d - hex: %X\n",
                       info.QuestID, info.Name.c_str(), MHXX::QuestLv::GetStr(info.QuestLevel), info.Value, info.Value);

                fout.write(buffer, size);
            }
        // add 3 new lines
        if (i != (Collector::_vec_unique_ids.size() - 1))
            for (auto i = 0; i < 3; i++)
                fout << std::endl;
    }

    fout.close();
    return true;
}

} // Utils
