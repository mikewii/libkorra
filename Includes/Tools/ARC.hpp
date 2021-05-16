#pragma once
#include "types.h"
#include "Global.hpp"

#include <vector>
#include <zlib.h>

static const char ARC_MAGIC[5] = "ARC\0";
static const char CRA_MAGIC[5] = "\0CRA";

enum class ARCVersion {
    None,
    MH4U,
    MHXX,
    LP1,
    LP2
};

class ARC
{
private:

    union {
        u32 bitfield{0};
        struct{
            bool LE : 1;
            bool BE : 1;
            bool isARC : 1;
        };
    } b;

    void    Read(CContainer& _data);
    void    PushFile(CContainer& _data, u32 n);


    bool    isARC(void);
    bool    isCRA(void);
    void    isARCFile(void);

    u32     isNeedPadding(ARCVersion _version);
    u32     isNeedPadding(u32 _version);
    u32     GetVersionValue(ARCVersion _version);

    u32     extractXORLock(u32 _decSize);

    void FixBE_Header(void);
    void FixBE_ARC_File_s(ARC_File_s* f);

    u32     Align(u32 _value);

    ARC_s*              __header = nullptr;
    std::vector<Pair>*  __list;
    static u32          __previousVersion;

public:

    ARC(){};
    ARC(CContainer& _arcdata, std::vector<Pair>* _outlist);
    ~ARC();

    void    Decompress(u32 n);
    int     Decompress(Pair& sourcePair, Pair& destPair);
    int     Compress(Pair& sourcePair, Pair& destPair);

    void    ExtractAll(void);


    void    PrintHeader(void);
    void    PrintPairsInfo(void);
    void    PrintFileInfo(ARC_File_s* f, u32 n);

    u32 GetFilesCount(void) const { return __header->FilesNum; }

    // Making ARC
    void MakeARC(CContainer& _output, std::vector<Pair>* _list, ARCVersion _version = ARCVersion::None);
    void MakeARC_File_s_Header(CContainer& _cc, std::vector<Pair>& _list, u32 _padding, u32 _zDataStart);
    void CopyZData(CContainer& _cc, std::vector<Pair>& _list, u32 _zDataStart);

};
