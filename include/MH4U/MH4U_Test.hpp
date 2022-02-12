#pragma once
#include "types.h"
#include "Global.hpp"

namespace MH4U {
namespace TEST {
void runTests(void);
void Extentions(const std::vector<Pair>& vector, const std::filesystem::path& path);
void Replace(std::vector<Pair>& vector, const std::filesystem::path& path);
}
}
