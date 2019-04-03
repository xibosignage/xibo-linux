#include "MainWindow.hpp"

#include <gtkmm/cssprovider.h>

#if GTKMM_MAJOR_VERSION>=3 && GTKMM_MINOR_VERSION>18
    const std::string DEFAULT_STYLE = "window { background-color: black; }";
#else
    const std::string DEFAULT_STYLE = "GtkMainWindow { background-color: black; }";
#endif

MainWindow::MainWindow(int width, int height) :
    Widget(m_handler)
{
    m_handler.signal_realize().connect(sigc::mem_fun(*this, &MainWindow::onRealized));
    loadDefaultStyle();

    m_handler.add_events(Gdk::KEY_PRESS_MASK);
    m_handler.signal_key_press_event().connect([this](GdkEventKey* event){
        m_keyPressed.emit(event->string);
        return false;
    });

    setSize(width, height);
}

void MainWindow::showAll()
{
    Widget::showAll();

    m_child->showAll();
}

void MainWindow::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleX);

    m_child->scale(scaleX, scaleY);
}

void MainWindow::onRealized()
{
    if(!m_cursorVisible)
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

    if(css_provider->load_from_data(DEFAULT_STYLE))
    {
        Glib::RefPtr<Gdk::Screen> screen = m_handler.get_screen();
        style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void MainWindow::setSize(int width, int height)
{
    m_originalWidth = width;
    m_originalHeight = height;

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

void MainWindow::addWidget(const std::shared_ptr<Widget>& child)
{
    m_handler.add(child->get());

    m_child = child;
}

void MainWindow::removeWidget()
{
    m_handler.remove();

    m_child.reset();
}

void MainWindow::move(int x, int y)
{
    m_handler.move(x, y);
}

void MainWindow::disableWindowResize()
{
    //m_handler.set_resizable(false);
}

void MainWindow::disableWindowDecoration()
{
    //m_handler.set_decorated(false);
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
    m_windowState = m_handler.signal_window_state_event().connect([=](GdkEventWindowState* ev){
        if(ev->new_window_state != GDK_WINDOW_STATE_FULLSCREEN)
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

SignalKeyPressed MainWindow::keyPressed()
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
    m_resizeSignal.emit();
}
