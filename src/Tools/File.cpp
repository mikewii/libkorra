#include "Tools/File.hpp"
#include <fstream>


void File::File_To_CC(const std::filesystem::path& path, CContainer& cc)
{
    std::fstream    file(path, std::ios::binary | std::ios::in);
    const auto      fsize = std::filesystem::file_size(path);

    if (file.is_open())
    {
        cc.resize(fsize);

        file.read(&cc.as<char>(0), fsize);

        file.close();
    }
}

void File::CC_To_File(const std::filesystem::path& path, const CContainer& cc)
{
    std::fstream    file(path, std::ios::binary | std::ios::out);

    if (file.is_open())
    {
        file.write(&cc.as_const<char>(0), cc.size());
        file.flush();
    }
}
