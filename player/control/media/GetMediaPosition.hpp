#pragma once

#include "MediaGeometry.hpp"

#include <utility>

class GetMediaPosition
{
public:
    GetMediaPosition(int regionWidth, int regionHeight);

    int getMediaLeft(int width, MediaGeometry::Align align) const;
    int getMediaTop(int height, MediaGeometry::Valign valign) const;

private:
    int m_regionWidth;
    int m_regionHeight;

};
