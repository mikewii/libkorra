#include "MHXX/Extentions/rem.hpp"
#include "MHXX/Strings/Items.hpp"

namespace MHXX {
namespace REM {

cRewardEm::cRewardEm()
{
}

cRewardEm::cRewardEm(const Pair &_pp)
{
    if (_pp.cc.size() == sizeof(sRewardEm_s) && _pp.info.ResourceHash == RESOURCE_HASH)
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sRewardEm_s));

        this->Set_PairInfo(_pp);
    }
    else NotifyError("Pair is not a sRewardEm_s");
}

cRewardEm::~cRewardEm()
{
}

void cRewardEm::make(Pair& _pp) { this->save(_pp); }
void cRewardEm::save(Pair& _pp)
{
    // Not safe!

    // TODO: last item must have    rate = -1
    // need more research

    Export<cRewardEm>::save(*this, _pp);
}


void cRewardEm::print(void) const
{
    printf("\n##### RewardEm file #####\n");

    printf("Magic:                  0x%08X\n", this->__data.Magic);
    printf("Version:                0x%08X\n", this->__data.Version);

    this->print_RewardFlags();

    this->print_FlagNums();

    this->print_RewardItems(true);
}

void cRewardEm::print_FlagNums(void) const
{
    u32 i = 1;
    for (auto& flagNum : this->__data.FlagNum)
    {
        printf("FlagNum#%02d:       %hhd\n", i, flagNum);
        i++;
    }
}

void cRewardEm::print_RewardFlags(void) const
{
    u32 i = 1;

    for (auto& rewardFlag : this->__data.Flag)
    {
        printf("Flag#%02d :Flag:    %hhd\n", i, rewardFlag.Flag);
        printf("Flag#%02d :ItemNum: %hhd\n", i, rewardFlag.ItemNum);
        i++;
    }
}

void cRewardEm::print_RewardItems(bool _useNames) const
{
    u32 i = 1;
    for (auto& rewardItem : this->__data.Item)
    {
        if (_useNames)
        {
            if (rewardItem.ID < ITEMS_MAX)
                printf("Item#%02d :ID:      %s\n", i, MHXX::Items.at(rewardItem.ID));
            else printf("Item#%02d :ID:      ID is out of range!\n", i);
        }
        else printf("Item#%02d :ID:      %hd\n", i, rewardItem.ID);

        printf("Item#%02d :Ammount: %d\n", i, rewardItem.Ammount);
        printf("Item#%02d :Rate:    %d\n\n", i, rewardItem.Rate);
        i++;
    }
}

////////// Getters //////////
sRewardFlag_s cRewardEm::Get_Flag(const u32 _id) const
{
    if (_id < sRewardEm_s::REM_FLAGS)
        return this->__data.Flag[_id];

    else return sRewardFlag_s{ REM_ERROR, REM_ERROR };
}

u8 cRewardEm::Get_FlagNum(const u32 _id) const
{
    if (_id < sRewardEm_s::REM_FLAG_NUMS)
        return this->__data.FlagNum[_id];

    else return REM_ERROR;
}

sRewardItem_s cRewardEm::Get_RewardItem(const u32 _id) const
{
    if (_id < sRewardEm_s::REM_ITEMS)
        return this->__data.Item[_id];

    else return sRewardItem_s{ REM_ERROR, REM_ERROR, REM_ERROR };
}

////////// Setters //////////
bool cRewardEm::Set_Flag(const sRewardFlag_s _flag, const u32 _id)
{
    if (_id < sRewardEm_s::REM_FLAGS)
    {
        this->__data.Flag[_id] = _flag;
        return true;
    }
    else return false;
}

bool cRewardEm::Set_FlagNum(const s8 _flagNum, const u32 _id)
{
    if (_id < sRewardEm_s::REM_FLAG_NUMS)
    {
        this->__data.FlagNum[_id] = _flagNum;
        return true;
    }
    else return true;
}

bool cRewardEm::Set_RewardItem(const sRewardItem_s _item, const u32 _id)
{
    if (_id < sRewardEm_s::REM_ITEMS)
    {
        this->__data.Item[_id] = _item;
        return true;
    }
    else return true;
}

} // REM
} // MHXX
