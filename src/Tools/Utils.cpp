#include "Tools/Utils.hpp"
#include <string.h>
#include <algorithm>

#include <fstream>

#include "MHXX/Quest/Common.hpp"

#ifdef _WIN32
    #include <Shlobj.h>
#elif __linux__
    #include <stdlib.h>
#elif __APPLE__
    #include <NSSystemDirectories.h>
#endif

void NotifyError(std::string _str){
    fprintf(stderr, "\n%s\n", _str.c_str());
}

namespace Utils {

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



u32 calculate_checksum(const CContainer& cc)
{
    u32 i = 0, checksum = 0;

    while (i < cc.size()) {
        checksum += cc.at<u8>(i) & 0xFF; i++;
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


#ifndef N3DS
std::filesystem::path Get_User_Home(const bool documents)
{
#ifdef _WIN32
    WCHAR   path[MAX_PATH];
    auto    id = documents ? CSIDL_MYDOCUMENTS : CSIDL_PROFILE;

    HRESULT result = SHGetSpecialFolderPath(HWND_DESKTOP, path, id, NULL);

    // TODO: handle result

    return std::filesystem::path(path);

#elif __linux__
    if (documents)
        return std::filesystem::path(std::getenv("HOME")).append("Documents");
    else return std::filesystem::path(std::getenv("HOME"));
#else
    if (documents)

        return std::filesystem::path(std::getenv("HOME")).append("Documents");
    else return std::filesystem::path(std::getenv("HOME"));
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
    std::fstream    fout(Collector::_path.append(Collector::_name), std::ios::out);


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

bool Filter::Is_InVector(const std::filesystem::__cxx11::path &path, const std::vector<std::string> &vector)
{
    if (!vector.empty())
    {
        bool found = false;

        for (auto& str : vector)
            if (str.compare(path.filename().string()) == 0)
            {
                found = true;
                break;
            }

        if (!found) return false;;
    }

    return true;
}
#endif

} // Utils
