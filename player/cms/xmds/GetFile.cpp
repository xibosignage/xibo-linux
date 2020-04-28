#include "GetFile.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::GetFile;

std::string GetFile::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, fileId, fileType, chunkOffset, chunkSize);
}

void GetFile::Response::parseBody(const XmlNode& node)
{
    base64chunk = node.get<std::string>(Resources::FileChunk);
}
