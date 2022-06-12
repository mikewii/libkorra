#pragma once
#include "types.h"
#include "Global.hpp"

namespace MHXX {

struct Flags {
    u32     unk[19];
    u32		challenge[2];
    u32		event[5];
};

enum Slot : u8 {
    Slot_1 = 0,
    Slot_2,
    Slot_3
};

struct SaveSlot {
static const u32 SLOT_SIZE = 0x11E7C0;

    char    name[32];

    u8      unk[0x11E7C0 - sizeof(name)];

    void    extract(CContainer& container) const;
};

struct Header {
    static const u32 VERSION = 0xC6;

    u32     version;
    u8      isSlotUsed[3];
    Slot    selectedSlot;
    u32     pData;
    u32     pFlags;
    u32     pSaveSlot[3];
};

struct ArcQuest {
    u8  data[0x1400];
};

struct SaveFile_s {
static const u32 VALID_SAVEFILE_SIZE = 0x481D88; // 4 726 152 bytes

    Header      header;

    u32         unk0[15894];
    // 0xF874

    ArcQuest    quest[200];
    // 0x109874

    u8          unk1[0x1CC00];

    SaveSlot    slot[3];

    u8          unk2[0x1D4]; // nulls at end of file
} PACKED;


class Savefile
{
public:
    Savefile(const CContainer& container);

    void print_Flags(void) const;

//private:
    SaveFile_s  savefile;   // 4.5mb in ram, maybe i use pointer later
};

}
