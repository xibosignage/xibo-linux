#include "SubmitLog.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitLog;

std::string SubmitLog::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, logXml);
}

void SubmitLog::Response::parseBody(const XmlNode& node)
{
    success = node.get<bool>(Resources::Success);
}
