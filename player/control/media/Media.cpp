#include "Media.hpp"

#include "constants.hpp"

#include "utils/TimerProvider.hpp"

Media::Media(const MediaOptions& model) :
    m_options(model),
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
    m_started.emit();
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

// FIXME check timer (maybe it should be stopped)
void Media::stop()
{
    stopAttachedMedia();
    m_stopped.emit();
}

SignalStarted Media::started()
{
    return m_started;
}

SignalStopped Media::stopped()
{
    return m_stopped;
}

SignalStopped Media::mediaFinished()
{
    return m_mediaFinished;
}

void Media::stopAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->stop();
    }
}
