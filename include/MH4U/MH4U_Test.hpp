#pragma once
#ifndef N3DS
#include "types.h"
#include "Global.hpp"

namespace MH4U {
namespace TEST {
void runTests(void);
void Extentions(const std::vector<Pair>& vector, const std::filesystem::path& path);
void Replace(std::vector<Pair>& vector, const std::filesystem::path& path);

void ExtractQuests(const std::filesystem::path& path);
void ExtractSave(const std::filesystem::path& path);
void mca(const std::filesystem::path& path);

void quest_prints(const std::filesystem::path& path);
void quest_ext_create(const std::filesystem::path& path);

}
}
#endif
