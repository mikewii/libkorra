#pragma once
#include <array>

namespace MHXX {

constexpr std::array<const char*, 3> StartType_str = {
    "Base Camp",
    "Random",
    "Elder Dragon Fight"
};

constexpr std::array<const char*, 15> QuestLv_str = {
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
    "Special"
};

constexpr std::array<const char*, 31> Maps_str = {
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
    "A. Steppe",
    "Primal Forest",
    "F. Seaway",
    "V. Hollow",
    "Forlorn Arena",
    "Sanctuary",
    "Ingle Isle",
    "Polar Field",
    "S. Pinnacle",
    "Wyvern's End",
    "Arena",
    "V. Slayground",
    "F. Slayground",
    "Desert",
    "Jungle",
    "Ruined Pinnacle",
    "Castle Schrade",
    "Fortress",
    "Forlorn Citadel"
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

constexpr std::array<const char*, 6> QuestType_str = {
    "Hunting",
    "Slaying",
    "Capture",
    "Gathering",
    "hunt-a-ton",
    "hunt-a-ton arena",
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
