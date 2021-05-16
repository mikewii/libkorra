#include "MH4U/Extentions/sem.hpp"

namespace MH4U {
namespace SEM {

sSetEmMain* __current = nullptr;

sSetEmMain::sSetEmMain( Pair& _pp ){

    if (( _pp.cc.size() == sizeof(sSetEmMain_s)))
        this->data = reinterpret_cast<sSetEmMain_s&>( * _pp.cc.data() );
    else NotifyError("Pair is not sSetEmMain_s");
}

void sSetEmMain::print( void )
{
    printf("\n##### SetEmMain File #####\n");

    printf("Magic:      0x%08X\n", this->data.Magic);
    printf("Version:    0x%X\n", this->data.Version);

    // actual data now
    printf("Wave №:     %d\n", this->data.WaveNo);
    printf("Area №:     %d\n", this->data.AreaNo);

    printf("Rotation:   %f\n", this->data.Position.Rotation);
    printf("PosX:       %f\n", this->data.Position.X);
    printf("PosY:       %f\n", this->data.Position.Y);
    printf("PosZ:       %f\n", this->data.Position.Z);
}

////////// Getters //////////

u32         sSetEmMain::getWaveNo( void ) const     { return this->data.WaveNo; }
u32         sSetEmMain::getAreaNo( void ) const     { return this->data.AreaNo; }
Geometry    sSetEmMain::getPosition( void ) const   { return this->data.Position; }


////////// Setters //////////

void        sSetEmMain::setWaveNo( u32 _num ) { this->data.WaveNo = _num; }
void        sSetEmMain::setAreaNo( u32 _num ) { this->data.AreaNo = _num; }

void        sSetEmMain::setPosition( float _rot, float _x, float _y, float _z )
{
    this->data.Position.Rotation  = _rot;
    this->data.Position.X         = _x;
    this->data.Position.Y         = _y;
    this->data.Position.Z         = _z;
}

} // SEM
} // MH4U
