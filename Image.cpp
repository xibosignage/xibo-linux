#include "Image.hpp"
#include "Region.hpp"

Image::Image(const std::string& fileName) :
    m_fileName(fileName)
{
}

void Image::init(Region* region, const Point& pos, const Size& size, int zindex)
{
    Media::init(region, pos, size, zindex);

    auto pixbuf = Gdk::Pixbuf::create_from_file(m_fileName, size.width, size.height);
    m_handler.set(pixbuf);
    region->add(m_handler);
    m_handler.show();
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
    return m_fileName;
}
