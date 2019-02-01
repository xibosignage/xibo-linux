#include "GetResource.hpp"
#include "Resources.hpp"

namespace Resources = XMDSResources::GetResource;

SOAP::RequestSerializer<GetResource::Request>::RequestSerializer(const GetResource::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<GetResource::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().layoutId, request().regionId, request().mediaId);
}

SOAP::ResponseParser<GetResource::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

GetResource::Result SOAP::ResponseParser<GetResource::Result>::doParse(const xml_node& node)
{
    GetResource::Result result;
    result.resource = node.get<std::string>(Resources::Resource);
    return result;
}
