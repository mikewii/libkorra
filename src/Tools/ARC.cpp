#include "Tools/ARC.hpp"
#include "Tools/Utils.hpp"

ARC::Version ARC::prev_Version = ARC::Version::None;

ARC::ARC(const CContainer& container, std::vector<Pair>& vOut)
    : __vPair(&vOut)
    , __container(&container)

{
    if (container.size() == 0) {
        NotifyError("ARC: CContainer is empty!");
        return;
    }

    ARC::header = container.as_const_ref<ARC::Header>(0);
    ARC::isARC |= ARC::isLE = std::equal(ARC_MAGIC, ARC_MAGIC + 4, ARC::header.magic);
    ARC::isARC |= ARC::isBE = std::equal(CRA_MAGIC, CRA_MAGIC + 4, ARC::header.magic);

    if (ARC::isARC)
    {
        if (ARC::isBE) ARC::header.BE_Swap();

        ARC::prev_Version   = ARC::header.version;

        ARC::Read(container);
    }
}

ARC::~ARC()
{
}

bool ARC::Is_ARC(void) const { return ARC::isARC; }

void ARC::print_Header(void)
{
    printf("Magic:          %s\n", ARC::header.magic);
    printf("Version:        %d\n", ARC::header.version);
    printf("Num of Files:   %d\n", ARC::header.fileNum);
}

void ARC::print_FileInfo(ARC::File_Header* f, u32 n)
{
    printf("\n");
    printf("File #%d\n", n);
    printf("Filepath:           %s\n",   f->fileName);
    printf("Resource hash:      0x%X\n", f->resourceHash);
    printf("Compressed size:    0x%X\n", f->compressedSize);
    printf("Decompressed size:  0x%X\n", f->decompressedSize);
    printf("Pointer to zdata:   0x%X\n", f->pZData);
}

void ARC::print_PairsInfo(void)
{
    for (const auto& p : *__vPair)
        p.info.print();
}

void ARC::Read(const CContainer& container)
{
    auto padding = 0;

    if (ARC::trust)
        padding = ARC::Is_NeedPadding(ARC::header.version);
    else padding = container.as_const_ref<u32>(2) == 0 ? 4 : 0;

    const auto& file_Header = container.at_const_ref<ARC::File_Header[]>(sizeof(ARC::Header) + padding);

    ARC::__vPair->resize(ARC::header.fileNum);

    for (auto i = 0; i < ARC::header.fileNum; i++)
    {
        auto xorLock = ARC::extract_XORLock(file_Header[i].decompressedSize);
        auto file_header_normalized = file_Header[i];


        if (ARC::isBE) file_header_normalized.BE_Swap();

        file_header_normalized.decompressedSize &= ~xorLock;

        ARC::vFile_Header.push_back(file_header_normalized);

        // pair:
        auto& pair = ARC::__vPair->at(i);
        Utils::copybytes(pair.info.Filename, &file_header_normalized.fileName, FNAME_SIZE);
        pair.info.ResourceHash  = file_header_normalized.resourceHash;
        pair.info.XORLock       = xorLock;
    }
}

void ARC::ExtractAll(void)
{
    for (int i = 0; i < ARC::header.fileNum; i++)
        Decompress(i);
}

void ARC::Decompress(const u32 id)
{
    const ARC::File_Header& file_header = ARC::vFile_Header.at(id);
    const Bytef*            source = nullptr;
    Pair&                   pair = __vPair->at(id);
    uLongf                  decSize = 0;

    decSize = file_header.decompressedSize;
    pair.cc.resize(decSize);

    source = reinterpret_cast<Bytef*>(reinterpret_cast<u64>(ARC::__container->data()) + file_header.pZData);

    if (uncompress(pair.cc.data(), &decSize, source, file_header.compressedSize) == Z_OK)
    {
        pair.info.DecSize           = decSize;
        pair.info.isDecompressed    = true;
    }
}

int ARC::Decompress(Pair& sourcePair, Pair& destPair)
{
    CContainer      temp;
    Bytef*          pTemp = nullptr;
    Bytef*          pSrc = nullptr;
    uLongf          decSize = 0;
    int             err;

    /* prepare temp bufer */
    decSize     = sourcePair.info.DecSize;
    temp.resize(decSize);
    pTemp       = reinterpret_cast<Bytef*>( temp.data() );

    /* prepare source */
    pSrc = reinterpret_cast<Bytef*>( sourcePair.cc.data() );

    err = uncompress(pTemp, &decSize, pSrc, sourcePair.cc.size());
    if (err == Z_OK)
    {
        destPair.info.DecSize           = decSize;
        destPair.info.isDecompressed    = true;
        destPair.cc.resize(decSize);
        Utils::copybytes(destPair.cc.data(), temp.data(), decSize);
    }

    return err;

}

int ARC::Compress(const Pair& sourcePair, Pair& destPair)
{
    CContainer      temp;
    Bytef*          pTemp = nullptr;
    const Bytef*    pSrc = nullptr;
    uLongf          compSize = 0;
    int             err;

    /* prepare temp buffer */
    compSize = compressBound(sourcePair.cc.size());
    temp.resize(compSize);
    pTemp = reinterpret_cast<Bytef*>( temp.data() );

    /* prepare source */
    pSrc = reinterpret_cast<Bytef*>( sourcePair.cc.data() );

    err = compress(pTemp, &compSize, pSrc, sourcePair.cc.size());
    if (err == Z_OK)
    {
        destPair.info.DecSize        = sourcePair.info.DecSize;
        destPair.info.XORLock        = sourcePair.info.XORLock;
        destPair.info.ResourceHash   = sourcePair.info.ResourceHash;

        destPair.cc.resize(compSize);
        Utils::copybytes(destPair.cc.data(), temp.data(), compSize);
        Utils::copybytes(destPair.info.Filename, sourcePair.info.Filename, FNAME_SIZE);
    }

    return err;
}

void ARC::MakeARC(CContainer& container, std::vector<Pair>& vPair, ARC::Version version)
{
    ARC::Header         header;
    std::vector<Pair>   listAfter;
    u32                 finalSize;
    u64                 zDataStart;
    u32                 padding;

    /* Making header */
    /**/    header.fileNum = vPair.size();
    /**/    if (version != ARC::Version::None)
    /**/    {
    /**/        header.version = version;
    /**/        padding = Is_NeedPadding(version);
    /**/    }
    /**/    else
    /**/    {
    /**/        header.version = ARC::prev_Version;
    /**/        padding = Is_NeedPadding(ARC::prev_Version);
    /**/    }
    /**/
    /**/    Utils::copybytes(header.magic, ARC_MAGIC, sizeof(header.magic)); // TODO: BE LE depending on version or arg
    /**/


    /* Compress all data */
    /**/    listAfter.resize(vPair.size());
    /**/    for (u32 i = 0; i < vPair.size(); i++)
    /**/        Compress(vPair.at(i), listAfter.at(i));


    /* Calculate final size */
    /**/    finalSize = sizeof(ARC::Header) + padding + (sizeof(ARC::File_Header) * listAfter.size());
    /**/    finalSize = Align(finalSize);
    /**/    zDataStart = finalSize += 16; // padding, maybe decided by version
    /**/    for (const auto& pp : listAfter)
    /**/        finalSize += pp.cc.size();


    /* Copy data to ARC file */
    /**/    container.resize(finalSize, true);
    /**/
    /**/    /* HEADER */
    /**/    Utils::copybytes(container.data(), &header, sizeof(header));
    /**/
    /**/    /* ARC FILES LIST */
    /**/    MakeARC_File_s_Header(container, listAfter, padding, zDataStart);
    /**/
    /**/    /* ZDATA */
    /**/    CopyZData(container, listAfter, zDataStart);
}

void ARC::CopyZData(CContainer& _cc, std::vector<Pair>& _list, u32 _zDataStart)
{
    u32     shift = 0;
    u8*     to = _cc.data() + _zDataStart;

    for (u32 i = 0; i < _list.size(); i++)
    {
        Pair&   pp = _list.at(i);
        u32     ammount;
        u8*     from;

        to          += shift;
        from        = pp.cc.data();
        ammount     = pp.cc.size();

        Utils::copybytes(to, from, ammount);

        shift = ammount;
    }
}

void ARC::MakeARC_File_s_Header(
        CContainer& _cc,
        std::vector<Pair>& _list,
        u32 _padding,
        u32 _zDataStart)
{
    ARC::File_Header*   arc;
    u32                 pZData;

    arc     = reinterpret_cast<ARC::File_Header*>( _cc.data() + sizeof(ARC::Header) + _padding );
    pZData  = _zDataStart;

    for (u32 i = 0; i < _list.size(); i++)
    {
        Pair& pair = _list.at(i);

        Utils::copybytes(&arc[i], pair.info.Filename, FNAME_SIZE);
        arc[i].decompressedSize = pair.info.DecSize ^ pair.info.XORLock;
        arc[i].compressedSize   = pair.cc.size();
        arc[i].resourceHash     = pair.info.ResourceHash;
        arc[i].pZData           = pZData;

        pZData  += pair.cc.size();
    }
}

u32 ARC::Is_NeedPadding(const ARC::Version version)
{
    switch(version){
    case ARC::Version::LP1: return 0;
    case ARC::Version::LP2: return 0;
    case ARC::Version::MH3U_3DS: return 4;
    case ARC::Version::MH4U_MHXX: return 4;
    case ARC::Version::MH4U_1: return 4;
    default:return 0;
    }

    return 0;
}

u32 ARC::extract_XORLock(const u32 decSize)
{
    return decSize & 0xF0000000; // lock will be first 4 bits
}

u32 ARC::Align(u32 value)
{
    constexpr const u32 align = sizeof(u64) * 2; // file alignment

    while (value % align != 0)
        value += 4;

    return value;
}
