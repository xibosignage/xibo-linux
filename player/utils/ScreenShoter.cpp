#include "ScreenShoter.hpp"

#include "utils/FilePath.hpp"

#include <glibmm/main.h>
#include <gdkmm.h>
#include <cairomm/xlib_surface.h>
#include <gdk/gdkx.h>
#include <fstream>
#include <boost/beast/core/detail/base64.hpp>

ScreenShoter::ScreenShoter(MainWindow& window) :
    m_window(window)
{
}

void ScreenShoter::takeBase64(ScreenShotTaken callback)
{
    takeScreenshot([=](const std::vector<unsigned char>& buffer){
        auto base64 = boost::beast::detail::base64_encode(buffer.data(), buffer.size());
        callback(base64);
    });
}

void ScreenShoter::takeScreenshot(ImageBufferCreated callback)
{
    takeXDisplayScreenshot([=](const Cairo::RefPtr<Cairo::Surface>& surface){
        callback(copySurfaceToBuffer(surface));
    });
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

void ScreenShoter::takeXDisplayScreenshot(SurfaceCreated callback)
{
    Glib::MainContext::get_default()->invoke([=](){
        auto window = m_window.get().get_window();
        if(window)
        {
            Display* display = XOpenDisplay(nullptr);
            Window active = gdk_x11_window_get_xid(window->gobj());
            XWindowAttributes gwa;
            XGetWindowAttributes(display, active, &gwa);

            auto surface = Cairo::XlibSurface::create(display, active, gwa.visual, gwa.width, gwa.height);
            callback(surface);
        }
        return false;
    });
}
