#pragma once

#include "IWidgetAdaptor.hpp"
#include "constants.hpp"

class Uri;

class IWebViewAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IWebViewAdaptor() = default;
    virtual void reload() = 0;
    virtual void load(const Uri& uri) = 0;
    virtual void enableTransparency() = 0;
};
