#pragma once

#include "IImageAdaptor.hpp"

#include <gtkmm/image.h>

class GtkImageAdaptor : public IImageAdaptor
{
public:
    GtkImageAdaptor();

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    void setColor(uint32_t hex_color_number) override;
    void setImage(const uint8_t* imageData, size_t dataSize) override;
    void show() override;
    Gtk::Image& get();

private:
    Gtk::Image m_handler;
    Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;

};
