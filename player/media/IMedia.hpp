#pragma once

#include <string>
#include <memory>
#include <functional>

class MediaVisitor;
class IWidgetAdaptor;

using OnMediaTimeout = std::function<void()>;

class IMedia
{
public:
    virtual ~IMedia() = default;

    virtual void stop() = 0;
    virtual void start() = 0;

    virtual int duration() const = 0;
    virtual void setDuration(int duration) = 0;

    virtual void attachMedia(std::unique_ptr<IMedia>&& media) = 0;
    virtual void connect(OnMediaTimeout callback) = 0;

    virtual void apply(MediaVisitor& visitor) = 0;

};

class IVisibleMedia : public IMedia
{
public:
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void scale(double scaleX, double scaleY) = 0;

    virtual IWidgetAdaptor& handler() = 0;

};

class IInvisibleMedia : public IMedia
{

};
