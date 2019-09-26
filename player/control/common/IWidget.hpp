#pragma once

#include <boost/signals2/signal.hpp>

using SignalShown = boost::signals2::signal<void()>;

class IWidget
{
public:
    virtual ~IWidget() = default;

    virtual void show() = 0;
    virtual void showAll() = 0;
    virtual void hide() = 0;
    virtual bool isShown() const = 0;

    virtual void scale(double scaleX, double scaleY) = 0;
    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void setOpacity(double value) = 0;
    virtual double opacity() const = 0;

    virtual SignalShown& shown() = 0;
};
