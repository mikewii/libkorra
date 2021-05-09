#pragma once
#include "types.h"
#include "Tools/Container.hpp"

#include <vector>
#include <zlib.h>

static const char ARC_MAGIC[5] = "ARC\0";
static const char CRA_MAGIC[5] = "\0CRA";

class ARC
{
private:
    struct ARC_File_s {
        char    Filename[64];
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

    struct Pairs
    {
        ARC_File_s* f;
        CContainer cc{};

        void FixPath(void);
    };

    union {
        u32 bitfield{0};
        struct{
            bool LE : 1;
            bool BE : 1;
            bool isARC : 1;
        };
    } b;

    ARC_s* header = nullptr;
    std::vector<Pairs> files;
    std::string filename, path;


    void Read(CContainer& _data);
    void PushFile(CContainer& _data, u32 n);
    void GetPWD(void);


    bool isARC(void);
    bool isCRA(void);
    void isARCFile(void);

public:
    ARC(CContainer& _data);
    ~ARC();

    void Extract(u32 n, bool writeToFile = false);

    void PrintHeader(void);
    void PrintFileInfo(ARC_File_s* f, u32 n);
    void SetFilename(std::string fname) { this->filename = fname + "_out"; }
    void SetPath(std::string _path);

};
