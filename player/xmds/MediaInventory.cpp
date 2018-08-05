#include "MediaInventory.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const MediaInventory::request& request)
{
    return create_request<MediaInventory::request>(request.server_key, request.hardware_key, request.media_inventory);
}

template<>
MediaInventory::response soap::create_response(const std::string& soap_response)
{
    MediaInventory::response result;
    result.success = xmds::parse_success_response(soap_response);
    return result;
}
