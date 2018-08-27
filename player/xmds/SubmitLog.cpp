#include "SubmitLog.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const SubmitLog::Request& request)
{
    return create_request<SubmitLog::Request>(request.server_key, request.hardware_key, request.log_xml);
}

template<>
SubmitLog::Response soap::create_response(const std::string& soap_response)
{
    SubmitLog::Response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
