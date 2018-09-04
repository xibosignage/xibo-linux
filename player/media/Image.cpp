#include "Image.hpp"

#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <filesystem>

Image::Image(int id, int width, int height, int duration, const std::string& uri,
             ScaleType scaleType, Align align, Valign valign) :
    Media(id, width, height, duration, Render::Native, uri),
    m_scaleType(scaleType), m_align(align), m_valign(valign)
{
    try
    {
        if(std::filesystem::exists(uri))
        {
            auto pixbuf = Gdk::Pixbuf::create_from_file(uri, width, height, isScaled());
            m_actualWidth = pixbuf->get_width();
            m_actualHeight = pixbuf->get_height();
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

void Image::setSize(int width, int height)
{
    Media::setSize(width, height);

    auto new_pixbuf = m_handler.get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_actualWidth = new_pixbuf->get_width();
    m_actualHeight = new_pixbuf->get_height();
    m_handler.set(new_pixbuf);
}

void Image::requestHandler()
{
    handlerRequested().emit(m_handler, getLeftPos(), getTopPos());
}

Image::ScaleType Image::scaleType() const
{
    return m_scaleType;
}

Image::Align Image::align() const
{
    return m_align;
}

Image::Valign Image::valign() const
{
    return m_valign;
}

bool Image::isScaled() const
{
    if(m_scaleType == ScaleType::Scaled)
        return true;
    return false;
}

int Image::getLeftPos() const
{
    switch(m_align)
    {
        case Align::Center:
            return (width() - m_actualWidth) / 2;
        case Align::Left:
            return DEFAULT_LEFT_POS;
        case Align::Right:
            return width() - m_actualWidth;
        default:
            break;
    }
    return INVALID_POS;
}

int Image::getTopPos() const
{
    switch(m_valign)
    {
        case Valign::Middle:
            return (height() - m_actualHeight) / 2;
        case Valign::Top:
            return DEFAULT_TOP_POS;
        case Valign::Bottom:
            return height() - m_actualHeight;
        default:
            break;
    }
    return INVALID_POS;
}
