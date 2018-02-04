#ifndef LAYOUTOVERLAY_HPP
#define LAYOUTOVERLAY_HPP

#include <gtkmm/overlay.h>
#include <map>

class LayoutOverlay : public Gtk::Overlay
{
public:
    LayoutOverlay() = default;
    void add_overlay(Gtk::Widget& widget, const Gdk::Rectangle& allocation);

protected:
    bool on_get_child_position(Gtk::Widget* widget, Gdk::Rectangle& allocation) override;

private:
    std::map<Gtk::Widget*, Gdk::Rectangle> m_saved_allocations;

};


#endif // LAYOUTOVERLAY_HPP
