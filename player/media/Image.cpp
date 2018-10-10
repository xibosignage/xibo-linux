#include "Image.hpp"

#include "constants.hpp"
#include "adaptors/IImageAdaptor.hpp"
#include "media/MediaVisitor.hpp"

#include <cassert>

Image::Image(Image::ScaleType type, Image::Align align, Image::Valign valign, std::unique_ptr<IImageAdaptor>&& handler) :
    m_handler(std::move(handler)), m_scaleType(type), m_align(align), m_valign(valign)
{
}

// FIXME change name after adding Visible/Invisible media
void Image::doStop()
{
    m_handler->hide();
}

// FIXME change name after adding Visible/Invisible media
void Image::doStart()
{
    m_handler->show();
}

int Image::width() const
{
    return m_handler->width();
}

int Image::height() const
{
    return m_handler->height();
}

IWidgetAdaptor& Image::handler()
{
    return *m_handler;
}

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
