#include "SubmitStats.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const SubmitStats::Request& request)
{
    return createRequest<SubmitStats::Request>(request.serverKey, request.hardwareKey, request.statXml);
}

template<>
SubmitStats::Response soap::createResponse(const std::string& soapResponse)
{
    SubmitStats::Response result;
    result.success = xmds::parseSuccessResponse(soapResponse);
    return result;
}
