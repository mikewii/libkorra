#include "MH4U/MH4U.hpp"

#include "Tools/Blowfish.hpp"
#include "Tools/Utils.hpp"

#include "Tools/Folder.hpp"
#include "Tools/File.hpp"

#include <cstring>
#include <random>

namespace MH4U {

bool    PostDecode_Save(CContainer& data);
bool    post_decode_extdata(CContainer& data);

void    PreEncode_Save(CContainer& data);
void    MHXOR(CContainer& data, u32 seed);
void    InsertValue(CContainer& data, u32 value);

static const char KeyEXTData[]    = "blowfish key iorajegqmrna4itjeangmb agmwgtobjteowhv9mope";
static const char KeyDLC_EUR_NA[] = "AgK2DYheaCjyHGPB";
static const char KeyDLC_JPN[]    = "AgK2DYheaCjyHGP8";
static const char KeyDLC_KOR[]    = "AgK2DYheaOjyHGP8";
static const char KeyDLC_TW[]     = "Capcom123 ";
static BlowFish cypher_static;
void decode(CContainer& in, CContainer& out, bool isQuest)
{
    u32                 outSize;
    BlowFish            cypher;
    bool                good = true;

    outSize = cypher.get_output_length(in.size());

    out.clear();
    out.resize(outSize);

    if (isQuest)
    {
        cypher.initialize(MH4U::KeyDLC_JPN, sizeof(MH4U::KeyDLC_JPN) -1);
        cypher.decode(in.data(), out.data(), outSize);

        //good = PostDecode_Save(out);

        //TODO: check for quest file header for error
    }
    else
    {
        cypher.initialize(MH4U::KeyEXTData, sizeof(MH4U::KeyEXTData) -1);
        cypher.decode(in.data(), out.data(), outSize);

        good = PostDecode_Save(out);

        //if (!good) NotifyError("Save file corrupt!");
    }
}

void encode(CContainer& in, CContainer& out, bool isQuest)
{
    u32                 outSize;
    BlowFish            cypher;

    if (isQuest)
    {
        outSize = cypher.get_output_length(in.size());
        out.resize(outSize - 4); // why -4? last 4 bytes is checksum?

        cypher.initialize(MH4U::KeyDLC_EUR_NA, sizeof(MH4U::KeyDLC_EUR_NA) -1);
        cypher.encode(in.data(), out.data(), outSize);
    }
    else
    {
        PreEncode_Save(in);

        outSize = cypher.get_output_length(in.size());
        out.resize(outSize);

        cypher.initialize(MH4U::KeyEXTData, sizeof(MH4U::KeyEXTData) -1);
        cypher.encode(in.data(), out.data(), outSize);
    }
}

bool PostDecode_Save(CContainer& data)
{
    u32 seed;
    u32 checksum, checksum2;

    seed = data.as<u32>(0) >> 16;
    data.subBefore(sizeof(u32)); // remove seed

    MHXOR(data, seed);

    checksum = data.as<u32>(0);
    data.subBefore(sizeof(u32)); // remove checksum

    checksum2 = Utils::CalculateChecksum(data);

    if (checksum == checksum2) return true;
    return false;
}

void MHXOR(CContainer& data, u32 seed)
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

void PreEncode_Save(CContainer& data)
{
    std::mt19937 randomEngine(0x484D); // 0x484D = "MH"
    u16 random; // must be u16
    u32 seed, seed_header;
    u32 checksum;

    random          = randomEngine();
    seed            = seed_header = random;
    seed_header     = (seed_header << 16) + 0x10;
    checksum        = Utils::CalculateChecksum(data);

    InsertValue(data, checksum);

    MHXOR(data, seed);

    InsertValue(data, seed_header);
}

void InsertValue(CContainer& data, u32 value)
{
    u8 i = 0;
    value = Utils::swap_endianness<u32>(value);
    while (i < 4){
        if (i != 0) {value >>= 8;}
        data.addBefore(1);
        data.as<u8>(0) = value;
        i++;
    }
}

const bool blowfish_decode(const CContainer &in, CContainer &out, const Key key)
{
    const char* selected_key = nullptr;
    u32 out_size;

    out_size = cypher_static.get_output_length(in.size());

    out.clear();
    out.resize(out_size);

    selected_key = MH4U::get_key(key);

    if (selected_key) {
        auto size = std::strlen(selected_key);

        cypher_static.initialize(selected_key, size);
        cypher_static.decode(in.data(), out.data(), out_size);

        return true;
    } else return false;
}

const bool blowfish_encode(const CContainer &in, CContainer &out, const Key key)
{
    const char* selected_key = nullptr;
    u32 out_size;

    out_size = cypher_static.get_output_length(in.size());

    out.clear();
    out.resize(out_size);

    selected_key = MH4U::get_key(key);

    if (selected_key) {
        auto size = std::strlen(selected_key);

        cypher_static.initialize(selected_key, size);
        cypher_static.encode(in.data(), out.data(), out_size);

        return true;
    } else return false;
}

const bool xor_decode(CContainer &cc)
{
    u32 seed;
    u32 checksum, checksum2;

    seed = cc.as<u32>(0) >> 16;
    cc.subBefore(sizeof(u32)); // remove seed

    MHXOR(cc, seed);

    checksum = cc.as<u32>(0);
    cc.subBefore(sizeof(u32)); // remove checksum

    checksum2 = Utils::CalculateChecksum(cc);

    if (checksum == checksum2) return true;
    return false;
}

void xor_encode(CContainer &cc)
{
    std::mt19937 randomEngine(0x484D); // 0x484D = "MH"
    u16 random; // must be u16
    u32 seed, seed_header;
    u32 checksum;

    random          = randomEngine();
    seed            = seed_header = random;
    seed_header     = (seed_header << 16) + 0x10;
    checksum        = Utils::CalculateChecksum(cc);

    InsertValue(cc, checksum);

    MHXOR(cc, seed);

    InsertValue(cc, seed_header);
}

const char *get_key(const Key key)
{
    switch (key) {
    case Key::EXT_DATA: return MH4U::KeyEXTData;
    case Key::DLC_EUR_NA: return MH4U::KeyDLC_EUR_NA;
    case Key::DLC_JPN: return MH4U::KeyDLC_JPN;
    case Key::DLC_KOR: return MH4U::KeyDLC_KOR;
    case Key::DLC_TW: return MH4U::KeyDLC_TW;
    default:
    case Key::ENUM_LENGTH: return nullptr;
    }
}

const bool decode_save(const CContainer &in, CContainer &out)
{
    bool res = true;

    res &= MH4U::blowfish_decode(in, out, MH4U::Key::EXT_DATA);
    res &= MH4U::xor_decode(out);

    return res;
}

const bool encode_save(const CContainer &in, CContainer &out)
{
    bool res = true;

    MH4U::xor_encode(out);
    res &= MH4U::blowfish_encode(in, out, MH4U::Key::EXT_DATA);

    return res;

}

} // MH4U
