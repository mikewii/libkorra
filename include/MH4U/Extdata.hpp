#pragma once
#include "types.h"
#include "Global.hpp"
#include "MH4U/Crypto.hpp"

#include <filesystem>
#include <array>

namespace MH4U {
namespace extdata {
#define QUEST_SIZE 0x2010
#define XOR_MH4U_SIZE 8
#define EXT_QUEST_FILES_AMMOUNT 5
#define EXT_QUEST_DATA_AMMOUNT 40
#define EXT_QUEST_DATA_PADDING 0xf0
#define EXT_QUEST_DATA_SIZE (QUEST_SIZE * EXT_QUEST_DATA_AMMOUNT) + XOR_MH4U_SIZE + 0x80

// input is container with encoded quest data
class DLCExtractor : virtual protected Crypto
{
public:
    DLCExtractor();
    DLCExtractor(CContainer* ext_quest_data);
    ~DLCExtractor();

    void set_ext_quest_data(CContainer* data) { this->m_ext_quest_data = data; }

    void get_decoded(CContainer* output);
    void get_all_mib(std::vector<CContainer*>* output);

private:
    CContainer* m_ext_quest_data;
};

#ifndef N3DS
class DLCExtractor_Helper : protected DLCExtractor
{
public:
    DLCExtractor_Helper();
    DLCExtractor_Helper(const std::filesystem::path& path);
    ~DLCExtractor_Helper();

    void extract(const bool dump_decoded = false);

    void                            set_data_path(const std::filesystem::path& path) { this->m_data_path = path; }
    const std::filesystem::path&    get_data_path(void) const { return this->m_data_path; }

    void                            set_out_folder(const std::filesystem::path& path) { this->m_out_folder = path; }
    const std::filesystem::path&    get_out_folder(void) const { return this->m_out_folder; }

private:
    std::filesystem::path       m_data_path;
    std::filesystem::path       m_out_folder;
    std::vector<CContainer*>    m_dlc_vector;

    bool is_quest_ext_file(const std::filesystem::path &path) const;
    void clear_dlc_vector(void);

    void write(void) const;
};
#endif

////////////////////////////////////////////////////////////////////////////////

class DLCRepacker : virtual protected Crypto
{
public:
    DLCRepacker();
    DLCRepacker(std::vector<CContainer*>* dlc_vector);
    ~DLCRepacker();

    void perform(std::array<CContainer, EXT_QUEST_FILES_AMMOUNT>* output);

    void set_dlc_vector(std::vector<CContainer*>* vector) { this->m_dlc_vector = vector; }

private:
    std::vector<CContainer*>* m_dlc_vector;

    void quest_file_populate(CContainer& in, const size_t vector_begin_index, const size_t vector_end_index);
    void quest_file_encode(CContainer& in, CContainer& out);

    const std::array<u8, EXT_QUEST_FILES_AMMOUNT> split_by_id(void) const;
};

#ifndef N3DS
class DLCRepacker_Helper : protected DLCRepacker
{
public:
    DLCRepacker_Helper();
    DLCRepacker_Helper(const std::filesystem::path& path);
    ~DLCRepacker_Helper();

    void repack(void);
    void write(void) const;

    void                            set_data_path(const std::filesystem::path& path) { this->m_data_path = path; }
    const std::filesystem::path&    get_data_path(void) const { return this->m_data_path; }

    void                            set_out_folder(const std::filesystem::path& path) { this->m_out_folder = path; }
    const std::filesystem::path&    get_out_folder(void) const { return this->m_out_folder; }

private:
    std::filesystem::path       m_data_path;
    std::filesystem::path       m_out_folder;
    std::vector<CContainer*>    m_dlc_vector;
    std::array<CContainer, EXT_QUEST_FILES_AMMOUNT> m_encoded_ext_quests;


    void read_files(void);
    void dlc_vector_sort(void);
    const int is_dlc_file(const std::filesystem::path& file_path);

    enum {
        NOT_A_DLC = 0,
        DLC_DECODED,
        DLC_ENCODED
    };
};
#endif

}
}
