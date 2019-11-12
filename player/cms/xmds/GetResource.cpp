#include "GetResource.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::GetResource;

Soap::RequestSerializer<GetResource::Request>::RequestSerializer(const GetResource::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<GetResource::Request>::string()
{
    return createRequest(Resources::Name,
                         request().serverKey,
                         request().hardwareKey,
                         request().layoutId,
                         request().regionId,
                         request().mediaId);
}

Soap::ResponseParser<GetResource::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

GetResource::Result Soap::ResponseParser<GetResource::Result>::parseBody(const XmlNode& node)
{
    GetResource::Result result;
    result.resource = node.get<std::string>(Resources::Resource);
    return result;
}
