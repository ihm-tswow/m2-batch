#include "skin.h"

M2SkinHeader M2SkinHeader::read(std::ifstream& stream)
{
    M2SkinHeader header;
    stream.read(reinterpret_cast<char*>(&header), sizeof(M2SkinHeader));
    return header;
}