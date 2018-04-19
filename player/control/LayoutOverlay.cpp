#include "LayoutOverlay.hpp"

void LayoutOverlay::add_overlay(Gtk::Widget& widget, const Gdk::Rectangle& allocation)
{
    m_saved_allocations.emplace(&widget, allocation);
    Gtk::Overlay::add_overlay(widget);
}

bool LayoutOverlay::on_get_child_position(Gtk::Widget* widget, Gdk::Rectangle& allocation)
{
    int mock1, mock2;
    widget->get_preferred_height(mock1, mock2);
    widget->get_preferred_width(mock1, mock2);

    auto alloc = m_saved_allocations[widget];
    allocation.set_x(alloc.get_x());
    allocation.set_y(alloc.get_y());
    allocation.set_width(alloc.get_width());
    allocation.set_height(alloc.get_height());
    return true;
}

