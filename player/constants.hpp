#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <boost/property_tree/ptree_fwd.hpp>

using xlf_node = boost::property_tree::ptree;
using FilePath = std::filesystem::path;

const std::string LOGGER = "logger";

const unsigned int MSECS = 1000;
const int MAX_VOLUME = 100;

const int MIN_DISPLAY_WIDTH = 160;
const int MAX_DISPLAY_WIDTH = 9999;
const int MIN_DISPLAY_HEIGHT = 120;
const int MAX_DISPLAY_HEIGHT = 9999;

const int MIN_X_POS = 0;
const int MIN_Y_POS = 0;
const int DEFAULT_XPOS = 0;
const int DEFAULT_YPOS = 0;
const int INVALID_POS = -1;

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
