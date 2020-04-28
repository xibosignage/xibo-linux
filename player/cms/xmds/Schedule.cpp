#include "Schedule.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::Schedule;

Soap::ResponseParser<Schedule::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

Schedule::Response Soap::ResponseParser<Schedule::Response>::parseBody(const XmlNode& scheduleNode)
{
    Schedule::Response result;
    result.scheduleXml = scheduleNode.get<std::string>(Resources::ScheduleXml);
    return result;
}

std::string Schedule::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey);
}
