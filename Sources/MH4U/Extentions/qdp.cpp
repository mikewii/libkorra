#include "MH4U/Extentions/qdp.hpp"

namespace MH4U {
namespace QDP {

sQuestPlus::sQuestPlus( Pair& _pp )
{
    if ( _pp.cc.size() == sizeof(sQuestPlus_s) && _pp.ResourceHash == RESOURCE_HASH )
        this->__data = reinterpret_cast<sQuestPlus_s&>( * _pp.cc.data() );

    else NotifyError("Pair is not a sQuestPlus_s");
}

void sQuestPlus::print( void)
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

////////// Getters //////////

bool    sQuestPlus::getIsFence( void ) const { return this->__data.isFence; }
bool    sQuestPlus::getIsFenceFromStart( void ) const { return this->__data.isFenceFromStart; }
u16     sQuestPlus::getFenceOpenTime( void ) const { return this->__data.FenceOpenTime; }
u16     sQuestPlus::getFenceStartTime( void ) const { return this->__data.FenceStartTime; }
u16     sQuestPlus::getFenceReuseTime( void ) const { return this->__data.FenceReuseTime; }
bool    sQuestPlus::getIsDragonator( void ) const { return this->__data.isDragonator; }
u16     sQuestPlus::getDragonatorStartTime( void ) const { return this->__data.DragonatorStartTime; }
u16     sQuestPlus::getDragonatorReuseTime( void ) const { return this->__data.DragonatorReuseTime; }
u16     sQuestPlus::getFortHpS( void ) const { return this->__data.FortHpS; }
u16     sQuestPlus::getFortHpL( void ) const { return this->__data.FortHpL; }


////////// Setters //////////

void    sQuestPlus::setIsFence( bool _b ) { this->__data.isFence = _b; }
void    sQuestPlus::setIsFenceFromStart( bool _b ) { this->__data.isFenceFromStart = _b; }
void    sQuestPlus::setFenceOpenTime( u16 _num ) { this->__data.FenceOpenTime = _num; }
void    sQuestPlus::setFenceStartTime( u16 _num ) { this->__data.FenceStartTime = _num; }
void    sQuestPlus::setFenceReuseTime( u16 _num ) { this->__data.FenceReuseTime = _num; }
void    sQuestPlus::setIsDragonator( bool _b ) { this->__data.isDragonator = _b; }
void    sQuestPlus::setDragonatorStartTime( u16 _num ) { this->__data.DragonatorStartTime = _num; }
void    sQuestPlus::setDragonatorReuseTime( u16 _num ) { this->__data.DragonatorReuseTime = _num; }
void    sQuestPlus::setFortHpS( u16 _num ) { this->__data.FortHpS = _num; }
void    sQuestPlus::setFortHpL( u16 _num ) { this->__data.FortHpL = _num; }

} // QDP
} // MH4U
