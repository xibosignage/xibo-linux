#include "GetFile.hpp"
#include "xmds.hpp"

template<>
std::string soap::request_string(const GetFile::request& request)
{
    return create_request<GetFile::request>(request.server_key, request.hardware_key, request.file_id,
                                            request.file_type, request.chunk_offset, request.chunk_size);
}

template<>
GetFile::response soap::create_response(const std::string& soap_response)
{
    GetFile::response result;
    result.base64chunk = xmds::parse_file_response(soap_response);
    return result;
}
