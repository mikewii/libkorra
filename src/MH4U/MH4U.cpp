#include "MH4U/MH4U.hpp"

#include "Tools/Blowfish.hpp"
#include "Tools/Utils.hpp"
#include "Tools/Folder.hpp"
#include "Tools/File.hpp"

#include <random>

namespace MH4U {

bool    PostDecode_Save(CContainer& data);
void    PreEncode_Save(CContainer& data);
void    MHXOR(CContainer& data, u32 seed);
void    InsertValue(CContainer& data, u32 value);

static const u8 KeyEXTData[]    = "blowfish key iorajegqmrna4itjeangmb agmwgtobjteowhv9mope";
static const u8 KeyDLC[]         = "AgK2DYheaCjyHGPB";
static const u8 KeyDLC_TW[]      = "Capcom123 ";

void Decode(CContainer& in, CContainer& out,  bool isQuest)
{
    u32                 outSize;
    BlowFish            cypher;
    bool                good = true;

    outSize = cypher.GetOutputLength(in.size());

    out.resize(outSize);

    if (isQuest)
    {
        cypher.Initialize(MH4U::KeyDLC, sizeof(MH4U::KeyDLC) -1);
        cypher.Decode(in.data(), out.data(), outSize);

        //TODO: check for quest file header for error
    }
    else
    {
        cypher.Initialize(MH4U::KeyEXTData, sizeof(MH4U::KeyEXTData) -1);
        cypher.Decode(in.data(), out.data(), outSize);

        good = PostDecode_Save(out);

        if (!good) NotifyError("Save file corrupt!");
    }
}

void Encode(CContainer& in, CContainer& out, bool isQuest)
{
    u32                 outSize;
    BlowFish            cypher;

    if (isQuest)
    {
        outSize = cypher.GetOutputLength(in.size());
        out.resize(outSize - 4); // why -4? last 4 bytes is checksum?

        cypher.Initialize(MH4U::KeyDLC, sizeof(MH4U::KeyDLC) -1);
        cypher.Encode(in.data(), out.data(), outSize);
    }
    else
    {
        PreEncode_Save(in);

        outSize = cypher.GetOutputLength(in.size());
        out.resize(outSize);

        cypher.Initialize(MH4U::KeyEXTData, sizeof(MH4U::KeyEXTData) -1);
        cypher.Encode(in.data(), out.data(), outSize);
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

} // MH4U
