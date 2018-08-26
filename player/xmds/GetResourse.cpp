#include "GetResourse.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const GetResourse::request& request)
{
    return create_request<GetResourse::request>(request.server_key, request.hardware_key, request.layout_id,
                                                request.region_id, request.media_id);
}

template<>
GetResourse::response soap::create_response(const std::string& soap_response)
{
    GetResourse::response result;
    result.resource = xmds::parse_file_response(soap_response); // NOTE check in Postman
    return result;
}
