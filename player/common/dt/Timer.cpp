#include "Timer.hpp"

Timer::~Timer()
{
    stop();
}

void Timer::stop()
{
    if (active())
    {
        m_timerConnection.disconnect();
    }
}

bool Timer::active() const
{
    return m_timerConnection.connected();
}
