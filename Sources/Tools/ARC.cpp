#include "Tools/ARC.hpp"
#include <string.h>

ARC::ARC(CContainer& _data, std::vector<Pairs>* _list)
{
    header = reinterpret_cast<ARC_s*>(&_data.as_u8(0));
    __inList = _list;

    isARCFile();

    if (b.BE) FixBE_Header();

    PrintHeader();

    if(!b.isARC) return; // NotifyError("Not an ARC file!");

    Read(_data);
}

ARC::~ARC()
{
}

bool ARC::isARC(void) {
    return std::equal(ARC_MAGIC, ARC_MAGIC + 4, header->Magic);
}

bool ARC::isCRA(void) {
    return std::equal(CRA_MAGIC, CRA_MAGIC + 4, header->Magic);
}

void ARC::isARCFile(void)
{
    b.isARC |= b.LE = isARC();
    b.isARC |= b.BE = isCRA();
}

void ARC::PrintHeader(void)
{
    printf("Magic:          %s\n", header->Magic);
    printf("Version:        %d\n", header->Version);
    printf("Num of Files:   %d\n", header->FilesNum);
}

void ARC::PrintFileInfo(ARC_File_s* f, u32 n)
{
    printf("\n");
    printf("File #%d\n", n);
    printf("Filepath:           %s\n",   f->Filename);
    printf("Resource hash:      0x%X\n", f->ResourceHash);
    printf("Compressed size:    0x%X\n", f->CompressedSize);
    printf("Decompressed size:  0x%X\n", f->DecompressedSize);
    printf("Pointer to zdata:   0x%X\n", f->pZData);
}

void ARC::PushFile(CContainer& _data, u32 n)
{
    Pairs pair;
    u32 padding = 0;
    ARC_File_s* file;

    padding = _data.as_u32(2) == 0 ? 4 : 0;

    file = reinterpret_cast<ARC_File_s*>
            ( &_data.as_u8(0) + sizeof(ARC_s) + padding + (sizeof(ARC_File_s) * n) );

    if (b.BE) FixBE_ARC_File_s(file);

    file->DecompressedSize &= ~(1 << 30); // i.m. no file be this big anyway
    file->DecompressedSize &= ~(1 << 29);

    // copy and set things
    for (u32 i = 0; i < ARC_File_s::FNAME_SIZE; i++) pair.Filename[i] = file->Filename[i];
    pair.f = file;
    pair.ResourceHash = file->ResourceHash;
    pair.FixPath();


    __inList->push_back(pair);
}

void ARC::Read(CContainer& _data)
{
    for(int i = 0; i < header->FilesNum; i++)
    {
        PushFile(_data, i);
    }
}

void ARC::ExtractAll(void)
{
    for(int i = 0; i < header->FilesNum; i++)
    {
        Decompress(i);
    }
}

void ARC::Decompress(u32 n)
{
    Pairs& p = __inList->at(n);
    Bytef* source = nullptr;
    uLongf decSize = 0;

    decSize = p.f->DecompressedSize;
    p.cc.resize(decSize);

    source = reinterpret_cast<Bytef*>( reinterpret_cast<u64>(&header[0]) + p.f->pZData );

    if ( uncompress(p.cc.data(), &decSize, source, p.f->CompressedSize) == Z_OK )
    {
        p.DecSize = decSize;
        p.decompressed = true;
    }
}

int ARC::Decompress(Pairs& sourcePair, Pairs& destPair)
{
    CContainer temp;
    Bytef* pTemp = nullptr;
    Bytef* pSrc = nullptr;
    uLongf decSize = 0;
    int err = Z_OK;

    /* prepare temp bufer */
    decSize = sourcePair.DecSize;
    temp.resize(decSize);
    pTemp = reinterpret_cast<Bytef*>( temp.data() );

    /* prepare source */
    pSrc = reinterpret_cast<Bytef*>( sourcePair.cc.data() );

    err = uncompress(pTemp, &decSize, pSrc, sourcePair.cc.size());
    if ( err == Z_OK )
    {
        destPair.DecSize = decSize;
        destPair.decompressed = true;
        destPair.cc.resize(decSize);
#ifdef __linux__
        memcpy(destPair.cc.data(), temp.data(), decSize);
#else
        // for 3ds
        memmove(destPair.cc.data(), temp.data(), decSize);
#endif

        return err;
    }

    return err;

}

int ARC::Compress(Pairs& sourcePair, Pairs& destPair)
{
    CContainer temp;
    Bytef* pTemp = nullptr;
    Bytef* pSrc = nullptr;
    uLongf compSize = 0;
    int err = Z_OK;

    /* prepare temp buffer */
    compSize = compressBound(sourcePair.cc.size());
    temp.resize(compSize);
    pTemp = reinterpret_cast<Bytef*>( temp.data() );

    /* prepare source */
    pSrc = reinterpret_cast<Bytef*>( sourcePair.cc.data() );

    err = compress(pTemp, &compSize, pSrc, sourcePair.cc.size());
    if ( err == Z_OK )
    {
        destPair.DecSize = sourcePair.DecSize;
        destPair.cc.resize(compSize);
#ifdef __linux__
        memcpy(destPair.cc.data(), temp.data(), compSize);
#else
        // for 3ds
        memmove(destPair.cc.data(), temp.data(), compSize);
#endif

        return err;
    }

    return err;
}

void ARC::Pairs::FixPath(void)
{
    for (auto& c : this->Filename)
        if (c == '\\') c = '/';
}

void ARC::FixBE_Header(void)
{
    swap_endian<u16>(header->Version);
    swap_endian<u16>(header->FilesNum);
}

void ARC::FixBE_ARC_File_s(ARC_File_s* f)
{
    swap_endian<u32>(f->ResourceHash);
    swap_endian<u32>(f->CompressedSize);
    swap_endian<u32>(f->DecompressedSize);
    swap_endian<u32>(f->pZData);
}

void ARC::GetPWD(void)
{

}

void ARC::SetPath(std::string _path)
{

}
