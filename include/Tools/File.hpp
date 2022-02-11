#pragma once
#include "types.h"
#include "Global.hpp"


class File
{
private:
    static char CWD[PATH_MAX];

    static void             FixPathSeparators(char* _str, bool revert = false);
    static void             FixPathSeparators(std::string& _str, bool revert = false);
    static std::string      makepath(const char* _path);
    static std::string      makepath(std::string& _path);
    static std::string      extractExt(CContainer& _cc, bool BE = false);
    static std::string      popName(std::string& _path);

public:
    static void SetCWD(void);
    static int makedir(const char* dir);

    /**
     * @brief Extract last string from path divided by separator
     * @param _path     Path string with filename
     * @return          Filename string
     */
    static std::string extractName(const char* _path);

    /**
     * @brief Extract last string from path divided by separator
     * @param _path     Path string with filename
     * @return          Filename string
     */
    static std::string extractName(std::string& _path);

    /**
     * @brief Write CContainer from Pair to file
     * @param _pair         Pair to work on
     * @param _fname        Root folder name
     * @param _firstPath    Path to save toor folder at
     */
    static void PairToFile(Pair& _pair, std::string& _fname, std::string _firstPath = "");

    /**
     * @brief Write all CContainers from Pairs in vector to files
     * @param _vec          Vector of Pairs
     * @param _fname        Root folder name
     * @param _firstPath    Path to save root folder at
     */
    static void PairVectorToFiles(std::vector<Pair>& _vec, std::string& _fname, std::string _firstPath = "");

    /**
     * @brief Read file to CContainer
     * @param _fpath    Path with filename
     * @param _cc       CContainer to hold data
     * @return          True if read successfull
     */
    static bool FileToCC(const char* _path, CContainer* _cc);
    static bool FileToCC(std::string& _path, CContainer* _cc);

    /**
     * @brief Write data from CContainer to file
     * @param _fpath    Path with filename
     * @param _cc       CContainer to work with
     * @param _makedir  If true then attempt to make dirs from _fpath
     * @return          True if write successfull
     */
    static bool CCtoFile(const char* _path, CContainer* _cc, bool _makedir = false);
    static bool CCtoFile(std::string& _path, CContainer* _cc, bool _makedir = false);
};
