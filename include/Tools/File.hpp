#pragma once
#ifndef N3DS
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
    static bool file_to_cc(const std::filesystem::path& path, CContainer& cc, const u32 magic = 0);
    static bool file_to_cc_size(const std::filesystem::path& path, CContainer& cc, const int size);

    /**
     * @brief Write data from CContainer to file
     * @param _fpath    Path with filename
     * @param _cc       CContainer to work with
     */
    static void CC_To_File(const std::filesystem::path& path, const CContainer& cc, const bool flush = false);

    static void Data_To_File(const std::filesystem::path& path, const void* data, const int size, const bool flush = false);

    static CContainer oneFile;

private:
    static bool Probe(std::fstream& file, const u32 magic);
};
#endif
