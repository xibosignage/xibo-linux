#pragma once

#include "utils/uri/Uri.hpp"
#include "MediaGeometry.hpp"

struct MediaOptions
{
    int id;
    Uri uri;
    int duration;
    MediaGeometry geometry;
};
