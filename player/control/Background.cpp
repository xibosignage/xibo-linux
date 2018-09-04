#include "Background.hpp"
#include "constants.hpp"

#include "adaptors/GtkImageAdaptor.hpp"

#include <regex>

const int SHORT_COLOR_WITHOUT_ALPHA = 3;
const int COLOR_WITHOUT_ALPHA = 6;
const int COLOR_BASE = 16;
const std::string DEFAULT_ALPHA_CHANNEL = "FF";

Background::Background(int width, int height) :
    Background(width, height, std::make_unique<GtkImageAdaptor>())
{

}

Background::Background(int width, int height, std::unique_ptr<IImageAdaptor> handler) :
    m_handler(std::move(handler))
{
    setSize(width, height);
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
    if(width < MIN_DISPLAY_WIDTH || width > MAX_DISPLAY_WIDTH || height < MIN_DISPLAY_HEIGHT || height > MAX_DISPLAY_HEIGHT)
        throw std::runtime_error("Width or height is too small/large");

    m_handler->setSize(width, height);
}

uint32_t Background::hexColorNumber() const
{
    return colorToHexNumber(hexColor());
}

const std::string& Background::hexColor() const
{
    if(m_hexColor.empty())
        throw std::runtime_error("Color doesn't exist");

    return m_hexColor;
}

void Background::setColor(const std::string& hexColor)
{
    uint32_t hexColorNumber = colorToHexNumber(hexColor);
    m_handler->setColor(hexColorNumber);
    m_hexColor = hexColor;
}

void Background::setImage(const uint8_t* imageData, size_t dataSize)
{
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

uint32_t Background::colorToHexNumber(const std::string& hexColor) const
{
    std::smatch match;
    std::regex hexColorRegex("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3}|[A-Fa-f0-9]{8})$");
    if(!std::regex_match(hexColor, match, hexColorRegex))
        throw std::invalid_argument("HEX color should be 3, 6, or 8 digits with # at the beginning");

    std::string colorWithoutNumberSign = match[1];

    if(colorWithoutNumberSign.size() == SHORT_COLOR_WITHOUT_ALPHA)
        colorWithoutNumberSign = std::string(2, colorWithoutNumberSign[0]) +
                                 std::string(2, colorWithoutNumberSign[1]) +
                                 std::string(2, colorWithoutNumberSign[2]);

    if(colorWithoutNumberSign.size() == COLOR_WITHOUT_ALPHA)
        colorWithoutNumberSign += DEFAULT_ALPHA_CHANNEL;

    return static_cast<uint32_t>(std::stoul(colorWithoutNumberSign, nullptr, COLOR_BASE));
}
