#pragma once

#include <string>
#include <regex>
#include <iostream>

class filepath
{
    std::string _filepath;
    std::string convert(std::string search) const;
public:
    filepath(std::string const& filename);
    std::string string() const;
    char const* c_str() const;
    bool ends_with(std::string const& ending) const;
    bool starts_with(std::string const& start) const;
    bool contains(std::string const& match) const;
    bool match(std::regex const& regex) const;
    size_t size() const;
    friend std::ostream& operator<<(std::ostream& os, const filepath& dt);
};