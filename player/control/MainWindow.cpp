#include "MainWindow.hpp"

#include "constants.hpp"
#include "control/common/OverlayLayout.hpp"

#include <gtkmm/cssprovider.h>

#if GTKMM_MAJOR_VERSION >= 3 && GTKMM_MINOR_VERSION > 18
const std::string DefaultStyle = "window { background-color: black; }";
#else
const std::string DefaultStyle = "GtkWindow#mainWindow { background-color: black; }";
#endif

MainWindow::MainWindow() :
    Widget(handler_),
    layout_(std::make_unique<OverlayLayout>(MinDisplayWidth, MinDisplayHeight))
{
    handler_.signal_realize().connect(std::bind(&MainWindow::onRealized, this));
    loadDefaultStyle();

    handler_.set_name("mainWindow");
    handler_.add(getHandler(*layout_));
    handler_.add_events(Gdk::KEY_PRESS_MASK);
    handler_.signal_key_press_event().connect([this](GdkEventKey* event) {
        keyPressed_(event->string);
        return false;
    });
}

void MainWindow::showAll()
{
    Widget::showAll();

    layout_->showAll();
}

void MainWindow::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleX);

    layout_->scale(scaleX, scaleY);
}

void MainWindow::onRealized()
{
    if (!cursorVisible_)
    {
        auto window = handler_.get_window();
        auto cursor = Gdk::Cursor::create(Gdk::BLANK_CURSOR);
        window->set_cursor(cursor);
    }
}

void MainWindow::loadDefaultStyle()
{
    Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
    Glib::RefPtr<Gtk::StyleContext> style_context = Gtk::StyleContext::create();

    if (css_provider->load_from_data(DefaultStyle))
    {
        Glib::RefPtr<Gdk::Screen> screen = handler_.get_screen();
        style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void MainWindow::setSize(int width, int height)
{
    originalWidth_ = width;
    originalHeight_ = height;

    layout_->setSize(width, height);
    setWindowSize(width, height);
}

int MainWindow::width() const
{
    int width, _;
    handler_.get_default_size(width, _);
    return width;
}

int MainWindow::height() const
{
    int _, height;
    handler_.get_default_size(_, height);
    return height;
}

int MainWindow::x() const
{
    return xPos_;
}

int MainWindow::y() const
{
    return yPos_;
}

void MainWindow::setMainLayout(const std::shared_ptr<IWidget>& child)
{
    layout_->setMainChild(child);
}

void MainWindow::setOverlays(const std::vector<std::shared_ptr<IOverlayLayout>>& children)
{
    layout_->removeChildren();

    for (auto&& child : children)
    {
        layout_->addChild(child, DefaultXPos, DefaultYPos, 0);
    }
}

void MainWindow::move(int x, int y)
{
    xPos_ = x;
    yPos_ = y;
    handler_.move(x, y);
}

void MainWindow::disableWindowResize()
{
    handler_.set_resizable(false);
}

void MainWindow::disableWindowDecoration()
{
    handler_.set_decorated(false);
}

void MainWindow::fullscreen()
{
    auto area = getCurrentMonitorGeometry();

    setWindowSize(area.get_width(), area.get_height());
    handler_.fullscreen();
}

Gdk::Rectangle MainWindow::getCurrentMonitorGeometry() const
{
    Gdk::Rectangle area;
    auto screen = Glib::RefPtr<Gdk::Screen>::cast_const(handler_.get_screen());
    auto monitor = screen->get_monitor_at_window(screen->get_active_window());

    screen->get_monitor_geometry(monitor, area);

    return area;
}

void MainWindow::unfullscreen()
{
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

void MainWindow::setCursorVisible(bool cursorVisible)
{
    cursorVisible_ = cursorVisible;
}

SignalKeyPressed& MainWindow::keyPressed()
{
    return keyPressed_;
}

void MainWindow::setKeepAbove(bool keep_above)
{
    handler_.set_keep_above(keep_above);
}

Gtk::Window& MainWindow::get()
{
    return handler_;
}

void MainWindow::setWindowSize(int width, int height)
{
    handler_.set_default_size(width, height);
    handler_.resize(width, height);
    resizeSignal_();
}
