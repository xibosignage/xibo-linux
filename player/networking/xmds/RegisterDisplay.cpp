#include "RegisterDisplay.hpp"

#include "Resources.hpp"
#include "common/Utils.hpp"

namespace Resources = XmdsResources::RegisterDisplay;

Soap::RequestSerializer<RegisterDisplay::Request>::RequestSerializer(const RegisterDisplay::Request& request) : BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<RegisterDisplay::Request>::string()
{
    return createRequest(Resources::Name,
                         request().clientCode,
                         request().clientType,
                         request().clientVersion,
                         request().displayName,
                         request().macAddress,
                         request().xmrChannel,
                         request().xmrPubKey,
                         request().serverKey,
                         request().hardwareKey);

}

Soap::ResponseParser<RegisterDisplay::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

RegisterDisplay::Result Soap::ResponseParser<RegisterDisplay::Result>::doParse(const xml_node& node)
{
    auto activationMessage = node.get<std::string>(Resources::ActivationMessage);
    auto displayNode = Utils::parseXmlFromString(activationMessage).get_child(Resources::Display);
    auto attrs = displayNode.get_child(Resources::DisplayAttrs);

    RegisterDisplay::Result result;
    result.status.code = static_cast<RegisterDisplay::Result::Status::Code>(attrs.get<int>(Resources::Status));
    result.status.message = attrs.get<std::string>(Resources::StatusMessage);
    if(result.status.code == RegisterDisplay::Result::Status::Code::Ready)
    {
        result.playerSettings.loadFrom(displayNode);
    }

    return result;
}
