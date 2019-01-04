#include "Schedule.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const Schedule::request& request)
{
    return createRequest<Schedule::request>(request.serverKey, request.hardwareKey);
}

template<>
Schedule::response soap::createResponse(const std::string& soapResponse)
{
    Schedule::response result;
    result.scheduleXml = soapResponse; // FIXME parse schedule
    return result;
}
