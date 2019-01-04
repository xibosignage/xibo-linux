#include "SubmitLog.hpp"
#include "xmds.hpp"

template<>
std::string soap::requestString(const SubmitLog::Request& request)
{
    return createRequest<SubmitLog::Request>(request.serverKey, request.hardwareKey, request.logXml);
}

template<>
SubmitLog::Response soap::createResponse(const std::string& soapResponse)
{
    SubmitLog::Response result;
    result.success = xmds::parseSuccessResponse(soapResponse);
    return result;
}
