#pragma once

#include <boost/signals2/signal.hpp>

using SignalShown = boost::signals2::signal<void()>;
using SignalResized = boost::signals2::signal<void()>;

namespace Xibo
{
    class Widget
    {
    public:
        virtual ~Widget() = default;

        virtual void show() = 0;
        virtual void showAll() = 0;
        virtual void skipShowAll() = 0;
        virtual void hide() = 0;
        virtual bool visible() const = 0;

        virtual void scale(double scaleX, double scaleY) = 0;
        virtual void setSize(int width, int height) = 0;
        virtual int width() const = 0;
        virtual int height() const = 0;

        virtual void setOpacity(double value) = 0;
        virtual double opacity() const = 0;

        virtual SignalShown& shown() = 0;
        virtual SignalResized& resized() = 0;
    };
}
