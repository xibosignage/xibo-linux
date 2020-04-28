#include "SubmitScreenShot.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitScreenShot;

std::string SubmitScreenshot::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, screenShot);
}

void SubmitScreenshot::Response::parseBody(const XmlNode& node)
{
    success = node.get<bool>(Resources::Success);
}
