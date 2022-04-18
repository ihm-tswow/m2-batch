#pragma once

#include <fstream>
#include <filesystem>

#include "filepath.h"

namespace fs = std::filesystem;

template <typename T>
struct File
{
    std::ifstream stream;
    filepath path;
    T value;
    File() = default;
    File(fs::path file, fs::path filename)
        : stream(std::ifstream(file))
        , path(filename.string())
    {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    }
};