#include "GetMediaPosition.hpp"

#include "constants.hpp"

GetMediaPosition::GetMediaPosition(int regionWidth, int regionHeight) :
    m_regionWidth(regionWidth), m_regionHeight(regionHeight)
{
}

int GetMediaPosition::getMediaX(int width, MediaGeometry::Align align) const
{
    switch(align)
    {
    case MediaGeometry::Align::Center:
        return (m_regionWidth - width) / 2;
    case MediaGeometry::Align::Left:
        return DEFAULT_XPOS;
    case MediaGeometry::Align::Right:
        return m_regionWidth - width;
    }
    return INVALID_POS;
}

int GetMediaPosition::getMediaY(int height, MediaGeometry::Valign valign) const
{
    switch(valign)
    {
    case MediaGeometry::Valign::Middle:
        return (m_regionHeight - height) / 2;
    case MediaGeometry::Valign::Top:
        return DEFAULT_YPOS;
    case MediaGeometry::Valign::Bottom:
        return m_regionHeight - height;
    }
    return INVALID_POS;
}
