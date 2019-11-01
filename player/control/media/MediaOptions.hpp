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

    enum class StatPolicy
    {
        Disable,
        Enable,
        Inherit
    };

    int id;
    Uri uri;
    int duration;
    StatPolicy statPolicy;
    MediaGeometry geometry;
};
