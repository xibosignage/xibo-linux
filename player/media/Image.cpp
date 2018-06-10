#include "Image.hpp"

#include <spdlog/spdlog.h>
#include <boost/filesystem/operations.hpp>

Image::Image(int id, const Size& size, int duration, const std::string& uri,
             ScaleType scale_type, Align align, Valign valign) :
    Media(id, size, duration, Render::Native, uri),
    m_scale_type(scale_type), m_align(align), m_valign(valign)
{
    try
    {
        if(boost::filesystem::exists(uri))
        {
            auto pixbuf = Gdk::Pixbuf::create_from_file(uri, size.width, size.height, is_scaled());
            m_actual_size.width = pixbuf->get_width();
            m_actual_size.height = pixbuf->get_height();
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
    m_actual_size.width = new_pixbuf->get_width();
    m_actual_size.height = new_pixbuf->get_height();
    m_handler.set(new_pixbuf);
}

void Image::request_handler()
{
    Point pos{get_left_pos(), get_top_pos()};
    handler_requested().emit(m_handler, pos);
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
            return (size().width - m_actual_size.width) / 2;
        case Align::Left:
            return DEFAULT_LEFT_POS;
        case Align::Right:
            return size().width - m_actual_size.width;
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
            return (size().height - m_actual_size.height) / 2;
        case Valign::Top:
            return DEFAULT_TOP_POS;
        case Valign::Bottom:
            return size().height - m_actual_size.height;
        default:
            break;
    }
    return INVALID_POS;
}
