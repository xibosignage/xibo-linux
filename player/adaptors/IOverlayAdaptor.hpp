#pragma once

#include "IWidgetAdaptor.hpp"

class IBackground;
class IMediaContainer;
class IImageAdaptor;
class IFixedLayoutAdaptor;

class IOverlayAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IOverlayAdaptor() = default;
    virtual void show() = 0;
    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void addChild(IWidgetAdaptor& child, int x, int y, int width, int height) = 0;
    virtual void removeChildren() = 0;
    virtual void addMainChild(IWidgetAdaptor& mainChild) = 0;
    virtual void removeMainChild() = 0;
    virtual void reorderChild(IWidgetAdaptor& child, int position) = 0;
};
