#include "GtkFixedLayoutAdaptor.hpp"

GtkFixedLayoutAdaptor::GtkFixedLayoutAdaptor() :
    GtkAdaptor<IFixedLayoutAdaptor>(m_handler)
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

void GtkFixedLayoutAdaptor::scale(double scaleX, double scaleY)
{
    scaleChildren(scaleX, scaleY);
    GtkAdaptor::scale(scaleX, scaleY);
}

void GtkFixedLayoutAdaptor::scaleChildren(double scaleX, double scaleY)
{
    for(auto&& widget : m_handler.get_children())
    {
        int x = m_handler.child_property_x(*widget);
        int y = m_handler.child_property_y(*widget);

        m_handler.move(*widget,
                       static_cast<int>(x * scaleX),
                       static_cast<int>(y * scaleY));
    }
}

Gtk::Fixed& GtkFixedLayoutAdaptor::get()
{
    return m_handler;
}
