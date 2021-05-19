#include "Tools/ARC.hpp"
#include "Tools/Utils.hpp"

u32 ARC::__previousVersion = 0;

ARC::ARC(CContainer& _arcdata, std::vector<Pair>* _outlist)
{
    if ( _arcdata.size() == 0) {
        NotifyError("ARC: CContainer is empty!");
        return;
    }

    __header = reinterpret_cast<ARC_s*>(&_arcdata.as_u8(0));
    __list = _outlist;

    isARCFile();

    if (b.BE) FixBE_Header();

    ARC::__previousVersion = __header->Version;

    if(!b.isARC) return; // NotifyError("Not an ARC file!");

    Read(_arcdata);
}

ARC::~ARC()
{
}

bool ARC::isARC(void) {
    return std::equal(ARC_MAGIC, ARC_MAGIC + 4, __header->Magic);
}

bool ARC::isCRA(void) {
    return std::equal(CRA_MAGIC, CRA_MAGIC + 4, __header->Magic);
}

void ARC::isARCFile(void)
{
    b.isARC |= b.LE = isARC();
    b.isARC |= b.BE = isCRA();
}

u32     ARC::isNeedPadding(ARCVersion _version){
    return isNeedPadding(GetVersionValue(_version));
}

u32     ARC::isNeedPadding(u32 _version)
{
    switch(_version){

    case 17 : return 4; // MH4U MHXX
    case 19 : return 4; // MH4U
    case 7  : return 0; // LP1
    case 8  : return 0; // LP2

    default:return 0;
    }

    return 0;
}

u32     ARC::GetVersionValue(ARCVersion _version)
{
    switch(_version){

    case ARCVersion::MHXX:
    case ARCVersion::MH4U: return 17;
    case ARCVersion::LP1: return 7;
    case ARCVersion::LP2: return 8;

    default:return 0;
    }

    return 0;
}

u32     ARC::extractXORLock(u32 _decSize) {
    return _decSize & 0xF0000000; // lock be first 4 bits
}

void    ARC::PrintHeader(void)
{
    printf("Magic:          %s\n", __header->Magic);
    printf("Version:        %d\n", __header->Version);
    printf("Num of Files:   %d\n", __header->FilesNum);
}

void    ARC::PrintFileInfo(ARC_File_s* f, u32 n)
{
    printf("\n");
    printf("File #%d\n", n);
    printf("Filepath:           %s\n",   f->Filename);
    printf("Resource hash:      0x%X\n", f->ResourceHash);
    printf("Compressed size:    0x%X\n", f->CompressedSize);
    printf("Decompressed size:  0x%X\n", f->DecompressedSize);
    printf("Pointer to zdata:   0x%X\n", f->pZData);
}

void    ARC::PrintPairsInfo(void)
{
    u32 i = 0;
    for ( auto& p : *__list)
    {
        printf("\n");
        printf("##### Pair #%d #####\n", i++);
        printf("File Path:          %s\n", p.Filename);
        printf("XOR Lock:           0x%08X\n", p.XORLock);
        printf("Resource Hash:      0x%08X\n", p.ResourceHash);
        printf("Decompressed size:  %d\n", p.DecSize);
        printf("CContainer size:    %d\n", p.cc.size());
    }
}

void ARC::PushFile(CContainer& _data, u32 n)
{
    Pair       pair;
    u32         padding = 0;
    ARC_File_s* file;
    u32         xorLock;

    padding = _data.as_u32(2) == 0 ? 4 : 0;

    file = reinterpret_cast<ARC_File_s*>
            ( &_data.as_u8(0) + sizeof(ARC_s) + padding + (sizeof(ARC_File_s) * n) );

    if (b.BE) FixBE_ARC_File_s(file);


    xorLock = extractXORLock(file->DecompressedSize);
    file->DecompressedSize &= ~xorLock;

    // copy and set things
    Utils::copybytes(pair.Filename, file->Filename, ARC_File_s::FNAME_SIZE);
    pair.ResourceHash   = file->ResourceHash;
    pair.XORLock        = xorLock;
    pair.f              = file;


    __list->push_back(pair);
}

void ARC::Read(CContainer& _data)
{
    for(int i = 0; i < __header->FilesNum; i++)
        PushFile(_data, i);
}

void ARC::ExtractAll(void)
{
    if ( this->__header )
        for(int i = 0; i < __header->FilesNum; i++)
            Decompress(i);
}

void ARC::Decompress(u32 n)
{
    Pair&      p = __list->at(n);
    Bytef*      source = nullptr;
    uLongf      decSize = 0;

    decSize = p.f->DecompressedSize;
    p.cc.resize(decSize);

    source = reinterpret_cast<Bytef*>( reinterpret_cast<u64>(&__header[0]) + p.f->pZData );

    if ( uncompress(p.cc.data(), &decSize, source, p.f->CompressedSize) == Z_OK )
    {
        p.DecSize = decSize;
        p.isDecompressed = true;
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
    decSize = sourcePair.DecSize;
    temp.resize(decSize);
    pTemp = reinterpret_cast<Bytef*>( temp.data() );

    /* prepare source */
    pSrc = reinterpret_cast<Bytef*>( sourcePair.cc.data() );

    err = uncompress(pTemp, &decSize, pSrc, sourcePair.cc.size());
    if ( err == Z_OK )
    {
        destPair.DecSize = decSize;
        destPair.isDecompressed = true;
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
    if ( err == Z_OK )
    {
        destPair.DecSize        = sourcePair.DecSize;
        destPair.XORLock        = sourcePair.XORLock;
        destPair.ResourceHash   = sourcePair.ResourceHash;

        destPair.cc.resize(compSize);
        Utils::copybytes(destPair.cc.data(), temp.data(), compSize);
        Utils::copybytes(destPair.Filename, sourcePair.Filename, ARC_File_s::FNAME_SIZE);
    }

    return err;
}

void ARC::FixBE_Header(void)
{
    Utils::swap_endian<u16>(__header->Version);
    Utils::swap_endian<u16>(__header->FilesNum);
}

void ARC::FixBE_ARC_File_s(ARC_File_s* f)
{
    Utils::swap_endian<u32>(f->ResourceHash);
    Utils::swap_endian<u32>(f->CompressedSize);
    Utils::swap_endian<u32>(f->DecompressedSize);
    Utils::swap_endian<u32>(f->pZData);
}

void ARC::MakeARC(CContainer& _output, std::vector<Pair>* _list, ARCVersion _version)
{
    ARC_s               header;
    std::vector<Pair>  listAfter;
    u32                 finalSize;
    u64                 zDataStart;
    u32                 padding;

    /* Making header */
    /**/    header.FilesNum = _list->size();
    /**/    if ( _version != ARCVersion::None )
    /**/    {
    /**/        header.Version = GetVersionValue(_version);
    /**/        padding = isNeedPadding(_version);
    /**/    }
    /**/    else
    /**/    {
    /**/        header.Version = ARC::__previousVersion;
    /**/        padding = isNeedPadding(ARC::__previousVersion);
    /**/    }
    /**/
    /**/    Utils::copybytes(header.Magic, ARC_MAGIC, sizeof(header.Magic)); // TODO: BE LE depending on version or arg
    /**/



    /* Compress all data */
    /**/    listAfter.resize(_list->size());
    /**/    for ( u32 i = 0; i < _list->size(); i++ )
    /**/        Compress(_list->at(i), listAfter.at(i));


    /* Calculate final size */
    /**/    finalSize = sizeof(ARC_s) + padding + (sizeof(ARC_File_s) * listAfter.size());
    /**/    finalSize = Align(finalSize);
    /**/    zDataStart = finalSize += 16; // padding, maybe decided by version
    /**/    for( auto& pp : listAfter)
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

    while ( _value % align != 0 )
        _value += 4;

    return _value;
}

void ARC::CopyZData(CContainer& _cc, std::vector<Pair>& _list, u32 _zDataStart)
{
    u32     shift = 0;
    u8*     to = _cc.data() + _zDataStart;

    for ( u32 i = 0; i < _list.size(); i++ )
    {
        Pair&  pp = _list.at(i);
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
    ARC_File_s* arc;
    u32         pZData;

    arc     = reinterpret_cast<ARC_File_s*>( _cc.data() + sizeof(ARC_s) + _padding );
    pZData  = _zDataStart;

    for ( u32 i = 0; i < _list.size(); i++ )
    {
        Pair& pair = _list.at(i);

        Utils::copybytes(&arc[i], pair.Filename, ARC_File_s::FNAME_SIZE);
        arc[i].DecompressedSize = pair.DecSize ^ pair.XORLock;
        arc[i].CompressedSize   = pair.cc.size();
        arc[i].ResourceHash     = pair.ResourceHash;
        arc[i].pZData           = pZData;

        pZData  += pair.cc.size();

    }
}

