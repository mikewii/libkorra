#pragma once
#ifndef N3DS
#include "types.h"
#include "Global.hpp"

namespace MHXX {
namespace TEST {
void run_tests(void);
void Extentions(const std::vector<Pair>& vector, const std::filesystem::path& filename);
void savefile(void);
}
}
#endif
