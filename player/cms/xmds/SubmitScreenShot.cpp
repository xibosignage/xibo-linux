#include "SubmitScreenShot.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitScreenShot;

Soap::RequestSerializer<SubmitScreenshot::Request>::RequestSerializer(const SubmitScreenshot::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<SubmitScreenshot::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().screenShot);
}

Soap::ResponseParser<SubmitScreenshot::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitScreenshot::Response Soap::ResponseParser<SubmitScreenshot::Response>::parseBody(const XmlNode& node)
{
    SubmitScreenshot::Response result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}
