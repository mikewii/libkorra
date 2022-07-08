#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"
#include "MH4U/Extentions/lmd.hpp"
#include "MH4U/Quest.hpp"

namespace MH4U {
/*
 * TODO:
 * move out random engine
 * move out blowfish
 * create class MH4U?
*/


    void decode(CContainer& in, CContainer& out, bool isQuest = false);
    void encode(CContainer& in, CContainer& out, bool isQuest = false);

    enum Key {
        EXT_DATA = 0,
        DLC_EUR_NA,
        DLC_JPN,
        DLC_KOR,
        DLC_TW,

        ENUM_LENGTH
    };

    const bool decode_save(const CContainer& in, CContainer& out);
    const bool encode_save(const CContainer& in, CContainer& out);

    const bool  blowfish_decode(const CContainer& in, CContainer& out, const Key key);
    const bool  blowfish_encode(const CContainer& in, CContainer& out, const Key key);
    const bool  xor_decode(CContainer& cc);
    void        xor_encode(CContainer& cc);

    const char* get_key(const Key key);
};
