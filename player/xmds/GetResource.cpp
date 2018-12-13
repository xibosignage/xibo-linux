#include "GetResource.hpp"

constexpr const std::string_view REQUEST_NAME = "GetResource";

SOAP::RequestSerializer<GetResource::Request>::RequestSerializer(const GetResource::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<GetResource::Request>::string()
{
    return createRequest(REQUEST_NAME, request().serverKey, request().hardwareKey, request().layoutId, request().regionId, request().mediaId);
}

SOAP::ResponseParser<GetResource::Response>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

GetResource::Response SOAP::ResponseParser<GetResource::Response>::get()
{
    GetResource::Response result;
    result.resource = responseTree().get_child("resource").get_value<std::string>(); // TODO check in Postman
    return result;
}
