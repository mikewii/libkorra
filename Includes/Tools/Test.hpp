#pragma once
#include "types.h"
#include "Tools/Utils.hpp"
#include <typeinfo>

namespace TEST {

template<class T>
void test( Pair& _pp )
{
    Pair                    out;
    u32                     sum0, sum1;
    const char*             className = typeid(T).name();
    std::pair<u8*, u8*>     pointers;

    T c( _pp );

    c.make( out );

    sum0 = Utils::CalculateChecksum( _pp.cc );
    sum1 = Utils::CalculateChecksum( out.cc );

    if ( sum0 != sum1 )
    {
        fprintf(stderr, "Checksum diffrent! %s\n", className);

        // just for debugging
        pointers = Utils::FindDiff(_pp.cc.data(), out.cc.data(), out.cc.size());
    }
    else if ( _pp.cc.size() != out.cc.size() ) fprintf(stderr, "Size mismatch! %s\n", className);
    else printf("Test successfull fo %s!\n", className);
}

} // TEST
