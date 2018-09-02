#pragma once

class IOverlayWrapper;

class IWindowWrapper
{
public:
    virtual ~IWindowWrapper() = default;
    virtual void add(IOverlayWrapper& overlay) = 0;
    virtual void setDefaultSize(int width, int height) = 0;
    virtual void move(int x, int y) = 0;
    virtual void setResizable(bool resizable) = 0;
    virtual void setDecorated(bool decorated) = 0;
    virtual void fullscreen() = 0;
    virtual void setKeepAbove(bool keep_above) = 0;
};
