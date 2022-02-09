#include "MHXX/Extentions/ext.hpp"
#include "MHXX/Strings/Common.hpp"

namespace MHXX {
namespace EXT {

cEXT::cEXT(bool isMHGU)
{
    if (isMHGU) this->dataSize = EXT::MHGU_EXT_SIZE;
    else this->dataSize = EXT::MHXX_EXT_SIZE;
}

cEXT::cEXT(const Pair& _pp)
{
    if (_pp.info.ResourceHash == RESOURCE_HASH)
    {
        this->Set_PairInfo(_pp);

        this->read(_pp);
    }
    else NotifyError("Pair is not a sEXTHeader_s");
}

cEXT::~cEXT()
{
}

bool cEXT::save(Pair& _pp)
{
    if (this->dataSize == EXT::MHXX_EXT_SIZE) this->linksAmmount = 1;
    else if (this->dataSize == EXT::MHGU_EXT_SIZE) this->linksAmmount = 5;
    else return false;

    _pp.cc.resize(this->dataSize);

    Utils::copybytes(
                _pp.cc.data(),
                &this->header0,
                sizeof(sEXTHeader_p1_s));
    Utils::copybytes(
                _pp.cc.data() + sizeof(sEXTHeader_p1_s),
                &this->links,
                sizeof(GMDLink_s));
    Utils::copybytes(
                _pp.cc.data() + sizeof(sEXTHeader_p1_s) + (sizeof(GMDLink_s) * this->linksAmmount),
                &this->header1,
                sizeof(sEXTHeader_p2_s));

    if (cEXT::isPairInfoSet()) cEXT::GetPairInfo(_pp);
    else
    {
        _pp.info.DecSize        = this->dataSize;
        _pp.info.ResourceHash   = cEXT::getResourceHash();
        _pp.info.XORLock        = MHXX_XORLock;
        _pp.info.isDecompressed = true;
    }

    return true;
}
bool cEXT::Set_QuestID(const u32 id)
{
    // id check?
    this->header0.questID = id;
    return true;
}

void cEXT::Set_QuestType0(const QuestType0::e type) { this->header0.questType0 = type; }
void cEXT::Set_QuestType1(const QuestType1::e type) { this->header0.questType1 = type; }
void cEXT::Set_QuestLevel(const QuestLv::e level, const bool SpecialPermit)
{
    if (SpecialPermit) this->header0.questLv = QuestLv::e(level + MHXX::Special);
    else this->header0.questLv = level;
}
void cEXT::Set_BossLevel(const EnemyLv::e level) { this->header0.bossLv = level; }
void cEXT::Set_MapNo(const Maps::e id) { this->header0.mapNo = id; }
void cEXT::Set_StartType(const StartType::e type) { this->header0.startType = type; }
void cEXT::Set_QuestTime(const u8 minutes) { this->header0.questTime = minutes; }
void cEXT::Set_QuestLives(const u8 ammount) { this->header0.questLives = ammount; }
void cEXT::Set_AcEquipSetNo(const u8 value) { this->header0.acEquipSetNo = value; }
void cEXT::Set_BGMType(const BGMType::e type) { this->header0.bgmType = type; }
void cEXT::Set_EntryType(const EntryType::e type, const u8 id) { this->header0.entryType[id] = type; }
//void cEXT::Set_EntryTypeCombo(const bool isUseBoth) { this->header0.EntryTypeCombo = isUseBoth; }
void cEXT::Set_EntryFee(const u32 ammount) { this->header0.EntryFee = ammount; }
void cEXT::Set_VillagePoints(const u32 ammount) { this->header0.VillagePoints = ammount; }
void cEXT::Set_MainRewardMoney(const u32 ammount) { this->header0.MainRewardMoney = ammount; }
void cEXT::Set_SubRewardMoney(const u32 ammount) { this->header0.SubRewardMoney = ammount; }
void cEXT::Set_ClearRemVillagePoint(const u32 ammount) { this->header0.ClearRemVillagePoint = ammount; }
void cEXT::Set_FailedRemVillagePoint(const u32 ammount) { this->header0.FailedRemVillagePoint = ammount; }
void cEXT::Set_SubRemVillagePoint(const u32 ammount) { this->header0.SubRemVillagePoint = ammount; }
void cEXT::Set_ClearRemHunterPoint(const u32 ammount) { this->header0.ClearRemHunterPoint = ammount; }
void cEXT::Set_SubRemHunterPoint(const u32 ammount) { this->header0.SubRemHunterPoint = ammount; }
void cEXT::Set_Icon(const Icon::e icon, const u32 id) { this->header0.icon[id] = icon; }

void cEXT::print(void) const
{
    this->print_Magic();
    this->print_Version();
    this->print_Index();
    this->print_QuestID();
    /////
    this->print_QuestType0();
    this->print_QuestType1();
    this->print_QuestLevel();
    this->print_BossLevel();
    this->print_Map();
    this->print_StartType();
    this->print_QuestTime();
    this->print_QuestLives();
    this->print_AcEquipSetNo();
    this->print_BGMType();
    this->print_EntryType0();
    this->print_EntryType1();
    this->print_EntryTypeCombo();
    this->print_ClearType();
    this->print_GekitaiHp();

    // Target
    this->print_TargetMain0();
    this->print_TargetMain1();
    this->print_TargetSub();
    // /Target

    this->print_CarvingLevel();
    this->print_GatheringLevel();
    this->print_FishingLevel();

    this->print_EntryFee();
    this->print_VillagePoints();
    this->print_MainRewardMoney();
    this->print_SubRewardMoney();

    this->print_ClearRemVillagePoint();
    this->print_FailedRemVillagePoint();
    this->print_SubRemVillagePoint();
    this->print_ClearRemHunterPoint();
    this->print_SubRemHunterPoint();

    this->print_Rem();

    this->print_Supply0();
    this->print_Supply1();

    this->print_Boss1();
    this->print_Boss2();
    this->print_Boss3();
    this->print_Boss4();
    this->print_Boss5();

    this->print_SmallEm();
    this->print_Em0();
    this->print_Em1();

    printf("\n");
    this->print_IsBossRushType();

    this->print_Appear1();
    this->print_Appear2();
    this->print_Appear3();
    this->print_Appear4();
    this->print_Appear5();

    this->print_Stray();

    this->print_SpecialPermitExtraTicketNum();

    this->print_Icons();

    this->print_GMDLinks();


    this->print_VillagePointG();
    this->print_Flag();
}

void cEXT::print_QuestType0(void) const
{
    Utils::print_help_arr<decltype (QuestType0::str)>("Quest type0", this->header0.questType0, QuestType0::str);
}

void cEXT::print_QuestType1(void) const
{
    Utils::print_help_arr<decltype (QuestType1::str)>("Quest type1", this->header0.questType1 - 1, QuestType1::str);
}

void cEXT::print_QuestLevel(void) const
{
    u32 level = this->header0.questLv;

    if (this->header0.questLv > MHXX::Special) level -= MHXX::Special;
    Utils::print_help_arr<decltype (QuestLv::str)>("Quest level", level, QuestLv::str);
}

void cEXT::print_BossLevel(void) const
{
    Utils::print_help_arr<decltype (EnemyLv::str)>("Boss level", this->header0.bossLv, EnemyLv::str);
}

void cEXT::print_Map(void) const
{
    printf("%-25s%s | %08X|%d\n", "Map", Maps::GetStr(this->header0.mapNo), this->header0.mapNo, this->header0.mapNo);
}

void cEXT::print_StartType(void) const
{
    Utils::print_help_arr<decltype (StartType::str)>("Start type", this->header0.startType, StartType::str);
}

void cEXT::print_TargetMain0(void) const
{
    printf("\nTarget #0\n");
    Utils::print_help("[0]Is used", this->header0.TargetMain[0].isClearParam);
    Utils::print_help("[0]Clear ID", this->header0.TargetMain[0].Target_Item);
    Utils::print_help("[0]Clear Num", this->header0.TargetMain[0].ClearNum);
}

void cEXT::print_TargetMain1(void) const
{
    printf("\nTarget #1\n");
    Utils::print_help("[1]Is used", this->header0.TargetMain[1].isClearParam);
    Utils::print_help("[1]Clear ID", this->header0.TargetMain[1].Target_Item);
    Utils::print_help("[1]Clear Num", this->header0.TargetMain[1].ClearNum);
}

void cEXT::print_TargetSub(void) const
{
    printf("\nTarget sub\n");
    Utils::print_help("[sub]Is used", this->header0.TargetSub.isClearParam);
    Utils::print_help("[sub]Clear ID", this->header0.TargetSub.Target_Item);
    Utils::print_help("[sub]Clear Num", this->header0.TargetSub.ClearNum);
}

void cEXT::print_Rem(void) const
{
    printf("\nRem\n");
    Utils::print_help("RemAddFrame[0]", this->header0.RemAddFrame[0]);
    Utils::print_help("RemAddFrame[1]", this->header0.RemAddFrame[1]);
    Utils::print_help("RemAddLotMax", this->header0.RemAddLotMax);
}

void cEXT::print_Supply0(void) const
{
    printf("\nSupply #0\n");
    Utils::print_help("[0] SuppLabel", this->header0.Supply[0].Label);
    Utils::print_help("[0] SuppType", this->header0.Supply[0].Type);
    Utils::print_help_str("[0] SuppTarget", Enemy::GetStr(this->header0.Supply[0].Target_Em), this->header0.Supply[0].Target_Item);
    Utils::print_help("[0] SuppTargetNum", this->header0.Supply[0].TargetNum);
}

void cEXT::print_Supply1(void) const
{
    printf("\nSupply #1\n");
    Utils::print_help("[1] SuppLabel", this->header0.Supply[1].Label);
    Utils::print_help("[1] SuppType", this->header0.Supply[1].Type);
    Utils::print_help_str("[1] SuppTarget", Enemy::GetStr(this->header0.Supply[1].Target_Em), this->header0.Supply[1].Target_Item);
    Utils::print_help("[1] SuppTargetNum", this->header0.Supply[1].TargetNum);
}


void cEXT::print_Boss(const u32 id) const
{
    printf("\nBoss #%d\n", id);
    Utils::print_help_str_numered(id, "EmType", Enemy::GetStr(this->header0.Boss[id].EmType), this->header0.Boss[id].EmType.id);
    Utils::print_help_numered(id, "EmSubType", this->header0.Boss[id].EmSubType);
    Utils::print_help_numered(id, "AuraType", this->header0.Boss[id].AuraType);
    Utils::print_help_numered(id, "RestoreNum", this->header0.Boss[id].RestoreNum);
    Utils::print_help_numered(id, "VitalTblNo", this->header0.Boss[id].VitalTblNo);
    Utils::print_help_numered(id, "AttackTblNo", this->header0.Boss[id].AttackTblNo);
    Utils::print_help_numered(id, "OtherTblNo", this->header0.Boss[id].OtherTblNo);
    Utils::print_help_numered(id, "Difficulty", this->header0.Boss[id].Difficulty);
    Utils::print_help_numered(id, "Scale", this->header0.Boss[id].Scale);
    Utils::print_help_numered(id, "ScaleTbl", this->header0.Boss[id].ScaleTbl);
    Utils::print_help_numered(id, "StaminaTbl", this->header0.Boss[id].StaminaTbl);
}

void cEXT::print_SmallEm(void) const
{
    printf("\nSmallEm\n");
    Utils::print_help("SmallEmHP", this->header0.SmallEmHP_tbl);
    Utils::print_help("SmallEmAttack", this->header0.SmallEmAttack_tbl);
    Utils::print_help("SmallEmOther", this->header0.SmallEmOther_tbl);
}

void cEXT::print_Appear(const u32 id) const
{
    printf("\nAppear #%d\n", id);
    Utils::print_help_numered(id, "AppearType", this->header0.Appear[id].AppearType);
    Utils::print_help_numered(id, "AppearTargetType", this->header0.Appear[id].AppearTargetType);
    Utils::print_help_numered(id, "AppearTargetNum", this->header0.Appear[id].AppearTargetNum);
}

void cEXT::print_Stray(void) const
{
    printf("\nStray\n");
    Utils::print_help("StrayRand", this->header0.invader_AppearChance);
    Utils::print_help("StrayStartTime", this->header0.invader_StartTime);
    Utils::print_help("StrayStartRand", this->header0.invader_StartRandChance);
    Utils::print_help("StrayLimit[0]", this->header0.strayLimit[0]);
    Utils::print_help("StrayLimit[1]", this->header0.strayLimit[1]);
    Utils::print_help("StrayLimit[2]", this->header0.strayLimit[2]);
    Utils::print_help("StrayRand2[0]", this->header0.strayRand2[0]);
    Utils::print_help("StrayRand2[1]", this->header0.strayRand2[1]);
    Utils::print_help("StrayRand2[2]", this->header0.strayRand2[2]);
}

void cEXT::print_Icons(void) const
{
    printf("\nIcons\n");
    this->print_Icon(0);
    this->print_Icon(1);
    this->print_Icon(2);
    this->print_Icon(3);
    this->print_Icon(4);
}

void cEXT::print_GMDLinks(void) const
{
    printf("\nGMDLinks");

    switch(cEXT::dataSize){
    default:break;
    case MHXX_EXT_SIZE:{
        this->print_GMDLink(0);
        break;
    }
    case MHGU_EXT_SIZE:{
        for (auto i = 0; i < 5; i++)
            this->print_GMDLink(i);
        break;
    }
    }
}

void cEXT::print_Em(const u32 id) const
{
    printf("\nEm #%d\n", id);
    Utils::print_help_numered(id, "EmSetType", this->header0.Em[id].EmSetType);
    Utils::print_help_str_numered(id, "EmSetTargetID", Enemy::GetStr(this->header0.Em[id].EmSetTargetID), this->header0.Em[id].EmSetTargetID.id);
    Utils::print_help_numered(id, "EmSetTargetNum", this->header0.Em[id].EmSetTargetNum);
}

void cEXT::print_Icon(const u32 id) const
{
    Utils::print_help_numered(id, "Icon", this->header0.icon[id]);
}

void cEXT::print_GMDLink(const u32 id) const
{
    printf("\nGMDLink #%d\n", id);
    Utils::print_help("ProgNo", this->links[id].ProgNo);
    Utils::print_help("Resource", this->links[id].Resource);
    printf("%-25s%s\n", "Message", this->links[id].GMDFileName);
}

bool cEXT::read(const Pair &_pp)
{
    cEXT::dataSize = _pp.cc.size();

    if (this->dataSize == EXT::MHXX_EXT_SIZE) this->linksAmmount = 1;
    else if (this->dataSize == EXT::MHGU_EXT_SIZE) this->linksAmmount = 5;
    else return false;

    Utils::copybytes(
                &this->header0,
                _pp.cc.data(),
                sizeof(sEXTHeader_p1_s));
    Utils::copybytes(
                &this->links,
                _pp.cc.data() + sizeof(sEXTHeader_p1_s),
                sizeof(GMDLink_s));
    Utils::copybytes(
                &this->header1,
                _pp.cc.data() + sizeof(sEXTHeader_p1_s) + (sizeof(GMDLink_s) * this->linksAmmount),
                sizeof(sEXTHeader_p2_s));

    return true;
}

}
} // MHXX
