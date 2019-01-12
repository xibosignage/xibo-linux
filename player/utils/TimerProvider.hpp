#pragma once

#include "ITimerProvider.hpp"

#include <sigc++/connection.h>

class TimerProvider : public ITimerProvider
{
public:
    TimerProvider() = default;
    ~TimerProvider() override;

    void start(unsigned int msecs, std::function<bool()> handler) override;
    void startSeconds(unsigned int secs, std::function<bool()> handler) override;
    void startOnce(unsigned int msecs, std::function<void()> handler) override;
    void startOnceSeconds(unsigned int secs, std::function<void()> handler) override;
    void stop() override;
    bool active() const override;

private:
    bool onTimeout(std::function<void()> handler);

private:
    bool m_active = false;
    sigc::connection m_activeTimer;

};
