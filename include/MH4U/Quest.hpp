#pragma once
#include "types.h"

#include "MH4U.hpp"

namespace MH4U {
#define QUEST_SIZE 0x2010

struct sFlags {

    u8  QuestType; // quest type
    u8  AllowRepel; // repel related
    u8  SubAsMain; // subquest related
    u8  Flag4; // ship integrity
    u32 PostingFee;
    u32 RewardZenny;
    u32 PenaltyZenny;
    u32 SubRewardZenny;
    u32 QuestTime; // minutes
    u32 IntruderMonsterProbability;
    u32 p_TextLanguages;
    u16 QuestID;
    u16 QuestStars;
    u8  Map;
    u8  Requirements[2];
    u8  unk1;
    u32 unk2;
    u32 MainObjectiveType1;
    u16 MainObjectiveID1;
    u16 MainObjectiveCount1;
    u32 MainObjectiveType2;
    u16 MainObjectiveID2;
    u16 MainObjectiveCount2;
    u32 SubObjectiveType;
    u16 SubObjectiveID;
    u16 SubObjectiveCount;
    u32 pChallengePresets;
    u16 QuestIconID[5];
    u16 unk3;
};

struct MonsterStats {
    u16	SizePercentage;
    u8  SizeVariation;
    u8  Health;
    u8  Attack;
    u8  Defence;
    u8  Stamina;
    u8  Unk;
};

struct SmallMonsterUnk  {
    u32	unk1;
    u16	unk2;
    u8  unk3;
    u8  unk4;
};

union HRPoints {
    u32	u;
    s32 s;
};

struct sQuest {
    u32             p_Flags;
    char        	version[4];
    u32             p_SupplyBox;
    u8              RefillBoxSettings[4];
    u32             p_RefillBox1;
    u32             p_RefillBox2;
    u32             p_RefillBox3;
    u32             p_MainRewardABox;		// 0x1C
    u32             p_MainRewardBBox;
    u32             p_SubRewardBox;
    u32         	p_LargeMonsterWaves;	// 0x28
    u32         	p_SmallMonsterWaves;
    u32         	p_IntruderMonsterWaves;
    MonsterStats	Boss1Stats;
    MonsterStats	Boss2Stats;
    MonsterStats	Boss3Stats;
    MonsterStats	Boss4Stats;
    MonsterStats	Boss5Stats;
    MonsterStats	SmallMonsStats;
    SmallMonsterUnk	SmallMonsUnk[2];
    HRPoints    	RewardHRP;
    HRPoints    	PenaltyHRP;
    HRPoints    	SubRewardHRP;
    u8              unk3[8];
    u8              unk4; // 0x6C
    u8              GatheringRank;
    u8              CarvingRank;
    u8              MonstersAI;
    u8              PlayerSpawn;
    u8              ArenaFence;
    u8              unk5[2];
    u32             unk6[4];

    bool        check_version(void) const;
    sFlags*     GetFlags(void);

    static void ExtractQuests_Directory(const std::filesystem::path& dir);
    static void ExtractQuests_File(const std::filesystem::path& path);
};

}
