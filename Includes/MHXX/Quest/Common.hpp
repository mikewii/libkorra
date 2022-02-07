#pragma once
#include "types.h"

namespace MHXX {

static const u32 Special = 100;

enum Maps_e:s8 {
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
    Forlorn_Citadel     = 27,

    ErrorMapID          = 32
};

enum QuestLv_e:u8 {
    Star1 = 1,
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

enum EnemyLv_e:u8 { // signed?
    Dumb,
    LR,
    LR_Plus,
    HR,
    HR_Plus,
    G,
    G_Plus
};

enum QuestType_e:u8 {
    Hunting,
    Slaying,
    Capture,
    Gathering,
    hunt_a_ton,
    hunt_a_ton__arena
};

enum Village_e:u8 { // research
    Kokoto_0        = 1,
    Pokke_0         = 2,
    Yukumo_0        = 3,
    Bherna_0        = 4,
    Kokoto_1        = 5,
    Pokke_1         = 6,
    Yukumo_1        = 7,
    Bherna_1        = 8,
    Prowler         = 9,
    Hunter          = 10,
    SpecialPermit   = 11
};

enum StartType_e:u8 {
    BaseCamp,
    Random,
    ElderDragonFight // ?
};


//  BMGType 1 in these, 0 is everything else
//  q0120002.arc Grudge Match: Gypceros
//  q0120003.arc Grudge Match: Bird Wyverns
//  q0110704.arc Meownster Hunter
//  q0100603.arc Surrounded by Blue and Green
//  q0120006.arc Grudge Match: Congalala
//  q0120005.arc Grudge Match: Tetsucabra
//  q0120001.arc Grudge Match: Great Maccao
//  q0110303.arc Out of the Frying Pan
//  q0111405.arc Meownster Hunter Ultimate
//  q0120004.arc Grudge Match: Barroth
//  q0101005.arc Drome Rush!
enum BGMType_e:u8 {
    DefaultBGM = 0,
    SpecialBGM
};

enum EntryType_e:u8 {
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
