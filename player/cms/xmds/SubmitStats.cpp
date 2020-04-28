#include "SubmitStats.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitStats;

Soap::ResponseParser<SubmitStats::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitStats::Response Soap::ResponseParser<SubmitStats::Response>::parseBody(const XmlNode& node)
{
    SubmitStats::Response result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}

std::string SubmitStats::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, statXml);
}
