#pragma once

#include "IOverlayLayout.hpp"
#include "control/common/Widget.hpp"

#include <gtkmm/overlay.h>

class OverlayLayout : public Widget<IOverlayLayout>
{
public:
    struct WidgetInfo
    {
        std::shared_ptr<IWidget> widget;
        int x;
        int y;
        int z;
    };

    using WidgetsWithInfo = std::vector<WidgetInfo>;

    OverlayLayout(int width, int height);

    void addChild(const std::shared_ptr<IWidget>& child, int x, int y, int z) override;
    void removeChildren() override;
    void reorderChild(const std::shared_ptr<IWidget>& child, int z) override;
    void setMainChild(const std::shared_ptr<IWidget>& mainChild) override;

    void showAll() override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Overlay& get() override;

private:
    void removePreviousMainChild();
    void sortRegionsByZindex();
    void reorderChildren();
    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);
    WidgetsWithInfo::iterator findChild(Gtk::Widget* widget);
    void scaleChildren(double scaleX, double scaleY);
    void updateOffsets();

private:
    Gtk::Overlay handler_;
    std::shared_ptr<IWidget> mainChild_;
    WidgetsWithInfo children_;
    int xOffset_ = 0;
    int yOffset_ = 0;
};
