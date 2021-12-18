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
    cQuestPlus(Pair& _pp);
    ~cQuestPlus(){};

    void print(void);

    void make(Pair& _pp);
    void save(Pair& _pp);

    // Getters
    bool    getIsFence(void) const;
    bool    getIsFenceFromStart(void) const;
    u16     getFenceOpenTime(void) const;
    u16     getFenceStartTime(void) const;
    u16     getFenceReuseTime(void) const;
    bool    getIsDragonator(void) const;
    u16     getDragonatorStartTime(void) const;
    u16     getDragonatorReuseTime(void) const;
    u16     getFortHpS(void) const;
    u16     getFortHpL(void) const;


    // Setters
    void    setIsFence(bool _b);
    void    setIsFenceFromStart(bool _b);
    void    setFenceOpenTime(u16 _num);
    void    setFenceStartTime(u16 _num);
    void    setFenceReuseTime(u16 _num);
    void    setIsDragonator(bool _b);
    void    setDragonatorStartTime(u16 _num);
    void    setDragonatorReuseTime(u16 _num);
    void    setFortHpS(u16 _num);
    void    setFortHpL(u16 _num);

private:
    sQuestPlus_s    __data;

    u32 getResourceHash(void) const { return MHXX::QDP::RESOURCE_HASH; }

    friend class Export<cQuestPlus>;

};

} // QDP
} // MHXX
