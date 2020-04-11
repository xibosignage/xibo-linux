#pragma once

#include "common/dt/Timer.hpp"
#include "control/widgets/Window.hpp"

#include <memory>

class ScreenShoter;

class ScreenShotInterval
{
public:
    ScreenShotInterval(Xibo::Window& window);

    void updateInterval(int interval);
    void takeScreenShot();

private:
    void restartTimer();
    void submitScreenShot(const std::string& screenShot);

private:
    std::unique_ptr<ScreenShoter> screenShoter_;
    int interval_;
    Timer timer_;
};
