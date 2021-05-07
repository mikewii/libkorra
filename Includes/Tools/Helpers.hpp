#pragma once
#include "types.h"

#include <string>
#include <vector>
#include <climits>

namespace MH4U {

    std::vector<u8> Decode(std::vector<u8> in, bool isQuest = false);
    std::vector<u8> Encode(std::vector<u8> in, bool isQuest = false);

    void XORSaveData(std::vector<u8> &vec);
    void MHXOR(std::vector<u8> &vec, u32 seed);
    u32 CalculateChecksum(std::vector<u8> &vec);

    void PrepareSaveForEncode(std::vector<u8> &vec);
    void InsertData(std::vector<u8> &vec, u32 data);
}


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


std::vector<u8> FileToVector(std::string filename);
