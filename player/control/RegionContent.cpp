#include "RegionContent.hpp"

#include "media/IMedia.hpp"
#include "adaptors/GtkDrawingAreaAdaptor.hpp"
#include "constants.hpp"

#include <cassert>

RegionContent::RegionContent(std::unique_ptr<IMedia>&& media, std::unique_ptr<ITimerProvider>&& timer) : m_media(std::move(media)), m_timer(std::move(timer))
{
    assert(m_media);
    assert(m_timer);

    checkVisibleMedia();
    checkPlayableMedia();

    subscribeToEvents(*m_media);
}

void RegionContent::checkVisibleMedia()
{
    if(auto visibleMedia = dynamic_cast<IVisible*>(m_media.get()))
    {
        m_mediaHandler = &visibleMedia->handler();
        subscribe(EventType::ScaleMedia, std::bind(&IMedia::handleEvent, m_media.get(), std::placeholders::_1));
    }
}

void RegionContent::checkPlayableMedia()
{
    if(dynamic_cast<IPlayable*>(m_media.get()))
    {
        m_media->subscribe(EventType::PlaybackFinished, [=](const Event&){
           pushEvent(new DurationExpiredEvent{});
        });
    }
}

void RegionContent::start()
{
    startTimer();
    pushEvent(new StartMediaEvent{});
}

void RegionContent::startTimer()
{
    if(m_media->duration() > 0)
    {
        m_timer->startOnce(static_cast<unsigned int>(m_media->duration()) * MSECS, [=](){
            pushEvent(new DurationExpiredEvent{});
        });
    }
}

void RegionContent::stop()
{
    pushEvent(new StopMediaEvent{});
}

void RegionContent::attachMedia(std::unique_ptr<IMedia>&& attachedMedia)
{
    assert(attachedMedia);

    m_attachedMedia = std::move(attachedMedia);
    subscribeToEvents(*m_attachedMedia);
}

void RegionContent::scale(double scaleX, double scaleY)
{
    pushEvent(new ScaleMediaEvent{scaleX, scaleY});
}

IWidgetAdaptor& RegionContent::handler()
{
    if(!m_mediaHandler)
        return blankHandler();

    return *m_mediaHandler;
}

IWidgetAdaptor& RegionContent::blankHandler()
{
    static GtkDrawingAreaAdaptor handler;
    return handler;
}

void RegionContent::subscribeToEvents(IMedia& media)
{
    auto handler = std::bind(&IMedia::handleEvent, &media, std::placeholders::_1);

    subscribe(EventType::StartMedia, handler);
    subscribe(EventType::StopMedia, handler);
    subscribe(EventType::DurationExpired, handler);
}
