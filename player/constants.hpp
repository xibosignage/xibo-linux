#pragma once

#include <string>
#include <glib.h>
#include <boost/property_tree/ptree.hpp>
#include "constants.hpp"

using Params = boost::property_tree::ptree;
using xlf_node = boost::property_tree::ptree;

const std::string LOGGER = "logger";
const int DEFAULT_X_POS = 0;
const int DEFAULT_Y_POS = 0;
const uint MSECS = 1000;

enum class MediaType
{
    Image,
    Video,
    WebView,
    Audio
};

inline std::ostream& operator<<(std::ostream& os, MediaType type)
{
    os << static_cast<int>(type);
    return os;
}

inline std::istream& operator>>(std::istream& is, MediaType& type)
{
    int temp;
    is >> temp;
    type = static_cast<MediaType>(temp);
    return is;
}

struct Point
{
    int left;
    int top;
};

struct Size
{
    int width;
    int height;
};
