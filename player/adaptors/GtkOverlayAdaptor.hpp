#pragma once

#include "IOverlayAdaptor.hpp"

#include <gtkmm/overlay.h>

class GtkOverlayAdaptor : public IOverlayAdaptor
{
public:
    struct ChildInfo
    {
        int width;
        int height;
        int x;
        int y;
    };

    GtkOverlayAdaptor();
    void show() override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;
    void addChild(IFixedLayoutAdaptor& child, int width, int height, int x, int y) override;
    void removeChildren() override;
    void addMainChild(IImageAdaptor& background) override;
    void removeMainChild() override;
    void reorderChild(IFixedLayoutAdaptor& child, int position) override;
    Gtk::Overlay& get();

private:
    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);

private:
    Gtk::Overlay m_handler;
    std::map<Gtk::Widget*, ChildInfo> m_children;

};
