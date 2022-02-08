#pragma once
#include <array>

namespace MHXX {

constexpr static std::array<const char*, 3> clearType = {
    "None",
    "And",
    "Or"
};

constexpr static std::array<const char*, 9> clearParam = {
    "Capture",
    "Hunt",
    "Gathering",
    "Hunt All",
    "Slay All",
    "Deliver",
    "",
    "",
    "Break"
};

} // MHXX
