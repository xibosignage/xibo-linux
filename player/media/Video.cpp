#include "Video.hpp"

#include "media/MediaVisitor.hpp"
#include "utils/Utilities.hpp"

Video::Video(int id, int width, int height, const FilePath& path, std::unique_ptr<IVideoHandler>&& handler) :
    Media(id), m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
    m_handler->load(path);
    m_handler->connect(std::bind(&Video::onVideoFinished, this));
}

void Video::onVideoFinished()
{
    if(duration() == 0)
    {
        mediaTimeout().emit();
        return;
    }

    if(m_looped)
    {
        Utils::logger()->debug("Looping enabled. Restarting...");
        m_handler->play();
    }
}

void Video::doStop()
{
    m_handler->stop();
}

void Video::doStart()
{
    m_handler->play();
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
