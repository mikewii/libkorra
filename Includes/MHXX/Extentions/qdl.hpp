#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace QDL {
static const u32 RESOURCE_HASH  = 0x00E0BB1C;

#define QDL_ERROR 0xF0F0F0F0


enum sQDLItemOrder {
    Boss1,
    Boss2,
    Boss3,
    Boss4,
    Boss5,
    Intruder, // righ order?
    SmallEm1,
    SmallEm2,
    RewardMainA,
    RewardMainB,
    RewardExtraA,
    RewardExtraB,
    RewardSub,
    Supply,
    QuestPlus
};

struct sQDLItem_s {
    static const u32 QDL_NAME_MAX   = 16;

    u32     Resource;
    char    Name[QDL_NAME_MAX];
};

struct sQuestDataLink_s {
    static const u32 MAGIC          = 0x434B0000;
    static const u32 VERSION        = 1;
    static const u32 QDL_ITEMS_MAX  = 15;

    u32         Magic = MAGIC;
    u32         Version = VERSION;

    sQDLItem_s  Item[QDL_ITEMS_MAX];

};


class cQuestDataLink : public PairInfoKeeper
{
public:
    cQuestDataLink();
    cQuestDataLink( Pair& _pp );
    ~cQuestDataLink();

    void make( Pair& _pp );
    void save( Pair& _pp );

    void print( void );
    void printItem( sQDLItem_s& _item );

    // Getters
    sQDLItem_s  getItem( sQDLItemOrder _id ) const;

    // Setters
    void        setItem( sQDLItem_s& _item, sQDLItemOrder _id );

private:
    sQuestDataLink_s    __data;

    u32 getResourceHash( void ) const { return MHXX::QDL::RESOURCE_HASH; }

    friend class Export<cQuestDataLink>;

};

} // QDL
} // MHXX
