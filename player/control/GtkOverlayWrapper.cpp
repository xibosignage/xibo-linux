#include "GtkOverlayWrapper.hpp"

GtkOverlayWrapper::GtkOverlayWrapper()
{
    m_handler.signal_get_child_position().
            connect(sigc::mem_fun(*this, &GtkOverlayWrapper::on_get_child_position), false);
}

void GtkOverlayWrapper::show()
{
    m_handler.show();
}

void GtkOverlayWrapper::add_child(Gtk::Fixed& child, int top, int left, int width, int height)
{
    m_children.insert(std::make_pair(&child, ChildInfo{top, left, width, height}));
    m_handler.add_overlay(child);
}

void GtkOverlayWrapper::add(Gtk::Image& child)
{
    m_handler.add(child);
}

void GtkOverlayWrapper::remove()
{
    m_handler.remove();
}

void GtkOverlayWrapper::set_size(int width, int height)
{
    m_handler.set_size_request(width, height);
}

int GtkOverlayWrapper::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

int GtkOverlayWrapper::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}

void GtkOverlayWrapper::reorder_overlay(Gtk::Fixed& child, int position)
{
    m_handler.reorder_overlay(child, position);
}

Gtk::Overlay&GtkOverlayWrapper::get()
{
    return m_handler;
}

bool GtkOverlayWrapper::on_get_child_position(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = m_children.find(widget);
    if(it != m_children.end())
    {
        ChildInfo info = it->second;
        alloc.set_x(info.top); // scale
        alloc.set_y(info.left); // scale
        alloc.set_width(info.width);
        alloc.set_height(info.height);
        return true;
    }
    return false;
}
