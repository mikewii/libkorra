#include "MH4U/Extentions/sem.hpp"

namespace MH4U {
namespace SEM {

sSetEmMain* __current = nullptr;

sSetEmMain::sSetEmMain( Pair& _pp )
{
    if (( _pp.cc.size() == sizeof(sSetEmMain_s) && _pp.ResourceHash == RESOURCE_HASH ))
        this->__data = reinterpret_cast<sSetEmMain_s&>( * _pp.cc.data() );

    else NotifyError("Pair is not sSetEmMain_s");
}

void sSetEmMain::print( void )
{
    sSetEmMain_s& sem = this->__data;
    printf("\n##### SetEmMain File #####\n");

    printf("Magic:      0x%08X\n", sem.Magic);
    printf("Version:    0x%X\n", sem.Version);

    // actual data now
    printf("Wave №:     %d\n", sem.WaveNo);
    printf("Area №:     %d\n", sem.AreaNo);

    printf("Rotation:   %f\n", sem.Position.Rotation);
    printf("PosX:       %f\n", sem.Position.X);
    printf("PosY:       %f\n", sem.Position.Y);
    printf("PosZ:       %f\n", sem.Position.Z);
}

////////// Getters //////////

u32         sSetEmMain::getWaveNo( void ) const     { return this->__data.WaveNo; }
u32         sSetEmMain::getAreaNo( void ) const     { return this->__data.AreaNo; }
Geometry    sSetEmMain::getPosition( void ) const   { return this->__data.Position; }


////////// Setters //////////

void        sSetEmMain::setWaveNo( u32 _num ) { this->__data.WaveNo = _num; }
void        sSetEmMain::setAreaNo( u32 _num ) { this->__data.AreaNo = _num; }

void        sSetEmMain::setPosition( float _rot, float _x, float _y, float _z )
{
    sSetEmMain_s& sem = this->__data;

    sem.Position.Rotation  = _rot;
    sem.Position.X         = _x;
    sem.Position.Y         = _y;
    sem.Position.Z         = _z;
}

} // SEM
} // MH4U
