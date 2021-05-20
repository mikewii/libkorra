#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "MHXX/MHXX.hpp"
#include "Tools/Utils.hpp"

#include "Tools/Test.hpp"


int main()
{
    Utils::File::SetCWD();

    Pair out, pgmdout;
    std::vector<Pair> list;
    std::string fpath;
    std::string fname;


    fpath = "/run/media/mw/data2/test/q0000226.arc";
    //fpath = "/run/media/mw/data2/test/q0000101.arc";
    //fpath = "/run/media/mw/data2/test/quest.arc";
    //fpath = "/run/media/mw/data2/test/q0001053.arc"; // Beware the Comet of Disaster
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();
    a.PrintPairsInfo();

    // Run tests
    for ( auto& pair : list )
    {
        if ( pair.info.ResourceHash == MHXX::GMD::RESOURCE_HASH )
        {
            TEST::test<MHXX::GMD::cGMD>( pair );

//            MHXX::GMD::cGMD gmd(pair);
//            gmd.printAllItems();
        }

        if ( pair.info.ResourceHash == MHXX::QDP::RESOURCE_HASH )
            TEST::test<MHXX::QDP::cQuestPlus>( pair );

        if ( pair.info.ResourceHash == MHXX::SEM::RESOURCE_HASH )
        {
            TEST::test<MHXX::SEM::cSetEmMain>( pair );

            MHXX::SEM::cSetEmMain sem(pair);
            sem.print();
        }

        if ( pair.info.ResourceHash == MHXX::REM::RESOURCE_HASH )
        {
            TEST::test<MHXX::REM::cRewardEm>( pair );

            MHXX::REM::cRewardEm rem(pair);
            //rem.print();
        }

        if ( pair.info.ResourceHash == MHXX::SUP::RESOURCE_HASH )
        {
            TEST::test<MHXX::SUP::cSupply>( pair );

            MHXX::SUP::cSupply sup(pair);
            //sup.print(true);
        }

        if ( pair.info.ResourceHash == MHXX::QDL::RESOURCE_HASH )
        {
            TEST::test<MHXX::QDL::cQuestDataLink>( pair );

            MHXX::QDL::cQuestDataLink qdl(pair);
            //qdl.print();

        }

        if ( pair.info.ResourceHash == MHXX::ESL::RESOURCE_HASH )
        {
            MHXX::ESL::cEmSetList esl( pair );

            //esl.print();
        }
    }

    //Utils::File::PairVectorToFiles(list, fname, "/run/media/mw/data2/test/");

    return 0;
}
// 6770909000
