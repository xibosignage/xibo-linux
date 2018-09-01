#pragma once

#include <string>

class IImageWrapper;

class IBackground
{
public:
    virtual ~IBackground() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void set_size(int width, int height) = 0;

    virtual uint32_t hex_color_number() const = 0;
    virtual const std::string& hex_color() const = 0;
    virtual void set_color(const std::string& hex_color) = 0;

    virtual void set_image(const std::string& image_path) = 0;
    virtual void show() = 0;
    virtual IImageWrapper& handler() = 0;
};
