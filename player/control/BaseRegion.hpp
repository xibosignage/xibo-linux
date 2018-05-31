#pragma once
#include <gtkmm/fixed.h>

struct Size;
struct Point;

class BaseRegion : public Gtk::Fixed
{
public:
    BaseRegion() = default;
    BaseRegion(const BaseRegion&) = delete;
    BaseRegion& operator=(const BaseRegion&) = delete;

    virtual int id() const = 0;
    virtual const Size& size() const = 0;
    virtual const Point& position() const = 0;
    virtual void set_size(int width, int height) = 0;
    virtual int zindex() const = 0;
    virtual bool looped() const = 0;
    virtual void show() = 0;
};
