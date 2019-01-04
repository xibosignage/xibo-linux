#pragma once

#include "IWidgetAdaptor.hpp"
#include "constants.hpp"

#include <string>

class IImageAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IImageAdaptor() = default;
    virtual void setColor(uint32_t hexColorNumber) = 0;
    virtual void loadImage(const FilePath& path, bool preserveAspectRatio) = 0;
};
