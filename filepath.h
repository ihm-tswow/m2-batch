#pragma once

#include <string>
#include <regex>
#include <iostream>

class filepath
{
    std::string _filepath;
    std::string convert(std::string search);
public:
    filepath(std::string const& filename);
    std::string string();
    char const* c_str();
    bool ends_with(std::string const& ending);
    bool starts_with(std::string const& start);
    bool contains(std::string const& match);
    bool match(std::regex const& regex);
    size_t size();
    friend std::ostream& operator<<(std::ostream& os, const filepath& dt);
};