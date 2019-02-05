#pragma once

#include "ImageProperties.hpp"
#include "IMedia.hpp"

#include <utility>

class GetMediaPosition
{
public:
    GetMediaPosition(int regionWidth, int regionHeight);

    std::pair<int, int> getMediaPos(IMedia* media) const;

private:
    int getMediaX(IVisible* media) const;
    int getMediaY(IVisible* media) const;

private:
    int m_regionWidth;
    int m_regionHeight;

};
