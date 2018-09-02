#include "GetResource.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const GetResource::Request& request)
{
    return createRequest<GetResource::Request>(request.serverKey, request.hardwareKey, request.layoutId,
                                               request.regionId, request.mediaId);
}

template<>
GetResource::Response soap::createResponse(const std::string& soapResponse)
{
    GetResource::Response result;
    result.resource = xmds::parseFileResponse(soapResponse); // NOTE check in Postman
    return result;
}
