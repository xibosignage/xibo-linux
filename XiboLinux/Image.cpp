#include "Image.hpp"

#include <spdlog/spdlog.h>

Image::Image(const Size& size, uint id, uint duration, bool use_duration, const std::string& uri, ScaleType scale_type, Align align, Valign valign) :
    Media(id, duration, use_duration, Render::Native, uri), m_scale_type(scale_type), m_align(align), m_valign(valign)
{
    auto pixbuf = Gdk::Pixbuf::create_from_file(m_uri, size.width, size.height);
    m_handler.set(pixbuf);
}

Image::ScaleType Image::scale_type() const
{
    return m_scale_type;
}

Image::Align Image::align() const
{
    return m_align;
}

Image::Valign Image::valign() const
{
    return m_valign;
}

Gtk::Widget& Image::handler()
{
    return m_handler;
}

void Image::hide()
{
    m_handler.hide();
    Media::hide();
}

void Image::show()
{
    m_handler.show();
    Media::show();
}
