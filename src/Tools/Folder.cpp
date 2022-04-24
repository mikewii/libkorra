#include "Tools/Folder.hpp"

Folder::Folder(const std::filesystem::path& path)
{
    if (std::filesystem::is_directory(path)) {
        auto prefered = path;
        prefered.make_preferred();

        this->__path.assign(prefered);
    }
    // TODO: action if not folder
}

const std::vector<std::filesystem::path> Folder::Get_ListOfFiles(const bool recursive) const
{
    std::vector<std::filesystem::path> out;

    if (std::filesystem::exists(this->__path)) {
        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(this->__path))
                if (entry.is_regular_file())
                    out.push_back(entry.path());
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(this->__path))
                if (entry.is_regular_file())
                    out.push_back(entry.path());
        }
    }

    return out;
}
