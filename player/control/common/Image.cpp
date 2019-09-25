#include "Image.hpp"

#include "common/fs/FilePath.hpp"

Image::Image(int width, int height) : Widget(m_handler)
{
    m_handler.set(Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height));
}

int Image::width() const
{
    return pixbuf()->get_width();
}

int Image::height() const
{
    return pixbuf()->get_height();
}

void Image::setSize(int width, int height)
{
    auto newPixbuf = pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_handler.set(newPixbuf);
}

void Image::setColor(uint32_t hex_color_number)
{
    pixbuf()->fill(hex_color_number);
}

void Image::loadFromFile(const FilePath& path, bool preserveAspectRatio)
{
    m_handler.set(Gdk::Pixbuf::create_from_file(path, width(), height(), preserveAspectRatio));
}

Gtk::Image& Image::get()
{
    return m_handler;
}

Glib::RefPtr<const Gdk::Pixbuf> Image::pixbuf() const
{
    return m_handler.get_pixbuf();
}

Glib::RefPtr<Gdk::Pixbuf> Image::pixbuf()
{
    return m_handler.get_pixbuf();
}
