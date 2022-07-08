#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "MH4U/Quest.hpp"

#include "Tools/Utils.hpp"
#include "Tools/File.hpp"

#include "Tools/Test.hpp"

#include "MH4U/MH4U_Test.hpp"
#include "MHXX/MHXX_Test.hpp"
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


int main(int argc UNUSED, char *argv[] UNUSED)
{
    //MHXX::TEST::savefile();
    //MHXX::TEST::run_tests();
    //MH4U::TEST::runTests();
    //MH4U::TEST::ExtractSave(Utils::Get_User_Home().append("test/MH4U/save/2021-06-22_18-37-56/user2"));
//    MH4U::TEST::mca(Utils::Get_User_Home().append("test/MH4U/mca/bgm_em011.mca"));
//    MH4U::TEST::mca(Utils::Get_User_Home().append("test/MH4U/mca/test1.mca"));
//    MH4U::TEST::mca(Utils::Get_User_Home().append("test/MH4U/mca/test2unpatched.mca"));
    //MH4U::TEST::mca(Utils::Get_User_Home().append("test/MH4U/mca/bgm_em_Named.mca"));
    //MH4U::Quest(Utils::Get_User_Home().append("test/MH4U/quest/MH4U EUR DLC/")).decrypt_all();
    //MH4U::TEST::quest_prints(Utils::Get_User_Home().append("test/MH4U/quest/MH4G DLC/quest/q60001.mib"));

    MH4U::TEST::quest_ext_create(Utils::Get_User_Home().append("test/MH4U/quest/Quests_MIB/mikewii/"));
    //MH4U::TEST::ExtractQuests(Utils::Get_User_Home().append("test/MH4U/quest/Quests_MIB/jdlc/m60005.mib"));
    //MH4U::Quest(Utils::Get_User_Home().append("test/MH4U/quest/test/")).decrypt_all();

#ifdef USE_GUI
    return GUI_RUN(argc, argv);
#else
    return 0;
#endif
}


