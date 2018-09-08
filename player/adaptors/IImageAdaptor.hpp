#pragma once

#include "IWidgetAdaptor.hpp"

#include <string>

class IImageAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IImageAdaptor() = default;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;
    virtual void setColor(uint32_t hexColorNumber) = 0;
    virtual void setImage(const uint8_t* imageData, size_t dataSize) = 0;
    virtual void show() = 0;
};
