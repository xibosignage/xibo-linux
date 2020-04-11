#include "GetFile.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::GetFile;

Soap::RequestSerializer<GetFile::Request>::RequestSerializer(const GetFile::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<GetFile::Request>::string()
{
    return createRequest(Resources::Name,
                         request().serverKey,
                         request().hardwareKey,
                         request().fileId,
                         request().fileType,
                         request().chunkOffset,
                         request().chunkSize);
}

Soap::ResponseParser<GetFile::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

GetFile::Response Soap::ResponseParser<GetFile::Response>::parseBody(const XmlNode& node)
{
    GetFile::Response result;
    result.base64chunk = node.get<std::string>(Resources::FileChunk);
    return result;
}
