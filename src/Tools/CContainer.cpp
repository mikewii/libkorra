#include "Tools/CContainer.hpp"
#include "Tools/Utils.hpp"
#include "Tools/File.hpp"


#ifndef N3DS
CContainer::CContainer(const std::filesystem::path& path)
    : m_data(nullptr), m_root(nullptr)
    , m_size(0), m_reserved_before(RESERVED_BEFORE), m_reserved_after(RESERVED_AFTER)
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

    this->m_reserved_after = cc.m_reserved_after;
    this->m_reserved_before = cc.m_reserved_before;
}

CContainer::CContainer()
    : m_data(nullptr), m_root(nullptr)
    , m_size(0), m_reserved_before(0), m_reserved_after(0)
{
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

void CContainer::clear(void)
{
    this->_free();
}

void CContainer::_free(void)
{
    if (this->m_root != nullptr)
    {
        free(this->m_root);
        this->m_root = nullptr;
        this->m_size = 0;
        this->m_reserved_before = RESERVED_BEFORE;
        this->m_reserved_after = RESERVED_AFTER;
    }
}

const bool CContainer::allocate(u32 _size, bool _zeroed)
{
    if (this->m_root != nullptr) this->_free();

    u32 extra       = this->m_reserved_before + this->m_reserved_after;
    u32 allocSize   = _size + extra;

    this->m_size    = _size;

    if (_zeroed)
        this->m_root    = static_cast<u8*>( ::calloc(allocSize, sizeof(u8)) );
    else
        this->m_root    = static_cast<u8*>( ::malloc(allocSize) );

    this->m_data    = this->m_root + this->m_reserved_before;

    return true;
}

const bool CContainer::addBefore(u32 _size)
{
    if (this->m_root == nullptr)
        return false;

    if (_size <= this->m_reserved_before && this->m_reserved_before != 0)
    {
        this->m_reserved_before -= _size;
        this->m_data            -= _size;

        this->m_size            += _size;

        return true;
    }

    return false;
}

const bool CContainer::addAfter(u32 _size)
{
    if (this->m_root == nullptr)
        return false;

    if (_size <= this->m_reserved_after && this->m_reserved_after != 0) {
        this->m_reserved_after  -= _size;
        this->m_size            += _size;
    } else {
        this->m_root = static_cast<u8*>( ::realloc(this->m_root, this->m_size += _size) );
        this->m_data = this->m_root + this->m_reserved_before;
    }

    return true;
}

const bool CContainer::subBefore(u32 _size)
{
    if (this->m_root == nullptr)
        return false;

    if (_size < this->m_size) {
        this->m_reserved_before += _size;
        this->m_data            += _size;
        this->m_size            -= _size;

        return true;
    } else return false;
}

const bool CContainer::subAfter(u32 _size)
{
    if (this->m_root == nullptr)
        return false;

    if (_size < this->m_size) {
        this->m_reserved_after  += _size;
        this->m_size            -= _size;

        return true;
    } else return false;
}

void CContainer::resize(u32 _size, bool _zeroed)
{
    if (this->m_root == nullptr) this->allocate(_size, _zeroed);
    else if (this->m_root != nullptr && !_zeroed) this->addAfter(_size);
    else this->allocate(_size, _zeroed);
}

#ifndef N3DS
void CContainer::read_File(const std::filesystem::path& path) { File::File_To_CC(path, *this); }
void CContainer::write_To_File(const std::filesystem::path& path) const { File::CC_To_File(path, *this); }
#endif
