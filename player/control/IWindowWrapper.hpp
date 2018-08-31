#pragma once

class IOverlayWrapper;

class IWindowWrapper
{
public:
    virtual ~IWindowWrapper() = default;
    virtual void add(IOverlayWrapper& overlay) = 0;
    virtual void set_default_size(int width, int height) = 0;
    virtual void move(int x, int y) = 0;
    virtual void set_resizable(bool resizable) = 0;
    virtual void set_decorated(bool decorated) = 0;
    virtual void fullscreen() = 0;
    virtual void set_keep_above(bool keep_above) = 0;
};
