#include "MHXX/Extentions/sup.hpp"
#include "MHXX/Strings/Items.hpp"

namespace MHXX {
namespace SUP {

cSupply::cSupply()
{
}

cSupply::cSupply(const Pair& _pp)
{
    if (_pp.cc.size() == sizeof(sSupply_s) && _pp.info.ResourceHash == RESOURCE_HASH)
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sSupply_s));

        this->Set_PairInfo(_pp);
    }
    else NotifyError("Pair is not sSupply_s");
}

cSupply::~cSupply()
{
}

void cSupply::make(Pair& _pp) { this->save(_pp); }
void cSupply::save(Pair& _pp) { Export<cSupply>::save(*this, _pp); }

void cSupply::print(bool _useNames) const
{
    u32 i = 0;

    printf("\n##### Supply file #####\n");

    printf("Magic:                  0x%08X\n", this->__data.Magic);
    printf("Version:                0x%08X\n", this->__data.Version);

    for (auto& item : this->__data.Item)
    {
        if (_useNames)
        {
            if (item.ID < ITEMS_MAX)
                printf("Item#%02d :ID:      %s\n", i, MHXX::Items.at(item.ID));
            else printf("Item#%02d :ID:      ID is out of range!\n", i);
        }
        else printf("Item#%02d :ID:      %hd\n", i, item.ID);

        printf("Item#%02d :Ammount: %hhd\n", i, item.Ammount);
        printf("Item#%02d :Label?   %hhd\n\n", i, item.Unk);
    }
}

////////// Getters //////////
sSupplyItem_s cSupply::Get_Item(const u32 _id) const
{
    if (_id < sSupply_s::ITEM_MAX)
        return this->__data.Item[_id];

    else return sSupplyItem_s{ SUP_ERROR, SUP_ERROR, SUP_ERROR };
}

////////// Setters //////////
bool cSupply::Set_Item(const u32 _id, const sSupplyItem_s& _item)
{
    if (_id < sSupply_s::ITEM_MAX)
    {
        this->__data.Item[_id] = _item;
        return true;
    }

    else return false;
}

} // SUP
} // MHXX
