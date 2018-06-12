#pragma once

#include "IBackground.hpp"
#include <gtkmm/image.h>

class Background : public IBackground
{
public:
    Background(int width, int height);

    void set_color(uint32_t hex_color) override;
    void set_image(const std::string& image_path) override;
    void set_size(int width, int height) override;
    void show() override;
    operator Gtk::Image&() override;

private:
    Gtk::Image m_handler;
    int m_width;
    int m_height;
};
