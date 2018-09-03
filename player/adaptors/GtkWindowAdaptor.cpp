#include "GtkWindowAdaptor.hpp"
#include "GtkOverlayAdaptor.hpp"

void GtkWindowAdaptor::add(IOverlayAdaptor& overlay)
{
    auto&& gtk_overlay = static_cast<GtkOverlayAdaptor&>(overlay);
    m_handler.add(gtk_overlay.get());
}

void GtkWindowAdaptor::setDefaultSize(int width, int height)
{
    m_handler.set_default_size(width, height);
}

void GtkWindowAdaptor::move(int x, int y)
{
    m_handler.move(x, y);
}

void GtkWindowAdaptor::setResizable(bool resizable)
{
    m_handler.set_resizable(resizable);
}

void GtkWindowAdaptor::setDecorated(bool decorated)
{
    m_handler.set_decorated(decorated);
}

void GtkWindowAdaptor::fullscreen()
{
    m_handler.fullscreen();
}

void GtkWindowAdaptor::setKeepAbove(bool keep_above)
{
    m_handler.set_keep_above(keep_above);
}

Gtk::Window& GtkWindowAdaptor::get()
{
    return m_handler;
}
