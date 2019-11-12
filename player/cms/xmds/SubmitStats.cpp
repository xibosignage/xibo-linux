#include "SubmitStats.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::SubmitStats;

Soap::RequestSerializer<SubmitStats::Request>::RequestSerializer(const SubmitStats::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<SubmitStats::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().statXml);
}

Soap::ResponseParser<SubmitStats::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

SubmitStats::Result Soap::ResponseParser<SubmitStats::Result>::parseBody(const XmlNode& node)
{
    SubmitStats::Result result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}
