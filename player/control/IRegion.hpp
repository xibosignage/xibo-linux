#pragma once

#include <memory>

class IMedia;
class IFixedLayoutAdaptor;

class IRegion
{
public:
    virtual ~IRegion() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;

    virtual int left() const = 0;
    virtual int top() const = 0;
    virtual void setPos(int left, int top) = 0;

    virtual int id() const = 0;
    virtual int zindex() const = 0;
    virtual bool looped() const = 0;
    virtual void show() = 0;

    virtual void addMedia(std::unique_ptr<IMedia> media) = 0;
    virtual IFixedLayoutAdaptor& handler() = 0;

};
