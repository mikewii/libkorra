#pragma once
#include "types.h"
#include "Global.hpp"
#include "MHXX/Quest/Common.hpp"

namespace MHXX {
namespace EXT {
static const u32 RESOURCE_HASH = 0x1BBFD18E;
static const u32 MHXX_EXT_SIZE = 0xF9;
static const u32 MHGU_EXT_SIZE = 0x149;

struct sEXTHeader_p1_s {
    static const u32 MAGIC      = 0x434B0000;
    static const u32 VERSION    = 1;

    u32             magic = MAGIC;
    u32             version = VERSION;
    u32             index;                  // ?
    u32             questID;

    QuestType0::e   questType0;
    QuestType1::e   questType1;
    QuestLv::e      questLv;
    EnemyLv::e      bossLv;
    Maps::e         mapNo;
    StartType::e    startType;

    u8              questTime;
    u8              questLives;

    u8              acEquipSetNo;           // null in every quest | arena related?
    BGMType::e      bgmType;

    EntryType::e    entryType[2];
    u8              entryTypeCombo;         //  null in every quest
    ClearType::e    clearType;
    u8              gekitaiHp; // fierce team / small mons? / repel, oppose

    Target_s        TargetMain[2];
    Target_s        TargetSub;

    CarvingLv::e    carvingLv;
    GatheringLv::e  gatherLv;
    FishingLv::e    fishingLv;

    u32             EntryFee;
    u32             VillagePoints;
    u32             MainRewardMoney;
    u32             SubRewardMoney;
    u32             ClearRemVillagePoint;
    u32             FailedRemVillagePoint;
    u32             SubRemVillagePoint;
    u32             ClearRemHunterPoint;
    u32             SubRemHunterPoint;

    // done
    // flags for reward panels
    u8              RemAddFrame[2]; // 0 - 4
    s8              RemAddLotMax;   // -2 -1 0 1 2 4

    // done, aligned
    Supply_s        Supply[2];  // 0 available from start, 1 available later on quest

    // done, aligned
    Boss_s          Boss[5];

    // done, aligned
    u8              SmallEmHP_tbl;      // also known as Zako / bits?
    u8              SmallEmAttack_tbl;  // bits?
    u8              SmallEmOther_tbl;   // bits?

    // done, aligned
    Em_s            Em[2];
    u8              isBossRushType;     // 0 - 8

    // done, aligned
    Appear_s        Appear[5];

    u8              invader_AppearChance;       // invader boss id?
    u8              invader_StartTime;
    u8              invader_StartRandChance;
    u8              strayLimit[3];              // bits?
    u8              strayRand2[3];              // chance 0 - 100
    u8              sp_ExtraTicketNum;          // 0 - 10

    Icon::e         icon[5];                    // 0 - 186
} PACKED;

struct sEXTHeader_p2_s {
    u8          padding0;
    u8          padding1[4];

    s32         VillagePointG;

    union{
        u8 raw;

        // possible values: 0, 1, 2, 3, 4, 8, 12, 16
        struct{
            u8 b0 : 1;
            u8 b1 : 1;
            u8 b2 : 1;
            u8 b3 : 1;
            u8 b4 : 1;
        };
    } Flag;

} PACKED;


class cEXT : public PairInfoKeeper
{
public:

    cEXT(bool isMHGU = false);
    cEXT(const Pair& _pp);
    ~cEXT();

    void make(Pair& _pp);
    bool save(Pair& _pp);

    bool Set_QuestID(const u32 id);
    void Set_QuestType0(const QuestType0::e type);
    void Set_QuestType1(const QuestType1::e type);
    void Set_QuestLevel(const QuestLv::e level, const bool SpecialPermit = false);
    void Set_BossLevel(const EnemyLv::e level);
    void Set_MapNo(const Maps::e id);
    void Set_StartType(const StartType::e type);
    void Set_QuestTime(const u8 minutes);
    void Set_QuestLives(const u8 ammount);

    void Set_AcEquipSetNo(const u8 value);
    void Set_BGMType(const BGMType::e type);

    void Set_EntryType0(const EntryType::e type) { this->Set_EntryType(type, 0); }
    void Set_EntryType1(const EntryType::e type) { this->Set_EntryType(type, 1); }
    //void Set_EntryTypeCombo(const bool isUseBoth); // nope
    void Set_ClearType(const ClearType::e type) { this->header0.clearType = type; }


    void Set_EntryFee(const u32 ammount);
    void Set_VillagePoints(const u32 ammount);
    void Set_MainRewardMoney(const u32 ammount);
    void Set_SubRewardMoney(const u32 ammount);
    void Set_ClearRemVillagePoint(const u32 ammount);
    void Set_FailedRemVillagePoint(const u32 ammount);
    void Set_SubRemVillagePoint(const u32 ammount);
    void Set_ClearRemHunterPoint(const u32 ammount);
    void Set_SubRemHunterPoint(const u32 ammount);

    void Set_Icon0(const Icon::e icon) { cEXT::Set_Icon(icon, 0); }
    void Set_Icon1(const Icon::e icon) { cEXT::Set_Icon(icon, 1); }
    void Set_Icon2(const Icon::e icon) { cEXT::Set_Icon(icon, 2); }
    void Set_Icon3(const Icon::e icon) { cEXT::Set_Icon(icon, 3); }
    void Set_Icon4(const Icon::e icon) { cEXT::Set_Icon(icon, 4); }


    void print(void) const;

    void print_Magic(void) const { Utils::print_help("Magic", this->header0.magic); }
    void print_Version(void) const { Utils::print_help("Version", this->header0.version); }
    void print_Index(void) const { Utils::print_help("Index", this->header0.index); }
    void print_QuestID(void) const { Utils::print_help("Quest ID", this->header0.questID); }
    void print_QuestType0(void) const;
    void print_QuestType1(void) const;
    void print_QuestLevel(void) const;
    void print_BossLevel(void) const;
    void print_Map(void) const;
    void print_StartType(void) const;
    void print_QuestTime(void) const { Utils::print_help("Quest time", this->header0.questTime); }
    void print_QuestLives(void) const { Utils::print_help("Quest lives", this->header0.questLives); }
    void print_AcEquipSetNo(void) const { Utils::print_help("AcEquipSetNo", this->header0.acEquipSetNo); }
    void print_BGMType(void) const { Utils::print_help("BGM type", this->header0.bgmType); }
    void print_EntryType0(void) const { Utils::print_help("Entry type 0", this->header0.entryType[0]); }
    void print_EntryType1(void) const { Utils::print_help("Entry type 1", this->header0.entryType[1]); }
    void print_EntryTypeCombo(void) const { Utils::print_help("Entry type combo", this->header0.entryTypeCombo); }
    void print_ClearType(void) const { Utils::print_help("Clear type", this->header0.clearType); }
    void print_GekitaiHp(void) const { Utils::print_help("GekitaiHp", this->header0.gekitaiHp); }
    void print_TargetMain0(void) const;
    void print_TargetMain1(void) const;
    void print_TargetSub(void) const;
    void print_CarvingLevel(void) const { Utils::print_help("Carging level", this->header0.carvingLv); }
    void print_GatheringLevel(void) const { Utils::print_help("Gathering level", this->header0.gatherLv); }
    void print_FishingLevel(void) const { Utils::print_help("Fishing level", this->header0.fishingLv); }
    void print_EntryFee(void) const { Utils::print_help("Entry fee", this->header0.EntryFee); }
    void print_VillagePoints(void) const { Utils::print_help("Village points", this->header0.VillagePoints); }
    void print_MainRewardMoney(void) const { Utils::print_help("Main reward money", this->header0.MainRewardMoney); }
    void print_SubRewardMoney(void) const { Utils::print_help("Sub reward money", this->header0.SubRewardMoney); }
    void print_ClearRemVillagePoint(void) const { Utils::print_help("ClearRemVillagePoint", this->header0.ClearRemVillagePoint); }
    void print_FailedRemVillagePoint(void) const { Utils::print_help("FailedRemVillagePoint", this->header0.FailedRemVillagePoint); }
    void print_SubRemVillagePoint(void) const { Utils::print_help("SubRemVillagePoint", this->header0.SubRemVillagePoint); }
    void print_ClearRemHunterPoint(void) const { Utils::print_help("ClearRemHunterPoint", this->header0.ClearRemHunterPoint); }
    void print_SubRemHunterPoint(void) const { Utils::print_help("SubRemHunterPoint", this->header0.SubRemHunterPoint); }
    void print_Rem(void) const;
    void print_Supply0(void) const;
    void print_Supply1(void) const;
    void print_Boss1(void) const { this->print_Boss(0); };
    void print_Boss2(void) const { this->print_Boss(1); };
    void print_Boss3(void) const { this->print_Boss(2); };
    void print_Boss4(void) const { this->print_Boss(3); };
    void print_Boss5(void) const { this->print_Boss(4); };
    void print_SmallEm(void) const;
    void print_Em0(void) const { this->print_Em(0); }
    void print_Em1(void) const { this->print_Em(1); }
    void print_IsBossRushType(void) const { Utils::print_help("isBossRushType", this->header0.isBossRushType); }
    void print_Appear1(void) const { this->print_Appear(0); }
    void print_Appear2(void) const { this->print_Appear(1); }
    void print_Appear3(void) const { this->print_Appear(2); }
    void print_Appear4(void) const { this->print_Appear(3); }
    void print_Appear5(void) const { this->print_Appear(4); }
    void print_Stray(void) const;
    void print_SpecialPermitExtraTicketNum(void) const { Utils::print_help("ExtraTicketNum", this->header0.sp_ExtraTicketNum); }
    void print_Icons(void) const;
    void print_GMDLinks(void) const;
    void print_VillagePointG(void) const { Utils::print_help("VillagePointG", this->header1.VillagePointG); }
    void print_Flag(void) const { Utils::print_help("Flag", this->header1.Flag.raw); }

    const sEXTHeader_p1_s& GetHeader0(void) const { return this->header0; }
    const sEXTHeader_p2_s& GetHeader1(void) const { return this->header1; }

private:
    sEXTHeader_p1_s header0;
    sEXTHeader_p2_s header1;
    GMDLink_s       links[5];
    u32             linksAmmount = 1;
    u32             dataSize = 0;

    u32 getResourceHash(void) const { return MHXX::EXT::RESOURCE_HASH; }

    bool read(const Pair& _pp);

    void Set_EntryType(const EntryType::e type, const u8 id);
    void Set_Icon(const Icon::e icon, const u32 id);

    void print_Boss(const u32 id) const;
    void print_Appear(const u32 id) const;
    void print_Em(const u32 id) const;
    void print_Icon(const u32 id) const;
    void print_GMDLink(const u32 id) const;
};

} // EXT
} // MHXX
