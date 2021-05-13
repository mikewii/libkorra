#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"

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
    struct ARC_File_s {
        static const u32 FNAME_SIZE = 64;

        u8    Filename[FNAME_SIZE];
        u32     ResourceHash;
        u32 	CompressedSize;
        u32     DecompressedSize; // xor 20000000 + 40000000 if version is 17
        u32     pZData; // 78 9C - zlib header
    };

    struct ARC_s {
        char Magic[4];
        u16  Version;
        u16  FilesNum;
    };

    union {
        u32 bitfield{0};
        struct{
            bool LE : 1;
            bool BE : 1;
            bool isARC : 1;
        };
    } b;

    void Read(CContainer& _data);
    void PushFile(CContainer& _data, u32 n);


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

    ARC_s* __header = nullptr;
    std::string filename, path;
    static u32 previousVersion;

public:



    struct Pairs {
        static const u32 FNAME_SIZE = 64;

        char    Filename[FNAME_SIZE];
        u32     ResourceHash;
        u32     XORLock;
        u32     DecSize = 0;

        bool    decompressed = false;

        CContainer cc{};
        ARC_File_s* f; // only in scope of class

        void FixPath(bool revert = false);
    };

    ARC(){};
    ARC(CContainer& _data, std::vector<Pairs>* _list);
    ~ARC();

    void Decompress(u32 n);
    int     Decompress(Pairs& sourcePair, Pairs& destPair);
    int     Compress(Pairs& sourcePair, Pairs& destPair);

    void ExtractAll(void);


    void PrintHeader(void);
    void PrintFileInfo(ARC_File_s* f, u32 n);
    void SetFilename(std::string fname) { this->filename = fname + "_out"; }

    u32 GetFilesCount(void) const { return __header->FilesNum; }

    // Making ARC
    void MakeARC(CContainer& _output, std::vector<Pairs>* _list, ARCVersion _version = ARCVersion::None);
    void MakeARC_File_s_Header(CContainer& _cc, std::vector<Pairs>& _list, u32 _padding, u32 _zDataStart);
    void CopyZData(CContainer& _cc, std::vector<Pairs>& _list, u32 _zDataStart);



    std::vector<Pairs>* __list;
};
