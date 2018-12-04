#pragma once

#include <memory>

class IBackground;
class IRegion;
class IOverlayAdaptor;

class IMainLayout
{
public:
    virtual ~IMainLayout() = default;

    virtual void scale(double scaleX, double scaleY) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void setBackground(std::unique_ptr<IBackground>&& background) = 0;
    virtual void addRegion(std::unique_ptr<IRegion>&& region, int x, int y) = 0;

    virtual IOverlayAdaptor& handler() = 0;
    virtual void show() = 0;

};
