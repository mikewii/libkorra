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

sFlags* sQuest::get_flags(void)
{
    return reinterpret_cast<sFlags*>(reinterpret_cast<char*>(this) + this->p_Flags);
}

const sFlags* sQuest::get_flags(void) const
{
    return reinterpret_cast<const sFlags*>(reinterpret_cast<const char*>(this) + this->p_Flags);
}

#ifndef N3DS
Quest::Quest(const std::filesystem::path &path)
    : m_current_path(path)
{
    this->m_current_path.make_preferred();

    if (std::filesystem::is_directory(this->m_current_path)) {
        this->make_output_dir();
        this->m_working_on_folder = true;
    } else if (std::filesystem::is_regular_file(path)) {
        this->m_working_on_folder = false;
    }
}

void Quest::extract_all(void)
{
    if (this->m_working_on_folder) {
        Folder  folder(this->m_current_path);
        auto    files = folder.Get_ListOfFiles();

        for (const auto& filepath : files) {
            if (this->is_quest_ext_file(filepath)) {
                this->m_current_filename = filepath.filename().string();
                this->extract_ext_quest_file();
            }
        }
    } else {

    }
}

void Quest::extract_ext_quest_file(void)
{
    int quest_count;

    this->load_and_decode();
    this->extract_signature();
    //this->dump_decoded();

    quest_count = this->m_out.size() / QUEST_SIZE; // drop decimal

    for (size_t i = 0; i < quest_count; i++) {
        sQuest* quest = reinterpret_cast<sQuest*>(this->m_out.data() + i * QUEST_SIZE);

        this->mib_to_file(quest);
    }
}

void Quest::extract_signature(void)
{
#define SIGNATURE_SIZE 0x100
    auto position = this->m_out.size() - SIGNATURE_SIZE;
    auto signature_path = this->m_out_path;

    signature_path.append(this->m_current_filename + ".sig");

    File::Data_To_File(signature_path, this->m_out.data() + position, SIGNATURE_SIZE);
}

void Quest::make_output_dir(void)
{
    this->m_out_path = this->m_current_path;
    this->m_out_path.append("quest");

    if (!std::filesystem::is_directory(this->m_out_path))
        std::filesystem::create_directory(this->m_out_path);
}

bool Quest::is_quest_ext_file(const std::filesystem::path &path) const
{
    static const std::string names[] = {
        "quest1",
        "quest2",
        "quest3",
        "quest4",
        "quest5"
    };

    for (const auto& name : names) {
        if (name.compare(path.filename()) == 0)
            return true;
    }
    return false;
}

void Quest::dump_decoded(void)
{
    std::filesystem::path path = this->m_current_path;

    path.append(this->m_current_filename + ".dec");

    this->m_out.write_To_File(path);
}

void Quest::load_and_decode(void)
{
    std::filesystem::path path = this->m_current_path;
    CContainer in;

    path.append(this->m_current_filename);

    this->m_out.clear();

    File::File_To_CC(path, in);

    Decode(in, this->m_out);
}

void Quest::mib_to_file(const sQuest *quest)
{
#define BUF_SIZE 32
    if (quest->check_version()) {
        char    buffer[BUF_SIZE];
        auto*   flags = quest->get_flags();
        auto    id = flags->QuestID;

        snprintf(buffer, BUF_SIZE, "q%05d.mib", id);

        auto outpath = this->m_out_path;
        outpath.append(buffer);

        File::Data_To_File(outpath, quest, QUEST_SIZE);
    }
}
#endif

}
