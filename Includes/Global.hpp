#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"

#define MHXX_XORLock 0x40000000

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

struct Geometry {
    float   Rotation;
    float   X;
    float   Y;
    float   Z;
};

struct Pair {

    char    Filename[ARC_File_s::FNAME_SIZE];
    u32     ResourceHash;
    u32     XORLock;
    u32     DecSize = 0;

    bool    isDecompressed = false;

    CContainer cc{};
    ARC_File_s* f;

    void print( void );
};

class PairInfo // for transfering info from one pair to another
{
public:

    /**
     * @brief Saves info from provided Pair
     * @param _pp   Pair to work on
     */
    void SetPairInfo( Pair& _pp );

    /**
     * @brief Writes saved info to provided Pair
     * @param _pp   Pair to work on
     */
    bool GetPairInfo( Pair& _pp );

    bool isPairInfoSet( void ) const { return this->__isSet; }

private:
    char    __filename[ARC_File_s::FNAME_SIZE];
    u32     __resourceHash;
    u32     __XORLock;
    u32     __decSize;
    bool    __decompressed;

    bool    __isSet = false;
};
