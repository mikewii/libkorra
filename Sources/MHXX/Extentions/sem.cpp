#include "MHXX/Extentions/sem.hpp"

#include "Tools/Utils.hpp"

namespace MHXX {
namespace SEM {

sSetEmMain* __current = nullptr;

sSetEmMain::sSetEmMain()
{
}

sSetEmMain::sSetEmMain( Pair& _pp )
{
    if (( _pp.cc.size() == sizeof(sSetEmMain_s) && _pp.ResourceHash == RESOURCE_HASH ))
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sSetEmMain_s)); // for safekeeping

        this->SetPairInfo(_pp);
        //this->__data = reinterpret_cast<sSetEmMain_s&>( * _pp.cc.data() );
    }
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

void sSetEmMain::make( Pair& _pp) { this->save(_pp); }
void sSetEmMain::save( Pair& _pp )
{
    _pp.ResourceHash    = MHXX::SEM::RESOURCE_HASH;
    _pp.XORLock         = MHXX_XORLock;

    _pp.cc.resize(sizeof(sSetEmMain_s));

    Utils::copybytes(_pp.cc.data(), &this->__data, sizeof(sSetEmMain_s));

    ///// Set Pair info
    if ( this->isPairInfoSet() ) this->GetPairInfo( _pp );
    else
    {
        _pp.DecSize         = sizeof(sSetEmMain_s);
        _pp.ResourceHash    = MHXX::SEM::RESOURCE_HASH;
        _pp.XORLock         = MHXX_XORLock;
        _pp.isDecompressed    = true;
    }
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
} // MHXX
