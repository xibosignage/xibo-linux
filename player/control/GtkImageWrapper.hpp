#pragma once

#include "IImageWrapper.hpp"

#include <gtkmm/image.h>

class GtkImageWrapper : public IImageWrapper
{
public:
    GtkImageWrapper(int width, int height);

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    void setColor(uint32_t hex_color_number) override;
    void setImage(const std::string& image_path) override;
    void show() override;
    Gtk::Image& get();

private:
    Glib::RefPtr<Gdk::Pixbuf> get_pixbuf() const;

private:
    Gtk::Image m_handler;
    Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;

};
