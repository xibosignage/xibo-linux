#pragma once

#include <gdkmm/rectangle.h>

class IMonitor
{
public:
    virtual Gdk::Rectangle get_area() = 0;
};
