#include "Image.hpp"
#include "control/Region.hpp"

#include <spdlog/spdlog.h>

Image::Image(const Region& region, int id, int duration, const std::string& uri,
             ScaleType scale_type, Align align, Valign valign) :
    Media(region, id, duration, Render::Native, uri)
{
    bool scaled = is_scaled(scale_type);
    auto pixbuf = Gdk::Pixbuf::create_from_file(m_uri, region.size().width, region.size().height, scaled);
    m_size.width = pixbuf->get_width();
    m_size.height = pixbuf->get_height();
    m_handler.set(pixbuf);

    Point pos{get_left_pos(align), get_top_pos(valign)};
    region.request_handler().connect([=]{
        handler_requested().emit(m_handler, pos);
    });
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

bool Image::is_scaled(ScaleType scale_type)
{
    if(scale_type == ScaleType::Scaled)
        return true;
    return false;
}

int Image::get_left_pos(Align align)
{
    switch(align)
    {
        case Align::Center:
            return (region().size().width - m_size.width) / 2;
        case Align::Left:
            return DEFAULT_LEFT_POS;
        case Align::Right:
            return region().size().width - m_size.width;
        default:
            break;
    }
    return INVALID_POS;
}

int Image::get_top_pos(Valign valign)
{
    switch(valign)
    {
        case Valign::Middle:
            return (region().size().height - m_size.height) / 2;
        case Valign::Top:
            return DEFAULT_TOP_POS;
        case Valign::Bottom:
            return region().size().height - m_size.height;
        default:
            break;
    }
    return INVALID_POS;
}
