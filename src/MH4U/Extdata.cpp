#include "MH4U/Extdata.hpp"
#include "MH4U/Quest.hpp"

#include "Tools/File.hpp"
#include "Tools/Folder.hpp"

namespace MH4U {
namespace extdata {
#define BUFFER_SIZE 16
#define PROBE_SIZE sizeof(u32) * 4


DLCExtractor::DLCExtractor()
    : m_ext_quest_data(nullptr)
{
}

DLCExtractor::DLCExtractor(CContainer *ext_quest_data)
    : m_ext_quest_data(ext_quest_data)
{
}

DLCExtractor::~DLCExtractor()
{
    this->m_ext_quest_data = nullptr;
}

void DLCExtractor::get_decoded(CContainer* output)
{
    Crypto::decode_ext_data(*this->m_ext_quest_data, *output);
}

void DLCExtractor::get_all_mib(std::vector<CContainer>& output)
{
    CContainer temp;
    size_t quest_count = 0;

    Crypto::decode_ext_data(*this->m_ext_quest_data, temp);

    quest_count = temp.size() / QUEST_SIZE;

    if (quest_count > EXT_QUEST_DATA_AMMOUNT)
        return;

    for (size_t i = 0; i < quest_count; i++) {
        const sQuest* quest = reinterpret_cast<sQuest*>(temp.data() + i * QUEST_SIZE);

        if (quest->check_version()) {
            output.push_back({});

            output.back().copy(quest, QUEST_SIZE);
        }
    }
}

#ifndef N3DS
DLCExtractor_Helper::DLCExtractor_Helper()
{
}

DLCExtractor_Helper::DLCExtractor_Helper(const std::filesystem::path &path)
    : m_data_path(path), m_out_folder(path)
{
}

void DLCExtractor_Helper::extract(const bool dump_decoded)
{
    auto files = Folder(this->m_data_path).Get_ListOfFiles();

    for (const auto& filepath : files) {
        if (this->is_quest_ext_file(filepath)) {
            CContainer encrypted_ext_quest;

            this->m_dlc_vector.clear();

            if (std::filesystem::file_size(filepath) != 0x50400)
                NotifyError("DLCExtractor_Helper:: quest file corrupt!");

            encrypted_ext_quest.read_file(filepath);

            if (dump_decoded) {
                CContainer decoded;

                DLCExtractor::get_decoded(&decoded);

                std::filesystem::path path = this->m_out_folder;

                path.append(filepath.filename().string() + ".dec");

                decoded.write_to_file(path);
            }

            DLCExtractor::set_ext_quest_data(&encrypted_ext_quest);
            DLCExtractor::get_all_mib(m_dlc_vector);

            this->write();
        }
    }
}

void DLCExtractor_Helper::write(void) const
{
    for (const auto& cc : this->m_dlc_vector) {
        const sQuest* quest = reinterpret_cast<const sQuest*>(cc.data());

        // no need to check, but we do it anyway
        if (quest->check_version()) {
            char    buffer[BUFFER_SIZE];
            auto*   flags = quest->get_sFlags();
            auto    id = flags->quest_id;

            snprintf(buffer, BUFFER_SIZE, "q%05d.mib", id);

            auto outpath = this->m_out_folder;
            outpath.append(buffer);

            File::Data_To_File(outpath, quest, QUEST_SIZE);
        }
    }
}

bool DLCExtractor_Helper::is_quest_ext_file(const std::filesystem::path &path) const
{
    static const std::string names[] = {
        "quest1",
        "quest2",
        "quest3",
        "quest4",
        "quest5"
    };

    for (const auto& name : names) {
        if (name.compare(path.filename().string()) == 0)
            return true;
    }
    return false;
}
#endif

DLCRepacker::DLCRepacker()
    : m_dlc_vector(nullptr)
{
}

DLCRepacker::DLCRepacker(std::vector<CContainer> *dlc_vector)
    : m_dlc_vector(dlc_vector)
{
}

DLCRepacker::~DLCRepacker()
{
    this->m_dlc_vector = nullptr;
}

void DLCRepacker::perform(std::array<CContainer, EXT_QUEST_FILES_AMMOUNT>* output)
{
    auto split = this->split_by_id();
    size_t begin = 0, end = 0;

    for (size_t i = 0; i < EXT_QUEST_FILES_AMMOUNT; i++) {
        CContainer  in;
        CContainer& out = output->at(i);
        const auto& value = split.at(i);

        end += value;

        this->quest_file_populate(in, begin, end);
        this->quest_file_encode(in, out);

        begin += value;
    }
}

void DLCRepacker::quest_file_populate(CContainer &in, const size_t vector_begin_index, const size_t vector_end_index)
{
    char    buffer[BUFFER_SIZE]{0};
    size_t  offset = 0;

    in.resize(EXT_QUEST_DATA_SIZE);

    if (vector_begin_index > vector_end_index || vector_begin_index == vector_end_index)
        return;

    for (size_t i = vector_begin_index; i < vector_end_index; i++) {
        const auto& quest_data = this->m_dlc_vector->at(i);
        const auto* quest = reinterpret_cast<const sQuest*>(quest_data.data());

        Utils::copybytes(in.data() + offset, quest_data.data(), quest_data.size());

        offset += QUEST_SIZE;

        // write quest name
        snprintf(buffer, BUFFER_SIZE, "m%5d.mib", quest->get_sFlags()->quest_id);
        Utils::copybytes(in.data() + offset - 0x10, buffer, BUFFER_SIZE);
    }
}

void DLCRepacker::quest_file_encode(CContainer &in, CContainer &out)
{
    Crypto::encode_ext_data(in, out);

    out.add_after(EXT_QUEST_DATA_PADDING);
}



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
const std::array<u8, 5> DLCRepacker::split_by_id(void) const
{
    std::array<u8, EXT_QUEST_FILES_AMMOUNT> arr{0, 0, 0, 0, 0};

    for (const auto& cc : *this->m_dlc_vector) {
        const auto* quest = reinterpret_cast<const sQuest*>(cc.data());
        const auto& id = quest->get_sFlags()->quest_id;

        if (id >= 60000 && id < 61000) { // quest1 quest2 quest3
            for (size_t i = 0; i < 3; i++) {
                if (arr[i] < EXT_QUEST_DATA_AMMOUNT) {
                    arr[i]++;
                    break;
                } else continue;
            }
        } else if (id >= 61000 && id < 62000) { // quest4
            if (arr[3] < EXT_QUEST_DATA_AMMOUNT)
                arr[3]++;
        } else if (id >= 62000) { // quest5
            if (arr[4] < EXT_QUEST_DATA_AMMOUNT)
                arr[4]++;
        }
    }

    return arr;
}


#ifndef N3DS
DLCRepacker_Helper::DLCRepacker_Helper(const std::filesystem::path &path)
    : m_data_path(path), m_out_folder(path)
{
}

void DLCRepacker_Helper::repack(void)
{
    this->read_files();

    // TODO: look for duplicates

    this->dlc_vector_sort();

    DLCRepacker::set_dlc_vector(&this->m_dlc_vector);
    DLCRepacker::perform(&this->m_encoded_ext_quests);
}

void DLCRepacker_Helper::read_files(void)
{
    auto files = Folder(this->m_data_path).Get_ListOfFiles();

    this->m_dlc_vector.reserve(EXT_QUEST_FILES_AMMOUNT * EXT_QUEST_DATA_AMMOUNT);

    for (const auto& path : files) {
        const auto& size = std::filesystem::file_size(path);
        int res;

        if (size > QUEST_SIZE + XOR_MH4U_SIZE)
            continue;
        else
            res = this->is_dlc_file(path);

        if (res == DLCRepacker_Helper::DLC_DECODED) {
            this->m_dlc_vector.push_back({});
            this->m_dlc_vector.back().read_file(path);
        } else if (res == DLCRepacker_Helper::DLC_ENCODED) {
            CContainer  encoded;

            encoded.read_file(path);

            this->m_dlc_vector.push_back({});

            Crypto::decode_quest(encoded, this->m_encoded_ext_quests.back());
        }
    }
}

void DLCRepacker_Helper::write(void) const
{
    for (size_t i = 0; i < EXT_QUEST_FILES_AMMOUNT; i++) {
        char buffer[BUFFER_SIZE]{0};
        auto path = this->m_out_folder;

        snprintf(buffer, BUFFER_SIZE, "quest%zu", i + 1);

        path.append(buffer);

        this->m_encoded_ext_quests.at(i).write_to_file(path);
    }
}


void DLCRepacker_Helper::dlc_vector_sort(void)
{
    auto filter = [&](const CContainer& left, const CContainer& right) -> const bool
    {
        sQuest* lquest = reinterpret_cast<sQuest*>(left.data());
        sQuest* rquest = reinterpret_cast<sQuest*>(right.data());

        return lquest->get_sFlags()->quest_id > rquest->get_sFlags()->quest_id;
    };

    // default sort move whole container, wasting a lot of time
    //std::sort(this->m_dlc_vector.begin(), this->m_dlc_vector.end(), filter);

    sort_swap_only(this->m_dlc_vector.begin(), this->m_dlc_vector.end(), filter);
}

const int DLCRepacker_Helper::is_dlc_file(const std::filesystem::path &file_path)
{
    CContainer  input(PROBE_SIZE); // 4 values by 4 bytes: [seed, checksum, data, version]
    CContainer  input_decoded;
    sQuest*     quest = nullptr;

    File::file_to_cc_size(file_path, input, PROBE_SIZE);

    quest = reinterpret_cast<sQuest*>(input.data());

    if (quest->check_version())
        return DLCRepacker_Helper::DLC_DECODED;
    else if (Crypto::decode_quest(input, input_decoded))
        return DLCRepacker_Helper::DLC_ENCODED;
    else
        return DLCRepacker_Helper::NOT_A_DLC;
}
#endif

}
}
