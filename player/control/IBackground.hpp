#pragma once

#include <string>

class IImageWrapper;

class IBackground
{
public:
    virtual ~IBackground() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;

    virtual uint32_t hexColorNumber() const = 0;
    virtual const std::string& hexColor() const = 0;
    virtual void setColor(const std::string& hexColor) = 0;

    virtual void setImage(const std::string& imagePath) = 0;
    virtual void show() = 0;
    virtual IImageWrapper& handler() = 0;
};
