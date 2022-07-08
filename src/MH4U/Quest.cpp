#include "MH4U/Quest.hpp"

#include "Tools/Folder.hpp"
#include "Tools/File.hpp"
#include <algorithm>

namespace MH4U {

#ifndef N3DS
Quest::Quest()
{
}

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

Quest::~Quest()
{
    for (auto* handle : this->m_quests) {
        if (handle) {
            delete handle;
            handle = nullptr;
        }
    }
}

void Quest::create_ext_quests_files(const std::filesystem::path &path)
{
    std::array<u8, EXT_QUEST_FILES_AMMOUNT> split;
    size_t  begin = 0, end = 0;
    auto    files = Folder(path).Get_ListOfFiles();


    this->m_quests.reserve(EXT_QUEST_FILES_AMMOUNT * EXT_QUEST_DATA_AMMOUNT);

    for (const auto& path : files) {
        const auto& size = std::filesystem::file_size(path);
        int res;

        if (size > QUEST_SIZE + XOR_MH4U_SIZE)
            continue;
        else
            res = this->is_dlc_probe(path);

        if (res == Quest::DLC_DECODED) {
            CContainer* decoded = new CContainer;

            decoded->read_file(path);

            this->m_quests.push_back(decoded);
        } else if (res == Quest::DLC_ENCODED) {
            CContainer  encoded;
            CContainer* decoded = new CContainer;

            encoded.read_file(path);

            Crypto::decode_quest(encoded, *decoded);

            this->m_quests.push_back(decoded);
        }
    }

    this->sort();

    split = this->split_by_id();

    for (size_t i = 0; i < EXT_QUEST_FILES_AMMOUNT; i++) {
        CContainer  in, out;
        const auto& value = split.at(i);

        end += value;

        this->populate_ext_quest_file(in, begin, end);
        this->encode_ext_quest_file(in, out);
        this->write_ext_quest_file(out, i + 1);

        begin += value;
    }

    return;
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
    this->dump_decoded();

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

    Crypto::decode_ext_data(in, this->m_out);
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

const int Quest::is_dlc_probe(const std::filesystem::path &path)
{
#define PROBE_SIZE sizeof(u32) * 4
    CContainer  probe_data(PROBE_SIZE); // [seed, checksum, data, version]
    CContainer  probe_data_decoded;
    sQuest*     quest = nullptr;

    File::file_to_cc_size(path, probe_data, PROBE_SIZE);

    quest = reinterpret_cast<sQuest*>(probe_data.data());

    if (quest->check_version())
        return Quest::DLC_DECODED;
    else if (Crypto::decode_quest(probe_data, probe_data_decoded))
        return Quest::DLC_ENCODED;
    else
        return Quest::NOT_A_DLC;
}

void Quest::sort(void)
{
    auto filter = [&](const CContainer* left, const CContainer* right) -> const bool
    {
        sQuest* lquest = reinterpret_cast<sQuest*>(left->data());
        sQuest* rquest = reinterpret_cast<sQuest*>(right->data());

        return lquest->get_sFlags()->quest_id < rquest->get_sFlags()->quest_id;
    };

    std::sort(this->m_quests.begin(), this->m_quests.end(), filter);
}

const std::array<u8, EXT_QUEST_FILES_AMMOUNT> Quest::split_by_id(void) const
{
    std::array<u8, EXT_QUEST_FILES_AMMOUNT> count{0, 0, 0, 0, 0};

    for (const auto* cc : this->m_quests) {
        const auto* quest = reinterpret_cast<const sQuest*>(cc->data());
        const auto& id = quest->get_sFlags()->quest_id;

        if (id >= 60000 && id < 61000) { // quest1 quest2 quest3
            for (size_t i = 0; i < 3; i++) {
                if (count[i] < EXT_QUEST_DATA_AMMOUNT) {
                    count[i]++;
                    break;
                } else continue;
            }
        } else if (id >= 61000 && id < 62000) { // quest4
            if (count[3] < EXT_QUEST_DATA_AMMOUNT)
                count[3]++;
        } else if (id >= 62000) { // quest5
            if (count[4] < EXT_QUEST_DATA_AMMOUNT)
                count[4]++;
        }
    }

    return count;
}

void Quest::populate_ext_quest_file(CContainer &in, const size_t vector_begin_index, const size_t vector_end_index)
{
#define BUFFER_SIZE 16
    char    buffer[BUFFER_SIZE]{0};
    size_t  offset = 0;

    in.resize(EXT_QUEST_DATA_SIZE);

    if (vector_begin_index > vector_end_index || vector_begin_index == vector_end_index)
        return;

    for (size_t i = vector_begin_index; i < vector_end_index; i++) {
        const auto* quest_data = this->m_quests.at(i);
        const auto* quest = reinterpret_cast<const sQuest*>(quest_data->data());

        Utils::copybytes(in.data() + offset, quest_data->data(), quest_data->size());

        offset += QUEST_SIZE;

        // write quest name
        snprintf(buffer, BUFFER_SIZE, "m%5d.mib", quest->get_sFlags()->quest_id);
        Utils::copybytes(in.data() + offset - 0x10, buffer, BUFFER_SIZE);
    }
}

void Quest::encode_ext_quest_file(CContainer &in, CContainer &out)
{
    Crypto::encode_ext_data(in, out);

    out.add_after(EXT_QUEST_DATA_PADDING);
}

void Quest::write_ext_quest_file(CContainer &cc, const size_t& file_num)
{
#define BUFFER_SIZE 16
    char buffer[BUFFER_SIZE]{0};
    auto out_path = this->m_out_path;

    snprintf(buffer, BUFFER_SIZE, "quest%zu", file_num);

    out_path.append(buffer);

    cc.write_To_File(out_path);
}

#endif

}
