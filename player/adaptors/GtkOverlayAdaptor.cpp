#include "GtkOverlayAdaptor.hpp"

GtkOverlayAdaptor::GtkOverlayAdaptor() :
    GtkAdaptor<IOverlayAdaptor>(m_handler)
{
    m_handler.signal_get_child_position().
            connect(sigc::mem_fun(*this, &GtkOverlayAdaptor::onGetChildPosition), false);
}

void GtkOverlayAdaptor::addChild(IWidgetAdaptor& child, int width, int height, int x, int y)
{
    auto&& handler = getHandler(child);

    m_children.insert(std::make_pair(&handler, ChildInfo{width, height, x, y}));
    m_handler.add_overlay(handler);
}

void GtkOverlayAdaptor::removeChildren()
{
    for(auto&& widget : m_handler.get_children())
    {
        m_handler.Gtk::Container::remove(*widget);
    }
    m_children.clear();
}

void GtkOverlayAdaptor::addMainChild(IWidgetAdaptor& mainChild)
{
    auto&& handler = getHandler(mainChild);
    m_handler.add(handler);
}

void GtkOverlayAdaptor::removeMainChild()
{
    m_handler.remove();
}

void GtkOverlayAdaptor::reorderChild(IWidgetAdaptor& child, int position)
{
    auto&& handler = getHandler(child);
    m_handler.reorder_overlay(handler, position);
}

void GtkOverlayAdaptor::scale(double scaleX, double scaleY)
{
    scaleChildren(scaleX, scaleY);
    GtkAdaptor::scale(scaleX, scaleY);
}

void GtkOverlayAdaptor::scaleChildren(double scaleX, double scaleY)
{
    for(auto&& [child, childInfo] : m_children)
    {
        childInfo.x *= scaleX;
        childInfo.y *= scaleY;
        childInfo.width *= scaleX;
        childInfo.height *= scaleY;
    }
}

Gtk::Overlay& GtkOverlayAdaptor::get()
{
    return m_handler;
}

bool GtkOverlayAdaptor::onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = m_children.find(widget);
    if(it != m_children.end())
    {
        ChildInfo info = it->second;

        alloc.set_x(info.x);
        alloc.set_y(info.y);
        alloc.set_width(info.width);
        alloc.set_height(info.height);
        return true;
    }
    return false;
}
