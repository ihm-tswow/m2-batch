#include "m2.h"

M2Header M2Header::read(std::ifstream& stream)
{
    M2Header header;
    stream.read(reinterpret_cast<char*>(&header), sizeof(M2Header));
    return header;
}

std::string M2String::read(std::ifstream& reader)
{
    reader.seekg(ofs_chars, std::ios::beg);
    std::string str;
    std::copy_n(std::istreambuf_iterator<char>(reader.rdbuf()),
        char_count, std::back_inserter(str));
    return str;
}
