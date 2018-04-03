#include "Image.hpp"
#include "control/Region.hpp"

Image::Image(const Region& region, int id, int duration, const std::string& uri,
             const std::string& scale_type, const std::string& align, const std::string& valign) :
    Media(region, id, duration, Render::Native, uri),
    m_scale_type(to_scale_type(scale_type)), m_align(to_align(align)), m_valign(to_valign(valign))
{
    auto pixbuf = Gdk::Pixbuf::create_from_file(m_uri, region.size().width, region.size().height);
    m_handler.set(pixbuf);

    region.request_handler().connect([=]{
        handler_requested().emit(m_handler, DEFAULT_X_POS, DEFAULT_Y_POS);
    });
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

void Image::stop()
{
    m_handler.hide();
    Media::stop();
}

void Image::start()
{
    m_handler.show();
    Media::start();
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
