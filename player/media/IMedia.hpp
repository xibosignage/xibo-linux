#pragma once

#include "events/IObserver.hpp"
#include "events/IObservable.hpp"

class MediaVisitor;
class IWidgetAdaptor;

class IMedia : public IObserver, public IObservable
{
public:
    virtual ~IMedia() = default;

    virtual int duration() const = 0;
    virtual void setDuration(int duration) = 0;

    virtual int id() const = 0;
    virtual void apply(MediaVisitor& visitor) = 0;
};

class IVisible
{
public:
    virtual ~IVisible() = default;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void scale(double scaleX, double scaleY) = 0;

    virtual IWidgetAdaptor& handler() = 0;

};

class IPlayable
{
public:
    virtual ~IPlayable() = default;

    virtual void play() = 0;
    virtual void stop() = 0;

};
