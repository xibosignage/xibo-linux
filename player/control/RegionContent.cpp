#include "RegionContent.hpp"

#include "media/IMedia.hpp"
#include "constants.hpp"

RegionContent::RegionContent(std::unique_ptr<IMedia>&& media, std::unique_ptr<ITimerProvider>&& timer) : m_media(std::move(media)), m_timer(std::move(timer))
{
    auto visibleMedia = dynamic_cast<IVisible*>(m_media.get());
    if(visibleMedia)
    {
        m_mediaHandler = &visibleMedia->handler();
    }

    subscribeToEvents(*m_media);
}

void RegionContent::start()
{
    startTimer();
    m_mediaPlaced.emit(StartMediaEvent{});
}

void RegionContent::startTimer()
{
    if(m_media->duration() > 0)
    {
        m_timer->startOnce(static_cast<unsigned int>(m_media->duration()) * MSECS, [=](){
            m_mediaTimeout.emit();
        });
    }
}

void RegionContent::stop()
{
    m_mediaRemoved.emit(StopMediaEvent{});
}

void RegionContent::attachMedia(std::unique_ptr<IMedia>&& attachedMedia)
{
    m_attachedMedia = std::move(attachedMedia);
    subscribeToEvents(*m_attachedMedia);
}

void RegionContent::connect(OnMediaTimeout callback)
{
    m_mediaTimeout.connect(callback);
}

void RegionContent::scale(double scaleX, double scaleY)
{
    m_mediaScaled.emit(ScaleEvent{scaleX, scaleY});
}

IWidgetAdaptor& RegionContent::handler()
{
    return *m_mediaHandler;
}

void RegionContent::subscribeToEvents(IMedia& media)
{
    m_mediaPlaced.connect(std::bind(&IMedia::handleEvent, &media, std::placeholders::_1));
    m_mediaRemoved.connect(std::bind(&IMedia::handleEvent, &media, std::placeholders::_1));
    m_mediaTimeout.connect(std::bind(&IMedia::handleEvent, &media, DurationExpiredEvent{}));
    m_mediaScaled.connect(std::bind(&IMedia::handleEvent, &media, std::placeholders::_1));
}
