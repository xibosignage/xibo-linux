#include "Media.hpp"

#include <spdlog/spdlog.h>
#include <iostream>

Media::Media(const Region& region, int id, int duration, Render render, const std::string& uri) :
    m_region(region), m_id(id), m_duration(duration), m_render(render), m_uri(uri)
{
}

void Media::stop()
{
    m_started = false;
}

void Media::start()
{
    m_started = true;
}

bool Media::is_running() const
{
    return m_started;
}

sigc::signal<void, Gtk::Widget&, int, int>& Media::handler_requested()
{
    return m_handler_requsted;
}

const Region& Media::region() const
{
    return m_region;
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
