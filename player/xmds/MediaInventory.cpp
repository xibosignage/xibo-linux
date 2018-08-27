#include "MediaInventory.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const MediaInventory::Request& request)
{
    return create_request<MediaInventory::Request>(request.server_key, request.hardware_key, request.media_inventory);
}

template<>
MediaInventory::Response soap::create_response(const std::string& soap_response)
{
    MediaInventory::Response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
