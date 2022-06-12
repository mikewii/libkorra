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



        void BE_Swap(void)
        {
            this->version = Utils::swap_endianness<ARC::Version>(this->version);
            this->fileNum = Utils::swap_endianness<u16>(this->version);
        }
    };

    struct File_Header {
        u8      fileName[FNAME_SIZE];
        u32     resourceHash;
        u32 	compressedSize;
        u32     decompressedSize; // xor 20000000 + 40000000 if version is 17
        u32     pZData; // 78 9C - zlib header

        void BE_Swap(void)
        {
            this->resourceHash        = Utils::swap_endianness<u32>(this->resourceHash);
            this->compressedSize      = Utils::swap_endianness<u32>(this->compressedSize);
            this->decompressedSize    = Utils::swap_endianness<u32>(this->decompressedSize);
            this->pZData              = Utils::swap_endianness<u32>(this->pZData);
        }
    };


    ARC(){};
    explicit ARC(const CContainer& container, std::vector<Pair>& vOut);
    explicit ARC(u8* src); // need size?
    ~ARC();

    const bool  IsARC(void) const;
    const bool  IsOpen(void) const;

    void    Decompress(const u32 id);
    int     Decompress(Pair& sourcePair, Pair& destPair);
    int     Compress(const Pair& sourcePair, Pair& destPair);

    void    ExtractAll(void);


    void    print_Header(void);
    void    print_PairsInfo(void);
    void    print_FileInfo(ARC::File_Header* f, u32 n);

    u32     GetFilesCount(void) const { return ARC::header.fileNum; }

    // Making ARC
    void MakeARC(CContainer& container, std::vector<Pair>& vPair, ARC::Version version = ARC::Version::None);
    void MakeARC_File_s_Header(CContainer& _cc, std::vector<Pair>& _list, u32 _padding, u32 _zDataStart);
    void CopyZData(CContainer& _cc, std::vector<Pair>& _list, u32 _zDataStart);

private:
    bool open = false;
    bool isLE = false;
    bool isBE = false;
    bool isARC = false;
    bool trust = true; // dont use on unknown versions

    ARC::Header                     header;
    std::vector<ARC::File_Header>   vFile_Header;

    std::vector<Pair>*              __vPair = nullptr;
    const CContainer*               __container = nullptr;

    static ARC::Version             prev_Version;
    static u32 Is_NeedPadding(const ARC::Version version);
    static u32 extract_XORLock(const u32 decSize);
    static u32 Align(u32 value);

    void    Read(const CContainer& container);
};
