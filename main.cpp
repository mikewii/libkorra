#include <iostream>
#include "Tools/Helpers.hpp"
#include "Tools/Container.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"

using namespace std;

int main()
{
    std::string fname = "/run/media/mw/data2/test/shader.arc";
    std::string fname2 = "/run/media/mw/data2/test/user2";
    CContainer cc(fname.c_str());
    CContainer save_enc(fname2.c_str());
    CContainer save_dec, enc2, dec3;

    ARC a(cc);
    a.SetFilename("shader.arc");
    a.Extract(0, true);

    MH4U b;
    b.Decode(save_enc, save_dec);

    b.Encode(save_dec, enc2);

    b.Decode(enc2, dec3);


    return 0;
}
