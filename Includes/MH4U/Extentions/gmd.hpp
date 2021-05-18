#pragma once
#include "types.h"
#include "Global.hpp"

#include <vector>

namespace MH4U {
namespace GMD {
static const u32 RESOURCE_HASH = 0x242BB29A; // mhxx

struct sGMD_Header_s {
    static const u32    MAGIC = 0x00444D47; // GMD
    static const u32    VERSION = 0x00010302; // u8 major 2, u8 minor 3, u8 patch 1, u8 padding

    u32     Magic = MAGIC;
    u32     Version = VERSION;

    u32     Padding0 = 0;
    float   Unk;
    u32     Padding1 = 0;

    u32     LabelsNum;
    u32     ItemsNum;
    u32     LabelsSize;
    u32     ItemsSize;

    u32     FilenameSize;
};

struct sGMD_Advanced1_s {
    u32 Unk[5];
};

struct sGMD_Advanced2_s {
    u32 Unk[256];
};

class sGMD
{
public:

    sGMD( Pair& _pp );
    ~sGMD();

    void printHeader( void );
    bool printFilename( void );
    bool printAllLabels( void );
    bool printAllItems( void );

    /**
     * @brief Save loaded gmd to new Pair
     * @param _pp
     */
    void save( Pair& _pp );

    /**
     * @brief Make new gmd and save it to Pair
     * @param _pp
     */
    void make( Pair& _pp );
    void makeAdv();

    // Getters
    u32             getItemsNum( void ) const;
    u32             getLabelsNum( void ) const;
    std::string     getFilenameStr( void ) const;
    std::string     getLabelStr( u32 _id ) const;
    std::string     getItemStr( u32 _id ) const;

    // Setters
    void            setFilenameStr( std::string _str );
    bool            setLabelStr( std::string _str, u32 _id );
    bool            setItemStr( std::string _str, u32 _id );
    void            appendLabelStr( std::string _str );
    void            appendItemStr( std::string _str );

    bool            removeLabelStr( u32 _id );
    bool            removeItemStr( u32 _id );

private:
    sGMD_Header_s*              __data = nullptr;
    u8*                         __dataAdv = nullptr;
    u32                         __pLabels = 0;
    u32                         __pItems = 0;
    float                       __unk = 0;
    std::string                 __filename = "example";
    std::vector<std::string>    __labelStrings;
    std::vector<std::string>    __itemStrings;

    void print( std::string& _str );
    void print( const char* _str );
    void readAll( void );
    void allocateDataAdv( void );

};

} // GMD
} // MH4U
