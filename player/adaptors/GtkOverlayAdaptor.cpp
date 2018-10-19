#include "GtkOverlayAdaptor.hpp"

GtkOverlayAdaptor::GtkOverlayAdaptor()
{
    m_handler.signal_get_child_position().
            connect(sigc::mem_fun(*this, &GtkOverlayAdaptor::onGetChildPosition), false);
}

void GtkOverlayAdaptor::show()
{
    m_handler.show();
}

void GtkOverlayAdaptor::hide()
{
    m_handler.hide();
}

bool GtkOverlayAdaptor::isShown() const
{
    return m_handler.is_visible();
}

void GtkOverlayAdaptor::scale(double scaleX, double scaleY)
{

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

void GtkOverlayAdaptor::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
}

int GtkOverlayAdaptor::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

int GtkOverlayAdaptor::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}

void GtkOverlayAdaptor::reorderChild(IWidgetAdaptor& child, int position)
{
    auto&& handler = getHandler(child);
    m_handler.reorder_overlay(handler, position);
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

        alloc.set_x(info.x); // scale
        alloc.set_y(info.y); // scale
        alloc.set_width(info.width);
        alloc.set_height(info.height);
        return true;
    }
    return false;
}
