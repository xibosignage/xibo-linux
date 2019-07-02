#pragma once

#include <gtkmm/widget.h>

class IGtkWidget
{
public:
    virtual ~IGtkWidget() = default;
    virtual Gtk::Widget& get() = 0;
};
