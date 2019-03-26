#pragma once

#include "Widget.hpp"

#include <gtkmm/overlay.h>

class OverlayLayout : public Widget
{
public:
    struct WidgetInfo
    {
        int width;
        int height;
        int x;
        int y;
    };

    using WidgetsWithInfo = std::map<std::shared_ptr<Widget>, WidgetInfo>;

    OverlayLayout(int width, int height);

    void addWidget(const std::shared_ptr<Widget>& child, int x, int y, int z);
    void removeWidget(const std::shared_ptr<Widget>& child);
    void removeWidgets();
    void addMainWidget(const std::shared_ptr<Widget>& mainChild);
    void removeMainWidget();
    bool hasMainWidget() const;
    void reorderWidget(Widget& child, int position);

    void showAll() override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Overlay& get() override;

private:
    bool onGetWidgetPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc);
    WidgetsWithInfo::iterator findWidget(Gtk::Widget* widget);
    void scaleWidgets(double scaleX, double scaleY);
    void updateOffsets();

private:
    Gtk::Overlay m_handler;
    std::shared_ptr<Widget> m_mainChild;
    WidgetsWithInfo m_children;
    int m_xOffset = 0;
    int m_yOffset = 0;

};
