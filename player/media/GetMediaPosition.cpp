#include "GetMediaPosition.hpp"

#include "constants.hpp"
#include "Image.hpp"

GetMediaPosition::GetMediaPosition(int regionWidth, int regionHeight) :
    m_regionWidth(regionWidth), m_regionHeight(regionHeight)
{
}

int GetMediaPosition::getMediaX() const
{
    return m_mediaX;
}

int GetMediaPosition::getMediaY() const
{
    return m_mediaY;
}

void GetMediaPosition::visit(Image& image)
{
    m_mediaX = getImageX(image.align(), image.width());
    m_mediaY = getImageY(image.valign(), image.height());
}

void GetMediaPosition::visit(Audio&)
{
    m_mediaX = DEFAULT_XPOS;
    m_mediaY = DEFAULT_YPOS;
}

void GetMediaPosition::visit(Video&)
{
    m_mediaX = DEFAULT_XPOS;
    m_mediaY = DEFAULT_YPOS;
}

void GetMediaPosition::visit(WebView&)
{
    m_mediaX = DEFAULT_XPOS;
    m_mediaY = DEFAULT_YPOS;
}

int GetMediaPosition::getImageX(ImageProperties::Align align, int imageWidth)
{
    switch(align)
    {
    case ImageProperties::Align::Center:
        return (m_regionWidth - imageWidth) / 2;
    case ImageProperties::Align::Left:
        return DEFAULT_XPOS;
    case ImageProperties::Align::Right:
        return m_regionWidth - imageWidth;
    }
    return INVALID_POS;
}

int GetMediaPosition::getImageY(ImageProperties::Valign valign, int imageHeight)
{
    switch(valign)
    {
    case ImageProperties::Valign::Middle:
        return (m_regionHeight - imageHeight) / 2;
    case ImageProperties::Valign::Top:
        return DEFAULT_YPOS;
    case ImageProperties::Valign::Bottom:
        return m_regionHeight - imageHeight;
    }
    return INVALID_POS;
}
