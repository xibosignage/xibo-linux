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

Soap::ResponseParser<GetFile::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

GetFile::Result Soap::ResponseParser<GetFile::Result>::parseBody(const XmlNode& node)
{
    GetFile::Result result;
    result.base64chunk = node.get<std::string>(Resources::FileChunk);
    return result;
}
