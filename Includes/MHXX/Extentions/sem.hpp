#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace SEM {
static const u32 RESOURCE_HASH = 0x2553701D;

struct sSetEmMain_s {
    static const u32 MAGIC = 0x3F800000;    // no its not

    u32         Magic = MAGIC;              // maybe
    u32         Version = 1;                // maybe
    u32         WaveNo = 0;
    u32         AreaNo = 0;
    Geometry    Position = {0,0,0,0};
};

class sSetEmMain : public PairInfo
{
public:

    sSetEmMain();
    sSetEmMain( Pair& _pp );
    ~sSetEmMain(){};

    void print( void );

    void make( Pair& _pp );
    void save( Pair& _pp );

    // Getters
    u32         getWaveNo( void ) const;
    u32         getAreaNo( void ) const;
    Geometry    getPosition( void ) const;

    // Setters
    void        setWaveNo( u32 _num );
    void        setAreaNo( u32 _num );
    void        setPosition( float _rot, float _x, float _y, float _z );

private:
    sSetEmMain_s    __data;

};

} // SEM
} // MHXX
