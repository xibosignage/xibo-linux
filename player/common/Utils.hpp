#pragma once

#include <boost/property_tree/ptree.hpp>

using ptree_node = boost::property_tree::ptree;
class FilePath;

namespace Utils
{
std::string md5hash(std::string_view data);
std::string md5hashFromFile(const FilePath& path);
std::string toBase64(const std::string& text);
std::string fromBase64(const std::string& text);

template <typename T>
std::string toString(T val);

template <typename Container>
bool containersEqual(const Container& first, const Container& second)
{
    if (first.size() != second.size()) return false;

    for (size_t i = 0; i != first.size(); ++i)
    {
        if (first[i] != second[i]) return false;
    }

    return true;
}
}
