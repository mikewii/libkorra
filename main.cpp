#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "Tools/Utils.hpp"


int main()
{
    std::string fname = "/run/media/mw/data2/test/d0001.arc";
    std::string fname2 = "/run/media/mw/data2/test/out.arc";

    CContainer cc(fname.c_str());
    CContainer cc2(fname2.c_str());

    CContainer arcout;

    std::vector<ARC::Pairs> list, list2;


    ARC a(cc, &list), a2(cc2, &list2);
    a.ExtractAll();
    a2.ExtractAll();

    for ( u32 i = 0; i < list.size(); i++ )
    {
        CContainer& data0 = list.at(i).cc;
        CContainer& data1 = list2.at(i).cc;

        u32 sum0 = 0, sum1 = 0;

        sum0 = Utils::CalculateChecksum(data0);
        sum1 = Utils::CalculateChecksum(data1);

        if ( sum0 != sum1 )
        {
            printf("error ffs!\n");
        }
    }
    //a.MakeARC(arcout, &list);

    //arcout.writeToFile("/run/media/mw/data2/test/out.arc");

    return 0;
}
