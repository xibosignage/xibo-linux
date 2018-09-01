#pragma once

#include <memory>

class IBackground;
class IRegion;
class IOverlayWrapper;

class IMainLayout
{
public:
    virtual ~IMainLayout() = default;

    virtual void set_size(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void set_background(std::shared_ptr<IBackground> background) = 0;
    virtual IBackground& background() = 0;

    virtual void add_region(std::shared_ptr<IRegion> region) = 0;
    virtual IRegion& region(size_t index) = 0;
    virtual size_t regions_count() const = 0;

    virtual IOverlayWrapper& handler() = 0;
    virtual void show() = 0;

};
