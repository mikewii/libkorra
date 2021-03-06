#pragma once
#include "types.h"
#include "Global.hpp"

#include <vector>

namespace MHXX {
namespace GMD {

#define NULL_TERMINATOR 1

static const u32 RESOURCE_HASH = 0x242BB29A; // mhxx

struct sGMD_Header_s {
    static const u32    MAGIC   = 0x00444D47; // GMD
    static const u32    VERSION = 0x00010302; // u8 major 2, u8 minor 3, u8 patch 1, u8 padding

    u32     Magic       = MAGIC;
    u32     Version     = VERSION;

    u32     Padding0    = 0;
    float   Unk;
    u32     Padding1    = 0;

    u32     LabelsNum;
    u32     ItemsNum;   // can be higher than labelsnum
    u32     LabelsSize;
    u32     ItemsSize;

    u32     FilenameSize;
};

struct sGMD_Advanced1_s {
    u32 Unk[5];
};

struct sGMD_Advanced2_s {
    u32 Unk[256];
};

class cGMD : public PairInfoKeeper
{
public:

    cGMD();
    cGMD(const Pair& _pp);
    cGMD(const cGMD&) = delete;
    ~cGMD();

    cGMD& operator=(const cGMD&) = delete; // maybe later

    void print_Header(void) const;
    bool print_Filename(void) const;
    bool print_AllLabels(void) const;
    bool print_AllItems(void) const;

    /**
     * @brief Save loaded gmd to new Pair
     * @param _pp       Pair to save data to
     * @param _vecAdv1  Vector of sGMD_Advanced1_s data
     * @param _adv2     sGMD_Advanced2_s data
     */
    void save(Pair& _pp, std::vector<sGMD_Advanced1_s>* _vecAdv1 = nullptr, sGMD_Advanced2_s* _adv2 = nullptr);

    /**
     * @brief Make new gmd and save it to Pair
     * @param _pp       Pair to save data to
     */
    void make(Pair& _pp);


    /**
     * @brief Make new advancew gmd and save it to Pair
     * @param _pp       Pair to save data to
     * @param _vecAdv1  Vector of sGMD_Advanced1_s data
     * @param _adv2     sGMD_Advanced2_s data
     */
    void makeAdv(Pair& _pp, std::vector<sGMD_Advanced1_s>* _vecAdv1 = nullptr, sGMD_Advanced2_s* _adv2 = nullptr);


    u32             Get_ItemsNum(void) const;
    u32             Get_LabelsNum(void) const;
    std::string     Get_FilenameStr(void) const;
    std::string     Get_LabelStr(u32 _id) const;
    std::string     Get_ItemStr(u32 _id) const;

    void            Set_FilenameStr(const std::string& _str);
    bool            Set_LabelStr(const std::string& _str, const u32 _id);
    bool            Set_ItemStr(const std::string& _str, const u32 _id);
    void            Append_LabelStr(const std::string& _str);
    void            Append_ItemStr(const std::string& _str);

    bool            Remove_LabelStr(const u32 _id);
    bool            Remove_ItemStr(const u32 _id);

private:
    sGMD_Header_s*                  __data = nullptr;
    std::string                     __filename = "example";
    std::vector<std::string>        __labelStrings;
    std::vector<std::string>        __itemStrings;
    float                           __unk = 0;
    std::vector<sGMD_Advanced1_s>   __dataAdv1;
    sGMD_Advanced2_s*               __dataAdv2 = nullptr;

    void print(const std::string& _str) const;
    void print(const char* _str) const;
    void readAll(void);

    void Set_Header(sGMD_Header_s& _header, u32 _labelsSize, u32 _itemsSize);

};

} // GMD
} // MHXX
