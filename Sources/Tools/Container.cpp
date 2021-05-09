#include "Tools/Container.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

CContainer::CContainer(const char* fname)
{
    this->readFromFile(fname);
}

bool CContainer::allocate(u32 _size)
{
    if ( this->__root != nullptr) return false;

    u32 extra   = this->RESERVED_Before + this->RESERVED_After;

    this->__size   = _size;

    this->__root          = static_cast<u8*>( malloc(sizeof(u8) * _size + extra ));
    this->__data          = this->__root + this->RESERVED_Before;

    return true;
}

bool CContainer::addBefore(u32 _size)
{
    if (_size <= this->RESERVED_Before && this->RESERVED_Before != 0)
    {
        this->RESERVED_Before -= _size;
        this->__data -= _size;

        this->__size += _size;

        return true;
    }

    return false;
}

void CContainer::addAfter(u32 _size)
{
    if(_size <= this->RESERVED_After && this->RESERVED_After != 0)
    {
        this->RESERVED_After -= _size;
        this->__size += _size;
    }
    else
    {
        this->__root = static_cast<u8*>( realloc(this->__root, this->__size += _size) );
        this->__data = this->__root + this->RESERVED_Before;
    }
}

bool CContainer::subBefore(u32 _size)
{
    if (_size < this->__size)
    {
        this->RESERVED_Before += _size;
        this->__data += _size;
        this->__size -= _size;

        return true;
    }

    return false;
}

bool CContainer::subAfter(u32 _size)
{
    if (_size < this->__size)
    {
        this->RESERVED_After += _size;
        this->__size -= _size;

        return true;
    }

    return false;
}

void CContainer::resize(u32 _size)
{
    if (this->__root == nullptr) this->allocate(_size);
    else this->addAfter(_size);
}

u8&     CContainer::as_u8(u32 n){
    return static_cast<u8&>(this->__data[n]);
}

u16&    CContainer::as_u16(u32 n){
    return reinterpret_cast<u16&>(this->__data[n * sizeof(u16)]);
}

u32&    CContainer::as_u32(u32 n){
    return reinterpret_cast<u32&>(this->__data[n * sizeof(u32)]);
}

bool CContainer::readFromFile(const char* fname)
{
    FILE* f = nullptr;
    u32 fsize = 0;
    u32 rsize = 0;

    if ( access(fname, F_OK | R_OK ) == 0 )
    {
        this->flags.isFile = true;
        f = fopen(fname, "rb");

        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);

        this->allocate(fsize);

        rsize = fread(this->__data, sizeof(u8), fsize, f);
        fclose(f);
    }

    if (rsize > 0) return true;
    return false;
}

bool CContainer::writeToFile(const char* fname, bool makedir)
{
    FILE* f = nullptr;
    u32 wsize = 0;

    if (makedir)
    {
        std::string path = fname;
        path.resize(path.find_last_of("/"));
        this->makedir(path.c_str());
    }

    f = fopen(fname, "wb");

    if (f)
    {
        wsize = fwrite(this->data(), sizeof(u8), this->size(), f);
        fclose(f);
    }

    if (wsize > 0) return true;
    return false;
}

bool CContainer::makedir(const char* dir)
{
    int res = 0;

#ifdef __linux__
    char tmp[256];
    char *p = NULL;
    int perm = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
            tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
            if(*p == '/') {
                    *p = 0;
                    res = mkdir(tmp, perm);
                    *p = '/';
            }
    res = mkdir(tmp, perm);
   #else
       res = _mkdir(dir);
   #endif

    return res;
}
