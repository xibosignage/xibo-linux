#include "Background.hpp"
#include "constants.hpp"
#include "utils/utilities.hpp"

#include "GtkImageWrapper.hpp"

#include <boost/filesystem/operations.hpp>
#include <spdlog/spdlog.h>

std::shared_ptr<IBackground> Background::createOneColor(const std::string& hexColor, int width, int height)
{
    auto background = new Background(width, height, std::make_shared<GtkImageWrapper>());
    auto backgroundPtr = std::shared_ptr<Background>(background);
    background->setColor(hexColor);
    return backgroundPtr;
}

std::shared_ptr<IBackground> Background::createWithImage(const std::string& imagePath, int width, int height)
{
    auto background = new Background(width, height, std::make_shared<GtkImageWrapper>());
    auto backgroundPtr = std::shared_ptr<Background>(background);
    background->setImage((utils::resourcesDir() / imagePath).string());
    return backgroundPtr;
}

Background::Background(int width, int height, std::shared_ptr<IImageWrapper> handler)
{
    m_handler = handler;
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
    m_handler->setSize(width, height);
}

uint32_t Background::hexColorNumber() const
{
    return colorToHexNumber(m_hexColor);
}

const std::string& Background::hexColor() const
{
    return m_hexColor;
}

void Background::setColor(const std::string& hexColor)
{
    m_hexColor = hexColor;
    uint32_t hexColorNumber = colorToHexNumber(hexColor);
    m_handler->setColor(hexColorNumber);
}

void Background::setImage(const std::string& imagePath)
{
    if(!boost::filesystem::exists(imagePath))
        throw std::runtime_error("Path doesn't exist");

    m_handler->setImage(imagePath);
}

void Background::show()
{
    m_handler->show();
}

IImageWrapper& Background::handler()
{
    return *m_handler;
}

uint32_t Background::colorToHexNumber(const std::string& hexColor) const
{
    // remove '#' sign at the beginning
    std::string strHex = hexColor.substr(1);

    // convert 3-digit hex to 6-digit hex
    if(strHex.size() == 3)
        strHex = std::string(2, strHex[0]) + std::string(2, strHex[1]) + std::string(2, strHex[2]);
    // add default alpha channel
    if(strHex.size() == 6)
        strHex += "FF";

    return static_cast<uint32_t>(std::stoul(strHex, nullptr, 16));
}
