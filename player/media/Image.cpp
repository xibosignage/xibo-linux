#include "Image.hpp"

#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <boost/filesystem/operations.hpp>

Image::Image(int id, int width, int height, int duration, const std::string& uri,
             ScaleType scale_type, Align align, Valign valign) :
    Media(id, width, height, duration, Render::Native, uri),
    m_scale_type(scale_type), m_align(align), m_valign(valign)
{
    try
    {
        if(boost::filesystem::exists(uri))
        {
            auto pixbuf = Gdk::Pixbuf::create_from_file(uri, width, height, is_scaled());
            m_actual_width = pixbuf->get_width();
            m_actual_height = pixbuf->get_height();
            m_handler.set(pixbuf);
        }
        else
        {
            spdlog::get(LOGGER)->error("Could not find image: {}", uri);
        }
    }
    catch(const Gdk::PixbufError& error)
    {
        spdlog::get(LOGGER)->error("Image creation error: {}", std::string{error.what()});
    }
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

void Image::set_size(int width, int height)
{
    Media::set_size(width, height);

    auto new_pixbuf = m_handler.get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_actual_width = new_pixbuf->get_width();
    m_actual_height = new_pixbuf->get_height();
    m_handler.set(new_pixbuf);
}

void Image::request_handler()
{
    handler_requested().emit(m_handler, get_left_pos(), get_top_pos());
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

bool Image::is_scaled() const
{
    if(m_scale_type == ScaleType::Scaled)
        return true;
    return false;
}

int Image::get_left_pos() const
{
    switch(m_align)
    {
        case Align::Center:
            return (width() - m_actual_width) / 2;
        case Align::Left:
            return DEFAULT_LEFT_POS;
        case Align::Right:
            return width() - m_actual_width;
        default:
            break;
    }
    return INVALID_POS;
}

int Image::get_top_pos() const
{
    switch(m_valign)
    {
        case Valign::Middle:
            return (height() - m_actual_height) / 2;
        case Valign::Top:
            return DEFAULT_TOP_POS;
        case Valign::Bottom:
            return height() - m_actual_height;
        default:
            break;
    }
    return INVALID_POS;
}
