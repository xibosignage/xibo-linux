#pragma once

#include "common/dt/DateTime.hpp"

struct PlayingStat
{
    DateTime started;
    DateTime finished;

    void clear()
    {
        *this = PlayingStat{};
    }
};
