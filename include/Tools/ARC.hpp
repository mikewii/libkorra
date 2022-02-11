#pragma once
#include "types.h"
#include "Global.hpp"

#include <vector>
#include <zlib.h>

static const char ARC_MAGIC[5] = "ARC\0";
static const char CRA_MAGIC[5] = "\0CRA";

class ARC
{
public:
    enum Version:u16 {
        None        = 0,
        LP1         = 7,
        LP2         = 8,
        MH3U_3DS    = 16,
        MH4U_MHXX   = 17,
        MH4U_1      = 19
    };

    struct Header {
        char            magic[4];
        ARC::Version    version;
        u16             fileNum;
    };

    struct File_Header {
        u8      fileName[FNAME_SIZE];
        u32     resourceHash;
        u32 	compressedSize;
        u32     decompressedSize; // xor 20000000 + 40000000 if version is 17
        u32     pZData; // 78 9C - zlib header
    };


    ARC(){};
    ARC(CContainer& _arcdata, std::vector<Pair>* _outlist);
    ~ARC();

    bool    isARC(void) const;

    void    Decompress(u32 n);
    int     Decompress(Pair& sourcePair, Pair& destPair);
    int     Compress(Pair& sourcePair, Pair& destPair);

    void    ExtractAll(void);


    void    PrintHeader(void);
    void    PrintPairsInfo(void);
    void    PrintFileInfo(ARC::File_Header* f, u32 n);

    u32     GetFilesCount(void) const { return __header->fileNum; }

    // Making ARC
    void MakeARC(CContainer& _output, std::vector<Pair>* _list, ARC::Version _version = ARC::Version::None);
    void MakeARC_File_s_Header(CContainer& _cc, std::vector<Pair>& _list, u32 _padding, u32 _zDataStart);
    void CopyZData(CContainer& _cc, std::vector<Pair>& _list, u32 _zDataStart);

private:
    bool _isARC = true;

    union { // dont need it?
        u32 bitfield{0};
        struct{
            bool LE : 1;
            bool BE : 1;
            bool isARC : 1;
        };
    } b;

    void    Read(CContainer& _data);
    void    PushFile(CContainer& _data, u32 n);


    bool    __isARC(void);
    bool    isCRA(void);
    void    isARCFile(void);

    u32     isNeedPadding(ARC::Version _version);
    u32     isNeedPadding(u32 _version);
    u32     GetVersionValue(ARC::Version _version);

    u32     extractXORLock(u32 _decSize);

    void    FixBE_Header(void);
    void    FixBE_ARC_File_s(ARC::File_Header* f);

    u32     Align(u32 _value);

    std::vector<Pair>*              __list;
    std::vector<ARC::File_Header*>  __listARC_File_s;
    ARC::Header*                    __header = nullptr;
    static ARC::Version             __previousVersion;

};
