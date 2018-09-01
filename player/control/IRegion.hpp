#pragma once

#include <memory>

class IMedia;
class IFixedLayoutWrapper;

class IRegion
{
public:
    virtual ~IRegion() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void set_size(int width, int height) = 0;

    virtual int left() const = 0;
    virtual int top() const = 0;
    virtual void set_pos(int left, int top) = 0;

    virtual int id() const = 0;
    virtual int zindex() const = 0;
    virtual bool looped() const = 0;
    virtual void show() = 0;

    virtual void add_media(std::unique_ptr<IMedia> media) = 0;
    virtual IFixedLayoutWrapper& handler() = 0;

};
