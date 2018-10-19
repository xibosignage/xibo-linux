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

void GtkFixedLayoutAdaptor::scale(double scaleX, double scaleY)
{

}

void GtkFixedLayoutAdaptor::addChild(IWidgetAdaptor& child, int left, int top)
{
    auto&& handler = getHandler(child);
    m_handler.put(handler, left, top);
}

void GtkFixedLayoutAdaptor::removeChildren()
{
    for(auto&& widget : m_handler.get_children())
    {
        m_handler.remove(*widget);
    }
}

void GtkFixedLayoutAdaptor::show()
{
    m_handler.show();
}

void GtkFixedLayoutAdaptor::hide()
{
    m_handler.hide();
}

bool GtkFixedLayoutAdaptor::isShown() const
{
    return m_handler.is_visible();
}

Gtk::Fixed& GtkFixedLayoutAdaptor::get()
{
    return m_handler;
}
