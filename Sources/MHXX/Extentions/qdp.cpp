#include "MHXX/Extentions/qdp.hpp"

namespace MHXX {
namespace QDP {

cQuestPlus::cQuestPlus()
{
}

cQuestPlus::cQuestPlus( Pair& _pp )
{
    if ( _pp.cc.size() == sizeof(sQuestPlus_s) && _pp.info.ResourceHash == RESOURCE_HASH )
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sQuestPlus_s)); // we copy now

        this->SetPairInfo(_pp);

        //this->__data = reinterpret_cast<sQuestPlus_s&>( * _pp.cc.data() );
    }
    else NotifyError("Pair is not a sQuestPlus_s");
}

void cQuestPlus::print( void)
{
    sQuestPlus_s& qdp = this->__data;

    printf("\n##### QuestPlus file #####\n");

    printf("Magic:                  0x%08X\n", qdp.Magic);
    printf("Version:                0x%X\n", qdp.Version);

    // actual data now
    printf("isFence:                %hhd\n", qdp.isFence);
    printf("isFenceFromStart:       %hhd\n", qdp.isFenceFromStart);
    printf("FenceOpenTime:          %hd\n", qdp.FenceOpenTime);
    printf("FenceStartTime:         %hd\n", qdp.FenceStartTime);
    printf("FenceReuseTime:         %hd\n", qdp.FenceReuseTime);

    printf("isDragonator:           %hhd\n", qdp.isDragonator);
    printf("DragonatorStartTime:    %hd\n", qdp.DragonatorStartTime);
    printf("DragonatorReuseTime:    %hd\n", qdp.DragonatorReuseTime);

    printf("FortHpS:                %hd\n", qdp.FortHpS);
    printf("FortHpL:                %hd\n", qdp.FortHpL);
}

void cQuestPlus::make( Pair& _pp) { this->save(_pp); }
void cQuestPlus::save( Pair& _pp ) { Export<cQuestPlus>::save(*this, _pp); }

////////// Getters //////////

bool    cQuestPlus::getIsFence( void ) const { return this->__data.isFence; }
bool    cQuestPlus::getIsFenceFromStart( void ) const { return this->__data.isFenceFromStart; }
u16     cQuestPlus::getFenceOpenTime( void ) const { return this->__data.FenceOpenTime; }
u16     cQuestPlus::getFenceStartTime( void ) const { return this->__data.FenceStartTime; }
u16     cQuestPlus::getFenceReuseTime( void ) const { return this->__data.FenceReuseTime; }
bool    cQuestPlus::getIsDragonator( void ) const { return this->__data.isDragonator; }
u16     cQuestPlus::getDragonatorStartTime( void ) const { return this->__data.DragonatorStartTime; }
u16     cQuestPlus::getDragonatorReuseTime( void ) const { return this->__data.DragonatorReuseTime; }
u16     cQuestPlus::getFortHpS( void ) const { return this->__data.FortHpS; }
u16     cQuestPlus::getFortHpL( void ) const { return this->__data.FortHpL; }


////////// Setters //////////

void    cQuestPlus::setIsFence( bool _b ) { this->__data.isFence = _b; }
void    cQuestPlus::setIsFenceFromStart( bool _b ) { this->__data.isFenceFromStart = _b; }
void    cQuestPlus::setFenceOpenTime( u16 _num ) { this->__data.FenceOpenTime = _num; }
void    cQuestPlus::setFenceStartTime( u16 _num ) { this->__data.FenceStartTime = _num; }
void    cQuestPlus::setFenceReuseTime( u16 _num ) { this->__data.FenceReuseTime = _num; }
void    cQuestPlus::setIsDragonator( bool _b ) { this->__data.isDragonator = _b; }
void    cQuestPlus::setDragonatorStartTime( u16 _num ) { this->__data.DragonatorStartTime = _num; }
void    cQuestPlus::setDragonatorReuseTime( u16 _num ) { this->__data.DragonatorReuseTime = _num; }
void    cQuestPlus::setFortHpS( u16 _num ) { this->__data.FortHpS = _num; }
void    cQuestPlus::setFortHpL( u16 _num ) { this->__data.FortHpL = _num; }

} // QDP
} // MHXX
