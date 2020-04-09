#include "Schedule.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::Schedule;

Soap::RequestSerializer<Schedule::Request>::RequestSerializer(const Schedule::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<Schedule::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey);
}

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
