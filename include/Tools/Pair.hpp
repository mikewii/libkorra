#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"

struct PairInfo {
    char    Filename[FNAME_SIZE];
    u32     ResourceHash;
    u32     XORLock;
    u32     DecSize = 0;
    bool    isDecompressed = false;

    void print(void);
} PACKED;

struct Pair {

    PairInfo    info;
    CContainer  cc{};

    void print(void) { this->info.print(); };
};

class PairInfoKeeper // for transfering info from one pair to another
{
public:

    /**
     * @brief Saves info from provided Pair
     * @param _pp   Pair to work on
     */
    void Set_PairInfo(const Pair& _pp);

    /**
     * @brief Writes saved info to provided Pair
     * @param _pp   Pair to work on
     */
    bool GetPairInfo(Pair& _pp);

    bool isPairInfoSet(void) const { return this->__isSet; }

private:
    PairInfo    __info;
    bool        __isSet = false;
};
