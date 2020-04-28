#include "GetFile.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::GetFile;

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

std::string GetFile::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, fileId, fileType, chunkOffset, chunkSize);
}
