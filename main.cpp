#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "Tools/Utils.hpp"


int main()
{
    Utils::File::SetCWD();

    Pair out;
    std::vector<Pair> list;
    std::string fpath;
    std::string fname;


    //fpath = "/run/media/mw/data2/test/q0000226.arc";
    //fpath = "/run/media/mw/data2/test/q0000101.arc";
    fpath = "/run/media/mw/data2/test/quest.arc";
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();
    //a.PrintPairsInfo();

    std::string path = "/run/media/mw/data2/test/out.gmd";
    for ( auto& in : list)
    {
        if ( in.ResourceHash == MH4U::GMD::RESOURCE_HASH )
        {
            MH4U::GMD::sGMD gmd(in);

            gmd.save(out);

            u32 sum0, sum1;
            sum0 = Utils::CalculateChecksum(in.cc);
            sum1 = Utils::CalculateChecksum(out.cc);

            Utils::FindDiff(in.cc.data(), out.cc.data(), out.cc.size());

            if (in.cc.size() != out.cc.size()) {
                printf("");
            }

            if (sum0 == sum1)
            {
                printf("all fine\n");
                //out.cc.writeToFile(path.c_str());

                path.append("a");
            }

//            gmd.printHeader();
//            gmd.printFilename();

//            gmd.printAllItems();
            //gmd.printAllLabels();
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
