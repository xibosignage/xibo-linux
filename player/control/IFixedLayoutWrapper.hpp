#pragma once

#include <gtkmm/widget.h>

class IFixedLayoutWrapper
{
public:
    virtual ~IFixedLayoutWrapper() = default;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setSize(int width, int height) = 0;
    virtual void addChild(Gtk::Widget& widget, int left, int top) = 0;
    virtual void show() = 0;
};
