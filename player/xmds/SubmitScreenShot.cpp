#include "SubmitScreenShot.hpp"
#include "Resources.hpp"

namespace Resources = XMDSResources::SubmitScreenShot;

SOAP::RequestSerializer<SubmitScreenShot::Request>::RequestSerializer(const SubmitScreenShot::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<SubmitScreenShot::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().screenShot);
}

SOAP::ResponseParser<SubmitScreenShot::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitScreenShot::Result SOAP::ResponseParser<SubmitScreenShot::Result>::doParse(const xml_node& node)
{
    SubmitScreenShot::Result result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}

