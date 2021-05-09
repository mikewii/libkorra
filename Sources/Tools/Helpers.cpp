#include "Tools/Helpers.hpp"

#include "Tools/Blowfish.hpp"

#include <fstream>
#include <iterator>

void FileToVector(std::string filename, std::vector<u8>& vec)
{
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<u8>(file),
               std::istream_iterator<u8>());

}
