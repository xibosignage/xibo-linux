#pragma once

#include "common/types/Color.hpp"
#include "control/widgets/KeyboardKey.hpp"
#include "control/widgets/NativeWindow.hpp"
#include "control/widgets/SingleContainer.hpp"

#include <boost/signals2/signal.hpp>

using SignalKeyPressed = boost::signals2::signal<void(const KeyboardKey&)>;

namespace Xibo
{
    class Window : public SingleContainer
    {
    public:
        virtual int x() const = 0;
        virtual int y() const = 0;
        virtual void move(int x, int y) = 0;

        virtual void disableWindowResize() = 0;
        virtual void disableWindowDecoration() = 0;
        virtual void setKeepAbove(bool keepAbove) = 0;
        virtual void fullscreen() = 0;
        virtual void unfullscreen() = 0;
        virtual bool isFullscreen() const = 0;
        virtual void setCursorVisible(bool cursorVisible) = 0;
        virtual NativeWindow nativeWindow() = 0;

        virtual void setBackgroundColor(const Color& color) = 0;
        virtual SignalKeyPressed& keyPressed() = 0;
    };
}
