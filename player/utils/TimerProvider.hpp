#include "ITimerProvider.hpp"

class TimerProvider : public ITimerProvider
{
public:
    void start(unsigned int msec, std::function<void()> handler) override;
    void startOnce(unsigned int msec, std::function<void()> handler) override;
    void stop() override;

};
