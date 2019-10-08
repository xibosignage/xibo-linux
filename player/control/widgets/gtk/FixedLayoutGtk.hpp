#pragma once

#include "control/widgets/FixedLayout.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/fixed.h>

class FixedLayoutGtk : public WidgetGtk<Xibo::FixedLayout>
{
public:
    FixedLayoutGtk(int width, int height);

    void addChild(const std::shared_ptr<Xibo::Widget>& child, int left, int top) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Fixed& get() override;

private:
    void scaleMedia(double scaleX, double scaleY);

private:
    Gtk::Fixed handler_;
    std::vector<std::shared_ptr<Xibo::Widget>> media_;
};
