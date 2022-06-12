#pragma once
#include "types.h"

#ifndef N3DS
    #include <filesystem>
#endif

class CContainer {
public:

#ifndef N3DS
    explicit CContainer(const std::filesystem::path& path);
#endif
    explicit CContainer(const CContainer& cc);
    CContainer();
    ~CContainer();

    CContainer& operator=(const CContainer& _cc);


    const u32   size(void) const { return this->__size; }
    void        resize(u32 _size, bool zeroed = false);

    /* for in-memory ops */
    void    setData(u8* _ptr) { this->__data = _ptr; } // keep __root nullptr to avoid calling free on actuall in-memory data
    void    setSize(u32 _size) { this->__size = _size; }

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
    u8*     data(void) const { return this->__data; }

    template<typename T>
    T&      as(const u32 index)
    { return reinterpret_cast<T&>(this->__data[index * sizeof(T)]); }

    template<typename T>
    const T& as_const_ref(const u32 index) const
    { return reinterpret_cast<const T&>(this->__data[index * sizeof(T)]); }

    template<typename T>
    const T* at_const(const u32 index) const
    { return reinterpret_cast<T*>(this->__data[index]); }

    template<typename T>
    const T& at_const_ref(const u32 index) const
    { return reinterpret_cast<const T&>(this->__data[index]); }


private:
    u8*         __data = nullptr;
    u8*         __root = nullptr;

    u32         __size = 0; // data size, no need to keep track of root size

    u32         RESERVED_Before  = 0x100;
    u32         RESERVED_After   = 0x100;

    const bool  allocate(u32 _size, bool _zeroed = false);
    void        _free(void);
};
