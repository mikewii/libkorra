#include "MH4U/Quest.hpp"

#include "Tools/Folder.hpp"
#include "Tools/File.hpp"

namespace MH4U {
const char VERSION[5] = "v005";

bool sQuest::check_version(void) const
{
    if (std::equal(VERSION, VERSION + 4, this->version))
        return true;
    else return false;
}

sFlags* sQuest::GetFlags(void)
{
    return reinterpret_cast<sFlags*>(reinterpret_cast<char*>(this) + this->p_Flags);
}

#ifndef N3DS
void sQuest::ExtractQuests_Directory(const std::filesystem::__cxx11::path& dir)
{
    static const std::string names[] = {
        "quest1",
        "quest2",
        "quest3",
        "quest4",
        "quest5"
    };

    auto in_names = [&](const std::filesystem::__cxx11::path& filepath) -> bool
    {
        for (const auto& name : names) {
            if (name.compare(filepath.filename()) == 0)
                return true;
        }
        return false;
    };

    Folder folder(dir);

    auto files = folder.Get_ListOfFiles();

    for (const auto& file : files) {
        if (in_names(file)) {
            ExtractQuests_File(file);
        }
    }
}

void sQuest::ExtractQuests_File(const std::filesystem::__cxx11::path& path)
{
    CContainer  in, out;
    sQuest*     quest_array = nullptr;
    int         questCount = 0;



    File::File_To_CC(path, in);

    Decode(in, out);

    quest_array = reinterpret_cast<sQuest*>(out.data());
    questCount  = out.size() / QUEST_SIZE; // drop floating point

    for (int i = 0; i < questCount; i++) {
        if (quest_array[i].check_version()) {
            #define BUF_SIZE 32
            char    buffer[BUF_SIZE];
            auto*   flags = quest_array[i].GetFlags();
            auto    id     = flags->QuestID;

            snprintf(buffer, BUF_SIZE, "q%05d.mib", id);

            std::filesystem::path outpath = path;
            outpath.replace_filename(buffer);

            File::Data_To_File(outpath, &quest_array[i], QUEST_SIZE);
        }
    }
}
#endif

}
