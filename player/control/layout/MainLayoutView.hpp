#pragma once

#include "control/common/Widget.hpp"

#include <gtkmm/overlay.h>

class MainLayoutView : public Widget
{
public:
    struct WidgetInfo
    {
        std::shared_ptr<Widget> widget;
        int x;
        int y;
        int z;
    };

    using WidgetsWithInfo = std::vector<WidgetInfo>;

    MainLayoutView(int width, int height);

    void addRegion(const std::shared_ptr<Widget>& child, int x, int y, int z);
    void reorderRegion(const std::shared_ptr<Widget>& child, int z);
    void addBackground(const std::shared_ptr<Widget>& mainChild);
    bool hasBackground() const;

    void showAll() override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Overlay& get() override;

private:
    void sortRegionsByZindex();
    void reorderRegions();
    bool onGetRegionPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);
    WidgetsWithInfo::iterator findRegion(Gtk::Widget* widget);
    void scaleRegions(double scaleX, double scaleY);
    void updateOffsets();

private:
    Gtk::Overlay m_handler;
    std::shared_ptr<Widget> m_mainChild;
    WidgetsWithInfo m_regions;
    int m_xOffset = 0;
    int m_yOffset = 0;

};
