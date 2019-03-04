#include "Video.hpp"

#include "IVideoHandler.hpp"
#include "utils/logger/Logging.hpp"

Video::Video(int id, int width, int height, const Uri& uri, std::unique_ptr<IVideoHandler>&& handler) :
    Media(id), m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
    m_handler->load(uri);
    m_handler->connect(std::bind(&Video::onVideoFinished, this));
}

void Video::onVideoFinished()
{
    if(duration() == 0)
    {
        pushEvent(PlaybackFinishedEvent{});
        return;
    }

    if(m_looped == VideoOptions::Loop::Enable)
    {
        Log::debug("Looping enabled. Restarting...");
        m_handler->play();
    }
}

void Video::play()
{
    m_handler->play();
}

void Video::stop()
{
    m_handler->stop();
}

void Video::scale(double scaleX, double scaleY)
{
    m_handler->scale(scaleX, scaleY);
}

int Video::width() const
{
    return m_handler->width();
}

int Video::height() const
{
    return m_handler->height();
}

void Video::setLooped(VideoOptions::Loop looped)
{
    m_looped = looped;
}

VideoOptions::Loop Video::looped() const
{
    return m_looped;
}

void Video::setMuted(VideoOptions::Mute muted)
{
    m_handler->setVolume(muted == VideoOptions::Mute::Enable ? MIN_VOLUME : MAX_VOLUME);
}

MediaGeometry::Align Video::align() const
{
    return MediaGeometry::Align::Left;
}

MediaGeometry::Valign Video::valign() const
{
    return MediaGeometry::Valign::Top;
}

MediaGeometry::ScaleType Video::scaleType() const
{
    return MediaGeometry::ScaleType::Scaled;
}

IWidgetAdaptor& Video::handler()
{
    return m_handler->videoWindow();
}

void Video::handleEvent(const Event& ev)
{
    switch(ev.type())
    {
        case EventType::StartMedia:
            play();
            break;
        case EventType::StopMedia:
            stop();
            break;
        case EventType::DurationExpired:
            m_handler->stopPlayback();
            break;
        case EventType::ScaleMedia:
        {
            auto scaleEv = static_cast<const ScaleMediaEvent&>(ev);
            scale(scaleEv.scaleX(), scaleEv.scaleY());
            break;
        }
        default:
            break;
    }
}

