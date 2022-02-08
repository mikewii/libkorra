#pragma once
#include "types.h"
#include "Tools/Pair.hpp"

#include <climits>
#include <vector>

namespace Utils {

#define SEPARATOR       '/'
#define SEPARATOR_WIN   '\\'

u32 GetHeaderRelativePos(const void* const header, const void* const member);

static USED void print_help(const char* name, const u32 item)
{
    printf("%-25s%08X | %d\n", name, item, item);
}

static USED void print_help_str(const char* name, const char* str, const u32 item)
{
    printf("%-25s%s : %08X | %d\n", name, str, item, item);
}

static USED void print_help_numered(const u32 id, const char* name, const u32 item)
{
    printf("[%d] %-21s%08X | %d\n", id, name, item, item);
}

static USED void print_help_str_numered(const u32 id, const char* name, const char* str, const u32 item)
{
    printf("[%d] %-21s%s : %08X | %d\n", id, name, str, item, item);
}

template <typename T>
static void print_help_arr(const char* name, const u32 item, T arr)
{
    //auto arrSize = sizeof(arr)/sizeof(arr[0]);
    auto arrSize = arr.size();

    if (item <= arrSize)
        printf("%-25s%-20s | %08X | %d\n", name, arr[item], item, item);
    else printf("----->%s out of range! %08X | %d\n", name, item, item);
}

template<bool>
struct bool_to_str
{
    static constexpr const char* value = "true";
};

template<>
struct bool_to_str<false>
{
    static constexpr const char* value = "false";
};

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

/**
 * @brief Alignment safe memory copy
 * @param _dest     Destination pointer
 * @param _src      Source pointer
 * @param _size     Ammount to copy
 * @return
 */
extern void* copybytes(void* _dest, const void* _src, size_t _size);

/**
 * @brief Calculate sum of bytes
 * @param _data     CContainer to work on
 * @return          Sum of bytes
 */
u32 CalculateChecksum(CContainer& _data);

/**
 * @brief Calculate sum of bytes
 * @param _data     Pointer to data to work on
 * @param _size     Size of _data
 * @return          Sum of bytes
 */
u32 CalculateChecksum(u8* _data, u32 _size);

/**
 * @brief Prints address of first different byte between data
 * @param _data0    Pointer to data to check agains
 * @param _data1    Pointer to data to check agains
 * @param _size     Size of data, _data0 and _data1 must be same size
 * @return          Pointers to diff data in both data0 and data1
 */
std::pair<u8*, u8*> FindDiff(u8* _data0, u8* _data1, u32 _size);


std::string GetUserHome(void);



class File
{
private:
    static char CWD[NAME_MAX];

    static void             FixPathSeparators(char* _str, bool revert = false);
    static void             FixPathSeparators(std::string& _str, bool revert = false);
    static std::string      makepath(const char* _path);
    static std::string      makepath(std::string& _path);
    static std::string      extractExt(CContainer& _cc, bool BE = false);
    static std::string      popName(std::string& _path);

public:
    static void SetCWD(void);
    static int makedir(const char* dir);

    /**
     * @brief Extract last string from path divided by separator
     * @param _path     Path string with filename
     * @return          Filename string
     */
    static std::string extractName(const char* _path);

    /**
     * @brief Extract last string from path divided by separator
     * @param _path     Path string with filename
     * @return          Filename string
     */
    static std::string extractName(std::string& _path);

    /**
     * @brief Write CContainer from Pair to file
     * @param _pair         Pair to work on
     * @param _fname        Root folder name
     * @param _firstPath    Path to save toor folder at
     */
    static void PairToFile(Pair& _pair, std::string& _fname, std::string _firstPath = "");

    /**
     * @brief Write all CContainers from Pairs in vector to files
     * @param _vec          Vector of Pairs
     * @param _fname        Root folder name
     * @param _firstPath    Path to save root folder at
     */
    static void PairVectorToFiles(std::vector<Pair>& _vec, std::string& _fname, std::string _firstPath = "");

    /**
     * @brief Read file to CContainer
     * @param _fpath    Path with filename
     * @param _cc       CContainer to hold data
     * @return          True if read successfull
     */
    static bool FileToCC(const char* _path, CContainer* _cc);
    static bool FileToCC(std::string& _path, CContainer* _cc);

    /**
     * @brief Write data from CContainer to file
     * @param _fpath    Path with filename
     * @param _cc       CContainer to work with
     * @param _makedir  If true then attempt to make dirs from _fpath
     * @return          True if write successfull
     */
    static bool CCtoFile(const char* _path, CContainer* _cc, bool _makedir = false);
    static bool CCtoFile(std::string& _path, CContainer* _cc, bool _makedir = false);
};

class Collector {
public:
    struct Info {
        u32             QuestID;
        std::string     Name;
        u32             QuestLevel;

        s32             Value;
    };

    enum class Op {
        Equal,
        NotEqual,
        Less,
        Greater,
        LessEqual,
        GreaterEqual,

        Unique,

        Collect
    };

    Collector(){}
    Collector(const u32 value) : _value(value) {}

    void Disable(void) { Collector::_active = false; }
    bool IsActive(void) const { return Collector::_active; }

    void Set_Operator(const Collector::Op operation) { Collector::_operation = operation; }
    void Set_Value(const u32 value) { Collector::_value = value; }
    void Set_Path(const std::string& path) { Collector::_path = path; }

    void Add(const Collector::Info& in);

    void Show(const bool sorted = true);

private:
    bool                _active = true;
    Op                  _operation = Op::Equal;
    s32                 _value = 0;
    std::string         _path;
    std::string         _name = "collected_data.txt";
    std::vector<Info>   _vec;
    std::vector<s32>    _vec_unique_ids;

    bool Flush(void);
};

} // Utils
