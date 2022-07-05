#include "MH4U/Quest.hpp"

namespace MH4U {
const u32 sText::get_by_id(const size_t id) const
{
    switch (id) {
    default:
    case Text::TITLE: return this->p_Title;
    case Text::MAIN_GOAL: return this->p_MainGoal;
    case Text::FAILURE: return this->p_Failure;
    case Text::SUMMARY: return this->p_Summary;
    case Text::MAIN_MONSTER: return this->p_MainMonster;
    case Text::CLIENT: return this->p_Client;
    case Text::SUB_QUEST: return this->p_SubQuest;
    }
}

const u32 sTextLanguages::get_by_id(const size_t id) const
{
    switch (id) {
    default:
    case Language::ENGLISH: return this->p_sText_English;
    case Language::FRENCH: return this->p_sText_French;
    case Language::SPANISH: return this->p_sText_Spanish;
    case Language::GERMAN: return this->p_sText_German;
    case Language::ITALIAN: return this->p_sText_Italian;
    }
}

bool sQuest::check_version(void) const
{
    static const char VERSION[5] = "v005";

    if (std::equal(VERSION, VERSION + 4, this->version))
        return true;
    else return false;
}

sFlags* sQuest::get_sFlags(void)
{
    return reinterpret_cast<sFlags*>(reinterpret_cast<char*>(this) + this->p_sFlags);
}

const sFlags* sQuest::get_sFlags(void) const
{
    return reinterpret_cast<const sFlags*>(reinterpret_cast<const char*>(this) + this->p_sFlags);
}

sItemBox *sQuest::get_item_box(const ItemBoxID id)
{
    auto offset = this->get_item_box_by_id(id);

    if (offset == 0)
        return nullptr;

    return reinterpret_cast<sItemBox*>(reinterpret_cast<char*>(this) + offset);
}

const sItemBox *sQuest::get_item_box(const ItemBoxID id) const
{
    auto offset = this->get_item_box_by_id(id);

    if (offset == 0)
        return nullptr;

    return reinterpret_cast<const sItemBox*>(reinterpret_cast<const char*>(this) + offset);
}

std::vector<sSupplyBoxItem> sQuest::get_supply_box_items(const ItemBoxID id)
{
    std::vector<sSupplyBoxItem> supply_box_items;
    bool proceed = false;

    switch (id) {
    default:break;
    case ItemBoxID::SUPPLY_BOX: proceed = true; break;
    case ItemBoxID::REFILL_SUPPLIES_1: proceed = true; break;
    case ItemBoxID::REFILL_SUPPLIES_2: proceed = true; break;
    case ItemBoxID::REFILL_SUPPLIES_3: proceed = true; break;
    }

    if (proceed) {
        auto offset = this->get_item_box(id)->p_box_items;
        sSupplyBoxItem* item_array = reinterpret_cast<sSupplyBoxItem*>(reinterpret_cast<char*>(this) + offset);

        supply_box_items.reserve(SUPPLY_BOX_MAX_ITEMS);

        for (size_t i = 0; i < SUPPLY_BOX_MAX_ITEMS; i++)
            supply_box_items.push_back(item_array[i]);
    }

    return supply_box_items;
}

sTextLanguages *sQuest::get_sTextLanguages(void)
{
    auto offset = this->get_sFlags()->p_sTextLanguages;

    return reinterpret_cast<sTextLanguages*>(reinterpret_cast<char*>(this) + offset);
}

const sTextLanguages *sQuest::get_sTextLanguages(void) const
{
    auto offset = this->get_sFlags()->p_sTextLanguages;

    return reinterpret_cast<const sTextLanguages*>(reinterpret_cast<const char*>(this) + offset);
}

sText *sQuest::get_sText(const Language language)
{
    auto offset = this->get_sTextLanguages()->get_by_id(language);

    return reinterpret_cast<sText*>(reinterpret_cast<char*>(this) + offset);
}

const sText *sQuest::get_sText(const Language language) const
{
    auto offset = this->get_sTextLanguages()->get_by_id(language);

    return reinterpret_cast<const sText*>(reinterpret_cast<const char*>(this) + offset);
}

std::u16string sQuest::get_text(const Language language, const Text choice)
{
    auto offset = this->get_sText(language)->get_by_id(choice);

    return std::u16string(reinterpret_cast<std::u16string::value_type*>(reinterpret_cast<char*>(this) + offset));
}

const std::u16string sQuest::get_text(const Language language, const Text choice) const
{
    auto offset = this->get_sText(language)->get_by_id(choice);

    return std::u16string(reinterpret_cast<const std::u16string::value_type*>(reinterpret_cast<const char*>(this) + offset));
}

const u32 sQuest::get_item_box_by_id(const ItemBoxID id) const
{
    switch (id) {
    default:
    case ItemBoxID::SUPPLY_BOX: return this->p_supply_box;
    case ItemBoxID::REFILL_SUPPLIES_1: return this->p_refill_box_1;
    case ItemBoxID::REFILL_SUPPLIES_2: return this->p_refill_box_2;
    case ItemBoxID::REFILL_SUPPLIES_3: return this->p_refill_box_3;
    case ItemBoxID::MAIN_REWARD_BOX_A: return this->p_main_reward_box_a;
    case ItemBoxID::MAIN_REWARD_BOX_B: return this->p_main_reward_box_b;
    case ItemBoxID::SUB_REWARD_BOX: return this->p_sub_reward_box;
    }
}

void sQuest::print(const Language language) const
{
    std::wstring_convert<std::codecvt_utf8<std::u16string::value_type>, std::u16string::value_type> converter;
    std::vector<std::u16string> strings;

    // text:
    strings.push_back(this->get_text(language, Text::TITLE));
    strings.push_back(this->get_text(language, Text::MAIN_GOAL));
    strings.push_back(this->get_text(language, Text::FAILURE));
    strings.push_back(this->get_text(language, Text::SUMMARY));
    strings.push_back(this->get_text(language, Text::MAIN_MONSTER));
    strings.push_back(this->get_text(language, Text::CLIENT));
    strings.push_back(this->get_text(language, Text::SUB_QUEST));

    for (const auto& string : strings)
        printf("%s\n", converter.to_bytes(string).c_str());
}

}

