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
    startTimer();
    if(m_audio)
    {
        m_audio->start();
    }
}

bool Media::isRunning() const
{
    return m_started;
}

void Media::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void Media::startTimer()
{
    Glib::signal_timeout().connect_once([=](){
        m_mediaTimeout.emit();
    }, duration() * MSECS);
}

void Media::attachAudio(std::unique_ptr<IMedia> audio)
{
    m_audio = std::move(audio);
}

void Media::connect(OnMediaTimeout callback)
{
    m_mediaTimeout.connect(callback);
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

sigc::signal<void>& Media::mediaTimeout()
{
    return m_mediaTimeout;
}
