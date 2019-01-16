#include "Background.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include <cassert>

const bool DEFAULT_BACKGROUND_SCALED = true;

OneColorBackground::OneColorBackground(int width, int height, uint32_t color, std::unique_ptr<IImageAdaptor>&& handler) :
    Background(width, height, std::move(handler))
{
    this->handler().setColor(color);
}

ImageBackground::ImageBackground(int width, int height, const FilePath& path, std::unique_ptr<IImageAdaptor>&& handler) :
    Background(width, height, std::move(handler))
{
    this->handler().loadImage(path, DEFAULT_BACKGROUND_SCALED);
}

Background::Background(int width, int height, std::unique_ptr<IImageAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
}

int Background::width() const
{
    return m_handler->width();
}

int Background::height() const
{
    return m_handler->height();
}

void Background::scale(double scaleX, double scaleY)
{
    m_handler->scale(scaleX, scaleY);
}

void Background::show()
{
    m_handler->show();
}

IImageAdaptor& Background::handler()
{
    return *m_handler;
}
