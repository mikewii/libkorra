#include "MHXX/Extentions/qdl.hpp"
#include <string.h>

namespace MHXX {
namespace QDL {

cQuestDataLink::cQuestDataLink()
{
}

cQuestDataLink::cQuestDataLink( Pair& _pp )
{
    if ( _pp.cc.size() == sizeof(sQuestDataLink_s) && _pp.info.ResourceHash == getResourceHash() )
    {
        Utils::copybytes(&this->__data, _pp.cc.data(), sizeof(sQuestDataLink_s));

        this->SetPairInfo(_pp);
    }
    else NotifyError("Pair is not a sQuestDataLink_s");
}

cQuestDataLink::~cQuestDataLink()
{
}

void cQuestDataLink::make( Pair& _pp ) { this->save(_pp); }
void cQuestDataLink::save( Pair& _pp ) { Export<cQuestDataLink>::save(*this, _pp); }

void cQuestDataLink::print( void )
{
    printf("\n##### QuestDataLink file #####\n");

    printf("Magic:      0x%08X\n", this->__data.Magic);
    printf("Version:    0x%08X\n", this->__data.Version);

    for ( auto& item : this->__data.Item)
        printItem( item );

//    for ( auto& boss : this->__data.BossSetRes )
//        printItem( boss );

//    for ( auto& em : this->__data.EmSetList )
//        printItem( em );

//    for ( auto& rem : this->__data.RemTbl )
//        printItem( rem );

//    for ( auto& remAdd : this->__data.RemTblAdd )
//        printItem( remAdd );

//    printItem( this->__data.RemTblSub );
//    printItem( this->__data.SuppTbl );
//    printItem( this->__data.QuestPlus );
}

void cQuestDataLink::printItem( sQDLItem_s& _item )
{
    printf("Resource:   0x%08X\n", _item.Resource);
    printf("Name:       %s\n\n", _item.Name);
}

////////// Getters //////////
sQDLItem_s  cQuestDataLink::getItem( sQDLItemOrder _id ) const
{
    return this->__data.Item[_id];
}

std::string sQDLItem_s::getName( void ) const { return std::string(this->Name); }


////////// Setters //////////
void        cQuestDataLink::setItem( sQDLItem_s& _item, sQDLItemOrder _id )
{
    this->__data.Item[_id] = _item;
}

bool    sQDLItem_s::setName( std::string& _str )
{
    if ( _str.size() <= sQDLItem_s::QDL_NAME_MAX )
    {
        Utils::copybytes(this->Name, _str.c_str(), _str.size());
        return true;
    }
    else return false;
}

bool    sQDLItem_s::setName( const char* _str )
{
    u32 size = strlen(_str);
    if ( size <= sQDLItem_s::QDL_NAME_MAX )
    {
        Utils::copybytes(this->Name, _str, size);
        return true;
    }
    else return false;
}


}
} // MHXX
