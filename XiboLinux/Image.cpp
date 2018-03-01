#include "Image.hpp"

Image::Image(const Size& size, int id, int duration, bool use_duration, const std::string& uri,
             const std::string& scale_type, const std::string& align, const std::string& valign) :
    Media(id, duration, use_duration, Render::Native, uri),
          m_scale_type(to_scale_type(scale_type)), m_align(to_align(align)), m_valign(to_valign(valign))
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

Image::ScaleType Image::to_scale_type(const std::string& scale_type)
{
    if(scale_type == "center")
        return Image::ScaleType::Center;
    else if(scale_type == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align Image::to_align(const std::string& align)
{
    if(align == "left")
        return Image::Align::Left;
    else if(align == "center")
        return Image::Align::Center;
    else if(align == "right")
        return Image::Align::Right;
    else
        return Image::Align::Invalid;
}

Image::Valign Image::to_valign(const std::string& valign)
{
    if(valign == "top")
        return Image::Valign::Top;
    else if(valign == "middle")
        return Image::Valign::Middle;
    else if(valign == "bottom")
        return Image::Valign::Bottom;
    else
        return Image::Valign::Invalid;
}
