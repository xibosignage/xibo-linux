#include "Media.hpp"

#include "constants.hpp"

#include "utils/TimerProvider.hpp"

#include "common/logger/Logging.hpp"

Media::Media(const MediaOptions& model, const std::shared_ptr<Widget>& view) :
    m_options(model),
    m_view(view),
    m_timer(std::make_unique<TimerProvider>())
{
}

void Media::attachMedia(std::unique_ptr<Media>&& attachedMedia)
{
    m_attachedMedia = std::move(attachedMedia);
}

void Media::start()
{
    startAttachedMedia();
    startTimer(m_options.duration);
    onStarted();
}

void Media::startTimer(int duration)
{
    if(duration > 0)
    {
        m_timer->startOnce(static_cast<unsigned int>(duration) * MSECS, [this]{
            m_mediaFinished.emit();
        });
    }
}

void Media::startAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->start();
    }
}

void Media::onStarted()
{
    if(m_view)
    {
        m_view->show();
    }
}

void Media::stop()
{
    stopAttachedMedia();
    onStopped();
}

void Media::stopAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->stop();
    }
}

void Media::onStopped()
{
    if(m_view)
    {
        m_view->hide();
    }
}

std::shared_ptr<Widget> Media::view() const
{
    return m_view;
}

MediaGeometry::Align Media::align() const
{
    return m_options.geometry.align;
}

MediaGeometry::Valign Media::valign() const
{
    return m_options.geometry.valign;
}

SignalMediaFinished Media::mediaFinished()
{
    return m_mediaFinished;
}
