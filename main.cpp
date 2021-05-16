#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "Tools/Utils.hpp"


int main()
{
    Utils::File::SetCWD();

    std::vector<Pair> list;
    std::string fpath;
    std::string fname;


    fpath = "/run/media/mw/data2/test/q0000226.arc";
    //fpath = "/run/media/mw/data2/test/quest.arc";
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();
    //a.PrintPairsInfo();

    for ( auto& pp : list)
    {
        if ( pp.ResourceHash == MH4U::GMD::RESOURCE_HASH )
        {
            MH4U::GMD::sGMD gmd(pp);

            gmd.printHeader();
            gmd.printFilename();

            gmd.printAllItems();
            gmd.printAllLabels();
        }
    }

//    MH4U::SEM::sSetEmMain sem(list.at(0));
//    MH4U::QDP::sQuestPlus qdp(list.at(8));
//    MH4U::GMD::sGMD gmd(list.at(10));

//    sem.print();
//    qdp.print();
//    gmd.printHeader();
//    gmd.printFilename();
//    gmd.printAllItems();

//    MH4U::SEM::Init(list.at(0));
//    MH4U::SEM::print();




    //Utils::File::PairsVectorToFiles(list, fname, "/run/media/mw/data2/test/");

    return 0;
}
// 6770909000
