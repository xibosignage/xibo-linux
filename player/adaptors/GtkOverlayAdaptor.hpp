#pragma once

#include "IOverlayAdaptor.hpp"
#include "GtkBaseAdaptor.hpp"

#include <gtkmm/overlay.h>

class GtkOverlayAdaptor : public GtkBaseAdaptor<IOverlayAdaptor>
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
    bool isShown() const override;
    void scale(double scaleX, double scaleY) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;
    void addChild(IWidgetAdaptor& child, int width, int height, int x, int y) override;
    void removeChildren() override;
    void addMainChild(IWidgetAdaptor& mainChild) override;
    void removeMainChild() override;
    void reorderChild(IWidgetAdaptor& child, int position) override;
    Gtk::Overlay* get() override;
    void apply(AdaptorVisitor& visitor) override;

private:
    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);

private:
    Gtk::Overlay m_handler;
    std::map<Gtk::Widget*, ChildInfo> m_children;

};
