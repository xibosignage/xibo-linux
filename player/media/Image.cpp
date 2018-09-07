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

            Media::setSize(m_actualWidth, m_actualHeight);
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
    auto new_pixbuf = m_handler.get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
    m_actualWidth = new_pixbuf->get_width();
    m_actualHeight = new_pixbuf->get_height();
    m_handler.set(new_pixbuf);

    Media::setSize(m_actualWidth, m_actualHeight);
}

Gtk::Widget& Image::handler()
{
    return m_handler;
}

#include "media/MediaVisitor.hpp"
void Image::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
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
