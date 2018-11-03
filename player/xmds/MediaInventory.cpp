#include "MediaInventory.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const MediaInventory::Request& request)
{
    return createRequest<MediaInventory::Request>(request.serverKey, request.hardwareKey, request.mediaInventory);
}

template<>
MediaInventory::Response soap::createResponse(const std::string& soapResponse)
{
    MediaInventory::Response result;
    result.success = xmds::parseSuccessResponse(soapResponse);
    return result;
}
