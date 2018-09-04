#include "GtkImageAdaptor.hpp"

#include <gdkmm/pixbufloader.h>

const int DEFAULT_WIDTH = 1;
const int DEFAULT_HEIGHT = 1;

GtkImageAdaptor::GtkImageAdaptor()
{
    m_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

int GtkImageAdaptor::width() const
{
    return m_pixbuf->get_width();
}

int GtkImageAdaptor::height() const
{
    return m_pixbuf->get_height();
}

void GtkImageAdaptor::setSize(int width, int height)
{
    m_pixbuf = m_pixbuf->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_handler.set(m_pixbuf);
}

void GtkImageAdaptor::setColor(uint32_t hex_color_number)
{
    m_pixbuf->fill(hex_color_number);
}

void GtkImageAdaptor::setImage(const uint8_t* imageData, size_t dataSize)
{
    auto pixbuf = Gdk::PixbufLoader::create();
    pixbuf->write(imageData, dataSize);
    pixbuf->close();
    m_handler.set(pixbuf->get_pixbuf());
}

void GtkImageAdaptor::show()
{
    m_handler.show();
}

Gtk::Image& GtkImageAdaptor::get()
{
    return m_handler;
}
