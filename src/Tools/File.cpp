#include "Tools/File.hpp"
#include <fstream>

CContainer File::oneFile;

bool File::file_to_cc(const std::filesystem::path& path, CContainer& cc, const u32 magic)
{    
    std::filesystem::path prefered(path); prefered = prefered.make_preferred();

    std::fstream    file(prefered, std::ios::binary | std::ios::in);
    const auto      fsize = std::filesystem::file_size(prefered);
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

bool File::file_to_cc_size(const std::filesystem::path &path, CContainer &cc, const int size)
{
    std::filesystem::path prefered(path); prefered = prefered.make_preferred();
    std::fstream    file(prefered, std::ios::binary | std::ios::in);
    const auto      fsize = std::filesystem::file_size(prefered);

    if (file.is_open() && fsize >= size) {
        file.read(&cc.as<char>(0), size);

        file.close();
        return true;
    }

    return false;
}

void File::CC_To_File(const std::filesystem::path& path, const CContainer& cc, const bool flush)
{
    std::filesystem::path prefered(path); prefered = prefered.make_preferred();

    std::fstream file(prefered, std::ios::binary | std::ios::out);

    if (file.is_open())
    {
        file.write(&cc.as_const_ref<char>(0), cc.size());

        if (flush)
            file.flush();
    }
}

void File::Data_To_File(const std::filesystem::path& path, const void* data, const int size, const bool flush)
{
    std::filesystem::path prefered(path); prefered = prefered.make_preferred();

    std::fstream file(prefered, std::ios::binary | std::ios::out);

    const char* data0 = reinterpret_cast<const char*>(data);
    if (file.is_open())
    {
        file.write(data0, size);

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
