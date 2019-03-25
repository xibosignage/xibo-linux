#include "SubmitScreenShot.hpp"
#include "Resources.hpp"

namespace Resources = XmdsResources::SubmitScreenShot;

Soap::RequestSerializer<SubmitScreenShot::Request>::RequestSerializer(const SubmitScreenShot::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<SubmitScreenShot::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().screenShot);
}

Soap::ResponseParser<SubmitScreenShot::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitScreenShot::Result Soap::ResponseParser<SubmitScreenShot::Result>::doParse(const xml_node& node)
{
    SubmitScreenShot::Result result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}

