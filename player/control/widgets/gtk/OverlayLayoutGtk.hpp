#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "control/widgets/OverlayLayout.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/overlay.h>

class OverlayLayoutGtk : public WidgetGtk<Xibo::OverlayLayout>
{
public:
    struct ChildInfo
    {
        ChildInfo(const std::shared_ptr<Xibo::Widget>& widget_, int x_, int y_, int z_) :
            widget(widget_),
            x(x_),
            y(y_),
            z(z_)
        {
        }

        std::shared_ptr<Xibo::Widget> widget;
        int x;
        int y;
        int z;
    };

    using WidgetsWithInfo = std::vector<ChildInfo>;

    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    OverlayLayoutGtk(int width, int height);

    void addChild(const std::shared_ptr<Xibo::Widget>& child, int x, int y, int z) override;
    void removeChildren() override;
    void reorderChild(const std::shared_ptr<Xibo::Widget>& child, int z) override;
    void setMainChild(const std::shared_ptr<Xibo::Widget>& mainChild) override;

    void scale(double scaleX, double scaleY) override;

    Gtk::Overlay& get() override;

private:
    void removePreviousMainChild();
    void sortChildrenByZindex();
    void reorderChildren();
    void checkZorder(int z);
    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);
    WidgetsWithInfo::iterator findChild(Gtk::Widget* widget);
    void scaleChildren(double scaleX, double scaleY);

private:
    Gtk::Overlay handler_;
    std::shared_ptr<Xibo::Widget> mainChild_;
    WidgetsWithInfo children_;
};
