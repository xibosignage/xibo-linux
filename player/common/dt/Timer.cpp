#include "Timer.hpp"

Timer::~Timer()
{
    stop();
}

void Timer::stop()
{
    if (active())
    {
        timerConnection_.disconnect();
    }
}

bool Timer::active() const
{
    return timerConnection_.connected();
}
