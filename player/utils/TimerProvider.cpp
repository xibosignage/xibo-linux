#include "TimerProvider.hpp"

#include <glibmm/main.h>

TimerProvider::~TimerProvider()
{
    stop();
}

void TimerProvider::start(unsigned int, std::function<bool()>)
{

}

void TimerProvider::startSeconds(unsigned int, std::function<bool()>)
{

}

void TimerProvider::startOnce(unsigned int msecs, std::function<void()> handler)
{
    if(!m_active)
    {
        m_active = true;

        m_activeTimer = Glib::signal_timeout().connect([=](){
            return onTimeout(handler);
        }, msecs);
    }
}

void TimerProvider::startOnceSeconds(unsigned int secs, std::function<void()> handler)
{
    if(!m_active)
    {
        m_active = true;

        m_activeTimer = Glib::signal_timeout().connect_seconds([=](){
            return onTimeout(handler);
        }, secs);
    }
}

bool TimerProvider::onTimeout(std::function<void()> handler)
{
    m_active = false;
    handler();
    return false;
}

void TimerProvider::stop()
{
    if(m_active)
    {
        m_activeTimer.disconnect();
    }
}

bool TimerProvider::active() const
{
    return m_active;
}
