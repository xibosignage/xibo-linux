#include "GtkImageAdaptor.hpp"

#include "utils/Uri.hpp"

const int DEFAULT_WIDTH = 1;
const int DEFAULT_HEIGHT = 1;

GtkImageAdaptor::GtkImageAdaptor() :
    GtkAdaptor<IImageAdaptor>(m_handler)
{
    m_handler.set(Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, DEFAULT_WIDTH, DEFAULT_HEIGHT));
}

int GtkImageAdaptor::width() const
{
    return pixbuf()->get_width();
}

int GtkImageAdaptor::height() const
{
    return pixbuf()->get_height();
}

void GtkImageAdaptor::setSize(int width, int height)
{
    auto newPixbuf = pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_handler.set(newPixbuf);
}

void GtkImageAdaptor::setColor(uint32_t hex_color_number)
{
    pixbuf()->fill(hex_color_number);
}

void GtkImageAdaptor::loadImage(const Uri& uri, bool preserveAspectRatio)
{
    m_handler.set(Gdk::Pixbuf::create_from_file(uri.path(), width(), height(), preserveAspectRatio));
}

Gtk::Image& GtkImageAdaptor::get()
{
    return m_handler;
}

Glib::RefPtr<const Gdk::Pixbuf> GtkImageAdaptor::pixbuf() const
{
    return m_handler.get_pixbuf();
}

Glib::RefPtr<Gdk::Pixbuf> GtkImageAdaptor::pixbuf()
{
    return m_handler.get_pixbuf();
}
