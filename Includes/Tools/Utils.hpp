#pragma once
#include "types.h"
#include <climits>
#include "Tools/CContainer.hpp"

namespace Utils {

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (std::size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

extern void*    copybytes(void* _dest, const void* _src, size_t _size);
int             makedir(const char* dir);

bool            FileToCC(const char* fname, CContainer* cc);
bool            CCtoFile(const char* fname, CContainer* cc, bool makedir);

u32             CalculateChecksum(CContainer& data);


} // Utils
