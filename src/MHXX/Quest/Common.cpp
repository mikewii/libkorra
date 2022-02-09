#include "MHXX/Quest/Common.hpp"
#include "MHXX/Strings/Common.hpp"

namespace MHXX {

constexpr static const char* error = "Error in [%s] value out of range! : %d | %08X";

void err(const char* className, const u32 value) { printf(error, className, value, value); }

const std::array<const char*, 32> Maps::str = {
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
    "Forlorn Citadel"
};
const char *Maps::GetStr(const Maps::size id)
{
    auto choice = 0;

    switch(id){
    case Maps::e::J_Frontier_D:{break;}
    case Maps::e::J_Frontier_N:{choice = (id - Special) + 1; break;}
    case Maps::e::V_Hills_D:{choice = id + 1; break;}
    case Maps::e::V_Hills_N:{choice = (id - Special) + 2; break;}
    case Maps::e::A_Ridge_D:{choice = id + 2; break;}
    case Maps::e::A_Ridge_N:{choice = (id - Special) + 3; break;}
    case Maps::e::M_Peaks_D:{choice = id + 3; break;}
    case Maps::e::M_Peaks_N:{choice = (id - Special) + 4; break;}
    default:
    {
        if (id > Maps::e::Forlorn_Citadel || id < 0)
        {
            err("Maps", id);
            return "Maps error!";
        }
        else choice = id + 4;
    }
    }

    return Maps::str.at(choice);
}


const std::array<const char*, 4> EnemyLv::str = {
    "Training",
    "Low Rank",
    "High Rank",
    "G Rank"
};
const char* EnemyLv::GetStr(const size level)
{
    switch(level){
    default:{
        err("EnemyLv", level);
        return "EnemyLv error!";
    }
    case e::Training: return EnemyLv::str.at(0);
    case e::LR: return EnemyLv::str.at(1);
    case e::HR: return EnemyLv::str.at(2);
    case e::G: return EnemyLv::str.at(3);
    }
}

const std::array<const char*, 17> QuestLv::str = {
    "Training",
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
const char *QuestLv::GetStr(const QuestLv::size level)
{
    if (level > MHXX::Special) return QuestLv::str[level - MHXX::Special];
    else return QuestLv::str[level];
}

const std::array<const char*, 6> QuestType0::str = {
    "Hunting",
    "Slaying",
    "Capture",
    "Gathering",
    "hunt-a-ton",
    "hunt-a-ton arena"
};
const char *QuestType0::GetStr(const QuestType0::size type)
{
    if (type < QuestType0::e::Hunting || type > QuestType0::e::dummy1)
    {
        err("QuestType0", type);
        return nullptr;
    }
    else if (type == QuestType0::e::dummy0 || type == QuestType0::e::dummy1) return "dummy";

    return QuestType0::str.at(type);
}

const std::array<const char*, 12> QuestType1::str = {
    "Training/Arena/Dummy",

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
const char* QuestType1::GetStr(const size type) { return QuestType1::str.at(type); }


const std::array<const char*, 3> StartType::str = {
    "Base Camp",
    "Random",
    "Elder Dragon Fight"
};
const char* StartType::GetStr(const size type) { return StartType::str.at(type); }


const std::array<const char*, 3> BGMType::str = {
    "Default",
    "Prowler Special",
    "Training"
};
const char* BGMType::GetStr(const size type) { return BGMType::str.at(type); }


const std::array<const char*, 50> EntryType::str = {
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
const char* EntryType::GetStr(const size type) { return EntryType::str.at(type); }


const std::array<const char*, 3> ClearType::str = {
    "One target",
    "Two targets",
    "One target and ticket"
};
const char* ClearType::GetStr(const size type) { return ClearType::str.at(type); }


const std::array<const char*, 5> CarvingLv::str = {
    "LR Special",
    "LR",
    "HR",
    "Arena",
    "G"
};
const char* CarvingLv::GetStr(const size level) { return CarvingLv::str.at(level); }


const std::array<const char*, 8> GatheringLv::str = {
    "Arena 0",
    "LR",
    "HR",
    "Arena 1",
    "Special 0",
    "Special 1",
    "Special 2",
    "G"
};
const char* GatheringLv::GetStr(const size level)
{
    if (level == GatheringLv::e::G) return GatheringLv::str.at(7);
    else return GatheringLv::str.at(level);
}



const std::array<const char*, 4> FishingLv::str = {
    "LR",
    "HR",
    "Arena",
    "G"
};
const char* FishingLv::GetStr(const size level) { return FishingLv::str.at(level); }



const std::array<const char*, Icon::STR_ARRAY_SIZE> Icon::str = {
    "(None)",
    "Rathian",
    "Gold Rathian",
    "Dreadqueen",
    "Rathalos",
    "Silver Rathalos",
    "Dreadking",
    "Khezu",
    "Yiak Kut-Ku",
    "Gypceros",
    "Plesioth",
    "Kirin",
    "Velocidrome",
    "Gendrome",
    "Iodrome",
    "Cephadrome",
    "Yian Garuga",
    "Deadeye",
    "Daimyo Hermitaur",
    "Stonefist",
    "Shogun Ceanataur",
    "Blangonga",
    "Rajang",
    "Furious Rajang",
    "Kushala Daora",
    "Chameleos",
    "Teostra",
    "Bulldrome",
    "Tigrex",
    "Grimclaw",
    "Akantor",
    "Lavasioth",
    "Nargacuga",
    "Silverwind",
    "Ukanlos",
    "Deviljho",
    "Savage Deviljho",
    "Uragaan",
    "Crystalbeard",
    "Lagiacrus",
    "Royal Ludroth",
    "Agnaktor",
    "Alatreon",
    "Duramboros",
    "Nibelsnarf",
    "Zinogre",
    "Thunderlord",
    "Amatsu",
    "Arzuros",
    "Redhelm",
    "Lagombi",
    "Snowbaron",
    "Volvidon",
    "Brachydios",
    "Kecha Wacha",
    "Tetsucabra",
    "Drilltusk",
    "Zamtrios",
    "Najarala",
    "Seltas Queen",
    "Gore Magala",
    "Shagaru Magala",
    "Seltas",
    "Seregios",
    "Malfestio",
    "Glavenus",
    "Hellblade",
    "Astalos",
    "Mizutsune",
    "Gammoth",
    "Nakarkos",
    "Great Maccao",
    "Apnototh",
    "Apceros",
    "Kelbi",
    "Mosswine",
    "Hornetaur",
    "Vespoid ",
    "Felyne",
    "Melynx",
    "Velociprey",
    "Genprey",
    "Ioprey",
    "Cephalos",
    "Bullfango",
    "Popo",
    "Giaprey",
    "Anteka",
    "Remobra",
    "Hermitaur",
    "Ceanataur",
    "Blango",
    "Rhenoplos",
    "Bnahabra",
    "Altaroth",
    "Jaggi",
    "Jaggia",
    "Ludroth",
    "Uroktor",
    "Slagtoth",
    "Gargwa",
    "Zamite",
    "Konchu",
    "Maccao",
    "Larinoth",
    "Moofah",
    "Danger",
    "Cross Symbol",
    "Palico",
    "Egg",
    "Rocks",
    "Fish",
    "Bones",
    "Insect",
    "Mushroom",
    "Accounting",
    "Harvest Tour",
    "Box",
    "Nakarkos Head",
    "Nakarkos Tail",
    "Basarios",
    "Gravios",
    "Diablos",
    "Lao-Shan Lung",
    "Congalala",
    "Giadrome",
    "Barioth",
    "Barroth",
    "Raging Brachydios",
    "Nerscylla",
    "Chaotic Gore Magala",
    "Conga",
    "Great Thunderbug",
    "Bloodbath",
    "Boltreaver",
    "Elderfrost",
    "Soulseer",
    "Rustrazor",
    "Nightcloak",
    "Ahtal-Ka Mecha",
    "Ahtal-Ka",
    "Valstrax",
    "None (Hyper)",
    "Rathian (Hyper)",
    "Gold Rathian (Hyper)",
    "Rathalos (Hyper)",
    "Silver Rathalos (Hyper)",
    "Khezu (Hyper)",
    "Basarios (Hyper)",
    "Gravios (Hyper)",
    "Diablos (Hyper)",
    "Yian Kut-Ku (Hyper)",
    "Gypceros (Hyper)",
    "Plesioth (Hyper)",
    "Yian Garuga (Hyper)",
    "Daimyo Hermitaur (Hyper)",
    "Shogun Ceanataur (Hyper)",
    "Congalala (Hyper)",
    "Blangonga (Hyper)",
    "Rajang (Hyper)",
    "Tigrex (Hyper)",
    "Lavasioth (Hyper)",
    "Nargacuga (Hyper)",
    "Barioth (Hyper)",
    "Deviljho (Hyper)",
    "Barroth (Hyper)",
    "Uragaan (Hyper)",
    "Lagiacrus (Hyper)",
    "Royal Ludroth (Hyper)",
    "Agnaktor (Hyper)",
    "Duramboros (Hyper)",
    "Nibelsnarf (Hyper)",
    "Zinogre (Hyper)",
    "Brachydios (Hyper)",
    "Kecha Wacha (Hyper)",
    "Tetsucabra (Hyper)",
    "Zamtrios (Hyper)",
    "Najarala (Hyper)",
    "Seltas Queen (Hyper)",
    "Nerscylla (Hyper)",
    "Gore Magala (Hyper)",
    "Seregios (Hyper)",
    "Malfestio (Hyper)",
    "Glavenus (Hyper)",
    "Astalos (Hyper)",
    "Mizutsune (Hyper)",
    "Gammoth (Hyper)"
};
const char* Icon::GetStr(const size id) { return Icon::str.at(id); }



}
