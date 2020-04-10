#include "MediaInventory.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::MediaInventory;

Soap::RequestSerializer<MediaInventory::Request>::RequestSerializer(const MediaInventory::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<MediaInventory::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().inventory);
}

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
