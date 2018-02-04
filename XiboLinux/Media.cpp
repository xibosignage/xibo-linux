#include "Media.hpp"
#include <glibmm.h>

const uint MSECS = 1000;

#include <spdlog/spdlog.h>
#include <iostream>

Media::Media(uint id, uint duration, bool use_duration, Render render, const std::string& uri) :
    m_id(id), m_duration(duration), m_use_duration(use_duration), m_render(render), m_uri(uri)
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
    if(m_use_duration)
    {
        Glib::signal_timeout().connect_once([=](){
            m_media_timeout.emit();
        }, m_duration * MSECS);
    }

    m_visible = true;
}

uint Media::id() const
{
    return m_id;
}

uint Media::duration() const
{
    return m_duration;
}

bool Media::use_duration() const
{
    return m_use_duration;
}

Media::Render Media::render() const
{
    return m_render;
}

const std::string& Media::uri() const
{
    return m_uri;
}

sigc::signal<void>& Media::media_timeout()
{
    return m_media_timeout;
}

