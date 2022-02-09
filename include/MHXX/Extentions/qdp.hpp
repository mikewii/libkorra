#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace QDP {
static const u32 RESOURCE_HASH = 0x26BEC21C;

struct sQuestPlus_s {
    static const uint32_t	MAGIC = 0x3F800000; //nope

    u32     Magic = MAGIC;
    u32     Version = 1;

    bool    isFence = false;            // Rail
    bool    isFenceFromStart = false;
    u16     FenceOpenTime = 0;
    u16     FenceStartTime = 0;
    u16     FenceReuseTime = 0;

    bool    isDragonator = false;       // Gekiryu
    u16     DragonatorStartTime = 0;
    u16     DragonatorReuseTime = 0;

    u16     FortHpS = 0;                // fort on laoshan quest?
    u16     FortHpL = 0;

} PACKED;


class cQuestPlus : public PairInfoKeeper
{
public:

    cQuestPlus();
    cQuestPlus(const Pair& _pp);
    ~cQuestPlus(){};

    void print(void) const;

    void make(Pair& _pp);
    void save(Pair& _pp);


    bool    Get_IsFence(void) const;
    bool    Get_IsFenceFromStart(void) const;
    u16     Get_FenceOpenTime(void) const;
    u16     Get_FenceStartTime(void) const;
    u16     Get_FenceReuseTime(void) const;
    bool    Get_IsDragonator(void) const;
    u16     get_DragonatorStartTime(void) const;
    u16     Get_DragonatorReuseTime(void) const;
    u16     Get_FortHpS(void) const;
    u16     Get_FortHpL(void) const;

    void    Set_IsFence(const bool _b);
    void    Set_IsFenceFromStart(const bool _b);
    void    Set_FenceOpenTime(const u16 _num);
    void    Set_FenceStartTime(const u16 _num);
    void    Set_FenceReuseTime(const u16 _num);
    void    Set_IsDragonator(const bool _b);
    void    Set_DragonatorStartTime(const u16 _num);
    void    Set_DragonatorReuseTime(const u16 _num);
    void    Set_FortHpS(const u16 _num);
    void    Set_FortHpL(const u16 _num);

private:
    sQuestPlus_s    __data;

    u32 getResourceHash(void) const { return MHXX::QDP::RESOURCE_HASH; }

    friend class Export<cQuestPlus>;

};

} // QDP
} // MHXX
