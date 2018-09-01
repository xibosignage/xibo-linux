#include "GtkFixedLayoutWrapper.hpp"

int GtkFixedLayoutWrapper::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

int GtkFixedLayoutWrapper::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}

void GtkFixedLayoutWrapper::set_size(int width, int height)
{
    m_handler.set_size_request(width, height);
}

void GtkFixedLayoutWrapper::add_child(Gtk::Widget& widget, int left, int top)
{
    m_handler.put(widget, left, top);
}

void GtkFixedLayoutWrapper::show()
{
    m_handler.show();
}

Gtk::Fixed& GtkFixedLayoutWrapper::get()
{
    return m_handler;
}
