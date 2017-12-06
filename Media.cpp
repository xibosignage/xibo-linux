#include "Media.hpp"

Media::Media(int id, int duration, Render render, const std::string& uri) :
    m_id(id), m_duration(duration), m_render(render), m_uri(uri)
{

}

bool Media::is_visible() const
{
    return m_visible;
}

void Media::hide()
{
    m_visible = false;
}

void Media::show()
{
    m_visible = true;
}

int Media::id() const
{
    return m_id;
}

int Media::duration() const
{
    return m_duration;
}

Media::Render Media::render() const
{
    return m_render;
}

const std::string& Media::uri() const
{
    return m_uri;
}

