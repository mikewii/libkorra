#pragma once
#include "types.h"
#include "Global.hpp"
#include "MHXX/Quest/Common.hpp"

namespace MHXX {
namespace EXT {
static const u32 RESOURCE_HASH = 0x1BBFD18E;

struct sEXTHeader_s {
    static const u32 MAGIC      = 0x434B0000;
    static const u32 VERSION    = 1;

    u32         Magic = MAGIC;          // 0x00
    u32         Version = VERSION;
    u32         Index; // ?
    u32         QuestID;

    QuestType_e QuestType;              // 0x10
    u8          RequestVillage;
    QuestLv_e   QuestLv;
    EnemyLv_e   BossLv;
    Maps_e      MapNo;
    StartType_e StartType;
    u8          QuestTime;
    u8          QuestLifes;

    u8          AcEquipSetNo;           // 0x18
    u8          BGMType;
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

    u8          StrayRand;      // invader boss?
    u8          StrayStartTime;
    u8          StrayStartRand;
    u8          StrayLimit[3];
    u8          StrayRand2[3];
    u8          ExtraTicketNum;

    u8          Icon[5];        // broken, 1 bytes after
    u32         ProgNo;
    u32         Resource;
    u32         Message;
    u32         VillagePointG;
    u8          Flag;
} PACKED;



class cEXT : public PairInfoKeeper
{
public:

    cEXT();
    cEXT(Pair& _pp);
    ~cEXT();

    void print(void);

    void print_Magic(void) const { Utils::print_help("Magic", this->header.Magic); }
    void print_Version(void) const { Utils::print_help("Version", this->header.Version); }
    void print_Index(void) const { Utils::print_help("Index", this->header.Index); }
    void print_QuestID(void) const { Utils::print_help("Quest ID", this->header.QuestID); }
    void print_QuestType(void) const;
    void print_RequestVillage(void) const { Utils::print_help("Request village", this->header.RequestVillage); }
    void print_QuestLevel(void) const;
    void print_BossLevel(void) const;
    void print_Map(void) const;
    void print_StartType(void) const;
    void print_QuestTime(void) const { Utils::print_help("Quest time", this->header.QuestTime); }
    void print_QuestLifes(void) const { Utils::print_help("Quest lifes", this->header.QuestLifes); }
    void print_AcEquipSetNo(void) const { Utils::print_help("AcEquipSetNo", this->header.AcEquipSetNo); }
    void print_BGMType(void) const { Utils::print_help("BGM type", this->header.BGMType); }
    void print_EntryType0(void) const { Utils::print_help("Entry type 0", this->header.EntryType[0]); }
    void print_EntryType1(void) const { Utils::print_help("Entry type 1", this->header.EntryType[1]); }
    void print_EntryTypeCombo(void) const { Utils::print_help("Entry type combo", this->header.EntryTypeCombo); }
    void print_ClearType(void) const { Utils::print_help("Clear type", this->header.ClearType); }
    void print_GekitaiHp(void) const { Utils::print_help("GekitaiHp", this->header.GekitaiHp); }
    void print_TargetMain0(void) const;
    void print_TargetMain1(void) const;
    void print_TargetSub(void) const;
    void print_CarvingLevel(void) const { Utils::print_help("Carging level", this->header.CarvingLv); }
    inline void print_GatheringLevel(void) const { Utils::print_help("Gathering level", this->header.GatherLv); }
    inline void print_FishingLevel(void) const { Utils::print_help("Fishing level", this->header.FishingLv); }
    inline void print_EntryFee(void) const { Utils::print_help("Entry fee", this->header.EntryFee); }
    inline void print_VillagePoints(void) const { Utils::print_help("Village points", this->header.VillagePoints); }
    inline void print_MainRewardMoney(void) const { Utils::print_help("Main reward money", this->header.MainRewardMoney); }
    inline void print_SubRewardMoney(void) const { Utils::print_help("Sub reward money", this->header.SubRewardMoney); }
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
    void print_IsBossRushType(void) const { Utils::print_help("isBossRushType", this->header.isBossRushType); }
    void print_Appear1(void) const { this->print_Appear(1); }
    void print_Appear2(void) const { this->print_Appear(1); }
    void print_Appear3(void) const { this->print_Appear(2); }
    void print_Appear4(void) const { this->print_Appear(3); }
    void print_Appear5(void) const { this->print_Appear(4); }
    void print_Stray(void) const;
    void print_ExtraTicketNum(void) const
    {
        Utils::print_help("ExtraTicketNum", this->header.ExtraTicketNum);
        Utils::print_help("ExtraTicketNum_pos", Utils::GetHeaderRelativePos(&this->header, &this->header.ExtraTicketNum));
    }
    void print_Icons(void) const;
    void print_ProgNo(void) const { Utils::print_help("ProgNo", this->header.ProgNo); }
    void print_Resource(void) const { Utils::print_help("Resource", this->header.Resource); }
    void print_Message(void) const { Utils::print_help("Message", this->header.Message); }
    void print_VillagePointG(void) const { Utils::print_help("VillagePointG", this->header.VillagePointG); }
    void print_Flag(void) const { Utils::print_help("Flag", this->header.Flag); }

private:
    sEXTHeader_s header;

    void read(Pair& _pp);

    void print_Boss(const u32 id) const;
    void print_Appear(const u32 id) const;
    void print_Em(const u32 id) const;
    void print_Icon(const u32 id) const;

};

} // EXT
} // MHXX
