#include "Video.hpp"

#include "MediaVisitor.hpp"
#include "IVideoHandler.hpp"
#include "utils/Logger.hpp"

Video::Video(int id, int width, int height, const FilePath& path, std::unique_ptr<IVideoHandler>&& handler) :
    Media(id), m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
    m_handler->load(path);
    m_handler->connect(std::bind(&Video::onVideoFinished, this)); // FIXME change to subscribe
}

void Video::onVideoFinished()
{
    if(duration() == 0)
    {
        pushEvent(PlaybackFinishedEvent{});
        return;
    }

    if(m_looped)
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

void Video::setLooped(bool looped)
{
    m_looped = looped;
}

bool Video::looped() const
{
    return m_looped;
}

void Video::setMuted(bool muted)
{
    m_handler->setVolume(muted ? MIN_VOLUME : MAX_VOLUME);
}

IWidgetAdaptor& Video::handler()
{
    return m_handler->videoWindow();
}

void Video::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
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

