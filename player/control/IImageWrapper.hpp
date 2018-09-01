#pragma once

#include <string>

class IImageWrapper
{
public:
    virtual ~IImageWrapper() = default;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void set_size(int width, int height) = 0;
    virtual void set_color(uint32_t hex_color_number) = 0;
    virtual void set_image(const std::string& image_path) = 0;
    virtual void show() = 0;
};
