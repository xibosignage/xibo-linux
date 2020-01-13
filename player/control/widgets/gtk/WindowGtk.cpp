#include "WindowGtk.hpp"

#include "common/logger/Logging.hpp"

#include <boost/format.hpp>
#include <gdk/gdkx.h>
#include <gtkmm/cssprovider.h>

WindowGtk::WindowGtk() : SingleContainerGtk{handler_}, cursorVisible_(true), fullscreen_(false)
{
    handler_.signal_realize().connect(std::bind(&WindowGtk::onWindowRealized, this));
    handler_.add_events(Gdk::KEY_PRESS_MASK);
    handler_.signal_key_press_event().connect([this](GdkEventKey* event) {
        keyPressed_(KeyboardKey{event->string, event->keyval});
        return false;
    });
    handler_.signal_window_state_event().connect([this](GdkEventWindowState* event) {
        if (event->changed_mask & GDK_WINDOW_STATE_FULLSCREEN)
        {
            bool fullscreen = event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN;
            if (!fullscreen)
            {
                // setSize right after unfullscreen() only change virtual size of the window but not
                // physical because unfullscreen() is async operation and only after this execution we can restore
                // previous physical window dimensions.
                handler_.resize(this->width(), this->height());
            }
        }
        return false;
    });
}

void WindowGtk::addToHandler(const std::shared_ptr<Xibo::Widget>& child)
{
    handler_.add(handlerFor(child));
}

void WindowGtk::removeFromHandler(const std::shared_ptr<Xibo::Widget>& /*child*/)
{
    handler_.remove();
}

void WindowGtk::setSize(int width, int height)
{
    resizeWindow(width, height);
}

void WindowGtk::resizeWindow(int width, int height)
{
    SingleContainerGtk::setSize(width, height);
    handler_.resize(width, height);
}

int WindowGtk::x() const
{
    int x, _;
    handler_.get_position(x, _);
    return x;
}

int WindowGtk::y() const
{
    int _, y;
    handler_.get_position(_, y);
    return y;
}

void WindowGtk::move(int x, int y)
{
    handler_.move(x, y);
}

void WindowGtk::disableWindowResize()
{
    handler_.set_resizable(false);
}

void WindowGtk::disableWindowDecoration()
{
    handler_.set_decorated(false);
}

SignalKeyPressed& WindowGtk::keyPressed()
{
    return keyPressed_;
}

void WindowGtk::fullscreen()
{
    auto monitorGeometry = currentMonitorGeometry();
    if (!monitorGeometry.has_zero_area())
    {
        // Fullscreen means that window will be shown on monitor dimensions withour borders but children are not
        // automatically resized during this operation so we need to force them to update their sizes.
        // Also, we need to save previous window dimensions to restore them later
        resizeWindow(monitorGeometry.get_width(), monitorGeometry.get_height());

        fullscreen_ = true;
        handler_.fullscreen();  // TODO: probably use fullscreen on specific monitor
    }
    else
    {
        Log::error("[WindowGtk] Failed to get current monitor geometry");
    }
}

void WindowGtk::unfullscreen()
{
    if (isFullscreen())
    {
        fullscreen_ = false;
        handler_.unfullscreen();
    }
}

bool WindowGtk::isFullscreen() const
{
    return fullscreen_;
}

void WindowGtk::setCursorVisible(bool cursorVisible)
{
    cursorVisible_ = cursorVisible;
}

void WindowGtk::onWindowRealized()
{
    if (!cursorVisible_)
    {
        auto window = handler_.get_window();
        if (window)
        {
            window->set_cursor(Gdk::Cursor::create(Gdk::BLANK_CURSOR));
        }
        else
        {
            Log::error("[WindowGtk] Failed to set blank cursor");
        }
    }
}

Gdk::Rectangle WindowGtk::currentMonitorGeometry()
{
    Gdk::Rectangle geometry;
    auto screen = Glib::RefPtr<Gdk::Screen>::cast_const(handler_.get_screen());
    if (screen)
    {
        int monitor = screen->get_monitor_at_point(x(), y());
        screen->get_monitor_geometry(monitor, geometry);
    }
    return geometry;
}

void WindowGtk::setBackgroundColor(const Color& color)
{
    boost::format windowStyleFmt{"window { background-color: %1%; }"};

    auto cssProvider = Gtk::CssProvider::create();
    auto styleContext = Gtk::StyleContext::create();
    auto screen = handler_.get_screen();
    auto windowStyle = (windowStyleFmt % color.string()).str();

    if (cssProvider && styleContext && screen && cssProvider->load_from_data(windowStyle))
    {
        styleContext->add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    else
    {
        Log::error("[WindowGtk] Failed to set background color");
    }
}

NativeWindow WindowGtk::nativeWindow()
{
    auto window = handler_.get_window();
    if (window)
    {
        return gdk_x11_window_get_xid(window->gobj());
    }
    return DefaultNativeWindow;
}

void WindowGtk::setKeepAbove(bool keep_above)
{
    handler_.set_keep_above(keep_above);
}

Gtk::Window& WindowGtk::handler()
{
    return handler_;
}
