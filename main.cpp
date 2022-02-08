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


static std::vector<Pair> out;
static Utils::Collector col(2);

void Debug(std::vector<Pair>& vector, const char* filename);


int main(int argc, char *argv[])
{
    Utils::File::SetCWD();

    //const char*         test_folder = "test/MHXX_CQs/from_MHXX_ENG";
    const char*         test_folder = "test/MHXX_CQs/from_MHGU";
    DIR*                d = nullptr;
    struct dirent*      dir;

    col.Set_Path(Utils::GetUserHome() + "/test/");
    //col.Disable();
    d = opendir((Utils::GetUserHome() + '/' + test_folder).c_str());

    if (d)
    {
        std::vector<std::string> selected_files =
        {
            //"q0090100.arc",
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

                Debug(vector, dir->d_name);
            }
        }
        closedir(d);

        col.Show();

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

void Debug(std::vector<Pair>& vector, const char* filename)
{
    bool once = true;
    std::string quest_name;
    //printf("\n%s ", filename);
    for (auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        case MHXX::GMD::RESOURCE_HASH:{
            TEST::test<MHXX::GMD::cGMD>(pair);

            MHXX::GMD::cGMD gmd(pair);
            if (once)
            {
//                gmd.print_AllItems();
                quest_name = gmd.get_ItemStr(0);
                once = false;
            }

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

            auto header0 = ext.GetHeader0();
            auto header1 = ext.GetHeader1();

            //col.Disable();
            if (col.IsActive())
            {
                col.Set_Value(2);
                col.Set_Operator(Utils::Collector::Op::Unique);
                col.Add
                ({
                    header0.questID,
                    quest_name,
                    header0.questLv,

                    header0.entryType[0]
                });
                //out.push_back(pair);
            }
            else
            {
                ext.print();
            }
            //pair.cc.writeToFile((fpath + ".ext").c_str());
            break;
        }
        } // switch
    }
}
