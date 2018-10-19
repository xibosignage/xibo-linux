#pragma once

#include "IWidgetAdaptor.hpp"

#include <string>

class IImageAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IImageAdaptor() = default;
    virtual void setColor(uint32_t hexColorNumber) = 0;
    virtual void setImage(const std::string& path) = 0;
};
