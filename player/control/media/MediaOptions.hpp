#pragma once

#include "common/types/Uri.hpp"
#include "control/media/MediaGeometry.hpp"

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
    bool statEnabled;
    MediaGeometry geometry;
};
