#pragma once

#include "control/media/Media.hpp"

#include <utility>

class GetMediaPosition
{
public:
    GetMediaPosition(int regionWidth, int regionHeight);

    int getMediaX(int width, MediaGeometry::Align align) const;
    int getMediaY(int height, MediaGeometry::Valign valign) const;

private:
    int regionWidth_;
    int regionHeight_;
};
