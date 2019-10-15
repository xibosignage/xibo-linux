#pragma once

#include "control/widgets/Window.hpp"

#include <boost/signals2/signal.hpp>
#include <vector>

using ScreenShotTaken = std::function<void(const std::string&)>;
using SignalImageBufferCreated = boost::signals2::signal<void(const std::vector<unsigned char>&)>;

class ScreenShoter
{
public:
    ScreenShoter(Xibo::Window& window);
    virtual ~ScreenShoter() = default;

    void takeBase64(const ScreenShotTaken& callback);
    NativeWindow nativeWindow() const;

protected:
    virtual void takeScreenshotNative(NativeWindow window) = 0;

    SignalImageBufferCreated& imageBufferCreated();

private:
    Xibo::Window& window_;
    SignalImageBufferCreated imageBufferCreated_;
};
