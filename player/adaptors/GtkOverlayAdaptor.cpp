#include "GtkOverlayAdaptor.hpp"
#include "GtkImageAdaptor.hpp"
#include "GtkFixedLayoutAdaptor.hpp"

GtkOverlayAdaptor::GtkOverlayAdaptor()
{
    m_handler.signal_get_child_position().
            connect(sigc::mem_fun(*this, &GtkOverlayAdaptor::onGetChildPosition), false);
}

void GtkOverlayAdaptor::show()
{
    m_handler.show();
}

void GtkOverlayAdaptor::addChild(IFixedLayoutAdaptor& child, int width, int height, int x, int y)
{
    auto&& h = static_cast<GtkFixedLayoutAdaptor&>(child);

    m_children.insert(std::make_pair(&h.get(), ChildInfo{width, height, x, y}));
    m_handler.add_overlay(h.get());
}

void GtkOverlayAdaptor::removeChildren()
{
    for(auto&& widget : m_handler.get_children())
    {
        m_handler.Gtk::Container::remove(*widget);
    }
    m_children.clear();
}

void GtkOverlayAdaptor::addMainChild(IImageAdaptor& background)
{
    auto&& h = static_cast<GtkImageAdaptor&>(background);
    m_handler.add(h.get());
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

void GtkOverlayAdaptor::reorderChild(IFixedLayoutAdaptor& child, int position)
{
    auto&& h = static_cast<GtkFixedLayoutAdaptor&>(child);
    m_handler.reorder_overlay(h.get(), position);
}

Gtk::Overlay&GtkOverlayAdaptor::get()
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
