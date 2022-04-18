#include "filepath.h"

std::string filepath::convert(std::string search)
{
    std::replace(search.begin(), search.end(), '/', '\\');
    std::transform(search.begin(), search.end(), search.begin(),
        [](unsigned char c) { 
        return std::tolower(c); 
    });
    return search;
}

filepath::filepath(std::string const& filepath)
    : _filepath(convert(filepath))
{}

std::string filepath::string()
{
    return _filepath;
}

char const* filepath::c_str()
{
    return _filepath.c_str();
}

bool filepath::ends_with(std::string const& ending)
{
    return _filepath.ends_with(convert(ending));
}

bool filepath::starts_with(std::string const& start)
{
    return _filepath.starts_with(convert(start));
}

bool filepath::contains(std::string const& match)
{
    return _filepath.find(match) != std::string::npos;
}

bool filepath::match(std::regex const& regex)
{
    return std::regex_match(_filepath, regex);
}

uint32_t filepath::size()
{
    return _filepath.size();
}

std::ostream& operator<<(std::ostream& os, const filepath& dt)
{
    os << dt._filepath;
    return os;
}

