#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace SEM {
static const u32 RESOURCE_HASH = 0x2553701D;

struct sSetEmMain_s {
    static const u32 MAGIC = 0x3F800000;

    u32                 Magic       = MAGIC;            // maybe
    u32                 Version     = 1;                // maybe
    u32                 WaveNo      = 0;
    u32                 AreaNo      = 0;
    Geometry4<float>    Position    = {0,0,0,0};
};

class cSetEmMain : public PairInfoKeeper
{
public:

    cSetEmMain();
    cSetEmMain(const Pair& _pp);
    ~cSetEmMain(){};

    void print(void);

    void make(Pair& _pp);
    void save(Pair& _pp);


    u32                 Get_WaveNo(void) const;
    u32                 Get_AreaNo(void) const;
    Geometry4<float>    Get_Position(void) const;

    void                Set_WaveNo(const u32 _num);
    void                Set_AreaNo(const u32 _num);
    void                Set_Position(const Geometry4<float> pos);

private:
    sSetEmMain_s    __data;

    u32 getResourceHash(void) const { return MHXX::SEM::RESOURCE_HASH; }

    friend class Export<cSetEmMain>;

};

} // SEM
} // MHXX
