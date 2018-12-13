#include "GetResource.hpp"
#include "Resources.hpp"

namespace Resources = XMDSResources::GetResource;

SOAP::RequestSerializer<GetResource::Request>::RequestSerializer(const GetResource::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<GetResource::Request>::string()
{
    return createRequest(Resources::RequestName, request().serverKey, request().hardwareKey, request().layoutId, request().regionId, request().mediaId);
}

SOAP::ResponseParser<GetResource::Response>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

GetResource::Response SOAP::ResponseParser<GetResource::Response>::get()
{
    GetResource::Response result;
    result.resource = responseTree().get<std::string>(Resources::Resource);
    return result;
}
