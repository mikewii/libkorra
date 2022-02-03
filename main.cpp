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
////#define AAA
//#ifdef AAA
//    #include <locale.h>
//    setlocale(LC_ALL, "rus");


//#endif
//    TEST::runTests();
//    return 0;


    Utils::File::SetCWD();

    Pair out;
    std::vector<Pair> list;
    std::string fpath;
    std::string fname;

    // const char* test_folder = "test";
    //const char* test_folder = "test/MHXX_CQs/from_MHXX_ENG";
    const char* test_folder = "test/MHXX_CQs/from_MHXX";
    //const char* test_folder = "test/MHXX_CQs/from_MHGU";
    const char* test_files[] =
    {
        "q0001053.arc", // Beware the Comet of Disaster
        "q0000420.arc", // Hunt-a-thon: Daimyo Hermitaur, Village **** 3/5 4
        "q0000419.arc", // Deviled Legs, Village **** 3/5 3
        "q0000417.arc", // Island Menace, Village **** 3/5 2
        "q0000409.arc", // Wrath of the Rathian, Village **** 2/5 8
    };
    auto selected = test_files[2];

    fpath = Utils::GetUserHome() + '/' + test_folder + '/' + selected;
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();
    //a.PrintPairsInfo();



    DIR *d = nullptr;
    struct dirent *dir;
    d = opendir((Utils::GetUserHome() + '/' + test_folder).c_str());
    if (d)
    {
        std::string single_file = "q0000817.arc";

        while ((dir = readdir(d)) != NULL)
        {
            if (!single_file.empty() && single_file.compare(dir->d_name) != 0) continue;

            if (strlen(dir->d_name) == 12) // filename 8 + dot 1 + extention 3
            {
                std::vector<Pair>   vector;
                std::string         fpath = Utils::GetUserHome() + '/' + test_folder + '/' + dir->d_name;


                CContainer arc(fpath);
                ARC(arc, &vector).ExtractAll();

                PrintDebug(vector, dir->d_name);
            }
        }
        closedir(d);

        PrintUnique();

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
    printf("\n%s\n", filename);
    for (auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        case MHXX::GMD::RESOURCE_HASH:{
            TEST::test<MHXX::GMD::cGMD>(pair);

            MHXX::GMD::cGMD gmd(pair);
            //gmd.printAllItems();
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
            //ext.print_AcEquipSetNo();
            AddUnique(ext.header0.AcEquipSetNo);


            //pair.cc.writeToFile((fpath + ".ext").c_str());
            break;
        }
        } // switch
    }
}
