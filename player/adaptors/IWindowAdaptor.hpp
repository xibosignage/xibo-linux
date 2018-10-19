#pragma once

#include "IWidgetAdaptor.hpp"

class IOverlayAdaptor;

class IWindowAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IWindowAdaptor() = default;
    virtual void add(IWidgetAdaptor& child) = 0;
    virtual void move(int x, int y) = 0;
    virtual void disableWindowResize() = 0;
    virtual void disableWindowDecoration() = 0;
    virtual void setKeepAbove(bool keepAbove) = 0;
    virtual void fullscreen() = 0;
    virtual void unfullscreen() = 0;
    virtual void setCursorVisible(bool cursorVisible) = 0;
};
