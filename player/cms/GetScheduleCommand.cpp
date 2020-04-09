#include "GetScheduleCommand.hpp"

GetScheduleCommand::GetScheduleCommand(const std::string& host,
                                       const std::string& serverKey,
                                       const std::string& hardwareKey) :
    XmdsCommand(host),
    serverKey_(serverKey),
    hardwareKey_(hardwareKey)
{
}

GetScheduleCommand::SignalScheduleReady& GetScheduleCommand::scheduleReady()
{
    return scheduleReady_;
}

Schedule::Request GetScheduleCommand::prepareRequest()
{
    Schedule::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    return request;
}

void GetScheduleCommand::processResponse(const Schedule::Response& response)
{
    scheduleReady_(LayoutSchedule::fromString(response.scheduleXml));
}
