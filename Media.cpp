#include "Media.hpp"

int Media::GetX() const
{
    return m_pos.x;
}

int Media::GetY() const
{
    return m_pos.y;
}

int Media::GetZ() const
{
    return m_zindex;
}

int Media::GetWidth() const
{
    return m_size.width;
}

int Media::GetHeight() const
{
    return m_size.height;
}

bool Media::IsVisible() const
{
    return m_visible;
}

void Media::init(Region* , const Point& pos, const Size& size, int zindex)
{
    m_zindex = zindex;
    m_pos = pos;
    m_size = size;
}

void Media::hide()
{
    m_visible = false;
}

void Media::show()
{
    m_visible = true;
}

