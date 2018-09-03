#include "GtkFixedLayoutAdaptor.hpp"

int GtkFixedLayoutAdaptor::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

int GtkFixedLayoutAdaptor::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}

void GtkFixedLayoutAdaptor::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
}

void GtkFixedLayoutAdaptor::addChild(Gtk::Widget& widget, int left, int top)
{
    m_handler.put(widget, left, top);
}

void GtkFixedLayoutAdaptor::show()
{
    m_handler.show();
}

Gtk::Fixed& GtkFixedLayoutAdaptor::get()
{
    return m_handler;
}
