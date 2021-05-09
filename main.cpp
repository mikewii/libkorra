#include "Tools/CContainer.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"


int main()
{
    std::string fname = "/run/media/mw/data2/test/q0000101.arc";
    std::string fname2 = "/run/media/mw/data2/test/user2";
    CContainer cc(fname.c_str());
    CContainer save_enc(fname2.c_str());
    CContainer save_dec, enc2, dec3;
    ARC::Pairs comp, uncomp;

    std::vector<ARC::Pairs> list;



    ARC a(cc, &list);
    a.SetFilename("q0000101.arc");
    a.ExtractAll();

    a.Compress(list.at(0), comp);
    a.Decompress(comp, uncomp);

    MH4U b;
    b.Decode(save_enc, save_dec);

    b.Encode(save_dec, enc2);

    b.Decode(enc2, dec3);


    return 0;
}
