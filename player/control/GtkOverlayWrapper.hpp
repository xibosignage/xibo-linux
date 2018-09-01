#pragma once

#include "IOverlayWrapper.hpp"

#include <gtkmm/overlay.h>

class GtkOverlayWrapper : public IOverlayWrapper
{
public:
    struct ChildInfo
    {
        int top;
        int left;
        int width;
        int height;
    };

    GtkOverlayWrapper();
    void show() override;
    void set_size(int width, int height) override;
    int width() const override;
    int height() const override;
    void add_child(IFixedLayoutWrapper& child, int top, int left, int width, int height) override;
    void add(IImageWrapper& background) override;
    void remove() override;
    void reorder_child(IFixedLayoutWrapper& child, int position) override;
    Gtk::Overlay& get();

private:
    bool on_get_child_position(Gtk::Widget* widget, Gdk::Rectangle& alloc);

private:
    Gtk::Overlay m_handler;
    std::map<Gtk::Widget*, ChildInfo> m_children;
};
