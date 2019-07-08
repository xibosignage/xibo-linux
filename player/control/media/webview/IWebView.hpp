#pragma once

#include "control/common/IWidget.hpp"

class Uri;

class IWebView : public IWidget
{
public:
    virtual void reload() = 0;
    virtual void load(const Uri& uri) = 0;
    virtual void enableTransparency() = 0;
};
