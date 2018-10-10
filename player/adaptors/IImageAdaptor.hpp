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
    virtual void scale(double scaleX, double scaleY) = 0;
    virtual void setColor(uint32_t hexColorNumber) = 0;
    virtual void setImage(const std::string& path) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
};
