#pragma once

#include "cms/SubmitScreenshotCommand.hpp"
#include "common/dt/Timer.hpp"
#include "config/CmsSettings.hpp"
#include "control/widgets/Window.hpp"

#include <memory>

class ScreenShoter;
class SubmitScreenshotCommand;

class ScreenShotInterval
{
public:
    ScreenShotInterval(const CmsSettings& settings, Xibo::Window& window);

    void updateInterval(int interval);
    void takeScreenShot();

private:
    void restartTimer();
    void submitScreenShot(const std::string& screenShot);

private:
    std::unique_ptr<ScreenShoter> screenShoter_;
    std::unique_ptr<SubmitScreenshotCommand> screenshotCommand_;
    CmsSettings settings_;
    int interval_;
    Timer timer_;
};
