#include "Tools/CContainer.hpp"
#include "Tools/Utils.hpp"

CContainer::CContainer(const char* fname)
{
    this->readFromFile(fname);
}

bool CContainer::allocate(u32 _size, bool zeroed)
{
    if ( this->__root != nullptr) return false;

    u32 extra       = this->RESERVED_Before + this->RESERVED_After;
    u32 allocSize   = _size + extra;

    this->__size    = _size;

    if ( zeroed )
        this->__root    = static_cast<u8*>( calloc(allocSize, sizeof(u8)));
    else
        this->__root    = static_cast<u8*>( malloc(allocSize));

    this->__data    = this->__root + this->RESERVED_Before;

    this->mallocUsed = true;

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

void CContainer::resize(u32 _size, bool zeroed)
{
    if (this->__root == nullptr) this->allocate(_size, zeroed);
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
    return Utils::FileToCC(fname, this);
}

bool CContainer::writeToFile(const char* fname, bool makedir)
{
    return Utils::CCtoFile(fname, this, makedir);
}
