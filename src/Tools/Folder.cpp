#include "Tools/Folder.hpp"

Folder::Folder(const std::filesystem::path& path)
{
    auto prefered = path;
    Folder::__path = prefered.make_preferred();
}

const std::vector<std::filesystem::path> Folder::Get_ListOfFiles(const bool recursive) const
{
    std::vector<std::filesystem::path> out;

    if (std::filesystem::exists(Folder::__path))
    {
        if (recursive)
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(Folder::__path))
                if (entry.is_regular_file())
                    out.push_back(entry.path());
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(Folder::__path))
                if (entry.is_regular_file())
                    out.push_back(entry.path());
        }
    }

    return out;
}
