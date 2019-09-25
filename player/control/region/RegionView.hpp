#pragma once

#include "IRegionView.hpp"
#include "control/common/Widget.hpp"

#include <gtkmm/fixed.h>

class RegionView : public Widget<IRegionView>
{
public:
    RegionView(int width, int height);

    void addMedia(const std::shared_ptr<IWidget>& child, int left, int top) override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Fixed& get() override;

private:
    void scaleMedia(double scaleX, double scaleY);

private:
    Gtk::Fixed m_handler;
    std::vector<std::shared_ptr<IWidget>> m_media;
};
