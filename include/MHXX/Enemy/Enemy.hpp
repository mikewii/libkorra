#pragma once
#include "types.h"

namespace MHXX {

enum{
    NORMAL      = 0,
    SUB         = 1,       // ex Crimson fatalis
    RARE        = 1 << 1,  // ex Gold rathian
    DEVIANT     = 1 << 2,
    VARIANT     = SUB | DEVIANT,
    SMALL       = 1 << 4
};

struct EmID {
    u8  ID;
    u8  IDSub;
};

const char* getEnemyStr( EmID& _id );

} // MHXX
