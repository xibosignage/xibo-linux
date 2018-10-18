#include "TimerProvider.hpp"

#include <glibmm/main.h>

void TimerProvider::start(unsigned int msec, std::function<void ()> handler)
{

}

void TimerProvider::startOnce(unsigned int msec, std::function<void ()> handler)
{
    Glib::signal_timeout().connect_once(handler, msec);
}

void TimerProvider::stop()
{

}
