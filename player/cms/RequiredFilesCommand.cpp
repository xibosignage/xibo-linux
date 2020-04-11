#include "RequiredFilesCommand.hpp"

RequiredFilesCommand::RequiredFilesCommand(const std::string& host,
                                           const std::string& serverKey,
                                           const std::string& hardwareKey) :
    XmdsCommand(host, serverKey, hardwareKey)
{
}

std::unique_ptr<RequiredFilesCommand> RequiredFilesCommand::create(const std::string& host,
                                                                   const std::string& serverKey,
                                                                   const std::string& hardwareKey)
{
    return std::unique_ptr<RequiredFilesCommand>(new RequiredFilesCommand(host, serverKey, hardwareKey));
}

RequiredFilesCommand::SignalFilesReady& RequiredFilesCommand::filesReady()
{
    return filesReady_;
}

void RequiredFilesCommand::process(const RequiredFiles::Response& response)
{
    filesReady_(response.requiredFiles(), response.requiredResources());
}
