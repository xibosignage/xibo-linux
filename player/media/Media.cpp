#include "Media.hpp"
#include "constants.hpp"

#include <glibmm/main.h>

void Media::stop()
{
    stopAudio();
    doStop();
}

void Media::stopAudio()
{
    if(m_audio)
    {
        m_audio->stop();
    }
}

void Media::start()
{
    startTimer();
    startAudio();
    doStart();
}

void Media::startAudio()
{
    if(m_audio)
    {
        m_audio->start();
    }
}

void Media::startTimer()
{
    Glib::signal_timeout().connect_once([=](){
        m_mediaTimeout.emit();
    }, duration() * MSECS);
}

void Media::attachAudio(std::unique_ptr<IMedia>&& audio)
{
    m_audio = std::move(audio);
}

void Media::connect(OnMediaTimeout callback)
{
    m_mediaTimeout.connect(callback);
}

int Media::duration() const
{
    return m_duration;
}

void Media::setDuration(int duration)
{
    m_duration = duration;
}

sigc::signal<void>& Media::mediaTimeout()
{
    return m_mediaTimeout;
}
