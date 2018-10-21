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

Gtk::Fixed& GtkFixedLayoutAdaptor::get()
{
    return m_handler;
}
