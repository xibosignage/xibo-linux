#include "RegionContent.hpp"

#include "media/IMedia.hpp"
#include "adaptors/GtkDrawingAreaAdaptor.hpp"
#include "constants.hpp"

RegionContent::RegionContent(std::unique_ptr<IMedia>&& media, std::unique_ptr<ITimerProvider>&& timer) : m_media(std::move(media)), m_timer(std::move(timer))
{
    if(auto visibleMedia = dynamic_cast<IVisible*>(m_media.get()))
    {
        m_mediaHandler = &visibleMedia->handler();
    }
    if(dynamic_cast<IPlayable*>(m_media.get()))
    {
        m_media->subcribe(EventType::PlaybackFinished, [=](const Event&){
           pushEvent(DurationExpiredEvent{});
        });
    }

    subscribeToEvents(*m_media);
}

void RegionContent::start()
{
    startTimer();
    pushEvent(StartMediaEvent{});
}

void RegionContent::startTimer()
{
    if(m_media->duration() > 0)
    {
        m_timer->startOnce(static_cast<unsigned int>(m_media->duration()) * MSECS, [=](){
            pushEvent(DurationExpiredEvent{});
        });
    }
}

void RegionContent::stop()
{
    pushEvent(StopMediaEvent{});
}

void RegionContent::attachMedia(std::unique_ptr<IMedia>&& attachedMedia)
{
    m_attachedMedia = std::move(attachedMedia);
    subscribeToEvents(*m_attachedMedia);
}

void RegionContent::scale(double scaleX, double scaleY)
{
    pushEvent(ScaleMediaEvent{scaleX, scaleY});
}

IWidgetAdaptor& RegionContent::handler()
{
    static GtkDrawingAreaAdaptor blankHandler;

    if(!m_mediaHandler)
        return blankHandler;

    return *m_mediaHandler;
}

void RegionContent::subscribeToEvents(IMedia& media)
{
    auto handler = std::bind(&IMedia::handleEvent, &media, std::placeholders::_1);

    subcribe(EventType::StartMedia, handler);
    subcribe(EventType::StopMedia, handler);
    subcribe(EventType::ScaleMedia, handler);
    subcribe(EventType::DurationExpired, handler);
}
