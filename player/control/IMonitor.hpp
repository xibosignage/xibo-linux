#pragma once

#include <gdkmm/rectangle.h>

class IMonitor
{
public:
    virtual ~IMonitor() = default;
    virtual Gdk::Rectangle get_area() = 0;
};
