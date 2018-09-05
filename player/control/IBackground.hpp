#pragma once

#include <string>

class IImageAdaptor;

class IBackground
{
public:
    virtual ~IBackground() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;

    virtual void setColor(const std::string& hexColor) = 0;
    virtual void setImage(const uint8_t* imageData, size_t dataSize) = 0;

    virtual void show() = 0;
    virtual IImageAdaptor& handler() = 0;
};
