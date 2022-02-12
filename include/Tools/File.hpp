#pragma once
#include "types.h"
#include "Global.hpp"
#include <filesystem>


class File
{
public:
    /**
     * @brief Read file to CContainer
     * @param _fpath    Path with filename
     * @param _cc       CContainer to hold data
     */
    static void File_To_CC(const std::filesystem::path& path, CContainer& cc);

    /**
     * @brief Write data from CContainer to file
     * @param _fpath    Path with filename
     * @param _cc       CContainer to work with
     */
    static void CC_To_File(const std::filesystem::path& path, const CContainer& cc);

    static CContainer oneFile;
};
