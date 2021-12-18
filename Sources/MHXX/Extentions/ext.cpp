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


void cEXT::read(Pair &_pp)
{
    Utils::copybytes(&this->header, _pp.cc.data(), sizeof(sEXTHeader_s));
}

}
} // MHXX
