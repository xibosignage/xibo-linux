#include "SubmitLog.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const SubmitLog::request& request)
{
    return create_request<SubmitLog::request>(request.server_key, request.hardware_key, request.log_xml);
}

template<>
SubmitLog::response soap::create_response(const std::string& soap_response)
{
    SubmitLog::response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
