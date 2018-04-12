#include "Image.hpp"
#include "control/Region.hpp"

Image::Image(const Region& region, int id, int duration, const std::string& uri,
             ScaleType scale_type, Align align, Valign valign) :
    Media(region, id, duration, Render::Native, uri),
    m_scale_type(scale_type), m_align(align), m_valign(valign)
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
