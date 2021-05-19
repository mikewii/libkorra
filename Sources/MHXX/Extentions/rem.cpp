#include "MHXX/Extentions/rem.hpp"
#include "MHXX/Strings/Items.hpp"

namespace MHXX {
namespace REM {

cRewardEm::cRewardEm()
{
}

cRewardEm::cRewardEm(Pair &_pp)
{
    if ( _pp.cc.size() == sizeof(sRewardEm_s) && _pp.info.ResourceHash == RESOURCE_HASH )
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sRewardEm_s));

        this->SetPairInfo(_pp);
    }
    else NotifyError("Pair is not a sRewardEm_s");
}

cRewardEm::~cRewardEm()
{
}

void cRewardEm::make( Pair& _pp ) { this->save(_pp); }
void cRewardEm::save( Pair& _pp )
{
    // Not safe!

    // TODO: last item must have    rate = -1
    // need more research

    Export<cRewardEm>::save(*this, _pp);
}


void cRewardEm::print( void )
{
    printf("\n##### RewardEm file #####\n");

    printf("Magic:                  0x%08X\n", this->__data.Magic);
    printf("Version:                0x%08X\n", this->__data.Version);

    this->printRewardFlags();

    this->printFlagNums();

    this->printRewardItems( true );
}

void cRewardEm::printFlagNums( void )
{
    u32 i = 1;
    for ( auto& flagNum : this->__data.FlagNum)
    {
        printf("FlagNum#%02d:       %hhd\n", i, flagNum);
        i++;
    }
}

void cRewardEm::printRewardFlags( void )
{
    u32 i = 1;

    for( auto& rewardFlag : this->__data.Flag)
    {
        printf("Flag#%02d :Flag:    %hhd\n", i, rewardFlag.Flag);
        printf("Flag#%02d :ItemNum: %hhd\n", i, rewardFlag.ItemNum);
        i++;
    }
}

void cRewardEm::printRewardItems( bool _useNames )
{
    u32 i = 1;
    for ( auto& rewardItem : this->__data.Item)
    {
        if ( _useNames )
        {
            if ( rewardItem.ID < ITEMS_MAX )
                printf("Item#%02d :ID:      %s\n", i, MHXX::Items[rewardItem.ID]);
            else printf("Item#%02d :ID:      ID is out of range!\n", i);
        }
        else printf("Item#%02d :ID:      %hd\n", i, rewardItem.ID);

        printf("Item#%02d :Ammount: %d\n", i, rewardItem.Ammount);
        printf("Item#%02d :Rate:    %d\n\n", i, rewardItem.Rate);
        i++;
    }
}

////////// Getters //////////
sRewardFlag_s   cRewardEm::getFlag( u32 _id ) const
{
    if ( _id < sRewardEm_s::REM_FLAGS)
        return this->__data.Flag[_id];

    else return sRewardFlag_s{ REM_ERROR, REM_ERROR };
}

u8              cRewardEm::getFlagNum( u32 _id ) const
{
    if ( _id < sRewardEm_s::REM_FLAG_NUMS)
        return this->__data.FlagNum[_id];

    else return REM_ERROR;
}

sRewardItem_s   cRewardEm::getRewardItem( u32 _id ) const
{
    if ( _id < sRewardEm_s::REM_ITEMS)
        return this->__data.Item[_id];

    else return sRewardItem_s{ REM_ERROR, REM_ERROR, REM_ERROR };
}


////////// Setters //////////
bool            cRewardEm::setFlag( sRewardFlag_s _flag, u32 _id )
{
    if ( _id < sRewardEm_s::REM_FLAGS)
    {
        this->__data.Flag[_id] = _flag;
        return true;
    }
    else return false;
}

bool            cRewardEm::setFlagNum( s8 _flagNum, u32 _id )
{
    if ( _id < sRewardEm_s::REM_FLAG_NUMS)
    {
        this->__data.FlagNum[_id] = _flagNum;
        return true;
    }
    else return true;
}

bool            cRewardEm::setRewardItem( sRewardItem_s _item, u32 _id )
{
    if ( _id < sRewardEm_s::REM_ITEMS)
    {
        this->__data.Item[_id] = _item;
        return true;
    }
    else return true;
}



} // REM
} // MHXX
