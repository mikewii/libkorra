#pragma once
#include "types.h"

namespace MHXX {

enum Maps_e:u8 {
    J_Frontier_D,
    J_Frontier_N,
    V_Hills_D,
    V_Hills_N,
    A_Ridge_D,
    A_Ridge_N,
    M_Peaks_D,
    M_Peaks_N,
    Dunes,
    D_Island,
    Marshlands,
    Volcano,
    A_Steppe,
    Primal_Forest,
    F_Seaway,
    V_Hollow,
    Forlorn_Arena,
    Sanctuary,
    Ingle_Isle,
    Polar_Field,
    S_Pinnacle,
    Wyverns_End,
    Arena,
    V_Slayground,
    F_Slayground,
    Desert,
    Jungle,
    Ruined_Pinnacle,
    Castle_Schrade,
    Fortress,
    Forlorn_Citadel
};

enum QuestLv_e:u8 {
    Star1 = 0,
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
    Special
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

enum StartType_e:u8 {
    BaseCamp,
    Random,
    ElderDragonFight // ?
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
    u8      isClearParam;
    u16     ClearID;
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
    char        Message[11 + 1];    // 11+1 byte per gmd file
};

}
