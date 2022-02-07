#pragma once
#include <array>

namespace MHXX {

constexpr std::array<const char*, 3> StartType_str = {
    "Base Camp",
    "Random",
    "Elder Dragon Fight"
};

constexpr std::array<const char*, 17> QuestLv_str = {
    "(None)",
    "1*",
    "2*",
    "3*",
    "4*",
    "5*",
    "6*",
    "7*",
    "8*",
    "9*",
    "10*",
    "G1",
    "G2",
    "G3",
    "G4",
    "G5",
    "EX"
};

constexpr std::array<const char*, 33> Maps_str = {
    "(None)",
    "J. Frontier (D)",
    "J. Frontier (N)",
    "V. Hills (D)",
    "V. Hills (N)",
    "A. Ridge (D)",
    "A. Ridge (N)",
    "M. Peaks (D)",
    "M. Peaks (N)",
    "Dunes",
    "D. Island",
    "Marshlands",
    "Volcano",
    "Arena",
    "V. Slayground",
    "A. Steppe",
    "V. Hollow",
    "Primal Forest",
    "F. Seaway",
    "F. Slayground",
    "Sanctuary",
    "Forlorn Arena",
    "S. Pinnacle",
    "Ingle Isle",
    "Polar Field",
    "Wyvern's End",
    "Desert",
    "Jungle",
    "Ruined Pinnacle",
    "Castle Schrade",
    "Fortress",
    "Forlorn Citadel",

    "Error Map ID"
};

constexpr std::array<const char*, 50> EntryType = {
    "None",
    "HR 2 and up",
    "HR 3 and up",
    "HR 4 and up",
    "HR 5 and up",
    "HR 6 and up",
    "HR 7 and up",
    "HR 8 and up",
    "HR 9 and up",
    "HR 10 and up",
    "HR 11 and up",
    "HR 12 and up",
    "HR 13 and up",
    "HR 20 and up",
    "HR 25 and up",
    "HR 30 and up",
    "HR 45 and up",
    "HR 50 and up",
    "HR 60 and up",
    "HR 100 and up",
    "Great Swords only",
    "Long Swords only",
    "Sword & Shields only",
    "Dual Blades only",
    "Lances only",
    "Gunlances only",
    "Hammers only",
    "Hunting Horns only",
    "Switch Axes only",
    "Charge Blades only",
    "Insect Glaives only",
    "Light Bowguns only",
    "Heavy Bowguns only",
    "Bows only",
    "Blademasters only",
    "Gunners only",
    "Guild Style only",
    "Striker Style only",
    "Aerial Style only",
    "Adept Style only",
    "Valor Style only",
    "Alchemy Style only",
    "No Prowlers",
    "Prowlers only",
    "RARE 1 weapons only",
    "No armor/talismans",
    "On-site items only",
    "One player max",
    "Two players max",
    "Three players max"
};

constexpr std::array<const char*, 7> EnemyLv_str = {
    "Dumb",
    "Low Rank",
    "Low Rank+",
    "High Rank",
    "High Rank+",
    "G Rank",
    "G Rank+"
};

constexpr std::array<const char*, 3> clearType = {
    "None",
    "And",
    "Or"
};

constexpr std::array<const char*, 6> QuestType0_str = {
    "Hunting",
    "Slaying",
    "Capture",
    "Gathering",
    "hunt-a-ton",
    "hunt-a-ton arena",
};

constexpr std::array<const char*, 11> QuestType1_str = {
    "Kokoto (Unlockable)",
    "Pokke (Unlockable",
    "Yukumo (Unlockable",
    "Bherna (Unlockable",

    "Kokoto (Main)",
    "Pokke (Main)",
    "Yukumo (Main)",
    "Bherna (Main)",

    "ProwlerOnly",
    "Default",
    "SpecialPermit"
};

constexpr std::array<const char*, 9> clearParam = {
    "Capture",
    "Hunt",
    "Gathering",
    "Hunt All",
    "Slay All",
    "Deliver",
    "",
    "",
    "Break"
};

} // MHXX
