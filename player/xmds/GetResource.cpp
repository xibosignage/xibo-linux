#include "GetResource.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const GetResource::request& request)
{
    return create_request<GetResource::request>(request.server_key, request.hardware_key, request.layout_id,
                                                request.region_id, request.media_id);
}

template<>
GetResource::response soap::create_response(const std::string& soap_response)
{
    GetResource::response result;
    result.resource = xmds::parse_file_response(soap_response); // NOTE check in Postman
    return result;
}
