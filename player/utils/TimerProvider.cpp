#include "TimerProvider.hpp"

#include <glibmm/main.h>

void TimerProvider::start(unsigned int, std::function<bool()>)
{

}

void TimerProvider::startSeconds(unsigned int, std::function<bool()>)
{

}

void TimerProvider::startOnce(unsigned int msesc, std::function<void()> handler)
{
    if(!m_active)
    {
        m_active = true;

        Glib::signal_timeout().connect_once([=](){
            m_active = false;
            handler();
        }, msesc);
    }
}

void TimerProvider::startOnceSeconds(unsigned int secs, std::function<void()> handler)
{
    if(!m_active)
    {
        m_active = true;

        Glib::signal_timeout().connect_seconds_once([=](){
            m_active = false;
            handler();
        }, secs);
    }
}

void TimerProvider::stop()
{

}

bool TimerProvider::active() const
{
    return m_active;
}
