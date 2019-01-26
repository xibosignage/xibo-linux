#include "GetFile.hpp"
#include "Resources.hpp"

namespace Resources = XMDSResources::GetFile;

SOAP::RequestSerializer<GetFile::Request>::RequestSerializer(const GetFile::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<GetFile::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().fileId, request().fileType, request().chunkOffset, request().chunkSize);
}

SOAP::ResponseParser<GetFile::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

GetFile::Result SOAP::ResponseParser<GetFile::Result>::doParse(const xml_node& node)
{
    GetFile::Result result;
    result.base64chunk = node.get<std::string>(Resources::FileChunk);
    return result;
}
