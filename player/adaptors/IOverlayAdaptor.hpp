#pragma once

class IBackground;
class IRegion;
class IImageAdaptor;
class IFixedLayoutAdaptor;

class IOverlayAdaptor
{
public:
    virtual ~IOverlayAdaptor() = default;
    virtual void show() = 0;
    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void addChild(IFixedLayoutAdaptor& child, int top, int left, int width, int height) = 0;
    virtual void removeChildren() = 0;
    virtual void addMainChild(IImageAdaptor& background) = 0;
    virtual void removeMainChild() = 0;
    virtual void reorderChild(IFixedLayoutAdaptor& child, int position) = 0;
};
