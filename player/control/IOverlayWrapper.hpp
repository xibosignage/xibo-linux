#pragma once

#include <gtkmm/fixed.h>
#include <gtkmm/image.h>

class IBackground;
class IRegion;

class IOverlayWrapper
{
public:
    virtual ~IOverlayWrapper() = default;
    virtual void show() = 0;
    virtual void set_size(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void add_child(Gtk::Fixed& child, int top, int left, int width, int height) = 0;
    virtual void add(Gtk::Image& child) = 0;
    virtual void remove() = 0;
    virtual void reorder_overlay(Gtk::Fixed& child, int position) = 0;
};
