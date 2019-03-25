#pragma once

#include "Widget.hpp"

#include <gtkmm/fixed.h>

class FixedLayout : public Widget
{
public:
    FixedLayout(int width, int height);

    void addWidget(const std::shared_ptr<Widget>& widget, int left, int top);
    Widget& widget(std::size_t index);
    void removeWidgets();

    void scale(double scaleX, double scaleY) override;

    Gtk::Fixed& get() override;

private:
    void scaleChildren(double scaleX, double scaleY);

private:
    Gtk::Fixed m_handler;
    std::vector<std::shared_ptr<Widget>> m_children;

};
