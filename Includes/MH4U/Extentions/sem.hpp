#pragma once
#include "types.h"
#include "Global.hpp"

namespace MH4U {
namespace SEM {

struct sSetEmMain_s {
    static const u32 MAGIC = 0x3F800000;    // no its not

    u32         Magic   = MAGIC;            // maybe
    u32         Version = 1;                // maybe
    u32         WaveNo;
    u32         AreaNo;
    Geometry    Position;
};

struct sSetEmMain {

    sSetEmMain_s data;


    sSetEmMain( Pair& _pp );
    ~sSetEmMain(){};

    void print( void );

    // Getters
    u32         getWaveNo( void ) const;
    u32         getAreaNo( void ) const;
    Geometry    getPosition( void ) const;

    // Setters
    void        setWaveNo( u32 _num );
    void        setAreaNo( u32 _num );
    void        setPosition( float _rot, float _x, float _y, float _z );
};

} // SEM
} // MH4U
