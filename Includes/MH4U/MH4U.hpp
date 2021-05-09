#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"


class MH4U
{
public:
    void Decode(CContainer& in, CContainer& out, bool isQuest = false);
    void Encode(CContainer& in, CContainer& out, bool isQuest = false);

private:
    bool    PostDecode_Save(CContainer& data);
    void    PreEncode_Save(CContainer& data);

    void    MHXOR(CContainer& data, u32 seed);
    u32     CalculateChecksum(CContainer& data);
    void    InsertValue(CContainer& data, u32 value);

    static u8 KeySaveFile[];
    static u8 KeyDLC[];
    static u8 KeyDLC_TW[];
};
