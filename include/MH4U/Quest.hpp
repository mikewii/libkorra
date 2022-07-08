#pragma once
#include "types.h"
#include <array>

#include "MH4U/Crypto.hpp"

namespace MH4U {
#define QUEST_SIZE 0x2010
#define XOR_MH4U_SIZE 8
#define EXT_QUEST_FILES_AMMOUNT 5
#define EXT_QUEST_DATA_AMMOUNT 40
#define EXT_QUEST_DATA_PADDING 0xf0
#define EXT_QUEST_DATA_SIZE (QUEST_SIZE * EXT_QUEST_DATA_AMMOUNT) + XOR_MH4U_SIZE + 0x80


enum Language {
    JAPANESE = 0,
    ENGLISH = JAPANESE,
    FRENCH,
    SPANISH,
    GERMAN,
    ITALIAN
};

enum Text {
    TITLE = 0,
    MAIN_GOAL,
    FAILURE,
    SUMMARY,
    MAIN_MONSTER,
    CLIENT,
    SUB_QUEST
};

//////
enum MonsterType {
    LargeMonsters = 0,
    SmallMonsters,
    IntruderMonster
};

struct sIntruderMonster {
    u32         ChanceOfIntruder;
    u32         MonsterID;
    u32         SpawnCount;
    u8          SpawnCondition;
    u8          SpawnMapArea;
    u8          Unk1;
    u8          Unk2_sizeRelated;
    u8          Unk3_sizeRelated;
    u8          Unk4[2];
    u8          FrenzyStatus;
    float       PosX;
    float       PosY;
    float       PosZ;
    u32         Unk5[3];
};

struct sMonsterDefinitions {
    uint32_t	MonsterID;
    uint32_t	Respawn; // spawn count
    uint8_t		SpawnCondition;
    uint8_t		SpawnArea;
    uint8_t		unk[5]; // [0] [1] size related
    uint8_t		FrenzyStatus;
    float		posX;
    float		posY;
    float		posZ;
    uint32_t	unk1[3];
};

struct sSmallMonsterArea {
    sMonsterDefinitions     * pSmallMonster;
};
union sMonsterWave {
        sMonsterDefinitions * pMonsterDefinitions;
        sIntruderMonster    * pIntruderMonster;
        sSmallMonsterArea   * pSmallMonsterArea;
};
/////

/////
enum ItemBoxID {
    SUPPLY_BOX = 0,
    REFILL_SUPPLIES_1,
    REFILL_SUPPLIES_2,
    REFILL_SUPPLIES_3,
    MAIN_REWARD_BOX_A,
    MAIN_REWARD_BOX_B,
    SUB_REWARD_BOX
};

struct sRewardBoxItem {
    u16 chance;
    u16 id;
    u16 ammount;
};

struct sSupplyBoxItem {
    u16 id;
    u16 ammount;
};

struct sItemBox {
    u8  id;
    u8  num_of_slots;
    u16 Unk;

    u32 p_box_items;
};
//////

struct sText { // null terminated
    u32 p_Title;
    u32 p_MainGoal;
    u32 p_Failure;
    u32 p_Summary;
    u32 p_MainMonster;
    u32 p_Client;
    u32 p_SubQuest;

    const u32 get_by_id(const size_t id) const;
};

struct sTextLanguages {
    u32 p_sText_English;
    u32 p_sText_French;
    u32 p_sText_Spanish;
    u32 p_sText_German;
    u32 p_sText_Italian;

    const u32 get_by_id(const size_t id) const;
};

struct sFlags {
    u8  QuestType; // quest type
    u8  AllowRepel; // repel related
    u8  SubAsMain; // subquest related
    u8  Flag4; // ship integrity

    u32 posting_fee;
    u32 reward_zenny;
    u32 penalty_zenny;
    u32 sub_reward_zenny;

    u32 quest_time; // minutes
    u32 intruder_monster_probability;
    u32 p_sTextLanguages;
    u16 quest_id;
    u16 quest_stars;
    u8  map;
    u8  requirements[2];
    u8  unk1;
    u32 unk2;

    u32 main_objective_type_1;
    u16 main_objective_id_1;
    u16 main_objective_count_1;
    u32 main_objective_type_2;
    u16 main_objective_id_2;
    u16 main_objective_count_2;
    u32 sub_objective_type;
    u16 sub_objective_id;
    u16 sub_objective_count;

    u32 p_ChallengePresets;
    u16 quest_icon_id[5];
    u16 unk3;
};

struct MonsterStats {
    u16	size_percentage;
    u8  size_variation;
    u8  health;
    u8  attack;
    u8  defence;
    u8  stamina;
    u8  Unk;
};

struct SmallMonsterUnk  {
    u32	unk1;
    u16	unk2;
    u8  unk3;
    u8  unk4;
};

union HRPoints {
    u32	u;
    s32 s;
};

struct sQuest {
public:
    u32             p_sFlags;
    char        	version[4];

    u32             p_supply_box;
    u8              refill_box_settings[4];
    u32             p_refill_box_1;
    u32             p_refill_box_2;
    u32             p_refill_box_3;
    u32             p_main_reward_box_a;		// 0x1C
    u32             p_main_reward_box_b;
    u32             p_sub_reward_box;

    u32         	p_LargeMonsterWaves;	// 0x28
    u32         	p_SmallMonsterWaves;
    u32         	p_IntruderMonsterWaves;

    MonsterStats	boss_1_stats;
    MonsterStats	boss_2_stats;
    MonsterStats	boss_3_stats;
    MonsterStats	boss_4_stats;
    MonsterStats	boss_5_stats;
    MonsterStats	small_mons_stats;
    SmallMonsterUnk	small_mons_unk[2];

    HRPoints    	reward_points;
    HRPoints    	penalty_points;
    HRPoints    	sub_reward_points;

    u8              unk3[8];
    u8              unk4; // 0x6C
    u8              gathering_level;
    u8              carving_level;
    u8              monsters_ai_level;
    u8              player_spawn_mode;
    u8              is_arena_fence_enabled;
    u8              unk5[2];
    u32             unk6[4];


    void print(const Language language = Language::ENGLISH) const;

    bool                    check_version(void) const;

    sFlags*                 get_sFlags(void);
    const sFlags*           get_sFlags(void) const;

    sItemBox*               get_item_box(const ItemBoxID id);
    const sItemBox*         get_item_box(const ItemBoxID id) const;


    std::vector<sSupplyBoxItem> get_supply_box_items(const ItemBoxID id);

    std::u16string          get_text(const Language language, const Text choice);
    const std::u16string    get_text(const Language language, const Text choice) const;


private:
    const u32 get_item_box_by_id(const ItemBoxID id) const;

    sTextLanguages*         get_sTextLanguages(void);
    const sTextLanguages*   get_sTextLanguages(void) const;

    sText*                  get_sText(const Language language);
    const sText*            get_sText(const Language language) const;
};

class QuestEditor : virtual public Crypto
{
#define SUPPLY_BOX_MAX_ITEMS 5 * 8
#define MAIN_REWARD_BOX_A_MAX_ITEMS 2 * 8
#define MAIN_REWARD_BOX_B_MAX_ITEMS 4 * 8
#define SUB_REWARD_BOX_MAX_ITEMS 4 * 1
    /*
     * reward box screen 1 have 8*2 and 8*4 cells
     * reward box screen 2 have 4*1(green, sub) 4*1(grey) 2*8(red, base) 3*8(blue, wound/cap)
    */

    QuestEditor();
    QuestEditor(const CContainer& cc);
    QuestEditor(const std::filesystem::path& path);
    ~QuestEditor();

    void initialize(void);


private:
    CContainer m_data;
    sQuest* m_p_header;

    sQuest m_header;
    sFlags m_flags;

    sItemBox m_supply_box;
    sItemBox m_refill_box_1;
    sItemBox m_refill_box_2;
    sItemBox m_refill_box_3;
    sItemBox m_main_reward_box_a;
    sItemBox m_main_reward_box_b;
    sItemBox m_sub_reward_box;

    std::array<sSupplyBoxItem, SUPPLY_BOX_MAX_ITEMS> m_supply_box_items;
    std::array<sSupplyBoxItem, SUPPLY_BOX_MAX_ITEMS> m_refill_box_1_items;
    std::array<sSupplyBoxItem, SUPPLY_BOX_MAX_ITEMS> m_refill_box_2_items;
    std::array<sSupplyBoxItem, SUPPLY_BOX_MAX_ITEMS> m_refill_box_3_items;
    std::array<sSupplyBoxItem, MAIN_REWARD_BOX_A_MAX_ITEMS> m_main_reward_box_a_items;
    std::array<sSupplyBoxItem, MAIN_REWARD_BOX_B_MAX_ITEMS> m_main_reward_box_b_items;
    std::array<sSupplyBoxItem, SUB_REWARD_BOX_MAX_ITEMS> m_sub_reward_box_items;



    void read_header(void);
};

#ifndef N3DS
class Quest : virtual public Crypto
{
public:
    Quest();
    Quest(const std::filesystem::path& path);
    ~Quest();

    void set_out_path(const std::filesystem::path& path) { this->m_out_path = path; }

    void create_ext_quests_files(const std::filesystem::path& path);


    void decrypt_all(void);
    void encrypt_all(void);

    void decrypt_ext_quest_file(const std::string& name = "");

private:
    enum {
        NOT_A_DLC = 0,
        DLC_DECODED,
        DLC_ENCODED
    };

    CContainer m_out;
    std::filesystem::path m_current_path;
    std::filesystem::path m_out_path;
    std::string m_current_filename;

    std::vector<CContainer*> m_quests;

    bool m_working_on_folder = false;

    void extract_signature(void);
    void make_output_dir(void);
    bool is_quest_ext_file(const std::filesystem::path& path) const;
    void dump_decoded(void);

    void load_and_decode(void);
    void mib_to_file(const sQuest* quest);

    void id_check(void);

    const int is_dlc_probe(const std::filesystem::path& path);

    void sort(void);

    const std::array<u8, EXT_QUEST_FILES_AMMOUNT> split_by_id(void) const;

    void populate_ext_quest_file(CContainer& in, const size_t vector_begin_index, const size_t vector_end_index);
    void encode_ext_quest_file(CContainer& in, CContainer& out);
    void write_ext_quest_file(CContainer& cc, const size_t& file_num);

#endif
};

}
