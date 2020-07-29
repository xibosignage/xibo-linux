#pragma once

#include "control/widgets/Window.hpp"

#include <boost/signals2/signal.hpp>
#include <vector>

using ScreenShotTaken = std::function<void(const std::string&)>;
using ImageBuffer = std::vector<unsigned char>;
using ImageBufferCreated = std::function<void(const ImageBuffer&)>;

class ScreenShoter
{
public:
    ScreenShoter(Xibo::Window& window);
    virtual ~ScreenShoter() = default;

    void takeBase64(const ScreenShotTaken& callback);
    NativeWindow nativeWindow() const;

protected:
    virtual void takeScreenshotNative(NativeWindow window, const ImageBufferCreated& callback) = 0;

private:
    Xibo::Window& window_;
};
