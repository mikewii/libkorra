#include "MH4U/Quest.hpp"

#include "Tools/Folder.hpp"
#include "Tools/File.hpp"

namespace MH4U {

#ifndef N3DS
Quest::Quest(const std::filesystem::path &path)
    : m_current_path(path)
{
    this->m_current_path.make_preferred();

    if (std::filesystem::is_directory(this->m_current_path))
        this->m_working_on_folder = true;

    else if (std::filesystem::is_regular_file(path))
        this->m_working_on_folder = false;

    this->make_output_dir();
}

void Quest::create_ext_save(const std::filesystem::path &path, const bool by_extention)
{
    CContainer  cc(EXT_QUEST_DATA_SIZE);
    auto        file_list = Folder(path).Get_ListOfFiles();

    for (const auto& file_path : file_list) {
        auto size = std::filesystem::file_size(file_path);
        const auto extention = file_path.extension();

        if (size > QUEST_SIZE)
            continue;


        auto res = this->is_quest(file_path);

        continue;
    }
}

void Quest::decrypt_all(void)
{
    if (this->m_working_on_folder) {
        Folder  folder(this->m_current_path);
        auto    files = folder.Get_ListOfFiles();

        for (const auto& filepath : files) {
            if (this->is_quest_ext_file(filepath)) {
                this->m_current_filename = filepath.filename().string();

                this->decrypt_ext_quest_file();
            }
        }
    } else {
        this->decrypt_ext_quest_file();
    }
}

void Quest::decrypt_ext_quest_file(const std::string& name)
{
    int quest_count;

    if (!name.empty())
        this->m_current_filename = name;

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
    if (!this->m_working_on_folder) {
        this->m_current_filename = this->m_current_path.filename();
        this->m_current_path.remove_filename();
    }

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

    File::file_to_cc(path, in);

    // last 0x100 bytes clean, removing them from size to prevent blowfish writing data there
    // to avoid confusing checksum check
    in.set_size(in.size() - 0x100);

    if (in.size() != 0x50300)
        NotifyError("quest file corrupt!");

    decode(in, this->m_out);
}

void Quest::mib_to_file(const sQuest *quest)
{
#define BUF_SIZE 32
    if (quest->check_version()) {
        char    buffer[BUF_SIZE];
        auto*   flags = quest->get_sFlags();
        auto    id = flags->quest_id;

        snprintf(buffer, BUF_SIZE, "q%05d.mib", id);

        auto outpath = this->m_out_path;
        outpath.append(buffer);

        File::Data_To_File(outpath, quest, QUEST_SIZE);
    }
}

void Quest::id_check(void)
{
    /* ranges:
     * 60000 to 61000 - normal quest
     * 61000 to 62000 - arena challenge quests
     * 62000 -> ~ - gq?
     *
     * 61200 - 62000
     *
     * from ext quests:
     * quest1: 60001 - 60207
     * quest2: 60208 - 60251
     * quest3: empty in mh4u, 60229 - 60259 in mh4g
     * quest4: 61001 - 61208
     * quest5: 62101 - 62212
     *
     * types:
     * LR | HR | G
     * LR/HR Arena | G Arena
     * Episode events (possibly count as lr|hr|g)
    */
}

bool Quest::is_quest(const std::filesystem::path &path) const
{
#define PROBE_SIZE sizeof(u32) * 4
    CContainer  probe_data(PROBE_SIZE); // [seed, checksum, data, version]
    CContainer  probe_data_decoded;
    sQuest*     quest = nullptr;

    File::file_to_cc_size(path, probe_data, PROBE_SIZE);

    quest = reinterpret_cast<sQuest*>(probe_data.data());

    if (quest->check_version())
        return true;

    for (size_t key = 0; key < MH4U::Key::ENUM_LENGTH; key++) {
        MH4U::blowfish_decode(probe_data, probe_data_decoded, static_cast<MH4U::Key>(key));

        quest = reinterpret_cast<sQuest*>(probe_data_decoded.data());

        if (quest->check_version())
            return true;
    }

    return false;
}

#endif

}
