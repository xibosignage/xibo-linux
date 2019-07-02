#pragma once

#include "common/uri/Uri.hpp"
#include "MediaGeometry.hpp"

struct MediaOptions
{
    struct Type{
        std::string type;
        std::string render;
    } type;

    int id;
    Uri uri;
    int duration;
    MediaGeometry geometry;
};
