#include "Schedule.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::Schedule;

std::string Schedule::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey);
}

void Schedule::Response::parseBody(const XmlNode& node)
{
    scheduleXml = node.get<std::string>(Resources::ScheduleXml);
}
