#include "GtkWindowAdaptor.hpp"

#include <gtkmm/cssprovider.h>

#if GTKMM_MAJOR_VERSION>=3 && GTKMM_MINOR_VERSION>18
    const std::string DEFAULT_STYLE = "window { background-color: black; }";
#else
    const std::string DEFAULT_STYLE = "GtkWindow { background-color: black; }";
#endif

GtkWindowAdaptor::GtkWindowAdaptor() :
    GtkAdaptor<IWindowAdaptor>(m_handler)
{
    m_handler.signal_realize().connect(sigc::mem_fun(*this, &GtkWindowAdaptor::onRealized));
    loadDefaultStyle();
}

void GtkWindowAdaptor::onRealized()
{
    if(!m_cursorVisible)
    {
        auto window = m_handler.get_window();
        auto cursor = Gdk::Cursor::create(Gdk::BLANK_CURSOR);
        window->set_cursor(cursor);
    }
}

void GtkWindowAdaptor::loadDefaultStyle()
{
    Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
    Glib::RefPtr<Gtk::StyleContext> style_context = Gtk::StyleContext::create();

    if(css_provider->load_from_data(DEFAULT_STYLE))
    {
        Glib::RefPtr<Gdk::Screen> screen = m_handler.get_screen();
        style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void GtkWindowAdaptor::setSize(int width, int height)
{
    m_originalWidth = width;
    m_originalHeight = height;

    setWindowSize(width, height);
    m_handler.resize(width, height);
}

int GtkWindowAdaptor::width() const
{
    int width, _;
    m_handler.get_default_size(width, _);
    return width;
}

int GtkWindowAdaptor::height() const
{
    int _, height;
    m_handler.get_default_size(_, height);
    return height;
}

void GtkWindowAdaptor::connectToHandlerResize(std::function<void ()> handler)
{
    m_resizeSignal.connect(handler);
}

void GtkWindowAdaptor::add(IWidgetAdaptor& child)
{
    auto&& handler = getHandler(child);
    m_handler.add(handler);
}

void GtkWindowAdaptor::remove()
{
    m_handler.remove();
}

void GtkWindowAdaptor::move(int x, int y)
{
    m_handler.move(x, y);
}

void GtkWindowAdaptor::disableWindowResize()
{
    //m_handler.set_resizable(false);
}

void GtkWindowAdaptor::disableWindowDecoration()
{
    //m_handler.set_decorated(false);
}

void GtkWindowAdaptor::fullscreen()
{
    m_handler.fullscreen();

    auto area = getCurrentMonitorGeometry();

    setWindowSize(area.get_width(), area.get_height());
}

Gdk::Rectangle GtkWindowAdaptor::getCurrentMonitorGeometry() const
{
    Gdk::Rectangle area;
    auto screen = Glib::RefPtr<Gdk::Screen>::cast_const(m_handler.get_screen());
    auto monitor = screen->get_monitor_at_window(screen->get_active_window());

    screen->get_monitor_geometry(monitor, area);

    return area;
}

void GtkWindowAdaptor::unfullscreen()
{
    m_handler.unfullscreen();
    setWindowSize(m_originalWidth, m_originalHeight);
}

void GtkWindowAdaptor::setCursorVisible(bool cursorVisible)
{
    m_cursorVisible = cursorVisible;
}

void GtkWindowAdaptor::setKeepAbove(bool keep_above)
{
    m_handler.set_keep_above(keep_above);
}

Gtk::Window& GtkWindowAdaptor::get()
{
    return m_handler;
}

void GtkWindowAdaptor::setWindowSize(int width, int height)
{
    m_handler.set_default_size(width, height);
    m_resizeSignal.emit();
}
