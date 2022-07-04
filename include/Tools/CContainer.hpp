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
    explicit CContainer(const CContainer& cc);
    CContainer();
    ~CContainer();

    CContainer& operator=(const CContainer& _cc);

    void clear(void);

    const u32   size(void) const { return this->m_size; }
    void        resize(u32 _size, bool zeroed = false);

    /* for in-memory ops */
    void    setData(u8* _ptr) { this->m_data = _ptr; } // keep __root nullptr to avoid calling free on actuall in-memory data
    void    setSize(u32 _size) { this->m_size = _size; }

    // Expanding and shrinking
    const bool  addBefore(u32 _size);
    const bool  addAfter(u32 _size);
    const bool  subBefore(u32 _size);
    const bool  subAfter(u32 _size);

#ifndef N3DS
    void    read_File(const std::filesystem::path& path);
    void    write_To_File(const std::filesystem::path& path) const;
#endif

    // Access
    u8*     data(void) const { return this->m_data; }

    template<typename T>
    T&      as(const u32 index)
    { return reinterpret_cast<T&>(this->m_data[index * sizeof(T)]); }

    template<typename T>
    const T& as_const_ref(const u32 index) const
    { return reinterpret_cast<const T&>(this->m_data[index * sizeof(T)]); }

    template<typename T>
    const T* at_const(const u32 index) const
    { return reinterpret_cast<T*>(this->m_data[index]); }

    template<typename T>
    const T& at_const_ref(const u32 index) const
    { return reinterpret_cast<const T&>(this->m_data[index]); }


private:
    u8*         m_data;
    u8*         m_root;

    u32         m_size;
    u32         m_reserved_before;
    u32         m_reserved_after;

    const bool  allocate(u32 _size, bool _zeroed = false);
    void        _free(void);
};
