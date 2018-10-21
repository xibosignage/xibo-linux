#include "Video.hpp"

#include "media/MediaVisitor.hpp"
#include "utils/Utilities.hpp"

Video::Video(bool looped, std::unique_ptr<IVideoHandler>&& handler) :
    m_handler(std::move(handler)), m_looped(looped)
{
    m_handler->connect(std::bind(&Video::onVideoFinished, this));
}

void Video::onVideoFinished()
{
    Utils::logger()->debug("Playback finished");

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

IWidgetAdaptor& Video::handler()
{
    return m_handler->videoWindow();
}

void Video::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
}
