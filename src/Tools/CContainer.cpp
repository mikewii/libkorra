#include "Tools/CContainer.hpp"
#include "Tools/Utils.hpp"
#include "Tools/File.hpp"

#ifndef N3DS
CContainer::CContainer(const std::filesystem::path& path)
{
    auto prefered = path;

    CContainer::read_File(prefered.make_preferred());
}
#endif

CContainer::CContainer(const CContainer& cc)
{
    // copy data
    this->resize(cc.size(), true);
    Utils::copybytes(this->data(), cc.data(), cc.size());

    this->RESERVED_After = cc.RESERVED_After;
    this->RESERVED_Before = cc.RESERVED_Before;
}
CContainer::~CContainer() { this->_free(); }
CContainer& CContainer::operator=(const CContainer& _cc)
{
    // Guard self assignment
    if (this == &_cc) return *this;

    this->resize(_cc.size(), true);

    Utils::copybytes(this->data(), _cc.data(), _cc.size());

    return *this;
}

void CContainer::_free(void)
{
    if (this->__root != nullptr)
    {
        free(this->__root);
        this->__root = nullptr;
    }
}

bool CContainer::allocate(u32 _size, bool _zeroed)
{
    if (this->__root != nullptr) this->_free();

    u32 extra       = this->RESERVED_Before + this->RESERVED_After;
    u32 allocSize   = _size + extra;

    this->__size    = _size;

    if (_zeroed)
        this->__root    = static_cast<u8*>( ::calloc(allocSize, sizeof(u8)) );
    else
        this->__root    = static_cast<u8*>( ::malloc(allocSize) );

    this->__data    = this->__root + this->RESERVED_Before;

    return true;
}

bool CContainer::addBefore(u32 _size)
{
    if (_size <= this->RESERVED_Before && this->RESERVED_Before != 0)
    {
        this->RESERVED_Before   -= _size;
        this->__data            -= _size;

        this->__size            += _size;

        return true;
    }

    return false;
}

void CContainer::addAfter(u32 _size)
{
    if (_size <= this->RESERVED_After && this->RESERVED_After != 0)
    {
        this->RESERVED_After    -= _size;
        this->__size            += _size;
    }
    else
    {
        this->__root = static_cast<u8*>( ::realloc(this->__root, this->__size += _size) );
        this->__data = this->__root + this->RESERVED_Before;
    }
}

bool CContainer::subBefore(u32 _size)
{
    if (_size < this->__size)
    {
        this->RESERVED_Before   += _size;
        this->__data            += _size;
        this->__size            -= _size;

        return true;
    }

    return false;
}

bool CContainer::subAfter(u32 _size)
{
    if (_size < this->__size)
    {
        this->RESERVED_After    += _size;
        this->__size            -= _size;

        return true;
    }

    return false;
}

void CContainer::resize(u32 _size, bool _zeroed)
{
    if (this->__root == nullptr) this->allocate(_size, _zeroed);
    else if (this->__root != nullptr && !_zeroed) this->addAfter(_size);
    else this->allocate(_size, _zeroed);
}

#ifndef N3DS
void CContainer::read_File(const std::filesystem::path& path) { File::File_To_CC(path, *this); }
void CContainer::write_To_File(const std::filesystem::path& path) const { File::CC_To_File(path, *this); }
#endif
