#pragma once

#include "common/dt/DateTime.hpp"

namespace Stats
{
    struct PlayingTime
    {
        DateTime started;
        DateTime finished;

        void clear()
        {
            *this = PlayingTime{};
        }
    };
}
