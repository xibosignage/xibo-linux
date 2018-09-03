#include "GtkImageAdaptor.hpp"

GtkImageAdaptor::GtkImageAdaptor(int width, int height)
{
    m_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
}

int GtkImageAdaptor::width() const
{
    return get_pixbuf()->get_width();
}

int GtkImageAdaptor::height() const
{
    return get_pixbuf()->get_height();
}

void GtkImageAdaptor::setSize(int width, int height)
{
    m_pixbuf = get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_handler.set(m_pixbuf);
}

void GtkImageAdaptor::setColor(uint32_t hex_color_number)
{
    m_pixbuf->fill(hex_color_number);
}

void GtkImageAdaptor::setImage(const std::string& image_path)
{
    m_pixbuf = Gdk::Pixbuf::create_from_file(image_path, width(), height());
    m_handler.set(m_pixbuf);
}

void GtkImageAdaptor::show()
{
    m_handler.show();
}

Gtk::Image& GtkImageAdaptor::get()
{
    return m_handler;
}

Glib::RefPtr<Gdk::Pixbuf> GtkImageAdaptor::get_pixbuf() const
{
    return m_pixbuf;
}
