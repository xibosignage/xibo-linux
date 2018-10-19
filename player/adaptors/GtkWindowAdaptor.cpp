#include "GtkWindowAdaptor.hpp"

GtkWindowAdaptor::GtkWindowAdaptor()
{
    m_handler.signal_realize().connect(sigc::mem_fun(*this, &GtkWindowAdaptor::onRealized));
}

void GtkWindowAdaptor::show()
{
    m_handler.show();
}

void GtkWindowAdaptor::hide()
{
    m_handler.hide();
}

bool GtkWindowAdaptor::isShown() const
{
    return m_handler.is_visible();
}

void GtkWindowAdaptor::scale(double scaleX, double scaleY)
{

}

void GtkWindowAdaptor::setSize(int width, int height)
{
    m_handler.set_default_size(width, height);
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

void GtkWindowAdaptor::add(IWidgetAdaptor& child)
{
    auto&& handler = getHandler(child);
    m_handler.add(handler);
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
}

void GtkWindowAdaptor::unfullscreen()
{
    m_handler.unfullscreen();
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

void GtkWindowAdaptor::onRealized()
{
    if(!m_cursorVisible)
    {
        auto window = m_handler.get_window();
        auto cursor = Gdk::Cursor::create(Gdk::BLANK_CURSOR);
        window->set_cursor(cursor);
    }
}
