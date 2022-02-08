#pragma once
#include "types.h"
#include <array>

namespace MHXX {

static const u32 Special = 100;

struct Maps { // complete
    using size = s8;

    enum e:size {
        J_Frontier_D        = 1,
        J_Frontier_N        = J_Frontier_D + Special,
        V_Hills_D           = 2,
        V_Hills_N           = V_Hills_D + Special,
        A_Ridge_D           = 3,
        A_Ridge_N           = A_Ridge_D + Special,
        M_Peaks_D           = 4,
        M_Peaks_N           = M_Peaks_D + Special,
        Dunes               = 5,
        D_Island            = 6,
        Marshlands          = 7,
        Volcano             = 8,
        Arena               = 9,
        V_Slayground        = 10,
        A_Steppe            = 11,
        V_Hollow            = 12,
        Primal_Forest       = 13,
        F_Seaway            = 14,
        F_Slayground        = 15,
        Sanctuary           = 16,
        Forlorn_Arena       = 17,
        S_Pinnacle          = 18,
        Ingle_Isle          = 19,
        Polar_Field         = 20,
        Wyverns_End         = 21,
        Desert              = 22,
        Jungle              = 23,
        Ruined_Pinnacle     = 24,
        Castle_Schrade      = 25,
        Fortress            = 26,
        Forlorn_Citadel     = 27
    };
    static const std::array<const char*, 32> str;

    static const char* GetStr(const size id);
};

struct QuestLv { // complete
    using size = u8;

    enum e:size {
        Training = 0,
        Star1,
        Star2,
        Star3,
        Star4,
        Star5,
        Star6,
        Star7,
        Star8,
        Star9,
        Star10,
        G1,
        G2,
        G3,
        G4,
        G5,
        EX
    };
    static const std::array<const char*, 17> str;

    static const char* GetStr(const size level);
};

struct EnemyLv { // research if overlaping bits do anything
    using size = u8;  // signed?

    enum e:size {
        Training    = 0b000,
        LR          = 0b001,
        HR          = 0b011,
        G           = 0b101
    };
    static const std::array<const char*, 4> str;

    static const char* GetStr(const size level);
};

struct QuestType0 { // complete
    using size = u8;

    enum e:size {
        Hunting = 0,
        Slaying,
        Capture,
        Gathering,
        hunt_a_ton,
        hunt_a_ton_arena,

        dummy0 = 6, // mhgu
        dummy1 = 8  // mhgu
    };
    static const std::array<const char*, 6> str;

    static const char* GetStr(const size type);
};

struct QuestType1 { // complete
    using size = u8;

    enum e:size { // unlockable unlocks after completing main
        Training = 0, // or arena, or dummy

        Kokoto_Unlockable,
        Pokke_Unlockable,
        Yukumo_Unlockable,
        Bherna_Unlockable,

        Kokoto_Main,
        Pokke_Main,
        Yukumo_Main,
        Bherna_Main,

        ProwlerOnly,
        Default,
        SpecialPermit
    };
    static const std::array<const char*, 12> str;

    static const char* GetStr(const size type);
};

struct StartType { // complete
    using size = u8;

    enum e:size {
        BaseCamp = 0,
        Random,
        ElderDragonFight
    };
    static const std::array<const char*, 3> str;

    static const char* GetStr(const size type);
};

struct BGMType { // complete
    using size = u8;

    enum e:size {
        Default = 0,
        ProwlerSpecial,
        Training
    };
    static const std::array<const char*, 3> str;

    static const char* GetStr(const size type);
};

struct EntryType {
    using size = u8;

    enum e:size {
        None,
        HR_2_and_up,
        HR_3_and_up,
        HR_4_and_up,
        HR_5_and_up,
        HR_6_and_up,
        HR_7_and_up,
        HR_8_and_up,
        HR_9_and_up,
        HR_10_and_up,
        HR_11_and_up,
        HR_12_and_up,
        HR_13_and_up,
        HR_20_and_up,
        HR_25_and_up,
        HR_30_and_up,
        HR_45_and_up,
        HR_50_and_up,
        HR_60_and_up,
        HR_100_and_up,
        Great_Swords_only,
        Long_Swords_only,
        Sword_n_Shields_only,
        Dual_Blades_only,
        Lances_only,
        Gunlances_only,
        Hammers_only,
        Hunting_Horns_only,
        Switch_Axes_only,
        Charge_Blades_only,
        Insect_Glaives_only,
        Light_Bowguns_only,
        Heavy_Bowguns_only,
        Bows_only,
        Blademasters_only,
        Gunners_only,
        Guild_Style_only,
        Striker_Style_only,
        Aerial_Style_only,
        Adept_Style_only,
        Valor_Style_only,
        Alchemy_Style_only,
        No_Prowlers,
        Prowlers_only,
        RARE_1_weapons_only,
        No_armor_and_talismans,
        On_site_items_only,
        One_player_max,
        Two_players_max,
        Three_players_max
    };
    static const std::array<const char*, 50> str;

    static const char* GetStr(const size type);
};

struct Target_s {
    u8      isClearParam;   // 1 hunt, 5 deliver
    u16     ClearID;        // monsted id, item id
    u16     ClearNum;
} PACKED;

struct Supply_s {
    u8  SuppLabel;
    u8  SuppType;
    u16	SuppTarget;
    u16	SuppTargetNum;
} PACKED;

struct Boss_s {
    u16 EmType;
    u8  EmSubType;
    u8  AuraType;
    u8  RestoreNum;
    u8  VitalTblNo;
    u8  AttackTblNo;
    u8  OtherTblNo;
    u8  Difficulty;
    u16 Scale;
    u8  ScaleTbl;
    u8  StaminaTbl;
} PACKED;

struct Em_s {
    u8  EmSetType;
    u16	EmSetTargetID;
    u16	EmSetTargetNum;
} PACKED;

struct Appear_s {
    u8	AppearType;
    u16	AppearTargetType;
    u16	AppearTargetNum;
} PACKED;

struct Appear1_s {
    u16	AppearType;
    u16	AppearTargetType;
    u16	AppearTargetNum;
} PACKED;

struct GMDLink_s {
    u32         ProgNo;             // or just padding? always null
    u32         Resource;
    char        GMDFileName[11 + 1];    // 11+1 byte per gmd file
};

}
