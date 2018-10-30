#include "GtkOverlayAdaptor.hpp"

#include <gtkmm/window.h>

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

void GtkOverlayAdaptor::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
    updateOffsets();
}

void GtkOverlayAdaptor::scale(double scaleX, double scaleY)
{
    GtkAdaptor::scale(scaleX, scaleY);
    updateOffsets();
    scaleChildren(scaleX, scaleY);
}

void GtkOverlayAdaptor::updateOffsets()
{
    Gtk::Window* parent = static_cast<Gtk::Window*>(m_handler.get_toplevel());
    if(parent->get_is_toplevel())
    {
        int width, height;
        parent->get_default_size(width, height);

        m_xOffset = (width - this->width())/2;
        m_yOffset = (height - this->height())/2;
    }
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

        alloc.set_x(m_xOffset + info.x);
        alloc.set_y(m_yOffset + info.y);
        alloc.set_width(info.width);
        alloc.set_height(info.height);
        return true;
    }
    return false;
}
