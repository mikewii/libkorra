#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace SUP {
static const u32 RESOURCE_HASH = 0x54539AEE;

#define SUP_ERROR -18

struct sSupplyItem_s {
    s16 ID;
    s8  Ammount;
    s8  Unk;      // posible: rate, available after time, flags
};

struct sSupply_s {
    const static u32 MAGIC      = 0x3F800000;
    const static u32 VERSION    = 1;
    const static u32 ITEM_MAX   = 40;

    u32             Magic = MAGIC;
    u32             Version = VERSION;

    sSupplyItem_s   Item[ITEM_MAX];
};

class cSupply : public PairInfoKeeper
{
public:

    cSupply();
    cSupply( Pair& _pp );
    ~cSupply();

    void make( Pair& _pp );
    void save( Pair& _pp );

    void print( bool _useNames = false );

    // Getters
    sSupplyItem_s   getItem( u32 _id );

    // Setters
    bool            setItem( u32 _id, sSupplyItem_s& _item );

private:
    sSupply_s   __data;

    u32 getResourceHash( void ) const { return MHXX::SUP::RESOURCE_HASH; }

    friend class Export<cSupply>;

};

} //SUP
} // MHXX
