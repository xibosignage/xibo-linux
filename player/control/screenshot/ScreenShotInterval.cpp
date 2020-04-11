#include "ScreenShotInterval.hpp"

#include "common/logger/Logging.hpp"
#include "control/screenshot/ScreeShoterFactory.hpp"

const int DefaultInterval = 0;

ScreenShotInterval::ScreenShotInterval(const CmsSettings& settings, Xibo::Window& window) :
    screenShoter_(ScreenShoterFactory::create(window)),
    settings_(settings),
    interval_(DefaultInterval)
{
}

void ScreenShotInterval::updateInterval(int interval)
{
    if (interval_ != interval)
    {
        Log::debug("[ScreenShotInterval] Interval updated to {} minutes", interval);
        interval_ = interval;
        restartTimer();
    }
}

void ScreenShotInterval::restartTimer()
{
    timer_.stop();
    if (interval_ != DefaultInterval)
    {
        timer_.start(std::chrono::minutes(interval_), [this]() {
            takeScreenShot();
            return true;
        });
    }
}

void ScreenShotInterval::takeScreenShot()
{
    screenShoter_->takeBase64([this](const std::string& screenShot) {
        if (!screenShot.empty())
        {
            submitScreenShot(screenShot);
        }
    });
}

void ScreenShotInterval::submitScreenShot(const std::string& screenShot)
{
    screenshotCommand_ =
        SubmitScreenshotCommand::create(settings_.address(), settings_.key(), settings_.displayId(), screenShot);

    screenshotCommand_->error().connect(
        [](const PlayerError& error) { Log::error("[XMDS::SubmitScreenShot] {}", error); });
    screenshotCommand_->success().connect([]() { Log::debug("[XMDS::SubmitScreenShot] Success"); });
    screenshotCommand_->failed().connect([]() { Log::debug("[XMDS::SubmitScreenShot] Failed"); });

    screenshotCommand_->execute();
}
