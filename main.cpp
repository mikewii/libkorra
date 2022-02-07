#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "MHXX/MHXX.hpp"
#include "Tools/Utils.hpp"

#include "Tools/Test.hpp"

#include <dirent.h>
#include <string.h>
//#define USE_GUI

#ifdef USE_GUI
#include "GUI/mainwindow.h"
#include <QApplication>

inline static int GUI_RUN(int argc, char *argv[])
{
    QApplication    gui(argc, argv);
    MainWindow      w;

    w.show();

    return gui.exec();
}
#endif

struct debug {
    u32             QuestID;
    std::string     Name;
    u32             QuestLevel;

    u32             Value;
};

static std::vector<debug> unique;
static std::vector<Pair> out;
static bool isUnique = true;

void AddUnique(const debug& in)
{
    if (unique.empty()) unique.push_back(in);
    else
    {
        bool skip = false;

        for (auto& item : unique)
            if (item.Value == in.Value)
            {
                skip = true;
                break;
            }
        if (!skip) unique.push_back(in);
    }
}

void AddAllEqual(const u32 value, const debug& in) { if (in.Value == value) unique.push_back(in); }
void AddAllNonEqual(const u32 value, const debug& in) { if (in.Value != value) unique.push_back(in); }

#include <algorithm>
void PrintUnique(const bool sorted = true)
{
    if (sorted)
        std::sort(unique.begin(), unique.end(), [](const debug a, const debug b){ return a.QuestID < b.QuestID; });

    printf("\n");
    for (auto& item : unique)
        printf("q%07d %-30s lv:%-3d | %d\n",
               item.QuestID, item.Name.c_str(), item.QuestLevel, item.Value);
}
void PrintDebug(std::vector<Pair>& vector, const char* filename);


int main(int argc, char *argv[])
{
    Utils::File::SetCWD();

    const char*     test_folder = "test/MHXX_CQs/from_MHXX_ENG";
    DIR*            d = nullptr;
    struct dirent*  dir;

    d = opendir((Utils::GetUserHome() + '/' + test_folder).c_str());

    if (d)
    {
        std::vector<std::string> selected_files =
        {
//            "q0010405.arc",
//            "q0110101.arc",
//            "q0040505.arc",
//            "q0000633.arc",
//            "q0000712.arc",
//            "q0011424.arc",
//            "q0000914.arc",
//            "q0011213.arc",
//            "q0000234.arc",
//            "q0000223.arc",
//            "q0010661.arc",
        };

        while ((dir = readdir(d)) != NULL)
        {
            if (!selected_files.empty())
            {
                bool found = false;

                for (auto& str : selected_files)
                    if (str.compare(dir->d_name) == 0)
                    {
                        found = true;
                        break;
                    }

                if (!found) continue;
            }
            if (strlen(dir->d_name) == 12) // filename 8 + dot 1 + extention 3
            {
                std::vector<Pair>   vector;
                std::string         fpath = Utils::GetUserHome() + '/' + test_folder + '/' + dir->d_name;
                std::string         fname = Utils::File::extractName(fpath);


                CContainer arc(fpath);
                ARC(arc, &vector).ExtractAll();

                PrintDebug(vector, dir->d_name);
            }
        }
        closedir(d);

        if (isUnique) PrintUnique();

        if (!out.empty())
            for (auto& pair : out)
            {
                auto name = Utils::File::extractName(pair.info.Filename);
                pair.cc.writeToFile({Utils::GetUserHome() + "/test/" + name});
            }

        //Utils::File::PairVectorToFiles(list, fname, "/run/media/mw/data2/test/");
    }

#ifdef USE_GUI
    return GUI_RUN(argc, argv);
#else
    return 0;
#endif
}

void PrintDebug(std::vector<Pair>& vector, const char* filename)
{
    std::string name;
    //printf("\n%s ", filename);
    for (auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        case MHXX::GMD::RESOURCE_HASH:{
            TEST::test<MHXX::GMD::cGMD>(pair);

            MHXX::GMD::cGMD gmd(pair);
            name = gmd.get_ItemStr(0);
            //printf("%s \n", gmd.get_ItemStr(0).c_str());
            //gmd.print_AllItems();
            break;
        }
        case MHXX::QDP::RESOURCE_HASH:{
            TEST::test<MHXX::QDP::cQuestPlus>(pair);
            break;
        }
        case MHXX::SEM::RESOURCE_HASH:{
            TEST::test<MHXX::SEM::cSetEmMain>(pair);

            MHXX::SEM::cSetEmMain sem(pair);
            //sem.print();
            break;
        }
        case MHXX::REM::RESOURCE_HASH:{
            TEST::test<MHXX::REM::cRewardEm>(pair);

            MHXX::REM::cRewardEm rem(pair);
            //rem.print();
            break;
        }
        case MHXX::SUP::RESOURCE_HASH:{
            TEST::test<MHXX::SUP::cSupply>(pair);

            MHXX::SUP::cSupply sup(pair);
            //sup.print(true);
            break;
        }
        case MHXX::QDL::RESOURCE_HASH:{
            TEST::test<MHXX::QDL::cQuestDataLink>(pair);

            MHXX::QDL::cQuestDataLink qdl(pair);
            //qdl.print();
            break;
        }
        case MHXX::ESL::RESOURCE_HASH:{ // broken for MHGU
            //MHXX::ESL::cEmSetList esl(pair);
            //esl.print();
            break;
        }
        case MHXX::EXT::RESOURCE_HASH:{
            MHXX::EXT::cEXT ext(pair);

            if (isUnique)
            {
                AddAllEqual
                (
                    1,
                    {
                        ext.header0.QuestID,
                        name,
                        ext.header0.QuestLv,
                        ext.header0.RequestVillage
                    }
                );
                //out.push_back(pair);
            }
            else
            {
                ext.print_RequestVillage();

            }
            //pair.cc.writeToFile((fpath + ".ext").c_str());
            break;
        }
        } // switch
    }
}
