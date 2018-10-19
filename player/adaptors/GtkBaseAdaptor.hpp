#pragma once

#include "IWidgetAdaptor.hpp"
#include <gtkmm/widget.h>

class GtkBaseAdaptor
{
public:
    virtual ~GtkBaseAdaptor() = default;
    virtual Gtk::Widget& get() = 0;

protected:
    Gtk::Widget& getHandler(IWidgetAdaptor& child)
    {
        return dynamic_cast<GtkBaseAdaptor&>(child).get();
    }

};
