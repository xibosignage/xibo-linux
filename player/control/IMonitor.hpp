#pragma once

#include <gdkmm/rectangle.h>

class IMonitor
{
public:
    virtual ~IMonitor() = default;
    virtual Gdk::Rectangle getArea() = 0;
};
