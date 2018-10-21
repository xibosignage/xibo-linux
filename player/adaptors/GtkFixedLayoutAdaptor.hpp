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

    Gtk::Fixed& get() override;

private:
    Gtk::Fixed m_handler;

};
