#pragma once
#include "types.h"
#include "Global.hpp"
#include <filesystem>

class Folder {
public:
    Folder(const std::filesystem::path& path);

    const std::vector<std::filesystem::path> Get_ListOfFiles(void) const;

private:
    std::filesystem::path __path;
};
