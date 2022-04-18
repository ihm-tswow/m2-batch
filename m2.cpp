#include "m2.h"

M2Header M2Header::read(std::ifstream& stream)
{
    M2Header header;
    stream.read(reinterpret_cast<char*>(&header), sizeof(M2Header));
    return header;
}
