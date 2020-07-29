#include "X11ScreenShoter.hpp"

#include "MainLoop.hpp"
#include "common/logger/Logging.hpp"

#include <cairomm/xlib_surface.h>

X11ScreenShoter::X11ScreenShoter(Xibo::Window& window) : ScreenShoter(window) {}

void X11ScreenShoter::takeScreenshotNative(NativeWindow window, const ImageBufferCreated& callback)
{
    MainLoop::pushToUiThread([=]() {
        try
        {
            Display* display = XOpenDisplay(nullptr);
            XWindowAttributes gwa;
            XGetWindowAttributes(display, window, &gwa);

            auto surface = Cairo::XlibSurface::create(display, window, gwa.visual, gwa.width, gwa.height);

            auto buffer = copySurfaceToBuffer(surface);
            callback(buffer);

            XCloseDisplay(display);
        }
        catch (std::exception& e)
        {
            Log::error("[X11ScreenShoter] {}", e.what());
        }
    });
}

std::vector<unsigned char> X11ScreenShoter::copySurfaceToBuffer(const Cairo::RefPtr<Cairo::Surface>& surface)
{
    std::vector<unsigned char> buffer;

    surface->write_to_png_stream([&buffer](const unsigned char* data, unsigned int length) {
        std::copy(data, data + length, std::back_inserter(buffer));
        return CAIRO_STATUS_SUCCESS;
    });

    return buffer;
}
