#include "Tools/ARC.hpp"
#include "Tools/Utils.hpp"

ARC::Version ARC::__previousVersion = ARC::Version::None;

ARC::ARC(CContainer& _arcdata, std::vector<Pair>* _outlist)
{
    if (_arcdata.size() == 0) {
        NotifyError("ARC: CContainer is empty!");
        return;
    }

    __header    = reinterpret_cast<ARC::Header*>(&_arcdata.as<u8>(0));
    __list      = _outlist;

    isARCFile();

    if (b.BE) FixBE_Header();

    ARC::__previousVersion = __header->version;

    if (!b.isARC)
    {
        ARC::_isARC = false;
        return;
    }

    __list->resize(__header->fileNum);
    __listARC_File_s.resize(__header->fileNum, nullptr);

    Read(_arcdata);
}

ARC::~ARC()
{
}

bool ARC::isARC(void) const { return ARC::_isARC; }

bool ARC::__isARC(void) { return std::equal(ARC_MAGIC, ARC_MAGIC + 4, __header->magic); }

bool ARC::isCRA(void) { return std::equal(CRA_MAGIC, CRA_MAGIC + 4, __header->magic); }
void ARC::isARCFile(void)
{
    b.isARC |= b.LE = __isARC();
    b.isARC |= b.BE = isCRA();
}

u32 ARC::isNeedPadding(ARC::Version _version)
{
    switch(_version){
    case ARC::Version::LP1: return 0;
    case ARC::Version::LP2: return 0;
    case ARC::Version::MH3U_3DS: return 4;
    case ARC::Version::MH4U_MHXX: return 4;
    case ARC::Version::MH4U_1: return 4;
    default:return 0;
    }

    return 0;
}

u32 ARC::extractXORLock(u32 _decSize) {
    return _decSize & 0xF0000000; // lock be first 4 bits
}

void    ARC::PrintHeader(void)
{
    printf("Magic:          %s\n", __header->magic);
    printf("Version:        %d\n", __header->version);
    printf("Num of Files:   %d\n", __header->fileNum);
}

void    ARC::PrintFileInfo(ARC::File_Header* f, u32 n)
{
    printf("\n");
    printf("File #%d\n", n);
    printf("Filepath:           %s\n",   f->fileName);
    printf("Resource hash:      0x%X\n", f->resourceHash);
    printf("Compressed size:    0x%X\n", f->compressedSize);
    printf("Decompressed size:  0x%X\n", f->decompressedSize);
    printf("Pointer to zdata:   0x%X\n", f->pZData);
}

void    ARC::PrintPairsInfo(void)
{
    for (auto& p : *__list)
        p.print();
}

void ARC::PushFile(CContainer& _data, u32 n)
{
    Pair&               pair = this->__list->at(n);
    u32                 padding = 0;
    ARC::File_Header*   file = nullptr;
    u32                 xorLock;

    padding = _data.as<u32>(2) == 0 ? 4 : 0;

    file = reinterpret_cast<ARC::File_Header*>
            ( &_data.as<u8>(0) + sizeof(ARC::Header) + padding + (sizeof(ARC::File_Header) * n) );

    if (b.BE) FixBE_ARC_File_s(file);


    xorLock = extractXORLock(file->decompressedSize);
    file->decompressedSize &= ~xorLock;

    // copy and set things
    Utils::copybytes(pair.info.Filename, file->fileName, FNAME_SIZE);
    pair.info.ResourceHash  = file->resourceHash;
    pair.info.XORLock       = xorLock;


    this->__listARC_File_s.at(n) = file;
}

void ARC::Read(CContainer& _data)
{
    for (int i = 0; i < __header->fileNum; i++)
        PushFile(_data, i);
}

void ARC::ExtractAll(void)
{
    if (this->__header)
        for (int i = 0; i < __header->fileNum; i++)
            Decompress(i);
}

void ARC::Decompress(u32 n)
{
    Pair&       p = __list->at(n);
    ARC::File_Header* f = __listARC_File_s.at(n);
    Bytef*      source = nullptr;
    uLongf      decSize = 0;

    decSize = f->decompressedSize;
    p.cc.resize(decSize);

    source = reinterpret_cast<Bytef*>(reinterpret_cast<u64>(&__header[0]) + f->pZData);

    if (uncompress(p.cc.data(), &decSize, source, f->compressedSize) == Z_OK)
    {
        p.info.DecSize = decSize;
        p.info.isDecompressed = true;
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

int ARC::Compress(Pair& sourcePair, Pair& destPair)
{
    CContainer      temp;
    Bytef*          pTemp = nullptr;
    Bytef*          pSrc = nullptr;
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

void ARC::FixBE_Header(void)
{
    Utils::swap_endianness<u16>(__header->version);
    Utils::swap_endianness<u16>(__header->fileNum);
}

void ARC::FixBE_ARC_File_s(ARC::File_Header* f)
{
    Utils::swap_endianness<u32>(f->resourceHash);
    Utils::swap_endianness<u32>(f->compressedSize);
    Utils::swap_endianness<u32>(f->decompressedSize);
    Utils::swap_endianness<u32>(f->pZData);
}

void ARC::MakeARC(CContainer& _output, std::vector<Pair>* _list, ARC::Version _version)
{
    ARC::Header         header;
    std::vector<Pair>   listAfter;
    u32                 finalSize;
    u64                 zDataStart;
    u32                 padding;

    /* Making header */
    /**/    header.fileNum = _list->size();
    /**/    if (_version != ARC::Version::None)
    /**/    {
    /**/        header.version = _version;
    /**/        padding = isNeedPadding(_version);
    /**/    }
    /**/    else
    /**/    {
    /**/        header.version = ARC::__previousVersion;
    /**/        padding = isNeedPadding(ARC::__previousVersion);
    /**/    }
    /**/
    /**/    Utils::copybytes(header.magic, ARC_MAGIC, sizeof(header.magic)); // TODO: BE LE depending on version or arg
    /**/


    /* Compress all data */
    /**/    listAfter.resize(_list->size());
    /**/    for (u32 i = 0; i < _list->size(); i++)
    /**/        Compress(_list->at(i), listAfter.at(i));


    /* Calculate final size */
    /**/    finalSize = sizeof(ARC::Header) + padding + (sizeof(ARC::File_Header) * listAfter.size());
    /**/    finalSize = Align(finalSize);
    /**/    zDataStart = finalSize += 16; // padding, maybe decided by version
    /**/    for (const auto& pp : listAfter)
    /**/        finalSize += pp.cc.size();


    /* Copy data to ARC file */
    /**/    _output.resize(finalSize, true);
    /**/
    /**/    /* HEADER */
    /**/    Utils::copybytes(_output.data(), &header, sizeof(header));
    /**/
    /**/    /* ARC FILES LIST */
    /**/    MakeARC_File_s_Header(_output, listAfter, padding, zDataStart);
    /**/
    /**/    /* ZDATA */
    /**/    CopyZData(_output, listAfter, zDataStart);
}


u32 ARC::Align(u32 _value)
{
    constexpr const u32 align = sizeof(u64) * 2; // file alignment

    while (_value % align != 0)
        _value += 4;

    return _value;
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

