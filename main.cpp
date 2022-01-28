#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "MHXX/MHXX.hpp"
#include "Tools/Utils.hpp"

#include "Tools/Test.hpp"


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
    const char* test_folder = "test/MHXX_CQs/from_MHXX_ENG";
    const char* test_files[] =
    {
        "q0000226.arc",
        "q0000226_en.arc",
        "q0000101.arc",
        "quest.arc",
        "q0001053.arc", // Beware the Comet of Disaster
        "q0001053_en.arc", // Beware the Comet of Disaster
        "q0000420_en.arc", // Hunt-a-thon: Daimyo Hermitaur, Village **** 3/5 4
        "q0000419.arc", // Hunt-a-thon: Daimyo Hermitaur, Village **** 3/5 4
    };
    auto selected = test_files[7];

    fpath = Utils::GetUserHome() + '/' + test_folder + '/' + selected;
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();
    //a.PrintPairsInfo();

    // Run tests
    for (auto& pair : list)
    {
        switch(pair.info.ResourceHash){
        case MHXX::GMD::RESOURCE_HASH:{
            TEST::test<MHXX::GMD::cGMD>(pair);

            MHXX::GMD::cGMD gmd(pair);
            gmd.printAllItems();
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
        case MHXX::ESL::RESOURCE_HASH:{
            MHXX::ESL::cEmSetList esl(pair);
            //esl.print();
            break;
        }
        case MHXX::EXT::RESOURCE_HASH:{
            MHXX::EXT::cEXT ext(pair);
            ext.print();

            //pair.cc.writeToFile((fpath + ".ext").c_str());
            break;
        }
        } // switch
    }

    //Utils::File::PairVectorToFiles(list, fname, "/run/media/mw/data2/test/");

#ifdef USE_GUI
    return GUI_RUN(argc, argv);
#else
    return 0;
#endif
}
