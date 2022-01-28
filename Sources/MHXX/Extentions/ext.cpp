#include "MHXX/Extentions/ext.hpp"
#include "MHXX/Strings/Common.hpp"

namespace MHXX {
namespace EXT {

cEXT::cEXT()
{
}

cEXT::cEXT(Pair& _pp)
{
    if (_pp.info.ResourceHash == RESOURCE_HASH)
    {
        this->SetPairInfo(_pp);

        this->read(_pp);
    }
    else NotifyError("Pair is not a sEXTHeader_s");
}

cEXT::~cEXT()
{
}

void cEXT::print(void)
{
    this->print_Magic();
    this->print_Version();
    this->print_Index();
    this->print_QuestID();
    /////
    this->print_QuestType();
    this->print_RequestVillage();
    this->print_QuestLevel();
    this->print_BossLevel();
    this->print_Map();
    this->print_StartType();
    this->print_QuestTime();
    this->print_QuestLifes();
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
    this->print_IsHuntaton();

    this->print_Appear1();
    this->print_Appear2();
    this->print_Appear3();
    this->print_Appear4();
    this->print_Appear5();

    this->print_Stray();

    this->print_ExtraTicketNum();

    this->print_Icons();

    this->print_ProgNo();
    this->print_Resource();
    this->print_Message();
    this->print_VillagePointG();
    this->print_Flag();
}

void cEXT::print_QuestType() const
{
    Utils::print_help_arr<decltype (QuestType_str)>("Quest type", this->header.QuestType, QuestType_str);
}

void cEXT::print_QuestLevel() const
{
    Utils::print_help_arr<decltype (QuestLv_str)>("Quest level", this->header.QuestLv, QuestLv_str);
}

void cEXT::print_BossLevel() const
{
    Utils::print_help_arr<decltype (EnemyLv_str)>("Boss level", this->header.BossLv, EnemyLv_str);
}

void cEXT::print_Map(void) const
{
    Utils::print_help_arr<decltype (Maps_str)>("Map", this->header.MapNo, Maps_str);
}

void cEXT::print_StartType() const
{
    Utils::print_help_arr<decltype (StartType_str)>("Start type", this->header.StartType, StartType_str);
}

void cEXT::print_TargetMain0(void) const
{
    printf("\nTarget #0\n");
    Utils::print_help("[0]Is used", this->header.TargetMain[0].isClearParam);
    Utils::print_help("[0]Clear ID", this->header.TargetMain[0].ClearID);
    Utils::print_help("[0]Clear Num", this->header.TargetMain[0].ClearNum);
}

void cEXT::print_TargetMain1(void) const
{
    printf("\nTarget #1\n");
    Utils::print_help("[1]Is used", this->header.TargetMain[1].isClearParam);
    Utils::print_help("[1]Clear ID", this->header.TargetMain[1].ClearID);
    Utils::print_help("[1]Clear Num", this->header.TargetMain[1].ClearNum);
}

void cEXT::print_TargetSub(void) const
{
    printf("\nTarget sub\n");
    Utils::print_help("[sub]Is used", this->header.TargetSub.isClearParam);
    Utils::print_help("[sub]Clear ID", this->header.TargetSub.ClearID);
    Utils::print_help("[sub]Clear Num", this->header.TargetSub.ClearNum);
}

void cEXT::print_Rem(void) const
{
    printf("\nRem\n");
    Utils::print_help("RemAddFrame[0]", this->header.RemAddFrame[0]);
    Utils::print_help("RemAddFrame[1]", this->header.RemAddFrame[1]);
    Utils::print_help("RemAddLotMax", this->header.RemAddLotMax);
}

void cEXT::print_Supply0(void) const
{
    printf("\nSupply #0\n");
    Utils::print_help("[0] SuppLabel", this->header.Supply[0].SuppLabel);
    Utils::print_help("[0] SuppType", this->header.Supply[0].SuppType);
    Utils::print_help("[0] SuppTarget", this->header.Supply[0].SuppTarget);
    Utils::print_help("[0] SuppTargetNum", this->header.Supply[0].SuppTargetNum);
}

void cEXT::print_Supply1(void) const
{
    printf("\nSupply #1\n");
    Utils::print_help("[1] SuppLabel", this->header.Supply[1].SuppLabel);
    Utils::print_help("[1] SuppType", this->header.Supply[1].SuppType);
    Utils::print_help("[1] SuppTarget", this->header.Supply[1].SuppTarget);
    Utils::print_help("[1] SuppTargetNum", this->header.Supply[1].SuppTargetNum);
}


void cEXT::print_Boss(const u32 id) const
{
    printf("\nBoss #%d\n", id);
    Utils::print_help_numered(id, "EmType", this->header.Boss[id].EmType);
    Utils::print_help_numered(id, "EmSubType", this->header.Boss[id].EmSubType);
    Utils::print_help_numered(id, "AuraType", this->header.Boss[id].AuraType);
    Utils::print_help_numered(id, "RestoreNum", this->header.Boss[id].RestoreNum);
    Utils::print_help_numered(id, "VitalTblNo", this->header.Boss[id].VitalTblNo);
    Utils::print_help_numered(id, "AttackTblNo", this->header.Boss[id].AttackTblNo);
    Utils::print_help_numered(id, "OtherTblNo", this->header.Boss[id].OtherTblNo);
    Utils::print_help_numered(id, "Difficulty", this->header.Boss[id].Difficulty);
    Utils::print_help_numered(id, "Scale", this->header.Boss[id].Scale);
    Utils::print_help_numered(id, "ScaleTbl", this->header.Boss[id].ScaleTbl);
    Utils::print_help_numered(id, "StaminaTbl", this->header.Boss[id].StaminaTbl);
}

void cEXT::print_SmallEm(void) const
{
    printf("\nSmallEm\n");
    Utils::print_help("SmallEmHP", this->header.SmallEmHP);
    Utils::print_help("SmallEmAttack", this->header.SmallEmAttack);
    Utils::print_help("SmallEmOther", this->header.SmallEmOther);
}

void cEXT::print_Em(const u32 id) const
{
    printf("\nEm #%d\n", id);
    Utils::print_help_numered(id, "EmSetType", this->header.Em[id].EmSetType);
    Utils::print_help_numered(id, "EmSetTargetID", this->header.Em[id].EmSetTargetID);
    Utils::print_help_numered(id, "EmSetTargetNum", this->header.Em[id].EmSetTargetNum);
}

void cEXT::print_Appear(const u32 id) const
{
    printf("\nAppear #%d\n", id);
    Utils::print_help_numered(id, "AppearType", this->header.Appear[id].AppearType);
    Utils::print_help_numered(id, "AppearTargetType", this->header.Appear[id].AppearTargetType);
    Utils::print_help_numered(id, "AppearTargetNum", this->header.Appear[id].AppearTargetNum);
}

void cEXT::print_Stray(void) const
{
    printf("\nStray\n");
    Utils::print_help("StrayRand", this->header.StrayRand);
    Utils::print_help("StrayStartTime", this->header.StrayStartTime);
    Utils::print_help("StrayStartRand", this->header.StrayStartRand);
    Utils::print_help("StrayLimit[0]", this->header.StrayLimit[0]);
    Utils::print_help("StrayLimit[1]", this->header.StrayLimit[1]);
    Utils::print_help("StrayLimit[2]", this->header.StrayLimit[2]);
    Utils::print_help("StrayRand2[0]", this->header.StrayRand2[0]);
    Utils::print_help("StrayRand2[1]", this->header.StrayRand2[1]);
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

void cEXT::print_Icon(const u32 id) const
{
    Utils::print_help_numered(id, "Icon", this->header.Icon[id]);
}

void cEXT::read(Pair &_pp)
{
    Utils::copybytes(&this->header, _pp.cc.data(), sizeof(sEXTHeader_s));
}

}
} // MHXX
