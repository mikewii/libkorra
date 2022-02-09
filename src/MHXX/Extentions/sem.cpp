#include "MHXX/Extentions/sem.hpp"

namespace MHXX {
namespace SEM {

cSetEmMain* __current = nullptr;

cSetEmMain::cSetEmMain()
{
}

cSetEmMain::cSetEmMain(const Pair& _pp)
{
    if (_pp.cc.size() == sizeof(sSetEmMain_s) && _pp.info.ResourceHash == RESOURCE_HASH)
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sSetEmMain_s)); // for safekeeping

        this->Set_PairInfo(_pp);
        //this->__data = reinterpret_cast<sSetEmMain_s&>( * _pp.cc.data() );
    }
    else NotifyError("Pair is not sSetEmMain_s");
}

void cSetEmMain::print(void)
{
    sSetEmMain_s& sem = this->__data;
    printf("\n##### SetEmMain File #####\n");

    printf("Magic:      0x%08X\n", sem.Magic);
    printf("Version:    0x%X\n", sem.Version);

    // actual data now
    printf("Wave №:     %d\n", sem.WaveNo);
    printf("Area №:     %d\n", sem.AreaNo);

    printf("Rotation:   %f\n", sem.Position.R);
    printf("PosX:       %f\n", sem.Position.X);
    printf("PosY:       %f\n", sem.Position.Y);
    printf("PosZ:       %f\n", sem.Position.Z);
}

void cSetEmMain::make(Pair& _pp) { this->save(_pp); }
void cSetEmMain::save(Pair& _pp ) { Export<cSetEmMain>::save(*this, _pp); }

////////// Getters //////////

u32 cSetEmMain::Get_WaveNo(void) const { return this->__data.WaveNo; }
u32 cSetEmMain::Get_AreaNo(void) const { return this->__data.AreaNo; }
Geometry4<float> cSetEmMain::Get_Position(void) const { return this->__data.Position; }


////////// Setters //////////
void cSetEmMain::Set_WaveNo(const u32 _num) { this->__data.WaveNo = _num; }
void cSetEmMain::Set_AreaNo(const u32 _num) { this->__data.AreaNo = _num; }
void cSetEmMain::Set_Position(const Geometry4<float> pos)
{
    sSetEmMain_s& sem = this->__data;

    sem.Position.R  = pos.R;
    sem.Position.X  = pos.X;
    sem.Position.Y  = pos.Y;
    sem.Position.Z  = pos.Z;
}

} // SEM
} // MHXX
