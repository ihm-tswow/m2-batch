#pragma once

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

template <typename T>
struct File
{
    std::ifstream stream;
    std::string filename;
    T value;
    File() = default;
    File(fs::path file, fs::path filename)
        : stream(std::ifstream(file))
        , filename(filename.string())
    {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    }
};