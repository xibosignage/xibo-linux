#pragma once

#include "control/BaseRegion.hpp"

class FakeRegion : public BaseRegion
{
public:
    FakeRegion(int zindex) { fake_zindex = zindex; }
    int id() const { return 0; }
    const Size& size() const { }
    const Point& position() const { }
    int zindex() const { return fake_zindex; }
    bool looped() const { return false; }
    void show() { is_shown = true; }
    void set_size(int, int) { }

    int fake_zindex = 0;
    bool is_shown = false;
};
