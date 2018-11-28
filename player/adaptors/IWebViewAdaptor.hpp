#pragma once

#include "IWidgetAdaptor.hpp"
#include "constants.hpp"

class FilePath;

class IWebViewAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IWebViewAdaptor() = default;
    virtual void reload() = 0;
    virtual void load(const FilePath& page) = 0;
    virtual void enableTransparency() = 0;
};
