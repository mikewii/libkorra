#pragma once
#include "types.h"
#include "Global.hpp"

#include <climits>
#include <vector>

namespace Utils {



template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (std::size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

extern void*    copybytes(void* _dest, const void* _src, size_t _size);

class File
{
private:
    static char CWD[NAME_MAX];

    static void     FixPathSeparators(char* _str, bool revert = false);
    static void     FixPathSeparators(std::string& _str, bool revert = false);

public:
    static void    SetCWD(void);

    static int     makedir(const char* dir);

    static std::string  makepath(const char* _path);
    static std::string  makepath(std::string& _path);
    static std::string  extractName(const char* _path);
    static std::string  extractName(std::string _path);
    static std::string  extractExt(CContainer& _cc, bool BE = false);
    static std::string  cutName(std::string& _path);

    static void PairToFiles( Pair& _pair, std::string& _fname, std::string _firstPath = "" );
    static void PairsVectorToFiles( std::vector<Pair>& _list, std::string& _fname, std::string _firstPath = "" );
};







bool            FileToCC(const char* fname, CContainer* cc);
bool            CCtoFile(const char* fname, CContainer* cc, bool makedir = false);

u32             CalculateChecksum( CContainer& _data );
u32             CalculateChecksum( u8* _data, u32 _size );

///
/// \brief FindDiff
/// \param _data0 = pointer to data to check agains
/// \param _data1 = pointer to data to check agains
/// \param _size = size data, _data0 and _data1 size must be same
///
void             FindDiff(u8* _data0, u8* _data1, u32 _size);


} // Utils
