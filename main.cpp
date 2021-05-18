#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "Tools/Utils.hpp"


int main()
{
    Utils::File::SetCWD();

    Pair out, pgmdout;
    std::vector<Pair> list;
    std::string fpath;
    std::string fname;

    MH4U::GMD::sGMD gmdout;

    gmdout.appendItemStr("yeah!");
    gmdout.appendItemStr("delete this!");
    gmdout.appendItemStr("second string");
    gmdout.setFilenameStr("TestGMD");

    gmdout.removeItemStr(1);

    gmdout.make(pgmdout);

    pgmdout.cc.writeToFile("/run/media/mw/data2/test/test.gmd");


    fpath = "/run/media/mw/data2/test/q0000226.arc";
    //fpath = "/run/media/mw/data2/test/q0000101.arc";
    //fpath = "/run/media/mw/data2/test/quest.arc";
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();

    for ( auto& in : list )
    {
        if ( in.ResourceHash == MH4U::GMD::RESOURCE_HASH )
        {
            MH4U::GMD::sGMD gmd(in);

            gmd.save(out);

            u32 sum0, sum1;
            sum0 = Utils::CalculateChecksum(in.cc);
            sum1 = Utils::CalculateChecksum(out.cc);

            Utils::FindDiff(in.cc.data(), out.cc.data(), out.cc.size());

            if (sum0 == sum1) printf("all fine\n");
        }
    }

    //Utils::File::PairsVectorToFiles(list, fname, "/run/media/mw/data2/test/");

    return 0;
}
// 6770909000
