#include "MH4U/Extentions/gmd.hpp"

#include "string.h"

namespace MH4U {
namespace GMD {

sGMD::sGMD( Pair& _pp )
{
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

std::string    sGMD::getLabelStr( u32 _id ) const{
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


} // GMD
} // MH4U
