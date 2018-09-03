#pragma once

#include <string>

class IImageAdaptor
{
public:
    virtual ~IImageAdaptor() = default;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;
    virtual void setColor(uint32_t hex_color_number) = 0;
    virtual void setImage(const std::string& image_path) = 0;
    virtual void show() = 0;
};
