#pragma once

#include "IFixedLayoutAdaptor.hpp"
#include "GtkAdaptor.hpp"

#include <gtkmm/fixed.h>

class GtkFixedLayoutAdaptor : public GtkAdaptor<IFixedLayoutAdaptor>
{
public:
    GtkFixedLayoutAdaptor();

    void addChild(IWidgetAdaptor& child, int left, int top) override;
    void removeChildren() override;
    void scale(double scaleX, double scaleY) override;

    Gtk::Fixed& get() override;

private:
    void scaleChildren(double scaleX, double scaleY);

private:
    Gtk::Fixed m_handler;

};
