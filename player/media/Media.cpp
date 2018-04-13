#include "Media.hpp"
#include "Audio.hpp"

#include <spdlog/spdlog.h>
#include <glibmm/main.h>
#include <iostream>

Media::Media(const Region& region, int id, int duration, Render render, const std::string& uri) :
    m_region(region), m_id(id), m_duration(duration), m_render(render), m_uri(uri)
{
}

void Media::stop()
{
    m_started = false;
    if(m_audio)
    {
        m_audio->stop();
    }
}

void Media::start()
{
    m_started = true;
    start_timer();
    if(m_audio)
    {
        m_audio->start();
    }
}

bool Media::is_running() const
{
    return m_started;
}

void Media::start_timer()
{
    Glib::signal_timeout().connect_once([=](){
        stop();
        media_stopped().emit();
    }, duration() * MSECS);
}

void Media::attach_audio(std::unique_ptr<Media> audio)
{
    m_audio = std::move(audio);
}

sigc::signal<void, Gtk::Widget&, Point>& Media::handler_requested()
{
    return m_handler_requsted;
}

sigc::signal<void>& Media::media_stopped()
{
    return m_media_stopped;
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
