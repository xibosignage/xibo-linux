#include "SubmitScreenshotCommand.hpp"

SubmitScreenshotCommand::SubmitScreenshotCommand(const std::string& host,
                                                 const std::string& serverKey,
                                                 const std::string& hardwareKey,
                                                 const std::string& screenshot) :
    SubmitXmdsCommand(host, serverKey, hardwareKey),
    screenshot_(screenshot)
{
}

std::unique_ptr<SubmitScreenshotCommand> SubmitScreenshotCommand::create(const std::string& host,
                                                                         const std::string& serverKey,
                                                                         const std::string& hardwareKey,
                                                                         const std::string& screenshot)
{
    return std::unique_ptr<SubmitScreenshotCommand>(
        new SubmitScreenshotCommand(host, serverKey, hardwareKey, screenshot));
}

void SubmitScreenshotCommand::prepare(SubmitScreenshot::Request& request)
{
    request.screenShot = std::move(screenshot_);
}
