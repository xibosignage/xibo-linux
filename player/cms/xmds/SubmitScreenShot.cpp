#include "SubmitScreenShot.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitScreenShot;

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

std::string SubmitScreenshot::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, screenShot);
}
