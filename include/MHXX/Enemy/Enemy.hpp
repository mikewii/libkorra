#pragma once
#include "types.h"
#include <array>

namespace MHXX {

struct Enemy {
    static const u32 EM_MAX = 139;

    enum{
        NORMAL      = 0,
        SUB         = 1,        // ex Crimson fatalis
        RARE        = 1 << 1,   // ex Gold rathian
        DEVIANT     = 1 << 2,
        VARIANT     = SUB | DEVIANT,

        ITEM        = 1 << 3,   // some delivery quests have that
        SMALL       = 1 << 4
    };

    struct ID {
        u8  id;
        u8  idSub;
    };

    struct Em {
        const char*     name;
        const Enemy::ID id;
    };
    static const std::array<const Enemy::Em, Enemy::EM_MAX> str;

    static const char* GetStr(const Enemy::ID& _id);
};
} // MHXX
