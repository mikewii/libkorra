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
    cRewardEm( Pair& _pp );
    ~cRewardEm();

    void make( Pair& _pp );
    void save( Pair& _pp );

    void print( void );
    void printFlagNums( void );
    void printRewardFlags( void );
    void printRewardItems( bool _useNames = false );

    //Getters
    sRewardFlag_s   getFlag( u32 _id ) const;
    u8              getFlagNum( u32 _id ) const;
    sRewardItem_s   getRewardItem( u32 _id ) const;

    //Setters
    bool            setFlag( sRewardFlag_s _flag, u32 _id );
    bool            setFlagNum( s8 _flagNum, u32 _id );
    bool            setRewardItem( sRewardItem_s _item, u32 _id );

private:
    sRewardEm_s     __data;

    u32 getResourceHash( void ) { return MHXX::REM::RESOURCE_HASH; }

    friend class Export<cRewardEm>;

};

} // REM
} // MHXX
