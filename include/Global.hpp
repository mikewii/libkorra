#pragma once
#include "types.h"
#include "Tools/Utils.hpp"

#define MHXX_XORLock 0x40000000

#ifdef N3DS
extern int printf (const char *__restrict __format, ...){return 0;}
#endif

template <class T>
class Export
{
public:
    static void save(T& _t, Pair& _pp)
    {
        u32 size = sizeof(_t.__data);

        _pp.cc.resize(size);

        Utils::copybytes(_pp.cc.data(), &_t.__data, size);

        if (_t.isPairInfoSet()) _t.GetPairInfo(_pp);
        else
        {
            _pp.info.DecSize        = size;
            _pp.info.ResourceHash   = _t.getResourceHash();
            _pp.info.XORLock        = MHXX_XORLock;
            _pp.info.isDecompressed = true;
        }
    }
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
