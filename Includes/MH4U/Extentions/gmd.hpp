#pragma once
#include "types.h"
#include "Global.hpp"

#include <vector>

namespace MH4U {
namespace GMD {
static const u32 RESOURCE_HASH = 0x242BB29A;

struct sGMD_Header_s {
    static const u32    MAGIC = 0x00444D47; // GMD
    static const u32    VERSION = 0x00010302; // u16 major 1, u8 minor 3, u8 patch 2

    u32     Magic = MAGIC;
    u32     Version = VERSION;
    u32     Padding0;
    float   Unk;
    u32     Padding1;
    u32     LabelsNum;
    u32     ItemsNum;
    u32     LabelsSize;
    u32     ItemsSize;
    u32     FilenameSize;
    //char    Filename[];
    // strings:
};

struct sGMD_Advanced1_s {
    u32 Unk[5];
};

struct sGMD_Advanced2_s {
    u32 Unk[256];
};

struct sGMD {

    sGMD_Header_s* __data;

    sGMD( Pair& _pp );

    void printHeader( void );
    bool printFilename( void );
    bool printAllLabels( void );
    bool printAllItems( void );

    // Getters
    u32             getItemsNum( void ) const;
    u32             getLabelsNum( void ) const;
    std::string     getFilenameStr( void ) const;
    std::string     getLabelStr( u32 _id ) const;
    std::string     getItemStr( u32 _id ) const;

    // Setters
    void    setFilenameStr( std::string _str );
    bool    setLabelStr( std::string _str, u32 _id );
    bool    setItemStr( std::string _str, u32 _id );
    void    appendLabelStr( std::string _str );
    void    appendItemStr( std::string _str );

private:
    u32                         __pLabels = 0;
    u32                         __pItems = 0;
    std::string                 __filename;
    std::vector<std::string>    __LabelsStrings;
    std::vector<std::string>    __ItemsStrings;

    void print( std::string& _str );
    void print( const char* _str );
    void readAll( void );

};

} // GMD
} // MH4U
