#pragma once
#include "types.h"
#include <filesystem>

class CContainer {
public:

    CContainer(){};
    CContainer(const std::filesystem::path& path);
    CContainer(const CContainer& cc);
    ~CContainer();

    CContainer& operator=(const CContainer& _cc);


    u32     size(void) const { return this->__size; }
    void    resize(u32 _size, bool zeroed = false);

    void    read_File(const std::filesystem::path& path);
    void    write_To_File(const std::filesystem::path& path) const;

    /* for in-memory ops */
    void    setData(u8* _ptr) { this->__data = _ptr; }
    void    setSize(u32 _size) { this->__size = _size; }

    // Expanding and shrinking
    bool    addBefore(u32 _size);
    void    addAfter(u32 _size);

    bool    subBefore(u32 _size);
    bool    subAfter(u32 _size);

    // Access
    u8*     data(void) const { return this->__data; }

    template<typename T>
    T&      as(const u32 _n)
    { return reinterpret_cast<T&>(this->__data[_n * sizeof(T)]); }

    template<typename T>
    const T& as_const(const u32 _n) const
    { return reinterpret_cast<const T&>(this->__data[_n * sizeof(T)]); }


private:
    u8*         __data = nullptr;
    u8*         __root = nullptr;

    u32         __size = 0; // data size, no need to keep track of root size

    u32         RESERVED_Before  = 0x100;
    u32         RESERVED_After   = 0x100;

    bool allocate(u32 _size, bool _zeroed = false);
    void _free(void);
};
