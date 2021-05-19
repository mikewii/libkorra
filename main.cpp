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
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();

    // Run tests
    for ( auto& in : list )
    {
        if ( in.ResourceHash == MHXX::GMD::RESOURCE_HASH )
            TEST::test<MHXX::GMD::sGMD>( in );

        if ( in.ResourceHash == MHXX::QDP::RESOURCE_HASH )
            TEST::test<MHXX::QDP::sQuestPlus>( in );

        if ( in.ResourceHash == MHXX::SEM::RESOURCE_HASH )
            TEST::test<MHXX::SEM::sSetEmMain>( in );
    }

    //Utils::File::PairsVectorToFiles(list, fname, "/run/media/mw/data2/test/");

    return 0;
}
// 6770909000
