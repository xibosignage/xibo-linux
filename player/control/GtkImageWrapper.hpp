#pragma once

#include "IImageWrapper.hpp"

#include <gtkmm/image.h>

class GtkImageWrapper : public IImageWrapper
{
public:
    GtkImageWrapper() = default;

    int width() const override
    {
        return m_width;
    }
    int height() const override
    {
        return m_height;
    }
    void setSize(int width, int height) override
    {
        m_width = width;
        m_height = height;
//        auto pixbuf = m_handler.get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
//        m_handler.set(pixbuf);
    }
    void setColor(uint32_t hex_color_number) override
    {
        auto pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, m_width, m_height);
        pixbuf->fill(hex_color_number);
        m_handler.set(pixbuf);
    }
    void setImage(const std::string& image_path) override
    {
        auto pixbuf = Gdk::Pixbuf::create_from_file(image_path, m_width, m_height);
        m_handler.set(pixbuf);
    }
    void show() override
    {
        m_handler.show();
    }
    Gtk::Image& get()
    {
        return m_handler;
    }

private:
    Gtk::Image m_handler;
    int m_width = 0;
    int m_height = 0;

};
