#include <iostream>
#include "Tools/Helpers.hpp"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    std::vector<u8> data = FileToVector("/run/media/mw/data2/test/user3");
    std::vector<u8> result, result2, result3;

    result = MH4U::Decode(data);

    result2 = MH4U::Encode(result);

    result3 = MH4U::Decode(result2);



    return 0;
}
