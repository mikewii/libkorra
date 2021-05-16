#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"

struct ARC_File_s {
    static const u32 FNAME_SIZE = 64;

    u8      Filename[FNAME_SIZE];
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

struct Pair {

    char    Filename[ARC_File_s::FNAME_SIZE];
    u32     ResourceHash;
    u32     XORLock;
    u32     DecSize = 0;

    bool    decompressed = false;

    CContainer cc{};
    ARC_File_s* f;
};

struct Geometry {
    float   Rotation;
    float   X;
    float   Y;
    float   Z;
};
