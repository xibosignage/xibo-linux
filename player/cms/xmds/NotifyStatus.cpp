#include "NotifyStatus.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::NotifyStatus;

Soap::RequestSerializer<NotifyStatus::Request>::RequestSerializer(const NotifyStatus::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<NotifyStatus::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey, request().status);
}

Soap::ResponseParser<NotifyStatus::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

NotifyStatus::Result Soap::ResponseParser<NotifyStatus::Result>::parseBody(const XmlNode& node)
{
    NotifyStatus::Result result;
    result.success = node.get<bool>(Resources::Success);
    return result;
}
