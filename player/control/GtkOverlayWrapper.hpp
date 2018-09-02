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
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;
    void addChild(IFixedLayoutWrapper& child, int top, int left, int width, int height) override;
    void add(IImageWrapper& background) override;
    void remove() override;
    void reorderChild(IFixedLayoutWrapper& child, int position) override;
    Gtk::Overlay& get();

private:
    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);

private:
    Gtk::Overlay m_handler;
    std::map<Gtk::Widget*, ChildInfo> m_children;
};
