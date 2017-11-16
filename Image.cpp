#include "Image.hpp"
#include "Region.hpp"

Image::Image(const std::string& filename) :
    m_filename(filename)
{
}

void Image::init(MyRegion* region, const Point& pos, const Size& size, int zindex)
{
    Media::init(region, pos, size, zindex);

    auto pixbuf = Gdk::Pixbuf::create_from_file(m_filename, size.width, size.height);
    m_handler.set(pixbuf);
    m_handler.show();

    region->add(m_handler);
}

void Image::hide()
{
    m_handler.hide();
}

void Image::show()
{
    m_handler.show();
}

std::string Image::get_filename() const
{
    return m_filename;
}
