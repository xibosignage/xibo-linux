#include "ScreenShotInterval.hpp"

#include "common/logger/Logging.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"
#include "screenshot/ScreenShoter.hpp"

const int DefaultInterval = 0;

ScreenShotInterval::ScreenShotInterval(XmdsRequestSender& sender, ScreenShoter& screenShoter) :
    sender_(sender),
    screenShoter_(screenShoter),
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
    screenShoter_.takeBase64([this](const std::string& screenShot) {
        if (!screenShot.empty())
        {
            submitScreenShot(screenShot);
        }
    });
}

void ScreenShotInterval::submitScreenShot(const std::string& screenShot)
{
    sender_.submitScreenShot(screenShot).then([](auto future) {
        auto [error, result] = future.get();
        if (error)
        {
            Log::error("[XMDS::SubmitScreenShot] {}", error);
        }
        else
        {
            std::string message = result.success ? "Submitted" : "Not submitted";
            Log::debug("[XMDS::SubmitScreenShot] {}", message);
        }
    });
}
