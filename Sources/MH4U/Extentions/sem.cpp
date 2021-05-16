#include "MH4U/Extentions/sem.hpp"

namespace MH4U {
namespace SEM {

sSetEmMain* __current = nullptr;

void Init( Pair& _pp ){
    setCurrent(_pp);
}



void print( void ) { print(getCurrent()); }
void print( sSetEmMain* _sem )
{
    printf("\n##### SetEmMain File #####\n");

    printf("Magic:      0x%08X\n", _sem->Magic);
    printf("Version:    0x%X\n", _sem->Version);

    // actual data now
    printf("Wave №:     %d\n", _sem->WaveNo);
    printf("Area №:     %d\n", _sem->AreaNo);

    printf("Rotation:   %f\n", _sem->Position.Rotation);
    printf("PosX:       %f\n", _sem->Position.X);
    printf("PosY:       %f\n", _sem->Position.Y);
    printf("PosZ:       %f\n", _sem->Position.Z);
}

////////// Getters //////////

sSetEmMain* getCurrent( void )  { return __current; }
u32         getWaveNo( void )   { return getCurrent()->WaveNo; }
u32         getAreaNo( void )   { return getCurrent()->AreaNo; }
Geometry    getPosition( void ) { return getCurrent()->Position; }


////////// Setters //////////

void        setCurrent( Pair& _pp ) {
    __current = reinterpret_cast<sSetEmMain*>( _pp.cc.data() );
}

void        setWaveNo( u32 _num ) { getCurrent()->WaveNo = _num; }
void        setAreaNo( u32 _num ) { getCurrent()->AreaNo = _num; }

void        setPosition( float _rot, float _x, float _y, float _z )
{
    sSetEmMain* sem = getCurrent();
    sem->Position.Rotation  = _rot;
    sem->Position.X         = _x;
    sem->Position.Y         = _y;
    sem->Position.Z         = _z;
}


} // SEM
} // MH4U
