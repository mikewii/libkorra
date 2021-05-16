#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"
#include "Tools/Utils.hpp"
#include "MH4U/Extentions/sem.hpp"


int main()
{
    using namespace MH4U;

    Utils::File::SetCWD();

    std::vector<Pair> list;
    std::string fpath;
    std::string fname;


    fpath = "/run/media/mw/data2/test/q0000101.arc";
    fname = Utils::File::extractName(fpath);


    CContainer arc(fpath);

    ARC a(arc, &list);
    a.ExtractAll();
    //a.PrintPairsInfo();

    MH4U::SEM::Init(list.at(0));
    MH4U::SEM::print();




    //Utils::File::PairsVectorToFiles(list, fname, "/run/media/mw/data2/test/");



    return 0;
}
// 6770909000
