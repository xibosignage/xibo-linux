#pragma once

#include "IWidgetAdaptor.hpp"
#include "constants.hpp"

class IWebViewAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IWebViewAdaptor() = default;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual void reload() = 0;
    virtual void load(const FilePath& page) = 0;
    virtual void enableTransparency() = 0;

    virtual void scale(double scaleX, double scaleY) = 0;
    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
};
