#pragma once

#include "IRegionContent.hpp"

#include "utils/Event.hpp"
#include "utils/ITimerProvider.hpp"

#include <boost/noncopyable.hpp>

using MediaTimeoutSignal = sigc::signal<void>;
class ITimerProvider;

class RegionContent : public IRegionContent, private boost::noncopyable
{
public:
    RegionContent(std::unique_ptr<IMedia>&& media, std::unique_ptr<ITimerProvider>&& timer);

    void start() override;
    void stop() override;
    void attachMedia(std::unique_ptr<IMedia>&& attachedMedia) override;
    void connect(OnMediaTimeout callback) override;
    void scale(double scaleX, double scaleY) override;
    IWidgetAdaptor& handler() override;

private:
    void subscribeToEvents(IMedia& media);
    void startTimer();

private:
    std::unique_ptr<IMedia> m_media;
    std::unique_ptr<ITimerProvider> m_timer;
    std::unique_ptr<IMedia> m_attachedMedia;
    IWidgetAdaptor* m_mediaHandler = nullptr;
    MediaTimeoutSignal m_mediaTimeout;
    EventPublisher m_mediaPlaced;
    EventPublisher m_mediaRemoved;
    EventPublisher m_mediaScaled;
};
