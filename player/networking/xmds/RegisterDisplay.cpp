#include "RegisterDisplay.hpp"
#include "Resources.hpp"
#include "common/Utils.hpp"

namespace Resources = XmdsResources::RegisterDisplay;

template<>
std::string Utils::toString(RegisterDisplay::Result::Status::Code val)
{
    switch (val)
    {
        case RegisterDisplay::Result::Status::Code::Ready: return "Ready";
        case RegisterDisplay::Result::Status::Code::Added: return "Added";
        case RegisterDisplay::Result::Status::Code::Waiting: return "Waiting";
        case RegisterDisplay::Result::Status::Code::Invalid: return "Invalid";
    }

    return "unknown";
}

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

RegisterDisplay::Result Soap::ResponseParser<RegisterDisplay::Result>::parseBody(const ptree_node& node)
{
    auto activationMessage = node.get<std::string>(Resources::ActivationMessage);
    auto displayNode = Parsing::xmlFromString(activationMessage).get_child(Resources::Display);
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
