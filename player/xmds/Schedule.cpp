#include "Schedule.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const Schedule::request& request)
{
    return create_request<Schedule::request>(request.server_key, request.hardware_key);
}

template<>
Schedule::response soap::create_response(const std::string& soap_response)
{
    Schedule::response result;
    result.schedule_xml = soap_response; // FIXME parse schedule
    return result;
}
