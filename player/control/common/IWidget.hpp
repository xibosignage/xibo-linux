#pragma once

#include <sigc++/signal.h>

using SignalShown = sigc::signal<void()>;

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

    virtual SignalShown shown() = 0;

};

