#include "GetScheduleCommand.hpp"

GetScheduleCommand::GetScheduleCommand(const std::string& host,
                                       const std::string& serverKey,
                                       const std::string& hardwareKey) :
    XmdsCommand(host, serverKey, hardwareKey)
{
}

std::unique_ptr<GetScheduleCommand> GetScheduleCommand::create(const std::string& host,
                                                               const std::string& serverKey,
                                                               const std::string& hardwareKey)
{
    return std::unique_ptr<GetScheduleCommand>(new GetScheduleCommand(host, serverKey, hardwareKey));
}

GetScheduleCommand::SignalScheduleReady& GetScheduleCommand::scheduleReady()
{
    return scheduleReady_;
}

void GetScheduleCommand::process(const Schedule::Response& response)
{
    scheduleReady_(LayoutSchedule::fromString(response.scheduleXml));
}
