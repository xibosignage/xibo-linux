#include "SubmitLog.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitLog;

Soap::ResponseParser<SubmitLog::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitLog::Response Soap::ResponseParser<SubmitLog::Response>::parseBody(const XmlNode& node)
{
    SubmitLog::Response result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}

std::string SubmitLog::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, logXml);
}
