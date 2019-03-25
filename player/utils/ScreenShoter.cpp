#include "ScreenShoter.hpp"

#include "utils/FilePath.hpp"

#include <cairomm/xlib_surface.h>
#include <fstream>
#include <boost/beast/core/detail/base64.hpp>

void ScreenShoter::take(const FilePath& path)
{
    auto buffer = takeScreenshot();

    std::ofstream out(path);
    out << boost::beast::detail::base64_encode(buffer.data(), buffer.size());
}

std::string ScreenShoter::takeBase64()
{
    auto buffer = takeScreenshot();

    return boost::beast::detail::base64_encode(buffer.data(), buffer.size());
}

std::vector<unsigned char> ScreenShoter::takeScreenshot()
{
    auto surface = takeXDisplayScreenshot();

    return copySurfaceToBuffer(surface);
}

std::vector<unsigned char> ScreenShoter::copySurfaceToBuffer(const Cairo::RefPtr<Cairo::Surface>& surface)
{
    std::vector<unsigned char> buffer;

    surface->write_to_png_stream([&buffer](const unsigned char* data, unsigned int length){
        std::copy(data, data + length, std::back_inserter(buffer));
        return CAIRO_STATUS_SUCCESS;
    });

    return buffer;
}

Cairo::RefPtr<Cairo::Surface> ScreenShoter::takeXDisplayScreenshot()
{
    Display* display = XOpenDisplay(nullptr);
    int screen = DefaultScreen(display);
    Window rootWindow = DefaultRootWindow(display);

    return Cairo::XlibSurface::create(display,
                                      rootWindow,
                                      DefaultVisual(display, screen),
                                      DisplayWidth(display, screen),
                                      DisplayHeight(display, screen));
}
