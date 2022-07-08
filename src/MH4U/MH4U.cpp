#include "MH4U/MH4U.hpp"
#include "MH4U/Quest.hpp"

#include "Tools/Utils.hpp"

#include "Tools/Folder.hpp"
#include "Tools/File.hpp"

#include <cstring>

namespace MH4U {
static const char* KeyEXTData    = "blowfish key iorajegqmrna4itjeangmb agmwgtobjteowhv9mope";
static const char* KeyDLC_EUR_NA = "AgK2DYheaCjyHGPB";
static const char* KeyDLC_JPN    = "AgK2DYheaCjyHGP8";
static const char* KeyDLC_KOR    = "AgK2DYheaOjyHGP8";
static const char* KeyDLC_TW     = "Capcom123 ";

static bool seed_rng_once = false;
static std::mt19937 rng;

Crypto::Crypto()
{
    if (!seed_rng_once) {
        seed_rng_once = true;
        rng.seed(RNG_SEED);
    }
}

const bool Crypto::decode_save(const CContainer &in, CContainer &out)
{
    bool res = true;

    res &= this->blowfish_decode(in, out, Crypto::Key::EXT_DATA);
    res &= this->xor_decode(out);

    return res;
}

const bool Crypto::encode_save(CContainer &in, CContainer &out)
{
    bool res = true;

    this->xor_encode(in);
    res &= this->blowfish_encode(in, out, Crypto::Key::EXT_DATA);

    return res;

}

const bool Crypto::decode_dlc(const CContainer &in, CContainer &out)
{
    for (size_t i = 0; i < Key::LENGTH; i++) {
        this->blowfish_decode(in, out, static_cast<Key>(i));

        sQuest* quest = reinterpret_cast<sQuest*>(out.data());

        if (quest->check_version())
            return true;
    }

    return false;
}

const bool Crypto::encode_dlc(CContainer &in, CContainer &out, const Key key)
{
    return this->blowfish_encode(in, out, key);
}

const bool Crypto::blowfish_decode(const CContainer &in, CContainer &out, const Key key)
{
    const char* selected_key = nullptr;
    u32 out_size;

    out_size = this->m_blowfish.get_output_length(in.size());

    out.clear();
    out.resize(out_size);

    selected_key = this->get_key(key);

    if (selected_key) {
        auto size = std::strlen(selected_key);

        this->m_blowfish.initialize(selected_key, size);
        this->m_blowfish.decode(in.data(), out.data(), out_size);

        return true;
    } else return false;
}

const bool Crypto::blowfish_encode(CContainer &in, CContainer &out, const Key key)
{
    const char* selected_key = nullptr;
    u32 out_size;

    out_size = this->m_blowfish.get_output_length(in.size());

    out.clear();
    out.resize(out_size);

    selected_key = this->get_key(key);

    if (selected_key) {
        auto size = std::strlen(selected_key);

        this->m_blowfish.initialize(selected_key, size);
        this->m_blowfish.encode(in.data(), out.data(), out_size);

        return true;
    } else return false;
}

const bool Crypto::xor_decode(CContainer &cc)
{
    u32 seed;
    u32 checksum, checksum2;

    seed = cc.as<u32>(0) >> 16;
    cc.sub_before(sizeof(u32)); // remove seed

    this->xor_mh4u(cc, seed);

    checksum = cc.as<u32>(0);
    cc.sub_before(sizeof(u32)); // remove checksum

    checksum2 = Utils::calculate_checksum(cc);

    if (checksum == checksum2) return true;
    return false;
}

void Crypto::xor_encode(CContainer &cc)
{
    u16 random; // must be u16
    u32 seed, seed_header;
    u32 checksum;

    random      = rng();
    seed        = seed_header = random;
    seed_header = (seed_header << 16) + 0x10;
    checksum    = Utils::calculate_checksum(cc);

    this->insert_value(cc, checksum);

    this->xor_mh4u(cc, seed);

    this->insert_value(cc, seed_header);
}

void Crypto::xor_mh4u(CContainer &data, u32 seed)
{
    u32 i = 0;

    while(i < data.size() / 2) // /2 because reading as u16
    {
        if (seed == 0){seed = 1;}
        seed = seed * 0xB0 % 0xFF53;
        data.as<u16>(i) ^= seed;
        i++;
    };
}

const char *Crypto::get_key(const Key key) const
{
    switch (key) {
    case Key::EXT_DATA: return MH4U::KeyEXTData;
    case Key::DLC_EUR_NA: return MH4U::KeyDLC_EUR_NA;
    case Key::DLC_JPN: return MH4U::KeyDLC_JPN;
    case Key::DLC_KOR: return MH4U::KeyDLC_KOR;
    case Key::DLC_TW: return MH4U::KeyDLC_TW;
    default:
    case Key::LENGTH: return nullptr;
    }
}

void Crypto::insert_value(CContainer& data, u32 value)
{
    data.add_before(sizeof(u32));
    data.as<u32>(0) = value;

//    size_t i = 0;
//    value = Utils::swap_endianness<u32>(value);


//    while (i < 4){
//        if (i != 0) {value >>= 8;}
//        data.add_before(1);
//        data.as<u8>(0) = value;
//        i++;
//    }
}

} // MH4U
