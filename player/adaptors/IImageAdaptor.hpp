#pragma once

#include "IWidgetAdaptor.hpp"
#include <cstdint>

class Uri;

class IImageAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IImageAdaptor() = default;
    virtual void setColor(uint32_t hexColorNumber) = 0;
    virtual void loadImage(const Uri& uri, bool preserveAspectRatio) = 0;
};
