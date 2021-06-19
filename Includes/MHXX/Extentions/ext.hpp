#pragma once
#include "types.h"
#include "Global.hpp"
#include "MHXX/Quest/Common.hpp"

namespace MHXX {
namespace EXT {
static const u32 RESOURCE_HASH = 0x1BBFD18E;





struct sEXTHeader_s {
    static const u32 MAGIC      = 0x434B0000;
    static const u32 VERSION    = 1;

    u32         Magic = MAGIC;
    u32         Version = VERSION;

    u32         Index; // ?
    u32         QuestID;
    QuestType   QuestType;
    u8          RequestVillage;
    QuestLv     QuestLv;
    EnemyLv     MonsterLv;
    Maps        MapNo;
    StartType   StartType;
    u8          QuestTime;
    u8          QuestLife;
    u8          AcEquipSetNo;
    u8          BMGType;
    EntryType   EntryType[2];
    u8          EntryTypeCombo;
    u8          ClearType;
    u8          GekitaiHp; // fierce team / small mons?

};



class cEXT
{
public:

    cEXT();
    cEXT( Pair& _pp );
    ~cEXT();

    void print( void );

private:


};

} // EXT
} // MHXX
