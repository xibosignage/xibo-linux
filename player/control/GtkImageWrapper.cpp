#include "GtkImageWrapper.hpp"

GtkImageWrapper::GtkImageWrapper(int width, int height)
{
    m_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, width, height);
}

int GtkImageWrapper::width() const
{
    return get_pixbuf()->get_width();
}

int GtkImageWrapper::height() const
{
    return get_pixbuf()->get_height();
}

void GtkImageWrapper::setSize(int width, int height)
{
    m_pixbuf = get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_handler.set(m_pixbuf);
}

void GtkImageWrapper::setColor(uint32_t hex_color_number)
{
    m_pixbuf->fill(hex_color_number);
}

void GtkImageWrapper::setImage(const std::string& image_path)
{
    m_pixbuf = Gdk::Pixbuf::create_from_file(image_path, width(), height());
    m_handler.set(m_pixbuf);
}

void GtkImageWrapper::show()
{
    m_handler.show();
}

Gtk::Image& GtkImageWrapper::get()
{
    return m_handler;
}

Glib::RefPtr<Gdk::Pixbuf> GtkImageWrapper::get_pixbuf() const
{
    return m_pixbuf;
}
