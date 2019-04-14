#pragma once

#include "common/uri/Uri.hpp"
#include "MediaGeometry.hpp"

struct MediaOptions
{
    int id;
    Uri uri;
    int duration;
    MediaGeometry geometry;
};
