#pragma once

#include <functional>

class ITimerProvider
{
public:
    virtual ~ITimerProvider() = default;
    virtual void start(unsigned int msecs, std::function<bool()> handler) = 0;
    virtual void startSeconds(unsigned int secs, std::function<bool()> handler) = 0;
    virtual void startOnce(unsigned int msecs, std::function<void()> handler) = 0;
    virtual void startOnceSeconds(unsigned int secs, std::function<void()> handler) = 0;
    virtual void stop() = 0;
    virtual bool active() const = 0;

};
