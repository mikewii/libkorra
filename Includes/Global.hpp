#pragma once
#include "types.h"
#include "Tools/Utils.hpp"

#define MHXX_XORLock 0x40000000

template <class T>
class Export
{
public:
    static void save( T& _t, Pair& _pp )
    {
        u32 size = sizeof( _t.__data );

        _pp.cc.resize( size );

        Utils::copybytes( _pp.cc.data(), &_t.__data, size );

        if ( _t.isPairInfoSet() ) _t.GetPairInfo( _pp );
        else
        {
            _pp.info.DecSize        = size;
            _pp.info.ResourceHash   = _t.getResourceHash();
            _pp.info.XORLock        = MHXX_XORLock;
            _pp.info.isDecompressed = true;
        }
    }
};


struct ARC_File_s {

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

template <typename T>
struct Geometry4 {
    T   R;
    T   X;
    T   Y;
    T   Z;
};

template <typename T>
struct Geometry3 {
    T   X;
    T   Y;
    T   Z;
};
