#pragma once

#include <functional>

class ITimerProvider
{
public:
    virtual ~ITimerProvider() = default;
    virtual void start(unsigned int msec, std::function<void()> handler) = 0;
    virtual void startOnce(unsigned int msec, std::function<void()> handler) = 0;
    virtual void stop() = 0;

};
