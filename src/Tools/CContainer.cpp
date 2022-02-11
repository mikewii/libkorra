#include "Tools/CContainer.hpp"
#include "Tools/Utils.hpp"

CContainer::CContainer(const char* _fname) { this->readFile(_fname); }
CContainer::CContainer(const std::string& _fname) { this->readFile(_fname.c_str()); }
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
    else if (this->__root != nullptr && _zeroed) this->allocate(_size, _zeroed);
    else this->addAfter(_size);
}

bool CContainer::readFile(const std::string& fname) { return this->readFile(fname.c_str()); }
bool CContainer::readFile(const char* fname) { return Utils::File::FileToCC(fname, this); }
bool CContainer::writeToFile(const std::string& _fname, bool _makedir) { return Utils::File::CCtoFile(_fname.c_str(), this, _makedir); }
bool CContainer::writeToFile(const char* _fname, bool _makedir) { return Utils::File::CCtoFile(_fname, this, _makedir); }
