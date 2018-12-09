#pragma once

#include "IRegionContent.hpp"

#include "utils/ITimerProvider.hpp"
#include "utils/Observable.hpp"
#include "adaptors/IWidgetAdaptor.hpp"

#include <boost/noncopyable.hpp>

class RegionContent : public Observable<IRegionContent>, private boost::noncopyable
{
public:
    RegionContent(std::unique_ptr<IMedia>&& media, std::unique_ptr<ITimerProvider>&& timer);

    void start() override;
    void stop() override;
    void attachMedia(std::unique_ptr<IMedia>&& attachedMedia) override;
    void scale(double scaleX, double scaleY) override;
    IWidgetAdaptor& handler() override;

protected:
    virtual IWidgetAdaptor& blankHandler();

private:
    void checkVisibleMedia();
    void checkPlayableMedia();
    void subscribeToEvents(IMedia& media);
    void startTimer();

private:
    std::unique_ptr<IMedia> m_media;
    std::unique_ptr<ITimerProvider> m_timer;
    std::unique_ptr<IMedia> m_attachedMedia;
    IWidgetAdaptor* m_mediaHandler = nullptr;
};


