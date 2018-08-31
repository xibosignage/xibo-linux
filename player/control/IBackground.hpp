#pragma once

#include <boost/filesystem/path.hpp>
#include <gtkmm/image.h>

class IBackground
{
public:
    virtual ~IBackground() = default;
    virtual void set_color(uint32_t) = 0;
    virtual void set_image(const std::string& image_path) = 0;
    virtual void set_size(int width, int height) = 0;
    virtual void show() = 0;
    virtual Gtk::Image& handler() = 0;
};
