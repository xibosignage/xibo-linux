#include "SubmitStats.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const SubmitStats::Request& request)
{
    return create_request<SubmitStats::Request>(request.server_key, request.hardware_key, request.stat_xml);
}

template<>
SubmitStats::Response soap::create_response(const std::string& soap_response)
{
    SubmitStats::Response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
