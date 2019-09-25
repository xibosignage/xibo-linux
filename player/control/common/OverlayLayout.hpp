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
    Gtk::Overlay m_handler;
    std::shared_ptr<IWidget> m_mainChild;
    WidgetsWithInfo m_children;
    int m_xOffset = 0;
    int m_yOffset = 0;
};
