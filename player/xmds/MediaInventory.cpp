#include "MediaInventory.hpp"
#include "Resources.hpp"

namespace Resources = XMDSResources::MediaInventory;

SOAP::RequestSerializer<MediaInventory::Request>::RequestSerializer(const MediaInventory::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<MediaInventory::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().inventory);
}

SOAP::ResponseParser<MediaInventory::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

MediaInventory::Result SOAP::ResponseParser<MediaInventory::Result>::doParse(const xml_node& node)
{
    MediaInventory::Result result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}
