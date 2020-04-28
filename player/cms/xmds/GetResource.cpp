#include "GetResource.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::GetResource;

Soap::ResponseParser<GetResource::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

GetResource::Response Soap::ResponseParser<GetResource::Response>::parseBody(const XmlNode& node)
{
    GetResource::Response result;
    result.resource = node.get<std::string>(Resources::Resource);
    return result;
}

std::string GetResource::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, layoutId, regionId, mediaId);
}
