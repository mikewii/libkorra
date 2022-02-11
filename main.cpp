#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"

#include "Tools/Utils.hpp"
#include "Tools/File.hpp"

#include "Tools/Test.hpp"

#include <dirent.h>
#include <string.h>
#include "MHXX/Quest/Common.hpp"

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


static std::vector<Pair> out;
static Utils::Collector col(2);

void Debug(std::vector<Pair>& vector, const char* filename);


int main(int argc UNUSED, char *argv[] UNUSED)
{
    File::SetCWD();

    //const char*         test_folder = "test/MHXX_CQs/from_MHXX_ENG";
    //const char*         test_folder = "test/MHXX_CQs/from_MHGU";
    const char*         test_folder = "test/MH4U";
    DIR*                d = nullptr;
    struct dirent*      dir;

    col.Set_Path(Utils::GetUserHome() + "/test/");
    //col.Disable();
    d = opendir((Utils::GetUserHome() + '/' + test_folder).c_str());

    if (d)
    {
        std::vector<std::string> selected_files =
        {
            //"q0000105.arc",
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
            //if (strlen(dir->d_name) == 12) // filename 8 + dot 1 + extention 3
            {
                std::vector<Pair>   vector;
                std::string         fpath = Utils::GetUserHome() + '/' + test_folder + '/' + dir->d_name;
                std::string         fname = File::extractName(fpath);


//                CContainer arc(fpath);
//                ARC(arc, &vector).ExtractAll();

                //MHXX::TEST::Extentions(vector, dir->d_name, col);

                CContainer data(fpath);
                MH4U::cLMD lmd(data);

                lmd.print_Data0();
                lmd.print_Data1();
                lmd.print_U16string_info();
                lmd.print_Strings();
                lmd.print_Filename();
                lmd.print_counts();

                CContainer ass;

                lmd.write(ass);

                //ass.writeToFile(fpath + "ass");
            }
        }
        closedir(d);

        col.Show();

        if (!out.empty())
            for (auto& pair : out)
            {
                auto name = File::extractName(pair.info.Filename);
                pair.cc.writeToFile({Utils::GetUserHome() + "/test/" + name});
            }

        //File::PairVectorToFiles(list, fname, "/run/media/mw/data2/test/");
    }

#ifdef USE_GUI
    return GUI_RUN(argc, argv);
#else
    return 0;
#endif
}


