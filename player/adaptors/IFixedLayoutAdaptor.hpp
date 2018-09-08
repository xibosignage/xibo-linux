#pragma once

#include "IWidgetAdaptor.hpp"

class IFixedLayoutAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IFixedLayoutAdaptor() = default;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;
    virtual void addChild(IWidgetAdaptor& child, int left, int top) = 0;
    virtual void removeChildren() = 0;
    virtual void show() = 0;
};
