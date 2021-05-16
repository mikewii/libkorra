#pragma once
#include "types.h"
#include "Tools/CContainer.hpp"

#include "MH4U/Extentions/sem.hpp"
#include "MH4U/Extentions/qdp.hpp"
#include "MH4U/Extentions/gmd.hpp"


namespace MH4U {

    void Decode(CContainer& in, CContainer& out, bool isQuest = false);
    void Encode(CContainer& in, CContainer& out, bool isQuest = false);

};
