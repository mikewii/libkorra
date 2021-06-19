#include "MHXX/Extentions/esl.hpp"

namespace MHXX {
namespace ESL {

cEmSetList::cEmSetList()
{
}

cEmSetList::cEmSetList( Pair& _pp )
{
    if ( _pp.info.ResourceHash == RESOURCE_HASH )
    {
        this->SetPairInfo(_pp);

        this->read(_pp);
    }
    else NotifyError("Pair is not a cEmSetList_s");
}

cEmSetList::~cEmSetList()
{
}

void cEmSetList::read( Pair &_pp )
{
    u32 shift = 0;

    // Copy Header
    Utils::copybytes(&this->__header, _pp.cc.data(), sizeof(sESLHeader_s));
    shift += sizeof(sESLHeader_s);

    this->__ESD.resize(2);
    for ( u32 i = 0; i < 2; i++ )
    {
        Utils::copybytes(&this->__ESD.at(i), _pp.cc.data() + shift, sizeof(sESData_s));
        shift += sizeof (sESData_s);

        this->__ESD.at(i).second.resize(this->__ESD.at(i).first.EmSmallNum);

        for ( u32 j = 0; j < this->__ESD.at(i).second.size(); j++ )
        {
            sEmSmallData_s pos;

            Utils::copybytes(&pos, _pp.cc.data() + shift, sizeof(sEmSmallData_s));
            shift += sizeof(sEmSmallData_s);

            this->__ESD.at(i).second.at(j) = pos;
        }

    }
}

void cEmSetList::print( void )
{
    printf("\n##### EmSetList file #####\n");

    printf("Magic:      0x%08X\n", this->__header.Magic);
    printf("Version:    0x%08X\n", this->__header.Version);

    for ( auto& esd : this->__ESD )
    {
        printf("Magic0:     0x%08X\n", esd.first.Magic0);
        printf("Magic1:     0x%08X\n", esd.first.Magic1);
        printf("EmSmallNum: %hd\n\n", esd.first.EmSmallNum);

        for ( auto& esdpos : esd.second )
        {
            printf("Enemy:      %s\n", getEnemyStr(esdpos.EnemyID));
            printf("Area:       %hhd\n", esdpos.Area);
            printf("Position:   R:%f|X:%f|Y:%f|Z:%f\n",
                   esdpos.Position.R, esdpos.Position.X, esdpos.Position.Y, esdpos.Position.Z);
            printf("Unk2:       0x%08X|0x%08X|0x%08X\n\n",
                   esdpos.Unk2[0], esdpos.Unk2[1], esdpos.Unk2[2]);
        }
    }
}

} // ESL
} // MHXX
