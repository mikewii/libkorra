#pragma once
#include "types.h"
#include "Tools/Utils.hpp"
#include <typeinfo>

#include <chrono>

//#define TEST

void runTests(void);

class Timer
{
    using TimePoint = std::chrono::system_clock::time_point;

public:
    void start(void);
    void stop(void);

private:
    std::pair<TimePoint, TimePoint> timer;

    TimePoint getTime(void) const { return std::chrono::system_clock::now(); }

};

template<class T>
void test(const Pair& _in)
{
#ifdef TEST
    Pair                    out;
    std::pair<u32, u32>     sumCC, sumPairInfo;
    std::pair<u8*, u8*>     pointers;
    const char*             className = typeid(T).name();

    T c( _in );

    c.make( out );

    sumCC.first         = Utils::CalculateChecksum( _in.cc );
    sumCC.second        = Utils::CalculateChecksum( out.cc );

    sumPairInfo.first   = Utils::CalculateChecksum( reinterpret_cast<u8*>( &_in.info ), sizeof(PairInfo));
    sumPairInfo.second  = Utils::CalculateChecksum( reinterpret_cast<u8*>( &out.info ), sizeof(PairInfo));

    if (sumCC.first != sumCC.second)
    {
        fprintf(stderr, "Checksum different! %s\n", className);

        // just for debugging
        pointers = Utils::FindDiff(_in.cc.data(), out.cc.data(), out.cc.size());
    }
    else if (sumPairInfo.first != sumPairInfo.second)
        fprintf(stderr, "Checksum of PairInfo is different! %s\n", className);

    else if (_in.cc.size() != out.cc.size())
        fprintf(stderr, "Size mismatch! %s\n", className);

    else printf("Test successfull for %s!\n", className);
#endif
}
