#include "Background.hpp"
#include "constants.hpp"
#include "utils/utilities.hpp"

#include "GtkImageWrapper.hpp"

#include <boost/filesystem/operations.hpp>
#include <spdlog/spdlog.h>

std::shared_ptr<IBackground> Background::create_one_color(const std::string& hex_color, int width, int height)
{
    auto background = new Background(width, height, std::make_shared<GtkImageWrapper>());
    auto background_ptr = std::shared_ptr<Background>(background);
    background->set_color(hex_color);
    return background_ptr;
}

std::shared_ptr<IBackground> Background::create_with_image(const std::string& image_path, int width, int height)
{
    auto background = new Background(width, height, std::make_shared<GtkImageWrapper>());
    auto background_ptr = std::shared_ptr<Background>(background);
    background->set_image((utils::resources_dir() / image_path).string());
    return background_ptr;
}

Background::Background(int width, int height, std::shared_ptr<IImageWrapper> handler)
{
    m_handler = handler;
    set_size(width, height);
}

int Background::width() const
{
    return m_handler->width();
}

int Background::height() const
{
    return m_handler->height();
}

void Background::set_size(int width, int height)
{
    m_handler->set_size(width, height);
}

uint32_t Background::hex_color_number() const
{
    return color_to_hex_number(m_hex_color);
}

const std::string& Background::hex_color() const
{
    return m_hex_color;
}

void Background::set_color(const std::string& hex_color)
{
    m_hex_color = hex_color;
    uint32_t hex_color_number = color_to_hex_number(hex_color);
    m_handler->set_color(hex_color_number);
}

void Background::set_image(const std::string& image_path)
{
    if(!boost::filesystem::exists(image_path))
        throw std::runtime_error("Path doesn't exist");

    m_handler->set_image(image_path);
}

void Background::show()
{
    m_handler->show();
}

IImageWrapper& Background::handler()
{
    return *m_handler;
}

uint32_t Background::color_to_hex_number(const std::string& hex_color) const
{
    // remove '#' sign at the beginning
    std::string str_hex = hex_color.substr(1);

    // convert 3-digit hex to 6-digit hex
    if(str_hex.size() == 3)
        str_hex = std::string(2, str_hex[0]) + std::string(2, str_hex[1]) + std::string(2, str_hex[2]);
    // add default alpha channel
    if(str_hex.size() == 6)
        str_hex += "FF";

    return static_cast<uint32_t>(std::stoul(str_hex, nullptr, 16));
}
