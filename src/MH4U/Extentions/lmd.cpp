#include "MH4U/Extentions/lmd.hpp"

#include <locale>
#include <codecvt>

#include <algorithm>

namespace MH4U {
namespace LMD {

cLMD::cLMD(const CContainer& data)
{
    const sLMD* header = reinterpret_cast<const sLMD*>(data.data());

    if (std::equal(LMD_MAGIC, LMD_MAGIC + 4, header->magic))
        this->read(data);
    else return; // error
}

cLMD::cLMD(const Pair& data)
{
    const sLMD* header = reinterpret_cast<const sLMD*>(data.cc.data());

    if (std::equal(LMD_MAGIC, LMD_MAGIC + 4, header->magic))
        this->read(data.cc);
    else return; // error
}

cLMD::~cLMD()
{
}


void cLMD::remove_Item(const u32 id)
{
    cLMD::vU16string_info.erase(cLMD::vU16string_info.begin() + id);
    cLMD::vStrings.erase(cLMD::vStrings.begin() + id);
}

void cLMD::replace_String(const std::string& str, const u32 id)
{
    std::wstring_convert<std::codecvt_utf8_utf16<utf16>, utf16> converter;

    cLMD::vStrings.at(id) = converter.from_bytes(str);
}


void cLMD::print_Data0(void) const
{
    printf("\n##### Data0 #####\n");
    for (const auto& data0 : cLMD::vData0)
        printf("0: %08X | 1: %08X\n", data0.unk[0], data0.unk[1]);
}

void cLMD::print_Data1(void) const
{
    printf("\n##### Data1 #####\n");
    for (const auto& data1 : cLMD::vData1)
        printf("0: %08X | 1: %08X | 2: %08X | 3: %08X\n", data1.unk0, data1.index, data1.unk1, data1.unk2);
}

void cLMD::print_U16string_info(void) const
{
    printf("\n##### Data2 #####\n");
    for (const auto& data2 : cLMD::vU16string_info)
        printf("0: %08X | 1: %08X | 2: %08X\n", data2.pU16Str, data2.str_size, data2.str_size_copy);
}

void cLMD::print_Strings(void) const
{
    for (const auto& str : cLMD::vStrings)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<utf16>, utf16> converter;

        printf(u8"%s\n", converter.to_bytes(str).c_str());
    }
}

void cLMD::print_Filename(void) const
{
    printf("%s\n", cLMD::filename.c_str());
}

void cLMD::print_counts(void) const
{
    printf("data0: %zu\ndata1: %zu\ndata2: %zu\nstrings: %zu\n\n",
           cLMD::vData0.size(), cLMD::vData1.size(), cLMD::vU16string_info.size(), cLMD::vStrings.size());
}

void cLMD::read(const CContainer& data)
{
    const sLMD* header = reinterpret_cast<const sLMD*>(data.data());

    const Data0*            pData0 = reinterpret_cast<Data0*>(header->pData0 + header->FixPointers());
    const Data1*            pData1 = reinterpret_cast<Data1*>(header->pData1 + header->FixPointers());
    const U16string_info*   pData2 = reinterpret_cast<U16string_info*>(header->pU16string_info + header->FixPointers());
    const char*             pFilename = reinterpret_cast<const char*>(header->pFilename + header->FixPointers());


    cLMD::vData0.reserve(header->data0_ammount);
    cLMD::vData1.reserve(header->data1_ammount);
    cLMD::vU16string_info.reserve(header->u16string_info_ammount);

    for (u32 i = 0; i < header->data0_ammount; i++)
        cLMD::vData0.push_back(pData0[i]);

    for (u32 i = 0; i < header->data1_ammount; i++)
        cLMD::vData1.push_back(pData1[i]);

    for (u32 i = 0; i < header->u16string_info_ammount; i++)
        cLMD::vU16string_info.push_back(pData2[i]);

    for (const auto& item : cLMD::vU16string_info)
    {
        const utf16* pStr = reinterpret_cast<const utf16*>(item.pU16Str + header->FixPointers());

        cLMD::vStrings.push_back(pStr);
    }

    cLMD::filename = pFilename;
}

template <typename T>
void Copy(const T& item, u32& offset, u8* dest)
{
    Utils::copybytes(dest + offset, &item, sizeof(item));

    offset += sizeof(item);
}


void cLMD::write(Pair& pair)
{
    auto size = cLMD::write(pair.cc);

    pair.info.DecSize = size;
}

u32 cLMD::write(CContainer& container)
{
    const auto  vData0_size             = cLMD::vData0.size() * sizeof(Data0);
    const auto  vData1_size             = cLMD::vData1.size() * sizeof(Data1);
    const auto  vU16string_info_size    = cLMD::vU16string_info.size() * sizeof(U16string_info);
    auto        vStrings_size           = 0;
    const auto  pStrings                = sizeof(sLMD) + vData0_size + vData1_size + vU16string_info_size;
    const auto  filename_size           = cLMD::filename.size() + sizeof(u8);
    u32         offset = 0;

    /*u16string_info*/    for (u32 i = 0; i < cLMD::vStrings.size(); i++)
    /*u16string_info*/    {
    /*u16string_info*/        const auto& str = cLMD::vStrings.at(i);
    /*u16string_info*/        auto&       info = cLMD::vU16string_info.at(i);
    /*u16string_info*/
    /*u16string_info*/        const auto  size = (str.size() * sizeof(u16)) + sizeof(u16);
    /*u16string_info*/        const auto  align = size % 4;
    /*u16string_info*/        const auto  bytes = size + align;
    /*u16string_info*/
    /*u16string_info*/        vStrings_size += bytes;
    /*u16string_info*/
    /*u16string_info*/        info.pU16Str = pStrings + offset;
    /*u16string_info*/        info.str_size = str.size();
    /*u16string_info*/        info.str_size_copy = str.size();
    /*u16string_info*/
    /*u16string_info*/        offset += bytes;
    /*u16string_info*/    }

    const auto final_size = sizeof(sLMD) + vData0_size + vData1_size + vU16string_info_size + vStrings_size + filename_size;

    /*header*/    sLMD header;
    /*header*/
    /*header*/    std::copy(LMD_MAGIC, LMD_MAGIC + 4, header.magic);
    /*header*/
    /*header*/    header.WriteVersion(1, 1, 11);
    /*header*/
    /*header*/    header.data0_ammount = cLMD::vData0.size();
    /*header*/    header.data1_ammount = cLMD::vData1.size();
    /*header*/    header.u16string_info_ammount = cLMD::vU16string_info.size();
    /*header*/
    /*header*/    header.pData0 = sizeof(sLMD);
    /*header*/    header.pData1 = sizeof(sLMD) + vData0_size;
    /*header*/    header.pU16string_info = sizeof(sLMD) + vData0_size + vData1_size;
    /*header*/
    /*header*/    header.pFilename = sizeof(sLMD) + vData0_size + vData1_size + vU16string_info_size + vStrings_size;


    /*copy*/      container.resize(final_size, true);
    /*copy*/
    /*copy*/      Utils::copybytes(container.data(), &header, sizeof(sLMD));
    /*copy*/
    /*copy*/      offset = sizeof(sLMD);
    /*copy*/
    /*copy*/      for (const auto& item : cLMD::vData0)
    /*copy*/          Copy<Data0>(item, offset, container.data());
    /*copy*/
    /*copy*/      for (const auto& item : cLMD::vData1)
    /*copy*/          Copy<Data1>(item, offset, container.data());
    /*copy*/
    /*copy*/      for (const auto& item : cLMD::vU16string_info)
    /*copy*/          Copy<U16string_info>(item, offset, container.data());
    /*copy*/
    /*copy*/      for (const auto& str : cLMD::vStrings)
    /*copy*/      {
    /*copy*/          const auto size = (str.size() * sizeof(u16)) + sizeof(u16);
    /*copy*/          const auto align = size % 4;
    /*copy*/          const auto bytes = size + align;
    /*copy*/
    /*copy*/          Utils::Copy_UTF16_String(container.data() + offset, str);
    /*copy*/
    /*copy*/          offset += bytes;
    /*copy*/      }
    /*copy*/
    /*copy*/      Utils::copybytes(container.data() + offset, cLMD::filename.data(), cLMD::filename.size());

    return final_size;
}

}
} // MH4U
