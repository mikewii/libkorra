#include "MH4U/Extentions/gmd.hpp"

#include "Tools/Utils.hpp"
#include "string.h"

namespace MH4U {
namespace GMD {

sGMD::~sGMD()
{
    if ( this->__dataAdv2 != nullptr )
        free(this->__dataAdv2);
}

sGMD::sGMD( Pair& _pp )
{
    u32             expectedSize = sizeof(sGMD_Header_s) + NULL_TERMINATOR;
    sGMD_Header_s*  gmd = nullptr;

    if ( _pp.ResourceHash == RESOURCE_HASH ) {
        this->__data = gmd = reinterpret_cast<sGMD_Header_s*>( _pp.cc.data() );

        expectedSize += gmd->FilenameSize + gmd->ItemsSize + gmd->LabelsSize;

        if ( gmd->LabelsNum > 0 )
            expectedSize += gmd->LabelsNum * sizeof(sGMD_Advanced1_s) + sizeof(sGMD_Advanced2_s);
    }
    else goto err;

    if ( gmd->Padding0 != 0 )
        NotifyError( "GMD Padding0 isnt 0" );

    if ( gmd->Padding1 != 0 )
        NotifyError( "GMD Padding1 isnt 0" );

    if ( expectedSize != _pp.cc.size() ) {
        fprintf( stderr, "Size mismatch! CC %d | GMD %d\n", _pp.cc.size(), expectedSize );
        goto err;
    }

    this->readAll();
    return;

err:
    NotifyError("Pair is not a GMD");
}


void sGMD::save( Pair& _pp )
{
    sGMD_Header_s   header;
    u32             totalSize   = 0;
    u32             shift       = 0;
    bool            isAdv       = this->__labelStrings.size() > 0 ? true : false;
    u8*             p_start     = nullptr;
    u32 o_filename  = 0;    u32 filenameSize    = 0;
    u32 o_dataAdv1  = 0;    u32 dataAdv1Size    = 0;
    u32 o_dataAdv2  = 0;    u32 dataAdv2Size    = 0;
    u32 o_labels    = 0;    u32 labelsSize      = 0;
    u32 o_items     = 0;    u32 itemsSize       = 0;


    _pp.ResourceHash    = GMD::RESOURCE_HASH;


    ///// Calcualte sizes
    filenameSize = this->__filename.size();

    for ( auto& label : this->__labelStrings )
        labelsSize += label.size() + NULL_TERMINATOR;

    for ( auto& item : this->__itemStrings )
        itemsSize += item.size() + NULL_TERMINATOR;

    totalSize = sizeof(sGMD_Header_s) + filenameSize + NULL_TERMINATOR + labelsSize + itemsSize;

    if ( isAdv )
    {
        dataAdv1Size    = this->__dataAdv1.size() * sizeof(sGMD_Advanced1_s);
        dataAdv2Size    = sizeof(sGMD_Advanced2_s);
        totalSize       += dataAdv1Size + dataAdv2Size;
    }

    ///// Calculate offsets
    o_filename  = sizeof(sGMD_Header_s);

    if( isAdv )
    {
        o_dataAdv1  = o_filename + filenameSize + NULL_TERMINATOR;
        o_dataAdv2  = o_dataAdv1 + dataAdv1Size;
        o_labels    = o_dataAdv2 + dataAdv2Size;
        o_items     = o_labels + labelsSize;
    }
    else o_items = o_filename + filenameSize + NULL_TERMINATOR;


    ///// Making header
    header.Magic        = header.MAGIC;
    header.Version      = header.VERSION;
    header.Unk          = this->__unk;

    header.LabelsNum    = this->__labelStrings.size();
    header.LabelsSize   = labelsSize;

    header.ItemsNum     = this->__itemStrings.size();
    header.ItemsSize    = itemsSize;

    header.FilenameSize = filenameSize;


    ///// Resize container to fit data
    _pp.cc.resize(totalSize, true);
    p_start = _pp.cc.data();


    ///// Copy data to CContainer
    // Copy header
    Utils::copybytes(p_start, &header, sizeof(sGMD_Header_s));

    // Copy filename
    Utils::copybytes(p_start + o_filename, this->__filename.c_str(), this->__filename.size());

    // Copy labels if they exist
    if ( isAdv )
    {
        // Copy adv data1
        for ( auto& data : this->__dataAdv1 )
        {
            Utils::copybytes(p_start + o_dataAdv1 + shift, &data, sizeof(sGMD_Advanced1_s));

            shift += sizeof(sGMD_Advanced1_s);
        } shift = 0;

        // Copy adv data2
        Utils::copybytes(p_start + o_dataAdv2, this->__dataAdv2, sizeof(sGMD_Advanced2_s));

        for ( auto& str : this->__labelStrings )
        {
            Utils::copybytes(p_start + o_labels + shift, str.c_str(), str.size());
            shift += str.size() + NULL_TERMINATOR;
        } shift = 0;
    }

    // Copy items if they exist (they should)
    if ( !this->__itemStrings.empty() )
    {
        for ( auto& str : this->__itemStrings )
        {
            Utils::copybytes(p_start + o_items + shift, str.c_str(), str.size());
            shift += str.size() + NULL_TERMINATOR;
        }
    }
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
    sGMD_Header_s*  gmd                 = this->__data;
    u8*             p_Start             = nullptr;
    u32             o_Filename          = 0;
    u32             o_sGMD_Advanced1_s  = 0;
    u32             o_sGMD_Advanced2_s  = 0;
    u32             o_Labels            = 0;
    u32             o_Items             = 0;
    u32             dataAdv1Size        = 0;
    u32             shift               = 0;
    bool            isAdv = gmd->LabelsNum > 0 ? true : false;

    // Resize before filling in data
    this->__labelStrings.resize(gmd->LabelsNum);
    this->__dataAdv1.resize(gmd->LabelsNum);
    this->__itemStrings.resize(gmd->ItemsNum);

    dataAdv1Size    = this->__labelStrings.size() * sizeof(sGMD_Advanced1_s);
    p_Start         = reinterpret_cast<u8*>( this->__data );

    // Calculate offsets
    o_Filename      = sizeof(sGMD_Header_s);

    if ( isAdv )
    {
        o_sGMD_Advanced1_s  = o_Filename + gmd->FilenameSize + NULL_TERMINATOR;
        o_sGMD_Advanced2_s  = o_sGMD_Advanced1_s + dataAdv1Size;
        o_Labels            = o_sGMD_Advanced2_s + sizeof(sGMD_Advanced2_s);
        o_Items             = o_Labels; // calculated later
    }
    else o_Items    = o_Filename + gmd->FilenameSize + NULL_TERMINATOR;


    ///// Read
    // Unknown value
    this->__unk     = this->__data->Unk;


    // Filename
    this->__filename = reinterpret_cast<const char*>( p_Start + o_Filename );


    // sGMD_Advanced1_s
    for ( auto& a : this->__dataAdv1 )
    {
        u8* p = p_Start + o_sGMD_Advanced1_s + shift;
        Utils::copybytes(&a, p, sizeof(sGMD_Advanced1_s));

        shift += sizeof(sGMD_Advanced1_s);
    } shift = 0;


    // sGMD_Advanced2_s
    if ( isAdv )
    {
        this->__dataAdv2 = reinterpret_cast<sGMD_Advanced2_s*>(
                    calloc( 1, sizeof(sGMD_Advanced2_s)) );
        Utils::copybytes(this->__dataAdv2, p_Start + o_sGMD_Advanced2_s, sizeof(sGMD_Advanced2_s));
    }


    // Labels
    for ( auto& str : this->__labelStrings )
    {
        char* pStr = reinterpret_cast<char*>( p_Start ) + o_Labels + shift;

        str = pStr;

        shift += strlen(pStr) + NULL_TERMINATOR;
    } o_Items += shift; shift = 0;


    // Items
    for ( auto& str : this->__itemStrings )
    {
        char* pStr = reinterpret_cast<char*>( p_Start ) + o_Items + shift;

        str = pStr;

        shift += strlen(pStr) + NULL_TERMINATOR;
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
    if ( this->__labelStrings.empty() ) return false;

    for ( auto& str : this->__labelStrings)
        this->print(str);

    return true;
}
bool sGMD::printAllItems( void )
{
    if ( this->__itemStrings.empty() ) return false;

    for ( auto& str : this->__itemStrings)
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
    if ( _id > this->__labelStrings.size() ) return "";
    else return this->__labelStrings.at(_id);
}

std::string    sGMD::getItemStr( u32 _id ) const {
    if ( _id > this->__itemStrings.size() ) return "";
    else return this->__itemStrings.at(_id);
}


////////// Setters //////////

void    sGMD::setFilenameStr( std::string _str ) { this->__filename = _str; }

bool    sGMD::setLabelStr( std::string _str, u32 _id ) {
    if ( _id > this->__labelStrings.size() ) return false;
    else this->__labelStrings.at(_id) = _str;

    return true;
}

bool    sGMD::setItemStr( std::string _str, u32 _id ) {
    if ( _id > this->__itemStrings.size() ) return false;
    else this->__itemStrings.at(_id) = _str;

    return true;
}

void    sGMD::appendLabelStr( std::string _str ) { this->__labelStrings.push_back(_str); }
void    sGMD::appendItemStr( std::string _str ) { this->__itemStrings.push_back(_str); }


bool    sGMD::removeLabelStr( u32 _id )
{
    if ( _id > this->__labelStrings.size() ) return false;
    else this->__labelStrings.erase(this->__labelStrings.begin() + _id);

    return true;
}

bool    sGMD::removeItemStr( u32 _id )
{
    if ( _id > this->__itemStrings.size() ) return false;
    else this->__itemStrings.erase(this->__itemStrings.begin() + _id);

    return true;
}

} // GMD
} // MH4U
