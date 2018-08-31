#include "Background.hpp"
#include "constants.hpp"

#include <boost/filesystem/operations.hpp>
#include <spdlog/spdlog.h>

Background::Background(int width, int height) :
    m_width(width),
    m_height(height)
{
}

void Background::set_color(uint32_t hex_color)
{
    try
    {
        auto pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, m_width, m_height);
        pixbuf->fill(hex_color);
        m_handler.set(pixbuf);
    }
    catch(const Gdk::PixbufError& error)
    {
        spdlog::get(LOGGER)->error("Could set background color: {}", std::string{error.what()});
    }
}

void Background::set_image(const std::string& image_path)
{
    if(boost::filesystem::exists(image_path))
    {
        try
        {
            auto pixbuf = Gdk::Pixbuf::create_from_file(image_path, m_width, m_height);
            m_handler.set(pixbuf);
        }
        catch(const Gdk::PixbufError& error)
        {
            spdlog::get(LOGGER)->error("Could set background image: {}", std::string{error.what()});
        }
    }
    else
    {
        spdlog::get(LOGGER)->warn("Background image doesn't exists");
    }
}

void Background::set_size(int width, int height)
{
    if(width != m_width || height != m_height)
    {
        auto pixbuf = m_handler.get_pixbuf()->scale_simple(width, height, Gdk::InterpType::INTERP_BILINEAR);
        m_handler.set(pixbuf);
    }
}

void Background::show()
{
    m_handler.show();
}

Gtk::Image& Background::handler()
{
    return m_handler;
}
