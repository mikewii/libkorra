#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"

namespace MH4U {

    void Decode(CContainer& in, CContainer& out, bool isQuest = false);
    void Encode(CContainer& in, CContainer& out, bool isQuest = false);

};
