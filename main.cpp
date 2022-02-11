#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"

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
    MHXX::TEST::run_tests();

#ifdef USE_GUI
    return GUI_RUN(argc, argv);
#else
    return 0;
#endif
}


