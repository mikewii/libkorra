#pragma once
#include "types.h"
#include "Global.hpp"
#include "MHXX/Enemy/Enemy.hpp"

namespace MHXX {
namespace ESL {
static const u32 RESOURCE_HASH = 0x32CA92F8;

struct sEmSmallData_s {
    EmID        EnemyID;
    u8          Unk0[3];
    u8          Area;
    u16         Unk1;
    Geometry4F  Position;
    u32         Unk2[3];
};

struct sESData_s {
    static const u32 MAGIC0 = 0x00445345; // ESD
    static const u32 MAGIC1 = 0x20151214; // both checked

    u32                 Magic0 = MAGIC0;
    u32                 Magic1 = MAGIC1;

    u16                 EmSmallNum;

    sEmSmallData_s      EmSmall[];
} PACKED;

struct sESLHeader_s { // 0x40 ?
    static const u32 MAGIC  = 0x006C7365; // esl
    static const u32 VERSION = 2; // is it?

    u32     Magic   = MAGIC;
    u32     Version = VERSION;

    u32     Unk[14];
};

class cEmSetList : public PairInfoKeeper
{
public:

    cEmSetList();
    cEmSetList( Pair& _pp );
    ~cEmSetList();

    void print( void );

private:
    sESLHeader_s                                                __header;
    std::vector<std::pair<sESData_s, std::vector<sEmSmallData_s>>>   __ESD;

    void read( Pair& _pp  );


};

} // ESL
} // MHXX
