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
    cSetEmMain(Pair& _pp);
    ~cSetEmMain(){};

    void print(void);

    void make(Pair& _pp);
    void save(Pair& _pp);

    // Getters
    u32                 getWaveNo(void) const;
    u32                 getAreaNo(void) const;
    Geometry4<float>    getPosition(void) const;

    // Setters
    void                setWaveNo(u32 _num);
    void                setAreaNo(u32 _num);
    void                setPosition(float _rot, float _x, float _y, float _z);

private:
    sSetEmMain_s    __data;

    u32 getResourceHash(void) const { return MHXX::SEM::RESOURCE_HASH; }

    friend class Export<cSetEmMain>;

};

} // SEM
} // MHXX
