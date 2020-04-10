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
