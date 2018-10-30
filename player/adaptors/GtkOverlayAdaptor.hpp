#pragma once

#include "IOverlayAdaptor.hpp"
#include "GtkAdaptor.hpp"

#include <gtkmm/overlay.h>

class GtkOverlayAdaptor : public GtkAdaptor<IOverlayAdaptor>
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

    void addChild(IWidgetAdaptor& child, int width, int height, int x, int y) override;
    void removeChildren() override;
    void addMainChild(IWidgetAdaptor& mainChild) override;
    void removeMainChild() override;
    void reorderChild(IWidgetAdaptor& child, int position) override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Overlay& get() override;

private:
    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);
    void scaleChildren(double scaleX, double scaleY);
    void updateOffsets();

private:
    Gtk::Overlay m_handler;
    std::map<Gtk::Widget*, ChildInfo> m_children;
    int m_xOffset = 0;
    int m_yOffset = 0;

};
