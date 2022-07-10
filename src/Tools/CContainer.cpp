#include "Tools/CContainer.hpp"
#include "Tools/Utils.hpp"
#include "Tools/File.hpp"

#ifndef N3DS
CContainer::CContainer(const std::filesystem::path& path)
    : m_data(nullptr), m_root(nullptr)
    , m_size(0), m_reserved_before(RESERVED_BEFORE), m_reserved_after(RESERVED_AFTER)
{
    auto prefered = path;

    CContainer::read_file(prefered.make_preferred());
}
#endif

CContainer::CContainer(const CContainer& cc)
    : m_data(nullptr), m_root(nullptr), m_size(0)
    , m_reserved_before(RESERVED_BEFORE), m_reserved_after(RESERVED_AFTER)
{
    if (cc.m_reserved_before != 0)
        this->m_reserved_before = cc.m_reserved_before;
    if (cc.m_reserved_after != 0)
        this->m_reserved_after = cc.m_reserved_after;

    if (cc.size() > 0) {
        this->resize(cc.size(), true);
        Utils::copybytes(this->data(), cc.data(), cc.size());
    }
}

CContainer::CContainer(const size_t size)
    : m_data(nullptr), m_root(nullptr)
    , m_reserved_before(RESERVED_BEFORE), m_reserved_after(RESERVED_AFTER)
{
    this->resize(size, true);
}

CContainer::CContainer()
    : m_data(nullptr), m_root(nullptr)
    , m_size(0), m_reserved_before(0), m_reserved_after(0)
{
}
CContainer::~CContainer() { this->_free(); }
CContainer& CContainer::operator=(const CContainer& cc)
{
    // Guard self assignment
    if (this == &cc) return *this;

    if (cc.m_reserved_before != 0)
        this->m_reserved_before = cc.m_reserved_before;
    else this->m_reserved_before = RESERVED_BEFORE;

    if (cc.m_reserved_after != 0)
        this->m_reserved_after = cc.m_reserved_after;
    else this->m_reserved_after = RESERVED_AFTER;

    if (cc.size() > 0) {
        this->resize(cc.size(), true);
        Utils::copybytes(this->data(), cc.data(), cc.size());
    } else {
        this->m_data = nullptr;
        this->m_root = nullptr;
        this->m_size = 0;
    }

    return *this;
}

void CContainer::swap(CContainer &right)
{
    auto data   = right.m_data;
    auto root   = right.m_root;
    auto size   = right.m_size;
    auto before = right.m_reserved_before;
    auto after  = right.m_reserved_after;

    right.m_data = this->m_data;
    right.m_root = this->m_root;
    right.m_size = this->m_size;
    right.m_reserved_before = this->m_reserved_before;
    right.m_reserved_after = this->m_reserved_after;

    this->m_data = data;
    this->m_root = root;
    this->m_size = size;
    this->m_reserved_before = before;
    this->m_reserved_after = after;
}

void CContainer::clear(void)
{
    this->_free();
}

void CContainer::copy(const void *data, const size_t data_size)
{
    if (this->m_size < data_size)
        this->resize(data_size);

    Utils::copybytes(this->m_data, data, data_size);
}

void CContainer::_free(void)
{
    if (this->m_root != nullptr)
    {
        free(this->m_root);
        this->m_root = nullptr;
        this->m_data = nullptr;
        this->m_size = 0;
        this->m_reserved_before = 0;
        this->m_reserved_after = 0;
    }
}

const bool CContainer::allocate(size_t size, bool zeroed)
{
    if (this->m_root != nullptr) this->_free();

    u32 extra = this->m_reserved_before + this->m_reserved_after;
    u32 alloc_size = size + extra;

    this->m_size = size;

    if (zeroed)
        this->m_root = static_cast<u8*>( ::calloc(alloc_size, sizeof(u8)) );
    else
        this->m_root = static_cast<u8*>( ::malloc(alloc_size) );

    this->m_data = this->m_root + this->m_reserved_before;

    return true;
}

const bool CContainer::add_before(size_t size)
{
    if (this->m_root == nullptr)
        return false;

    if (size <= this->m_reserved_before && this->m_reserved_before != 0)
    {
        this->m_reserved_before -= size;
        this->m_data            -= size;

        this->m_size            += size;

        return true;
    }

    return false;
}

const bool CContainer::add_after(size_t size)
{
    if (this->m_root == nullptr)
        return false;

    if (size <= this->m_reserved_after && this->m_reserved_after != 0) {
        this->m_reserved_after  -= size;
        this->m_size            += size;
    } else {
        this->m_root = static_cast<u8*>( ::realloc(this->m_root, this->m_size += size) );
        this->m_data = this->m_root + this->m_reserved_before;
    }

    return true;
}

const bool CContainer::sub_before(size_t _size)
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

const bool CContainer::sub_after(size_t size)
{
    if (this->m_root == nullptr)
        return false;

    if (size < this->m_size) {
        this->m_reserved_after  += size;
        this->m_size            -= size;

        return true;
    } else return false;
}

void CContainer::resize(size_t size, bool zeroed)
{
    if (this->m_root == nullptr) this->allocate(size, zeroed);
    else if (this->m_root != nullptr && !zeroed) this->add_after(size);
    else this->allocate(size, zeroed);
}

#ifndef N3DS
void CContainer::read_file(const std::filesystem::path& path) { File::file_to_cc(path, *this); }
void CContainer::write_to_file(const std::filesystem::path& path) const { File::CC_To_File(path, *this); }
#endif
