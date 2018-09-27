#include "Background.hpp"
#include "constants.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "utils/ColorToHexConverter.hpp"

#include <cassert>

Background::Background(std::unique_ptr<IImageAdaptor>&& handler) :
    m_handler(std::move(handler))
{
}

int Background::width() const
{
    return m_handler->width();
}

int Background::height() const
{
    return m_handler->height();
}

void Background::setSize(int width, int height)
{
    checkBackgroundNewSize(width, height);

    m_handler->setSize(width, height);
}

void Background::checkBackgroundNewSize(int width, int height)
{
    if(width < MIN_DISPLAY_WIDTH || width > MAX_DISPLAY_WIDTH || height < MIN_DISPLAY_HEIGHT || height > MAX_DISPLAY_HEIGHT)
        throw std::invalid_argument("Width or height is too small/large");
}

void Background::setColor(const std::string& hexColor)
{
    uint32_t hexColorNumber = colorToHexNumber(hexColor);
    m_handler->setColor(hexColorNumber);
}

uint32_t Background::colorToHexNumber(const std::string& color) const
{
    ColorToHexConverter converter;
    return converter.colorToHex(color);
}

void Background::setImage(const uint8_t* imageData, size_t dataSize)
{
    assert(imageData);

    m_handler->setImage(imageData, dataSize);
}

void Background::show()
{
    m_handler->show();
}

IImageAdaptor& Background::handler()
{
    return *m_handler;
}
