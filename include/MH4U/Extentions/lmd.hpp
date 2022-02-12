#pragma once
#include "types.h"
#include "Global.hpp"
#include <cwchar>

namespace MH4U {
namespace LMD {
static const u32    RESOURCE_HASH = 0x62440501;
static const u32    LMD_MAGIC_HEX = 0x00646D6C;
static const char   LMD_MAGIC[5] = "lmd\0";
static const char   EXT[5] = "lmd";

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
    u32     pU16Str;
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

    u32     pFilename;


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

    void write(Pair& pair);
    u32  write(CContainer& data);

    void replace_String(const std::u16string& str, const u32 id);
    void replace_String(const std::string& str, const u32 id);

    void remove_Item(const u32 id);

    u32 Get_Strings_Count(void) const { return cLMD::vStrings.size(); }
    u32 Get_Data0_Count(void) const { return cLMD::vData0.size(); }
    u32 Get_Data1_Count(void) const { return cLMD::vData1.size(); }

    void print_Data0(void) const;
    void print_Data1(void) const;
    void print_U16string_info(void) const;
    void print_Strings(void) const;
    void print_Filename(void) const;
    void print_counts(void) const;

    // for test
    const std::vector<Data0>& __Get_Data0_vector(void) const { return cLMD::vData0; }
    const std::vector<Data1>& __Get_Data1_vector(void) const { return cLMD::vData1; }
    const std::vector<U16string_info>& __Get_U16String_info_vector(void) const { return cLMD::vU16string_info; }
    const std::vector<std::u16string>  __Get_U16String_vector(void) const { return cLMD::vStrings; }

private:
    std::vector<Data0>          vData0;
    std::vector<Data1>          vData1;
    std::vector<U16string_info> vU16string_info;
    std::vector<std::u16string> vStrings;
    std::string                 filename;


    void read(const CContainer& _pp);

    u32 getResourceHash(void) const { return MH4U::LMD::RESOURCE_HASH; }
};

}
} // MH4U
