#include "SubmitStats.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitStats;

std::string SubmitStats::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, statXml);
}

void SubmitStats::Response::parseBody(const XmlNode& node)
{
    success = node.get<bool>(Resources::Success);
}
