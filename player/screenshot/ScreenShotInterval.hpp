#pragma once

#include "common/dt/Timer.hpp"

class ScreenShoter;
class XmdsRequestSender;

class ScreenShotInterval
{
public:
    ScreenShotInterval(XmdsRequestSender& sender, ScreenShoter& screenShoter_);

    void start();
    void updateInterval(int interval);

private:
    void restartTimer();
    void takeScreenShot();
    void submitScreenShot(const std::string& screenShot);

private:
    XmdsRequestSender& sender_;
    ScreenShoter& screenShoter_;
    int interval_;
    Timer timer_;
};
