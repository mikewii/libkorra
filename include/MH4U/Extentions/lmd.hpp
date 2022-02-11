#pragma once
#include "types.h"
#include "Global.hpp"
#include <cwchar>

namespace MH4U {

static const char LMD_MAGIC[5] = "lmd\0";

struct Data0 {
    u32 unk[2];
};

struct Data1 {
    u32     unk0;
    u32     index; // every first only
    u32     unk1;
    u32     unk2;
};

struct U16string_info {
    u32     pWStr;
    u32     str_size;
    u32     str_size_copy;
};

struct sLMD {
    char    magic[4];

    u8      version_patch;
    u8      version_minor;
    u16     version_major;

    u32     data0_ammount;
    u32     data1_ammount;
    u32     u16string_info_ammount;

    u32     pData0;
    u32     pData1;
    u32     pU16string_info;

    // utf16le strings

    u32     pFilename;

    // utf16le strings at data3_ammount * (3 * 4) + pData2

    u64 FixPointers(void) const { return reinterpret_cast<u64>(this); }
    void WriteVersion(const u16 major, const u8 minor, const u8 patch)
    {
        this->version_major = major;
        this->version_minor = minor;
        this->version_patch = patch;
    }
};

class cLMD {
public:
    cLMD(const CContainer& data);
    cLMD(const Pair& data);
    ~cLMD();

    void write(CContainer& data);

    void replace_String(const std::u16string& str, const u32 id);

    void remove_Item(const u32 id);

    void print_Data0(void) const;
    void print_Data1(void) const;
    void print_U16string_info(void) const;
    void print_Strings(void) const;
    void print_Filename(void) const;
    void print_counts(void) const;

private:
    std::vector<Data0>          vData0;
    std::vector<Data1>          vData1;
    std::vector<U16string_info> vU16string_info;
    std::vector<std::u16string> vStrings;
    std::string                 filename;


    void read(const CContainer& _pp);

};

} // MH4U
