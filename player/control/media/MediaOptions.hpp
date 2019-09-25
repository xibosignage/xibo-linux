#pragma once

#include "common/uri/Uri.hpp"

#include "MediaGeometry.hpp"
#include "control/common/transitions/Transition.hpp"

#include <map>

using ExtraOptions = std::map<std::string, std::string>;

struct MediaOptions
{
    struct Type
    {
        std::string type;
        std::string render;
    } type;

    int id;
    Uri uri;
    int duration;
    MediaGeometry geometry;
};
