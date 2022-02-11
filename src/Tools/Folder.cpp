#include "Tools/Folder.hpp"

Folder::Folder(const std::filesystem::path& path)
{
    auto prefered = path;
    Folder::__path = prefered.make_preferred();
}

const std::vector<std::filesystem::path> Folder::Get_ListOfFiles(void) const
{
    std::vector<std::filesystem::path> out;

    if (std::filesystem::exists(Folder::__path))
    {
        for (const auto& entry : std::filesystem::directory_iterator(Folder::__path))
        {
            out.push_back(entry.path());
        }
    }

    return out;
}
