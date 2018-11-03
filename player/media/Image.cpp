#include "Image.hpp"

#include "constants.hpp"
#include "adaptors/IImageAdaptor.hpp"
#include "media/MediaVisitor.hpp"

#include <cassert>

Image::Image(int id, int width, int height, const FilePath& path, ImageProperties props, std::unique_ptr<IImageAdaptor>&& handler) :
    Media(id), m_handler(std::move(handler)), m_scaleType(props.scaleType), m_align(props.align), m_valign(props.valign)
{
    assert(m_handler);

    m_handler->setSize(width, height);
    loadImage(path);
}

void Image::doStop()
{
    m_handler->hide();
}

void Image::doStart()
{
    m_handler->show();
}

void Image::loadImage(const FilePath& path)
{
    bool preserveAspectRatio = (m_scaleType == ImageProperties::ScaleType::Scaled) ? true : false;
    m_handler->loadImage(path, preserveAspectRatio);
}

int Image::width() const
{
    return m_handler->width();
}

int Image::height() const
{
    return m_handler->height();
}

void Image::scale(double scaleX, double scaleY)
{
    m_handler->scale(scaleX, scaleY);
}

IWidgetAdaptor& Image::handler()
{
    return *m_handler;
}

void Image::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
}

ImageProperties::ScaleType Image::scaleType() const
{
    return m_scaleType;
}

ImageProperties::Align Image::align() const
{
    return m_align;
}

ImageProperties::Valign Image::valign() const
{
    return m_valign;
}
