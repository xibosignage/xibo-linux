#include "GetResource.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::GetResource;

std::string GetResource::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, layoutId, regionId, mediaId);
}

void GetResource::Response::parseBody(const XmlNode& node)
{
    resource = node.get<std::string>(Resources::Resource);
}
