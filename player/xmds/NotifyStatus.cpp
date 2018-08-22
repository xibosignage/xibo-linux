#include "NotifyStatus.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const NotifyStatus::Request& request)
{
    return create_request<NotifyStatus::Request>(request.server_key, request.hardware_key, request.status);
}

template<>
NotifyStatus::Response soap::create_response(const std::string& soap_response)
{
    NotifyStatus::Response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
