#include "Tools/File.hpp"
#include <fstream>

CContainer File::oneFile;

bool File::File_To_CC(const std::filesystem::path& path, CContainer& cc, const u32 magic)
{
    std::fstream    file(path, std::ios::binary | std::ios::in);
    const auto      fsize = std::filesystem::file_size(path);
    bool            proceed = true;

    if (file.is_open())
    {
        if (magic)
            proceed = File::Probe(file, magic);

        if (proceed)
        {
            cc.resize(fsize);

            file.read(&cc.as<char>(0), fsize);
        }

        file.close();
        return proceed;
    }

    return false;
}

void File::CC_To_File(const std::filesystem::path& path, const CContainer& cc, const bool flush)
{
    std::fstream file(path, std::ios::binary | std::ios::out);

    if (file.is_open())
    {
        file.write(&cc.as_const_ref<char>(0), cc.size());

        if (flush)
            file.flush();
    }
}

bool File::Probe(std::fstream& file, const u32 magic)
{
    u32 buffer = 0;

    file.read(reinterpret_cast<char*>(&buffer), 4);
    file.seekg(0);

    return buffer == magic;
}
