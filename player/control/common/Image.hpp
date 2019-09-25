#pragma once

#include "IImage.hpp"
#include "Widget.hpp"

#include <gtkmm/image.h>

class Image : public Widget<IImage>
{
public:
    Image(int width, int height);

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    void setColor(uint32_t hex_color_number) override;
    void loadFromFile(const FilePath& path, bool preserveAspectRatio) override;

    Gtk::Image& get() override;

private:
    Glib::RefPtr<const Gdk::Pixbuf> pixbuf() const;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf();

private:
    Gtk::Image m_handler;
};
