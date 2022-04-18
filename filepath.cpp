#include "filepath.h"

std::string filepath::convert(std::string search) const
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

std::string filepath::string() const
{
    return _filepath;
}

char const* filepath::c_str() const
{
    return _filepath.c_str();
}

bool filepath::ends_with(std::string const& ending) const
{
    return _filepath.ends_with(convert(ending));
}

bool filepath::starts_with(std::string const& start) const
{
    return _filepath.starts_with(convert(start));
}

bool filepath::contains(std::string const& match) const
{
    return _filepath.find(match) != std::string::npos;
}

bool filepath::match(std::regex const& regex) const
{
    return std::regex_match(_filepath, regex);
}

size_t filepath::size() const
{
    return _filepath.size();
}

std::ostream& operator<<(std::ostream& os, const filepath& dt)
{
    os << dt._filepath;
    return os;
}

