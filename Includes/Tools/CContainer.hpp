#pragma once
#include "types.h"

struct CContainer {
    u8* __data = nullptr;
    u8* __root = nullptr;

    u32 __size = 0; // data size, no need to keep track of root size

    u32 RESERVED_Before  = 0x100;
    u32 RESERVED_After   = 0x100;

    bool mallocUsed = false;


    CContainer(){};
    CContainer(const char* fname);
    ~CContainer(){ if (this->mallocUsed) free(this->__root); }

    bool allocate(u32 size, bool zeroed = false);

    bool addBefore(u32 _size);
    void addAfter(u32 _size);

    bool subBefore(u32 _size);
    bool subAfter(u32 _size);

    u8&     as_u8(u32 n);
    u16&    as_u16(u32 n);
    u32&    as_u32(u32 n);

    bool readFromFile(const char* fname);
    bool writeToFile(const char* fname, bool makedir = false);
    bool makedir(const char* dir);

    /* defaults */
    u32 size(void) const { return this->__size; }
    u8* data(void) const { return this->__data; }
    void resize(u32 _size, bool zeroed = false);

    /* for in-memory ops */
    void setData(u8* ptr) { this->__data = ptr; }
    void setSize(u32 _size) { this->__size = _size; }
};
