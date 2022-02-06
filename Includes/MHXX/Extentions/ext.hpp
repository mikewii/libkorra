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

    u32         Magic = MAGIC;          // 0x00
    u32         Version = VERSION;
    u32         Index; // ?
    u32         QuestID;

    QuestType_e QuestType;              // 0x10
    u8          RequestVillage;         // village id
    QuestLv_e   QuestLv;
    EnemyLv_e   BossLv;
    Maps_e      MapNo;
    StartType_e StartType;
    u8          QuestTime;
    u8          QuestLives;

    u8          AcEquipSetNo;           // arena related? null in every quest
    u8          BGMType;                // used where?
    // print
    EntryType_e EntryType[2];
    u8          EntryTypeCombo;
    u8          ClearType;
    u8          GekitaiHp; // fierce team / small mons?

    Target_s 	TargetMain[2]; // second u8 is u16 ?
    Target_s    TargetSub;

    u8          CarvingLv;
    u8          GatherLv;
    u8          FishingLv; // is u16 ?

    u32         EntryFee;
    u32         VillagePoints;
    u32         MainRewardMoney;
    u32         SubRewardMoney;
    u32         ClearRemVillagePoint;
    u32         FailedRemVillagePoint;
    u32         SubRemVillagePoint;
    u32         ClearRemHunterPoint;
    u32         SubRemHunterPoint;

    // done
    // flags for reward panels
    u8          RemAddFrame[2];
    u8          RemAddLotMax;

    // done, aligned
    Supply_s	Supply[2];  // 0 available from start, 1 available later on quest

    // done, aligned
    Boss_s		Boss[5];

    // done, aligned
    u8          SmallEmHP_tbl;	// also known as Zako
    u8          SmallEmAttack_tbl;
    u8          SmallEmOther_tbl;

    // done, aligned
    Em_s		Em[2];
    u8          isBossRushType; // not

    // done, aligned
    Appear_s	Appear[5];

    u8          StrayRand;      // invader boss id?
    u8          StrayStartTime;
    u8          StrayStartRand;
    u8          StrayLimit[3];
    u8          StrayRand2[3];
    u8          ExtraTicketNum;

    u8          Icon[5];        // broken, 1 bytes after
} PACKED;

struct sEXTHeader_p2_s {
    u8          padding0;
    u8          padding1[4];

    u32         VillagePointG;
    u8          Flag;
} PACKED;


class cEXT : public PairInfoKeeper
{
public:

    cEXT(bool isMHGU = false);
    cEXT(Pair& _pp);
    ~cEXT();

    void make(Pair& _pp);
    bool save(Pair& _pp);

    bool Set_QuestID(const u32 id);
    void Set_QuestType(const QuestType_e type);
    void Set_QuestLevel(const QuestLv_e level);
    void Set_BossLevel(const EnemyLv_e level);
    void Set_MapNo(const Maps_e id);
    void Set_StartType(const StartType_e type);
    void Set_QuestTime(const u8 minutes);
    void Set_QuestLives(const u8 ammount);

    void Set_EntryFee(const u32 ammount);
    void Set_VillagePoints(const u32 ammount);
    void Set_MainRewardMoney(const u32 ammount);
    void Set_SubRewardMoney(const u32 ammount);
    void Set_ClearRemVillagePoint(const u32 ammount);
    void Set_FailedRemVillagePoint(const u32 ammount);
    void Set_SubRemVillagePoint(const u32 ammount);
    void Set_ClearRemHunterPoint(const u32 ammount);
    void Set_SubRemHunterPoint(const u32 ammount);

// prints:
    void print(void) const;

    void print_Magic(void) const { Utils::print_help("Magic", this->header0.Magic); }
    void print_Version(void) const { Utils::print_help("Version", this->header0.Version); }
    void print_Index(void) const { Utils::print_help("Index", this->header0.Index); }
    void print_QuestID(void) const { Utils::print_help("Quest ID", this->header0.QuestID); }
    void print_QuestType(void) const;
    void print_RequestVillage(void) const { Utils::print_help("Request village", this->header0.RequestVillage); }
    void print_QuestLevel(void) const;
    void print_BossLevel(void) const;
    void print_Map(void) const;
    void print_StartType(void) const;
    void print_QuestTime(void) const { Utils::print_help("Quest time", this->header0.QuestTime); }
    void print_QuestLives(void) const { Utils::print_help("Quest lives", this->header0.QuestLives); }
    void print_AcEquipSetNo(void) const { Utils::print_help("AcEquipSetNo", this->header0.AcEquipSetNo); }
    void print_BGMType(void) const { Utils::print_help("BGM type", this->header0.BGMType); }
    void print_EntryType0(void) const { Utils::print_help("Entry type 0", this->header0.EntryType[0]); }
    void print_EntryType1(void) const { Utils::print_help("Entry type 1", this->header0.EntryType[1]); }
    void print_EntryTypeCombo(void) const { Utils::print_help("Entry type combo", this->header0.EntryTypeCombo); }
    void print_ClearType(void) const { Utils::print_help("Clear type", this->header0.ClearType); }
    void print_GekitaiHp(void) const { Utils::print_help("GekitaiHp", this->header0.GekitaiHp); }
    void print_TargetMain0(void) const;
    void print_TargetMain1(void) const;
    void print_TargetSub(void) const;
    void print_CarvingLevel(void) const { Utils::print_help("Carging level", this->header0.CarvingLv); }
    void print_GatheringLevel(void) const { Utils::print_help("Gathering level", this->header0.GatherLv); }
    void print_FishingLevel(void) const { Utils::print_help("Fishing level", this->header0.FishingLv); }
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
    void print_ExtraTicketNum(void) const { Utils::print_help("ExtraTicketNum", this->header0.ExtraTicketNum); }
    void print_Icons(void) const;
    void print_GMDLinks(void) const;
    void print_VillagePointG(void) const { Utils::print_help("VillagePointG", this->header1.VillagePointG); }
    void print_Flag(void) const { Utils::print_help("Flag", this->header1.Flag); }

    sEXTHeader_p1_s header0; // moving to public for test
private:
    //sEXTHeader_p1_s header0;
    sEXTHeader_p2_s header1;
    GMDLink_s       links[5];
    u32             linksAmmount = 1;
    u32             dataSize = 0;

    u32 getResourceHash(void) const { return MHXX::EXT::RESOURCE_HASH; }

    bool read(Pair& _pp);

    u32 GetMapStrID(const Maps_e id) const;
    void print_Boss(const u32 id) const;
    void print_Appear(const u32 id) const;
    void print_Em(const u32 id) const;
    void print_Icon(const u32 id) const;
    void print_GMDLink(const u32 id) const;
};

} // EXT
} // MHXX
