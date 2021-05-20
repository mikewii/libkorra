#pragma once
#include "types.h"
#include "MHXX/Enemy/Enemy.hpp"

namespace MHXX {

struct Em {
    const char* Name;
    const EmID  ID;
};

static const u8 EnemyStrSize = 139;
static const Em EnemymStr[] =
{
    {"(None)",                  {0,  NORMAL}},
    {"Rathian",                 {1,  NORMAL}},
    {"Gold Rathian",            {1,  RARE}},
    {"Dreadqueen",              {1,  DEVIANT}},
    {"Rathalos",                {2,  NORMAL}},
    {"Silver Rathalos",         {2,  RARE}},
    {"Dreadking",               {2,  DEVIANT}},
    {"Khezu",                   {3,  NORMAL}},
    {"Basarios",                {4,  NORMAL}},
    {"Gravios",                 {5,  NORMAL}},
    // 6
    {"Diablos",                 {7,  NORMAL}},
    {"Bloodbath",               {7,  DEVIANT}},
    {"Yian Kut-Ku",             {8,  NORMAL}},
    {"Gypceros",                {9,  NORMAL}},
    {"Plesioth",                {10, NORMAL}},
    {"Kirin",                   {11, NORMAL}},
    {"Lao-Shan Lung",           {12, NORMAL}},
    {"Fatalis",                 {13, NORMAL}},
    {"Crimson Fatalis",         {13, SUB}},
    {"White Fatalis",           {13, RARE}},
    {"Velocidrome",             {14, NORMAL}},
    {"Gendrome",                {15, NORMAL}},
    {"Iodrome",                 {16, NORMAL}},
    {"Cephadrome",              {17, NORMAL}},
    {"Yian Garuga",             {18, NORMAL}},
    {"Deadeye",                 {18, DEVIANT}},
    {"Daimyo Hermitaur",        {19, NORMAL}},
    {"Stonefist",               {19, DEVIANT}},
    {"Shogun Ceanataur",        {20, NORMAL}},
    {"Rustrazor",               {20, DEVIANT}},
    {"Congalala",               {21, NORMAL}},
    {"Blangonga",               {22, NORMAL}},
    {"Rajang",                  {23, NORMAL}},
    {"Furious Rajang",          {23, VARIANT}},
    {"Kushala Daora",           {24, NORMAL}},
    {"Chameleos",               {25, NORMAL}},
    // 26
    {"Teostra",                 {27, NORMAL}},
    // 28
    // 29
    {"Bulldrome",               {30, NORMAL}},
    // 31
    {"Tigrex",                  {32, NORMAL}},
    {"Grimclaw",                {32, DEVIANT}},
    {"Akantor",                 {33, NORMAL}},
    {"Giadrome",                {34, NORMAL}},
    // 35
    {"Lavasioth",               {36, NORMAL}},
    {"Nargacuga",               {37, NORMAL}},
    {"Silverwind",              {37, DEVIANT}},
    {"Ukanlos",                 {38, NORMAL}},
    // 39
    // 40
    // 41
    {"Barioth",                 {42, NORMAL}},
    {"Deviljho",                {43, NORMAL}},
    {"Savage Deviljho",         {43, VARIANT}},
    {"Barroth",                 {44, NORMAL}},
    {"Uragaan",                 {45, NORMAL}},
    {"Crystalbeard",            {45, DEVIANT}},
    {"Lagiacrus",               {46, NORMAL}},
    {"Royal Ludroth",           {47, NORMAL}},
    // 48
    {"Agnaktor",                {49, NORMAL}},
    {"Alatreon",                {50, NORMAL}},
    // 51
    // 52
    // 53
    // 54
    {"Duramboros",              {55, NORMAL}},
    {"Nibelsnarf",              {56, NORMAL}},
    {"Zinogre",                 {57, NORMAL}},
    {"Thunderlord",             {57, DEVIANT}},
    {"Amatsu",                  {58, NORMAL}},
    // 59
    {"Arzuros",                 {60, NORMAL}},
    {"Redhelm",                 {60, DEVIANT}},
    {"Lagombi",                 {61, NORMAL}},
    {"Snowbaron",               {61, DEVIANT}},
    {"Volvidon",                {62, NORMAL}},
    {"Brachydios",              {63, NORMAL}},
    {"R. Brachydios",           {63, VARIANT}},
    // 64
    {"Kecha Wacha",             {65, NORMAL}},
    {"Tetsucabra",              {66, NORMAL}},
    {"Drilltusk",               {66, DEVIANT}},
    {"Zamtrios",                {67, NORMAL}},
    {"Najarala",                {68, NORMAL}},
    {"Seltas Queen",            {69, NORMAL}},
    {"Nerscylla",               {70, NORMAL}},
    {"Gore Magala",             {71, NORMAL}},
    {"Chaotic G. Magala",       {71, VARIANT}},
    {"Shagaru Magala",          {72, NORMAL}},
    // 73
    // 74
    // 75
    {"Seltas",                  {76, NORMAL}},
    {"Seregios",                {77, NORMAL}},
    // 78
    {"Malfestio",               {79, NORMAL}},
    {"Hidden Haze",             {79, DEVIANT}},
    {"Glavenus",                {80, NORMAL}},
    {"Hellblade",               {80, DEVIANT}},
    {"Astalos",                 {81, NORMAL}},
    {"Boltreaver",              {81, DEVIANT}},
    {"Mizutsune",               {82, NORMAL}},
    {"Divinesight",             {82, DEVIANT}},
    {"Gammoth",                 {83, NORMAL}},
    {"Frostpeak",               {83, DEVIANT}},
    {"Nakarkos",                {84, NORMAL}},
    {"Great Maccao",            {85, NORMAL}},
    {"Valstrax",                {86, NORMAL}},
    // 87
    {"Mecha",                   {87, NORMAL}},
    {"Ahtal-Ka",                {88, NORMAL}},
    /////
    {"Aptonoth",                {1,  SMALL}},
    {"Apceros",                 {2,  SMALL}},
    {"Kelbi",                   {3,  SMALL}},
    {"Mosswine",                {4,  SMALL}},
    {"Hornetaur",               {5,  SMALL}},
    {"Vespoid",                 {6,  SMALL}},
    {"Felyne",                  {7,  SMALL}},
    {"Melynx",                  {8,  SMALL}},
    {"Velociprey",              {9,  SMALL}},
    {"Genprey",                 {10, SMALL}},
    {"Ioprey",                  {11, SMALL}},
    {"Cephalos",                {12, SMALL}},
    {"Bullfango",               {13, SMALL}},
    {"Popo",                    {14, SMALL}},
    {"Giaprey",                 {15, SMALL}},
    {"Anteka",                  {16, SMALL}},
    {"Blango",                  {17, SUB}}, // Fix
    {"Thunderbug",              {17, SMALL}}, // Fix
    // 18
    {"Remobra",                 {19, SMALL}},
    {"Hermitaur",               {20, SMALL}},
    {"Ceanataur",               {21, SMALL}},
    {"Conga",                   {22, SMALL}},
    // 23
    // 24
    {"Rhenoplos",               {25, SMALL}},
    {"Bnahabra",                {26, SMALL}},
    {"Altaroth",                {27, SMALL}},
    // 28
    // 29
    // 30
    // 31
    // 32
    // 33
    {"Jaggi",                   {34, SMALL}},
    {"Jaggia",                  {35, SMALL}},
    // 36
    // 37
    // 38
    {"Ludroth",                 {39, SMALL}},
    {"Uroktor",                 {40, SMALL}},
    {"Slagtoth",                {41, SMALL}},
    {"Gargwa",                  {42, SMALL}},
    // 43
    {"Zamite",                  {44, SMALL}},
    {"Konchu",                  {45, SMALL}},
    {"Maccao",                  {46, SMALL}},
    {"Larinoth",                {47, SMALL}},
    {"Moofah",                  {48, SMALL}},
    /////
    //
    /////
    {"Tentacle",                {99, SMALL}},
    {"Unknown",                 {100,SMALL}},
    {"Unknown",                 {101,SMALL}},
    {"Unknown",                 {102,SMALL}},
    {"Unknown",                 {103,SMALL}},
    {"Unknown",                 {104,SMALL}},
    {"Unknown",                 {105,SMALL}},
    {"Unknown",                 {106,SMALL}}
};
} // MHXX_Struct
