#pragma once

#include "control/media/MediaGeometry.hpp"
#include "control/media/MediaOptions.hpp"

struct ImageOptions : public MediaOptions
{
    MediaGeometry::ScaleType scaleType;
    MediaGeometry::Align align;
    MediaGeometry::Valign valign;
};
