#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"
#include "MH4U/Extentions/lmd.hpp"

#include "Tools/Blowfish.hpp"
#include <random>

#define RNG_SEED 0x484D // 0x484D == MH

namespace MH4U {

class Crypto
{
public:
    enum Key {
        EXT_DATA = 0,
        DLC_EUR_NA,
        DLC_JPN,
        DLC_KOR,
        DLC_TW,

        LENGTH
    };

    Crypto();


    const bool decode_save(const CContainer& in, CContainer& out);
    const bool encode_save(const CContainer& in, CContainer& out);

    const bool decode_ext_data(const CContainer& in, CContainer& out) { return this->decode_save(in, out); }
    const bool encode_ext_data(const CContainer& in, CContainer& out) { return this->encode_save(in, out); }

    const bool decode_quest(const CContainer& in, CContainer& out) { return this->decode_dlc(in, out); }
    const bool encode_quest(const CContainer& in, CContainer& out, const Key key = Key::DLC_EUR_NA) { return this->encode_dlc(in, out, key); }

    const bool decode_dlc(const CContainer& in, CContainer& out);
    const bool encode_dlc(const CContainer& in, CContainer& out, const Key key = Key::DLC_EUR_NA);

    const bool blowfish_decode(const CContainer& in, CContainer& out, const Key key);
    const bool blowfish_encode(const CContainer& in, CContainer& out, const Key key);

private:
    BlowFish        m_blowfish;

    const bool  xor_decode(CContainer& cc);
    void        xor_encode(CContainer& cc);
    void        xor_mh4u(CContainer& data, u32 seed);

    const char* get_key(const Key key) const;
    void insert_value(CContainer& cc, u32 value);
};
};

