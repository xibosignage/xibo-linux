#include "MediaInventory.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::MediaInventory;

Soap::ResponseParser<MediaInventory::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

MediaInventory::Response Soap::ResponseParser<MediaInventory::Response>::parseBody(const XmlNode& node)
{
    MediaInventory::Response result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}

std::string MediaInventory::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, inventory);
}
