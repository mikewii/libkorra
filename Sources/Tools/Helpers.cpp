#include "Tools/Helpers.hpp"

#include "Tools/Blowfish.hpp"
#include "MH4U/Keys.hpp"

#include <fstream>
#include <iterator>
#include <random>

namespace MH4U {

std::vector<u8> Decode(std::vector<u8> in, bool isQuest)
{
    std::vector<u8>     out;
    u32                 outSize;
    BlowFish            cypher;

    outSize     = cypher.GetOutputLength(in.size());

    out.resize(outSize);

    if (isQuest)
    {
        cypher.Initialize(MH4U::KeyDLC, sizeof(MH4U::KeyDLC) -1);
        cypher.Decode(in.data(), out.data(), outSize);

        return out;
    }
    else
    {
        cypher.Initialize(MH4U::KeySaveFile, sizeof(MH4U::KeySaveFile) -1);
        cypher.Decode(in.data(), out.data(), outSize);

        XORSaveData(out);

        return out;
    }
}

std::vector<u8> Encode(std::vector<u8> in, bool isQuest)
{
    std::vector<u8>     out;
    u32                 outSize;
    BlowFish            cypher;

    if (isQuest)
    {
        outSize     = cypher.GetOutputLength(in.size());
        out.resize(outSize - 4); // why -4? last 4 bytes is checksum?

        cypher.Initialize(MH4U::KeyDLC, sizeof(MH4U::KeyDLC) -1);
        cypher.Encode(in.data(), out.data(), outSize);

        return out;
    }
    else
    {
        PrepareSaveForEncode(in);

        outSize     = cypher.GetOutputLength(in.size());
        out.resize(outSize);

        cypher.Initialize(MH4U::KeySaveFile, sizeof(MH4U::KeySaveFile) -1);
        cypher.Encode(in.data(), out.data(), outSize);

        return out;
    }
}



void XORSaveData(std::vector<u8> &vec)
{
    u32 seed;
    u32 checksum, checksum2;

    seed = as_u32(vec.at(0)) >> 16;

    vec.erase(vec.begin(), vec.begin()+4); // i forgot what we erasing here

    MHXOR(vec, seed);

    checksum = as_u32(vec.at(0));

    vec.erase(vec.begin(), vec.begin()+4); // remove checksum

    checksum2 = CalculateChecksum(vec);

    // TODO
    if (checksum == checksum2) return; // we are good
    else return; // we are not good

}

void MHXOR(std::vector<u8> &vec, u32 seed)
{
    u32 i = 0;

    while(i < vec.size())
    {
        if (seed == 0){seed = 1;}
        seed = seed * 0xB0 % 0xFF53;
        as_u16(vec.at(i)) ^= seed;
        i+=2;
    };
}

u32 CalculateChecksum(std::vector<u8> &vec)
{
    u32 i = 0, checksum = 0;

    while (i < vec.size()) {
        checksum += vec[i] & 0xFF;
        i++;
    };

    return checksum;
}

void PrepareSaveForEncode(std::vector<u8> &vec)
{
    std::mt19937 randomEngine(0x484D); // 0x484D = "MH"
    u16 random; // must be u16
    u32 seed, seed_header;
    u32 checksum;

    random          = randomEngine();
    seed            = seed_header = random;
    seed_header     = (seed_header << 16) + 0x10;
    checksum        = CalculateChecksum(vec);

    InsertData(vec, checksum);

    MHXOR(vec, seed);

    InsertData(vec, seed_header);
}

void InsertData(std::vector<u8> &vec, u32 data)
{
    u8 i = 0;
    data = swap_endian<u32>(data);
    while (i < 4){
        if (i != 0) {data >>= 8;}
        vec.insert(vec.begin(), data);
        i++;
    };
}


} // MH4U




std::vector<u8> FileToVector(std::string filename)
{
//    FILE *finput;
//    finput = fopen("", "rb");
//    fseek(finput, 0, SEEK_END);
//    u32 finputSize = ftell(finput);
//    rewind(finput);

//    std::vector<char> inputBuffer(finputSize);
//    fread(inputBuffer.data(), finputSize, 1, finput);
//    fclose(finput);

    // cpp way:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<u8> vec;
    vec.reserve(fileSize);
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<u8>(file),
               std::istream_iterator<u8>());

    return vec;
}
