#pragma once

#include "SubmitXmdsCommand.hpp"
#include "cms/xmds/SubmitScreenShot.hpp"

class SubmitScreenshotCommand : public SubmitXmdsCommand<SubmitScreenshot>
{
public:
    static std::unique_ptr<SubmitScreenshotCommand> create(const std::string& host,
                                                           const std::string& serverKey,
                                                           const std::string& hardwareKey,
                                                           const std::string& screenshot);

private:
    SubmitScreenshotCommand(const std::string& host,
                            const std::string& serverKey,
                            const std::string& hardwareKey,
                            const std::string& screenshot);

    void prepare(SubmitScreenshot::Request& request) override;

private:
    std::string screenshot_;
};
