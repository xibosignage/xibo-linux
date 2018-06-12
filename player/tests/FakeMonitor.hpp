#pragma once

#include "control/IMonitor.hpp"

struct FakeMonitor : IMonitor
{
    FakeMonitor(int _x, int _y, int _width, int _height)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
    Gdk::Rectangle get_area()
    {
        get_area_called = true;
        return Gdk::Rectangle(x, y, width, height);
    }
    bool get_area_called = false;

    int x;
    int y;
    int width;
    int height;
};
