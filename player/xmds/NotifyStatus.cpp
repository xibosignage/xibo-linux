#include "NotifyStatus.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const NotifyStatus::request& request)
{
    return create_request<NotifyStatus::request>(request.server_key, request.hardware_key, request.status);
}

template<>
NotifyStatus::response soap::create_response(const std::string& soap_response)
{
    NotifyStatus::response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
