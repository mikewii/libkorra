#include "Tools/CContainer.hpp"
#include "Tools/Utils.hpp"

CContainer::CContainer( const char* _fname ) {
    this->readFromFile(_fname);
}

CContainer::CContainer( std::string& _fname ) {
    this->readFromFile(_fname.c_str());
}

CContainer::~CContainer()
{
    this->_free();
}

void CContainer::_free( void ) { if (this->mallocUsed) free(this->__root); }

bool CContainer::allocate( u32 _size, bool _zeroed )
{
    if ( this->__root != nullptr) this->_free();

    u32 extra       = this->RESERVED_Before + this->RESERVED_After;
    u32 allocSize   = _size + extra;

    this->__size    = _size;

    if ( _zeroed )
        this->__root    = static_cast<u8*>( calloc(allocSize, sizeof(u8)));
    else
        this->__root    = static_cast<u8*>( malloc(allocSize));

    this->__data    = this->__root + this->RESERVED_Before;

    this->mallocUsed = true;

    return true;
}

bool CContainer::addBefore( u32 _size )
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

void CContainer::addAfter( u32 _size )
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

bool CContainer::subBefore( u32 _size )
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

bool CContainer::subAfter( u32 _size )
{
    if (_size < this->__size)
    {
        this->RESERVED_After += _size;
        this->__size -= _size;

        return true;
    }

    return false;
}

void CContainer::resize( u32 _size, bool _zeroed )
{
    if (this->__root == nullptr) this->allocate(_size, _zeroed);
    else if ( this->__root != nullptr && _zeroed ) this->allocate(_size, _zeroed);
    else this->addAfter(_size);
}

u8&     CContainer::as_u8( u32 _n ){
    return static_cast<u8&>(this->__data[_n]);
}

u16&    CContainer::as_u16(u32 _n){
    return reinterpret_cast<u16&>(this->__data[_n * sizeof(u16)]);
}

u32&    CContainer::as_u32(u32 _n){
    return reinterpret_cast<u32&>(this->__data[_n * sizeof(u32)]);
}

bool CContainer::readFromFile(const char* _fname)
{
    return Utils::FileToCC(_fname, this);
}

bool CContainer::writeToFile(const char* _fname, bool _makedir)
{
    return Utils::CCtoFile(_fname, this, _makedir);
}
