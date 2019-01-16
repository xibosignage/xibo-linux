#pragma once

#include "events/IObservable.hpp"
#include <memory>

using OnMediaTimeout = std::function<void()>;

class IMedia;
class IWidgetAdaptor;

class IRegionContent : public IObservable
{
public:
    virtual ~IRegionContent() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void attachMedia(std::unique_ptr<IMedia>&& attachedMedia) = 0;
    virtual void scale(double scaleX, double scaleY) = 0;
    virtual IWidgetAdaptor& handler() = 0;
};
