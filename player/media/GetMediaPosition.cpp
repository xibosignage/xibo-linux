#include "GetMediaPosition.hpp"

#include "constants.hpp"
#include "Image.hpp"

GetMediaPosition::GetMediaPosition(int containerWidth, int containerHeight) :
    m_containerWidth(containerWidth), m_containerHeight(containerHeight)
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
        return (m_containerWidth - imageWidth) / 2;
    case ImageProperties::Align::Left:
        return DEFAULT_XPOS;
    case ImageProperties::Align::Right:
        return m_containerWidth - imageWidth;
    }
    return INVALID_POS;
}

int GetMediaPosition::getImageY(ImageProperties::Valign valign, int imageHeight)
{
    switch(valign)
    {
    case ImageProperties::Valign::Middle:
        return (m_containerHeight - imageHeight) / 2;
    case ImageProperties::Valign::Top:
        return DEFAULT_YPOS;
    case ImageProperties::Valign::Bottom:
        return m_containerHeight - imageHeight;
    }
    return INVALID_POS;
}
