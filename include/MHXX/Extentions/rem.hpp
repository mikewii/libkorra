#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace REM {
static const u32 RESOURCE_HASH = 0x5B3C302D;

#define REM_ERROR -19

struct sRewardFlag_s {
    s8  Flag;
    s8  ItemNum;
};

struct sRewardItem_s {
    s16 ID;
    s8  Ammount;
    s8  Rate;       // -1 mark last item in sRewardItem_s ?
};

struct sRewardEm_s {
    static const u32 MAGIC          = 0x3F800000;
    static const u32 VERSION        = 1;
    static const u32 REM_ITEMS      = 40;
    static const u32 REM_FLAGS      = 4;
    static const u32 REM_FLAG_NUMS  = 8;

    u32             Magic = MAGIC;
    u32             Version = VERSION;

    sRewardFlag_s   Flag[REM_FLAGS];    // 4 rows?
    s8              FlagNum[8];

    sRewardItem_s   Item[REM_ITEMS];    // 10 per 1 row?
};

class cRewardEm : public PairInfoKeeper
{
public:

    cRewardEm();
    cRewardEm(const Pair& _pp);
    ~cRewardEm();

    void make(Pair& _pp);
    void save(Pair& _pp);

    void print(void) const;
    void print_FlagNums(void) const;
    void print_RewardFlags(void) const;
    void print_RewardItems(bool _useNames = false) const;


    sRewardFlag_s   Get_Flag(const u32 _id) const;
    u8              Get_FlagNum(const u32 _id) const;
    sRewardItem_s   Get_RewardItem(const u32 _id) const;

    bool            Set_Flag(const sRewardFlag_s _flag, const u32 _id);
    bool            Set_FlagNum(const s8 _flagNum, const u32 _id);
    bool            Set_RewardItem(const sRewardItem_s _item, const u32 _id);

private:
    sRewardEm_s     __data;

    u32 getResourceHash(void) { return MHXX::REM::RESOURCE_HASH; }

    friend class Export<cRewardEm>;

};

} // REM
} // MHXX
