#include "GetMediaPosition.hpp"
#include "constants.hpp"

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
    m_mediaX = DEFAULT_X_POS;
    m_mediaY = DEFAULT_Y_POS;
}

void GetMediaPosition::visit(Video&)
{
    m_mediaX = DEFAULT_X_POS;
    m_mediaY = DEFAULT_Y_POS;
}

void GetMediaPosition::visit(WebView&)
{
    m_mediaX = DEFAULT_X_POS;
    m_mediaY = DEFAULT_Y_POS;
}

int GetMediaPosition::getImageX(Image::Align align, int imageWidth)
{
    switch(align)
    {
    case Image::Align::Center:
        return (m_containerWidth - imageWidth) / 2;
    case Image::Align::Left:
        return DEFAULT_X_POS;
    case Image::Align::Right:
        return m_containerWidth - imageWidth;
    default:
        break;
    }
    return INVALID_POS;
}

int GetMediaPosition::getImageY(Image::Valign valign, int imageHeight)
{
    switch(valign)
    {
    case Image::Valign::Middle:
        return (m_containerHeight - imageHeight) / 2;
    case Image::Valign::Top:
        return DEFAULT_Y_POS;
    case Image::Valign::Bottom:
        return m_containerHeight - imageHeight;
    default:
        break;
    }
    return INVALID_POS;
}
