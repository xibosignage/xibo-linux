#include "WindowGtk.hpp"

#include "common/logger/Logging.hpp"
#include "constants.hpp"
#include "control/widgets/OverlayLayoutFactory.hpp"

#include <boost/format.hpp>
#include <gtkmm/cssprovider.h>

WindowGtk::WindowGtk() : WidgetGtk(handler_), cursorVisible_(true), fullscreen_(false)
{
    handler_.signal_realize().connect(std::bind(&WindowGtk::onRealized, this));
    handler_.add_events(Gdk::KEY_PRESS_MASK);
    handler_.signal_key_press_event().connect([this](GdkEventKey* event) {
        keyPressed_(KeyboardKey{event->string, event->keyval});
        return false;
    });
}

void WindowGtk::showAll()
{
    handler_.show();

    if (child_)
    {
        child_->showAll();
    }
}

void WindowGtk::scale(double scaleX, double scaleY)
{
    WidgetGtk::scale(scaleX, scaleX);

    if (child_)
    {
        child_->scale(scaleX, scaleY);
    }
}

void WindowGtk::onRealized()
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
            Log::error("[WindowGtk] Blank cursor hasn't been set");
        }
    }
}

int WindowGtk::width() const
{
    int width, _;
    handler_.get_default_size(width, _);
    return width;
}

int WindowGtk::height() const
{
    int _, height;
    handler_.get_default_size(_, height);
    return height;
}

void WindowGtk::setSize(int width, int height)
{
    if (width != this->width() || height != this->height())
    {
        checkSize(width, height);

        originalWidth_ = width;
        originalHeight_ = height;

        if (child_)
        {
            child_->setSize(width, height);
        }
        setWindowSize(width, height);
    }
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
    if (x != this->x() || y != this->y())
    {
        handler_.move(x, y);
    }
}

void WindowGtk::setChild(const std::shared_ptr<Xibo::Widget>& child)
{
    assert(child);

    child_ = child;
    handler_.add(getHandler(*child_));
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
    auto area = getCurrentMonitorGeometry();

    if (!area.has_zero_area())
    {
        fullscreen_ = true;
        setWindowSize(area.get_width(), area.get_height());
        handler_.fullscreen();
    }
    else
    {
        Log::error("[WindowGtk] Fullscreen mode hasn't been activated");
    }
}

Gdk::Rectangle WindowGtk::getCurrentMonitorGeometry() const
{
    Gdk::Rectangle area;
    auto screen = Glib::RefPtr<Gdk::Screen>::cast_const(handler_.get_screen());
    if (screen)
    {
        auto activeWindow = screen->get_active_window();
        if (activeWindow)
        {
            auto monitor = screen->get_monitor_at_window(activeWindow);

            screen->get_monitor_geometry(monitor, area);
        }
    }
    return area;
}

void WindowGtk::unfullscreen()
{
    fullscreen_ = false;
    handler_.unfullscreen();
    windowState_ = handler_.signal_window_state_event().connect([=](GdkEventWindowState* ev) {
        if (ev->new_window_state != GDK_WINDOW_STATE_FULLSCREEN)
        {
            windowState_.disconnect();
            setWindowSize(originalWidth_, originalHeight_);
        }
        return true;
    });
}

bool WindowGtk::isFullscreen() const
{
    return fullscreen_;
}

void WindowGtk::setCursorVisible(bool cursorVisible)
{
    cursorVisible_ = cursorVisible;
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
        Log::error("[WindowGtk] Background color hasn't been set");
    }
}

void WindowGtk::setKeepAbove(bool keep_above)
{
    handler_.set_keep_above(keep_above);
}

Gtk::Window& WindowGtk::get()
{
    return handler_;
}

void WindowGtk::setWindowSize(int width, int height)
{
    handler_.set_default_size(width, height);
    handler_.resize(width, height);
    resizeSignal_();
}
