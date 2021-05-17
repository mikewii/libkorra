#include "MH4U/Extentions/gmd.hpp"

#include "Tools/Utils.hpp"
#include "string.h"

namespace MH4U {
namespace GMD {

sGMD::sGMD()
{

}

sGMD::~sGMD()
{
    if ( this->__dataAdv != nullptr )
        free( this->__dataAdv );
}

sGMD::sGMD( Pair& _pp )
{
    u32             pDataAdv = sizeof(sGMD_Header_s) + 1;
    u32             dataAdvSize = 0;
    u32             expect = sizeof(sGMD_Header_s) + 1; // 1 cuz filename null terminator
    sGMD_Header_s*  gmd = nullptr;

    if ( _pp.ResourceHash == RESOURCE_HASH ) {
        this->__data = gmd = reinterpret_cast<sGMD_Header_s*>( _pp.cc.data() );


        this->__pItems += expect + gmd->FilenameSize;
        expect += gmd->FilenameSize + gmd->ItemsSize + gmd->LabelsSize;

        if ( gmd->LabelsNum > 0 )
        {
            this->__pItems  += gmd->LabelsNum * sizeof(sGMD_Advanced1_s) +
                    sizeof(sGMD_Advanced2_s);
            expect          += gmd->LabelsNum * sizeof(sGMD_Advanced1_s) +
                    sizeof(sGMD_Advanced2_s);

            // allocate and copy adv data
            allocateDataAdv();

            pDataAdv += gmd->FilenameSize;
            dataAdvSize = (gmd->LabelsNum * sizeof(sGMD_Advanced1_s)) + sizeof(sGMD_Advanced2_s);

            Utils::copybytes(this->__dataAdv, reinterpret_cast<u8*>( this->__data) + pDataAdv, dataAdvSize);


            u32 sum0, sum1;
            sum0 = Utils::CalculateChecksum(_pp.cc.data() + pDataAdv, dataAdvSize);
            sum1 = Utils::CalculateChecksum(this->__dataAdv, dataAdvSize);

            if ( sum0 != sum1 )
                fprintf( stderr, "something wrong with dataadv copy!\n");
        }
    }
    else goto err;

    if ( gmd->Padding0 != 0 )
        fprintf( stderr, "GMD Padding0 isnt 0\n" );

    if ( gmd->Padding1 != 0 )
        fprintf( stderr, "GMD Padding1 isnt 0\n" );

    if ( expect != _pp.cc.size() ) {
        fprintf( stderr, "Size mismatch! CC %d | GMD %d\n", _pp.cc.size(), expect );
        goto err;
    }

    this->readAll();
    return;

err:
    NotifyError("Pair is not a GMD");
}

void sGMD::make( Pair& _pp )
{
    sGMD_Header_s   header;
    u32             itemsSize = 0;
    u32             labelsSize = 0;
    u32             filenameSize = 0;
    u32             dataAdvSize = 0;
    u32             totalSize = 0;
    u32             shift = 0;


    ///// Setting sizes
    filenameSize = this->__filename.size();

    for ( auto& label : this->__LabelsStrings )
        labelsSize += label.size() + 1;

    for ( auto& item : this->__ItemsStrings )
        itemsSize += item.size() + 1;

    totalSize = sizeof(sGMD_Header_s) + filenameSize + 1 + labelsSize + itemsSize;

    if ( !this->__LabelsStrings.empty() )
        totalSize += dataAdvSize = (this->__LabelsStrings.size() * sizeof(sGMD_Advanced1_s)) + sizeof(sGMD_Advanced2_s);

    ///// Making header
    header.Magic        = header.MAGIC;
    header.Version      = header.VERSION;
    header.Unk          = this->__data->Unk;

    header.LabelsNum    = this->__LabelsStrings.size();
    header.LabelsSize   = labelsSize;

    header.ItemsNum     = this->__ItemsStrings.size();
    header.ItemsSize    = itemsSize;

    header.FilenameSize = filenameSize;

    ///// Copy data to CContainer
    _pp.cc.resize(totalSize, true);

    // Copy header
    Utils::copybytes(_pp.cc.data(), &header, sizeof(sGMD_Header_s));
    shift += sizeof(sGMD_Header_s);

    // Copy filename
    Utils::copybytes(_pp.cc.data() + shift, this->__filename.c_str(), this->__filename.size());
    shift += filenameSize + 1;

    // Copy labels if they exist
    if ( !this->__LabelsStrings.empty() )
    {
        // Copy dataAdv first
        Utils::copybytes(_pp.cc.data() + shift, this->__dataAdv, dataAdvSize);
        shift += dataAdvSize;

        for ( auto& str : this->__LabelsStrings )
        {
            Utils::copybytes(_pp.cc.data() + shift, str.c_str(), str.size());
            shift += str.size() + 1;
        }
    }

    // Copy items if they exist (they should)
    if ( !this->__ItemsStrings.empty() )
    {
        for ( auto& str : this->__ItemsStrings )
        {
            Utils::copybytes(_pp.cc.data() + shift, str.c_str(), str.size());
            shift += str.size() + 1;
        }
    }
}


void sGMD::allocateDataAdv( void )
{
    u32 size = (this->__data->LabelsNum * sizeof(sGMD_Advanced1_s)) + sizeof(sGMD_Advanced2_s);
    this->__dataAdv = reinterpret_cast<u8*>( calloc(size, sizeof(u8)) );
}

void sGMD::printHeader( void )
{
    sGMD_Header_s* gmd = this->__data;

    printf("\n##### GMD File Header #####\n");

    printf("Magic:          0x%08X\n", gmd->Magic);
    printf("Version:        0x%08X\n", gmd->Version);

    // actual data now
    printf("Padding0:       0x%08X\n", gmd->Padding0);
    printf("Unk:            %f\n", gmd->Unk);
    printf("Padding1:       0x%08X\n", gmd->Padding1);

    printf("LabelsNum:      %d\n", gmd->LabelsNum);
    printf("ItemsNum:       %d\n", gmd->ItemsNum);
    printf("LabelsSize:     %d\n", gmd->LabelsSize);
    printf("ItemsSize:      %d\n", gmd->ItemsSize);

    printf("FilenameSize:   %d\n", gmd->FilenameSize);
}

void sGMD::readAll( void )
{
    sGMD_Header_s*  gmd = this->__data;
    u32             shift = 0;

    this->__LabelsStrings.resize(gmd->LabelsNum);
    this->__ItemsStrings.resize(gmd->ItemsNum);

    this->__pLabels = this->__pItems;

    // Filename
    this->__filename = reinterpret_cast<const char*>( gmd ) + sizeof(sGMD_Header_s);

    // Labels
    for ( auto& str : this->__LabelsStrings )
    {
        char* pStr = reinterpret_cast<char*>( gmd ) + this->__pLabels + shift;

        str = pStr;

        shift += strlen(pStr) + 1;
    }

    // Items
    this->__pItems  += shift;
    shift           = 0;

    for ( auto& str : this->__ItemsStrings )
    {
        char* pStr = reinterpret_cast<char*>( gmd ) + this->__pItems + shift;

        str = pStr;

        shift += strlen(pStr) + 1;
    }
}

bool sGMD::printFilename( void )
{
    if ( this->__filename.empty() ) return false;

    this->print(this->__filename);

    return true;
}

bool sGMD::printAllLabels( void )
{
    if ( this->__LabelsStrings.empty() ) return false;

    for ( auto& str : this->__LabelsStrings)
        this->print(str);

    return true;
}
bool sGMD::printAllItems( void )
{
    if ( this->__ItemsStrings.empty() ) return false;

    for ( auto& str : this->__ItemsStrings)
        this->print(str);

    return true;
}

void sGMD::print( std::string& _str ) { printf("%s\n\n", _str.c_str()); }
void sGMD::print( const char* _str ) { printf("%s\n\n", _str); }

////////// Getters //////////

u32             sGMD::getItemsNum( void ) const { return this->__data->ItemsNum; }
u32             sGMD::getLabelsNum( void ) const { return this->__data->LabelsNum; }

std::string     sGMD::getFilenameStr() const {
    return std::string( reinterpret_cast<const char*>( this->__data + sizeof(sGMD_Header_s)) );
}

std::string    sGMD::getLabelStr( u32 _id ) const {
    if ( _id > this->__LabelsStrings.size() ) return "";
    else return this->__LabelsStrings.at(_id);
}

std::string    sGMD::getItemStr( u32 _id ) const {
    if ( _id > this->__ItemsStrings.size() ) return "";
    else return this->__ItemsStrings.at(_id);
}


////////// Setters //////////

void    sGMD::setFilenameStr( std::string _str ) { this->__filename = _str; }

bool    sGMD::setLabelStr( std::string _str, u32 _id ) {
    if ( _id > this->__LabelsStrings.size() ) return false;
    else this->__LabelsStrings.at(_id) = _str;

    return true;
}

bool    sGMD::setItemStr( std::string _str, u32 _id ) {
    if ( _id > this->__ItemsStrings.size() ) return false;
    else this->__ItemsStrings.at(_id) = _str;

    return true;
}

void    sGMD::appendLabelStr( std::string _str ) { this->__LabelsStrings.push_back(_str); }
void    sGMD::appendItemStr( std::string _str ) { this->__ItemsStrings.push_back(_str); }


bool    sGMD::removeLabelStr( u32 _id )
{
    if ( _id > this->__LabelsStrings.size() ) return false;
    else this->__LabelsStrings.erase(this->__LabelsStrings.begin() + _id);

    return true;
}

bool    sGMD::removeItemStr( u32 _id )
{
    if ( _id > this->__ItemsStrings.size() ) return false;
    else this->__ItemsStrings.erase(this->__ItemsStrings.begin() + _id);

    return true;
}

} // GMD
} // MH4U
