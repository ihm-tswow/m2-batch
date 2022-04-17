#pragma once

#include <fstream>
#include <filesystem>

template <typename T>
struct File
{
    std::ifstream stream;
    T value;
    File() = default;
    File(std::filesystem::path file)
        : stream(std::ifstream(file))
    {
        stream.read(reinterpret_cast<char*>(&value), sizeof(T));
    }
};