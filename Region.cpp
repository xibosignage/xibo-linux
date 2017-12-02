#include "Region.hpp"

Region::Region(int id, int width, int height, int top, int left, int zindex, bool loop, const Transition& transition) :
    m_id(id), m_width(width), m_height(height), m_top(top), m_left(left), m_zindex(zindex), m_loop(loop), m_transition(transition)
{

}

int Region::id() const
{
    return m_id;
}

int Region::width() const
{
    return m_width;
}

int Region::height() const
{
    return m_height;
}

int Region::top() const
{
    return m_top;
}

int Region::left() const
{
    return m_left;
}

int Region::zindex() const
{
    return m_zindex;
}

bool Region::loop() const
{
    return m_loop;
}

const Transition& Region::transition() const
{
    return m_transition;
}

void Region::add_media(const std::shared_ptr<Media>& media)
{
    m_medias.push_back(media);
}
