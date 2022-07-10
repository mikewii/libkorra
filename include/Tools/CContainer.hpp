#pragma once
#include "types.h"

#ifndef N3DS
    #include <filesystem>
#endif

class CContainer {
public:
#define RESERVED_BEFORE 0x100
#define RESERVED_AFTER RESERVED_BEFORE

#ifndef N3DS
    explicit CContainer(const std::filesystem::path& path);
#endif
    explicit CContainer(const size_t size);
    CContainer(const CContainer& cc);
    CContainer();
    ~CContainer();

    CContainer& operator=(const CContainer& cc);

    void clear(void);

    void copy(const void* data, const size_t data_size);

    const size_t    size(void) const { return this->m_size; }
    void            resize(size_t size, bool zeroed = false);

    /* for in-memory ops */
    void    set_data(u8* _ptr) { this->m_data = _ptr; } // keep __root nullptr to avoid calling free on actuall in-memory data
    void    set_size(size_t size) { this->m_size = size; }

    // Expanding and shrinking
    const bool  add_before(size_t size);
    const bool  add_after(size_t size);
    const bool  sub_before(size_t size);
    const bool  sub_after(size_t size);

#ifndef N3DS
    void    read_file(const std::filesystem::path& path);
    void    write_to_file(const std::filesystem::path& path) const;
#endif

    // Access
    u8*   data(void) const { return this->m_data; }

    template<typename T>
    T&      as(const size_t index)
    { return reinterpret_cast<T&>(this->m_data[index * sizeof(T)]); }

    template<typename T>
    const T& as_const_ref(const size_t index) const
    { return reinterpret_cast<const T&>(this->m_data[index * sizeof(T)]); }

    template<typename T>
    const T* at_const(const size_t index) const
    { return reinterpret_cast<T*>(this->m_data[index]); }

    template<typename T>
    const T& at(const size_t index) const
    { return reinterpret_cast<const T&>(this->m_data[index]); }

    template<typename T>
    T& at(const size_t index)
    { return reinterpret_cast<T&>(this->m_data[index]); }


private:
    u8*         m_data;
    u8*         m_root;

    size_t      m_size;
    size_t      m_reserved_before;
    size_t      m_reserved_after;

    const bool  allocate(size_t size, bool zeroed = false);
    void        _free(void);
};
