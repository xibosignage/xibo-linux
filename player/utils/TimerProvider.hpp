#pragma once

#include "ITimerProvider.hpp"

class TimerProvider : public ITimerProvider
{
public:
    void start(unsigned int msecs, std::function<bool()> handler) override;
    void startSeconds(unsigned int secs, std::function<bool()> handler) override;
    void startOnce(unsigned int msecs, std::function<void()> handler) override;
    void startOnceSeconds(unsigned int secs, std::function<void()> handler) override;
    void stop() override;
    bool active() const override;

private:
    bool m_active = false;

};
