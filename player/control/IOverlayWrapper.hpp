#pragma once

class IBackground;
class IRegion;
class IImageWrapper;
class IFixedLayoutWrapper;

class IOverlayWrapper
{
public:
    virtual ~IOverlayWrapper() = default;
    virtual void show() = 0;
    virtual void set_size(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void add_child(IFixedLayoutWrapper& child, int top, int left, int width, int height) = 0;
    virtual void add(IImageWrapper& background) = 0;
    virtual void remove() = 0;
    virtual void reorder_child(IFixedLayoutWrapper& child, int position) = 0;
};
