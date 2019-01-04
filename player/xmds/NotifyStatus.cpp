#include "NotifyStatus.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const NotifyStatus::Request& request)
{
    return createRequest<NotifyStatus::Request>(request.serverKey, request.hardwareKey, request.status);
}

template<>
NotifyStatus::Response soap::createResponse(const std::string& soapResponse)
{
    NotifyStatus::Response result;
    result.success = xmds::parseSuccessResponse(soapResponse);
    return result;
}
