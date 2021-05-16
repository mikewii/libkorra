#pragma once
#include "types.h"
#include "Global.hpp"

namespace MH4U {
namespace SEM {

const u32 MAGIC = 0x3F800000;

struct sSetEmMain {
    u32         Magic   = MAGIC;    // maybe
    u32         Version = 1;        // maybe
    u32         WaveNo;
    u32         AreaNo;
    Geometry    Position;
};

void Init( Pair& _pp );


// Getters
sSetEmMain* getCurrent( void );
u32         getWaveNo( void );
u32         getAreaNo( void );
Geometry    getPosition( void );

// Setters
void        setCurrent( Pair& _pp );
void        setWaveNo( u32 _num );
void        setAreaNo( u32 _num );
void        setPosition( float _rot, float _x, float _y, float _z );

void print( void );
void print( sSetEmMain* _sem );

extern sSetEmMain* __current;

} // SEM
} // MH4U
