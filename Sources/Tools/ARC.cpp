#include "Tools/ARC.hpp"

ARC::ARC(CContainer& _data)
{
    header = reinterpret_cast<ARC_s*>(&_data.as_u8(0));

    isARCFile();

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
    ARC_File_s* file = reinterpret_cast<ARC_File_s*>
            ( &_data.as_u8(0) + sizeof(ARC_s) + (sizeof(ARC_File_s) * n) );

    file->DecompressedSize &= ~(1 << 30); // i.m. no file be this big anyway

    pair.f = file;
    files.push_back(pair);

    PrintFileInfo(file, n);
}

void ARC::Read(CContainer& _data)
{
    for(int i = 0; i < header->FilesNum; i++)
    {
        PushFile(_data, i);
    }
}

void ARC::Extract(u32 n, bool writeToFile)
{
    Pairs& p = files.at(n);
    Bytef* data = nullptr;
    uLongf decSize = 0;

    decSize = p.f->DecompressedSize;
    p.cc.resize(decSize);

    data = reinterpret_cast<Bytef*>( reinterpret_cast<u64>(&header[0]) + p.f->pZData );

    uncompress(p.cc.data(), &decSize, data, p.f->CompressedSize);

    p.FixPath();

    if ( p.f->DecompressedSize == decSize && writeToFile)
    {
        std::string path = "/run/media/mw/data2/test";
        std::string ext = ".test";
        path += '/' + this->filename + ext + '/' + p.f->Filename;

        p.cc.writeToFile(path.c_str(), true);
    }
}

void ARC::Pairs::FixPath(void)
{
    for (auto& c : this->f->Filename)
        if (c == '\\') c = '/';
}

void ARC::GetPWD(void)
{

}

void ARC::SetPath(std::string _path)
{

}
