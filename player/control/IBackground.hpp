#pragma once

#include <string>
#include <gtkmm/widget.h>

class IBackground
{
public:
    virtual void set_color(uint32_t) = 0;
    virtual void set_image(const std::string& image_path) = 0;
    virtual void show() = 0;
    virtual operator Gtk::Widget&() = 0;
};
