#include "GetMediaPosition.hpp"

#include "constants.hpp"

GetMediaPosition::GetMediaPosition(int regionWidth, int regionHeight) :
    m_regionWidth(regionWidth), m_regionHeight(regionHeight)
{
}

std::pair<int, int> GetMediaPosition::getMediaPos(IMedia* media) const
{
    if(auto visibleMedia = dynamic_cast<IVisible*>(media))
    {
        return std::pair{getMediaX(visibleMedia), getMediaY(visibleMedia)};
    }
    return std::pair{DEFAULT_XPOS, DEFAULT_YPOS};
}

int GetMediaPosition::getMediaX(IVisible* media) const
{
    switch(media->align())
    {
    case MediaGeometry::Align::Center:
        return (m_regionWidth - media->width()) / 2;
    case MediaGeometry::Align::Left:
        return DEFAULT_XPOS;
    case MediaGeometry::Align::Right:
        return m_regionWidth - media->width();
    }
    return INVALID_POS;
}

int GetMediaPosition::getMediaY(IVisible* media) const
{
    switch(media->valign())
    {
    case MediaGeometry::Valign::Middle:
        return (m_regionHeight - media->height()) / 2;
    case MediaGeometry::Valign::Top:
        return DEFAULT_YPOS;
    case MediaGeometry::Valign::Bottom:
        return m_regionHeight - media->height();
    }
    return INVALID_POS;
}
