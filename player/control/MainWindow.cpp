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
    Widget(m_handler),
    m_layout(std::make_unique<OverlayLayout>(MinDisplayWidth, MinDisplayHeight))
{
    m_handler.signal_realize().connect(std::bind(&MainWindow::onRealized, this));
    loadDefaultStyle();

    m_handler.set_name("mainWindow");
    m_handler.add(getHandler(*m_layout));
    m_handler.add_events(Gdk::KEY_PRESS_MASK);
    m_handler.signal_key_press_event().connect([this](GdkEventKey* event) {
        m_keyPressed(event->string);
        return false;
    });
}

void MainWindow::showAll()
{
    Widget::showAll();

    m_layout->showAll();
}

void MainWindow::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleX);

    m_layout->scale(scaleX, scaleY);
}

void MainWindow::onRealized()
{
    if (!m_cursorVisible)
    {
        auto window = m_handler.get_window();
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
        Glib::RefPtr<Gdk::Screen> screen = m_handler.get_screen();
        style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void MainWindow::setSize(int width, int height)
{
    m_originalWidth = width;
    m_originalHeight = height;

    m_layout->setSize(width, height);
    setWindowSize(width, height);
}

int MainWindow::width() const
{
    int width, _;
    m_handler.get_default_size(width, _);
    return width;
}

int MainWindow::height() const
{
    int _, height;
    m_handler.get_default_size(_, height);
    return height;
}

int MainWindow::x() const
{
    return m_xPos;
}

int MainWindow::y() const
{
    return m_yPos;
}

void MainWindow::setMainLayout(const std::shared_ptr<IWidget>& child)
{
    m_layout->setMainChild(child);
}

void MainWindow::setOverlays(const std::vector<std::shared_ptr<IOverlayLayout>>& children)
{
    m_layout->removeChildren();

    for (auto&& child : children)
    {
        m_layout->addChild(child, DefaultXPos, DefaultYPos, 0);
    }
}

void MainWindow::move(int x, int y)
{
    m_xPos = x;
    m_yPos = y;
    m_handler.move(x, y);
}

void MainWindow::disableWindowResize()
{
    m_handler.set_resizable(false);
}

void MainWindow::disableWindowDecoration()
{
    m_handler.set_decorated(false);
}

void MainWindow::fullscreen()
{
    auto area = getCurrentMonitorGeometry();

    setWindowSize(area.get_width(), area.get_height());
    m_handler.fullscreen();
}

Gdk::Rectangle MainWindow::getCurrentMonitorGeometry() const
{
    Gdk::Rectangle area;
    auto screen = Glib::RefPtr<Gdk::Screen>::cast_const(m_handler.get_screen());
    auto monitor = screen->get_monitor_at_window(screen->get_active_window());

    screen->get_monitor_geometry(monitor, area);

    return area;
}

void MainWindow::unfullscreen()
{
    m_handler.unfullscreen();
    m_windowState = m_handler.signal_window_state_event().connect([=](GdkEventWindowState* ev) {
        if (ev->new_window_state != GDK_WINDOW_STATE_FULLSCREEN)
        {
            m_windowState.disconnect();
            setWindowSize(m_originalWidth, m_originalHeight);
        }
        return true;
    });
}

void MainWindow::setCursorVisible(bool cursorVisible)
{
    m_cursorVisible = cursorVisible;
}

SignalKeyPressed& MainWindow::keyPressed()
{
    return m_keyPressed;
}

void MainWindow::setKeepAbove(bool keep_above)
{
    m_handler.set_keep_above(keep_above);
}

Gtk::Window& MainWindow::get()
{
    return m_handler;
}

void MainWindow::setWindowSize(int width, int height)
{
    m_handler.set_default_size(width, height);
    m_handler.resize(width, height);
    m_resizeSignal();
}
