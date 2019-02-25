#pragma once

#include "IImageAdaptor.hpp"
#include "GtkAdaptor.hpp"

#include <gtkmm/image.h>

class GtkImageAdaptor : public GtkAdaptor<IImageAdaptor>
{
public:
    GtkImageAdaptor();

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    void setColor(uint32_t hex_color_number) override;
    void loadImage(const Uri& uri, bool preserveAspectRatio) override;

    Gtk::Image& get() override;

private:
    Glib::RefPtr<const Gdk::Pixbuf> pixbuf() const;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf();

private:
    Gtk::Image m_handler;

};
