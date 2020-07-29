#pragma once

#include "control/screenshot/ScreenShoter.hpp"

#include <cairomm/surface.h>

class X11ScreenShoter : public ScreenShoter
{
public:
    X11ScreenShoter(Xibo::Window& window);

protected:
    void takeScreenshotNative(NativeWindow window, const ImageBufferCreated& callback) override;

private:
    std::vector<unsigned char> copySurfaceToBuffer(const Cairo::RefPtr<Cairo::Surface>& surface);
};
