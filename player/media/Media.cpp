#include "Media.hpp"
#include "Audio.hpp"

#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <glibmm/main.h>
#include <iostream>

Media::Media(int id, int width, int height, int duration, Render render, const std::string& uri) :
    m_id(id), m_width(width), m_height(height), m_duration(duration), m_render(render), m_uri(uri)
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

void Media::set_size(int width, int height)
{
    m_width = width;
    m_height = height;
}

void Media::start_timer()
{
    Glib::signal_timeout().connect_once([=](){
        media_timeout().emit();
    }, duration() * MSECS);
}

void Media::attach_audio(std::unique_ptr<IMedia> audio)
{
    m_audio = std::move(audio);
}

sigc::signal<void, Gtk::Widget&, int, int>& Media::handler_requested()
{
    return m_handler_requsted;
}

sigc::signal<void>& Media::media_timeout()
{
    return m_media_timeout;
}

int Media::id() const
{
    return m_id;
}

int Media::width() const
{
    return m_width;
}

int Media::height() const
{
    return m_height;
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
