#include "SubmitLog.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitLog;

Soap::RequestSerializer<SubmitLog::Request>::RequestSerializer(const SubmitLog::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<SubmitLog::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().logXml);
}

Soap::ResponseParser<SubmitLog::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitLog::Result Soap::ResponseParser<SubmitLog::Result>::parseBody(const XmlNode& node)
{
    SubmitLog::Result result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}
