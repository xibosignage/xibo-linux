#include "GtkWindowWrapper.hpp"
#include "GtkOverlayWrapper.hpp"

void GtkWindowWrapper::add(IOverlayWrapper& overlay)
{
    auto&& gtk_overlay = static_cast<GtkOverlayWrapper&>(overlay);
    m_handler.add(gtk_overlay.get());
}

void GtkWindowWrapper::set_default_size(int width, int height)
{
    m_handler.set_default_size(width, height);
}

void GtkWindowWrapper::move(int x, int y)
{
    m_handler.move(x, y);
}

void GtkWindowWrapper::set_resizable(bool resizable)
{
    m_handler.set_resizable(resizable);
}

void GtkWindowWrapper::set_decorated(bool decorated)
{
    m_handler.set_decorated(decorated);
}

void GtkWindowWrapper::fullscreen()
{
    m_handler.fullscreen();
}

void GtkWindowWrapper::set_keep_above(bool keep_above)
{
    m_handler.set_keep_above(keep_above);
}

Gtk::Window& GtkWindowWrapper::get()
{
    return m_handler;
}
