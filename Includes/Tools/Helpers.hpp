#pragma once
#include "types.h"

#include <vector>
#include <string>
#include <climits>


inline u16& as_u16(u8& a){
    return reinterpret_cast<u16&>(a);
}

inline u32& as_u32(u8& a){
    return reinterpret_cast<u32&>(a);
}

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

void FileToVector(std::string filename, std::vector<u8>& vec);
