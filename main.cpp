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

static std::vector<u32> unique;
static std::vector<Pair> out;

void AddUnique(u32 value)
{
    if (unique.empty()) unique.push_back(value);
    else
    {
        if (unique.back() != value) unique.push_back(value);
    }
}

void PrintUnique(void)
{
    for (auto& value : unique)
        printf("%d\n", value);
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
            "q0000101.arc", // J. Frontier (D)  01
            "q0000107.arc", // J. Frontier (N)  65
            "q0000203.arc", // V. Hills (D)     02
            "q0000316.arc", // V. Hills (N)     66
            "q0000321.arc", // A. Ridge (D)     03
            "q0000322.arc", // A. Ridge (N)     67
            "q0000416.arc", // M. Peaks (D)     04
            "q0000415.arc", // M. Peaks (N)     68
            "q0000408.arc", // Dunes            05
            "q0000610.arc", // D. Island        06
            "q0000611.arc", // Marshlands       07
            "q0001038.arc", // Volcano          08
            "q0000636.arc", // Arena            09
            "q0020005.arc", // V. Slayground    0A
            "q0000924.arc", // A. Steppe        0B
            "q0000903.arc", // V. Hollow        0C
            "q0000628.arc", // Primal Forest    0D
            "q0000820.arc", // F. Seaway        0E
            "q0020006.arc", // F. Slayground    0F
            "q0000632.arc", // Sanctuary        10
            "q0000621.arc", // Forlorn Arena    11
            "q0001019.arc", // S. Pinnacle      12
            "q0001039.arc", // Ingle Isle       13
            "q0001018.arc", // Polar Field      14
            "q0001026.arc", // Wyvern's End     15
            "q0000914.arc", // Desert           16
            "q0000904.arc", // Jungle           17
            "q0000917.arc", // Ruined Pinnacle  18
            "q0011454.arc", // Castle Schrade   19
            "q0011319.arc", // Fortress         1A
            "q0011432.arc", // Forlorn Citadel  1B
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

        //PrintUnique();

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
    printf("\n%s ", filename);
    for (auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        case MHXX::GMD::RESOURCE_HASH:{
            TEST::test<MHXX::GMD::cGMD>(pair);

            MHXX::GMD::cGMD gmd(pair);
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
            ext.print_Map();

//            AddUnique(ext.header0.AcEquipSetNo);
//            out.push_back(pair);

            //pair.cc.writeToFile((fpath + ".ext").c_str());
            break;
        }
        } // switch
    }
}
