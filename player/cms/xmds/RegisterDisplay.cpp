#include "RegisterDisplay.hpp"

#include "cms/xmds/Resources.hpp"
#include "common/Utils.hpp"

namespace Resources = XmdsResources::RegisterDisplay;
namespace Settings = XmdsResources::RegisterDisplay::Settings;

template <>
std::string Utils::toString(RegisterDisplay::Response::Status::Code val)
{
    switch (val)
    {
        case RegisterDisplay::Response::Status::Code::Ready: return "Ready";
        case RegisterDisplay::Response::Status::Code::Added: return "Added";
        case RegisterDisplay::Response::Status::Code::Waiting: return "Waiting";
        case RegisterDisplay::Response::Status::Code::Invalid: return "Invalid";
    }

    return "unknown";
}

Soap::ResponseParser<RegisterDisplay::Response>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

RegisterDisplay::Response Soap::ResponseParser<RegisterDisplay::Response>::parseBody(const XmlNode& node)
{
    auto activationMessage = node.get<std::string>(Resources::ActivationMessage);
    auto displayNode = Parsing::xmlFrom(activationMessage).get_child(Resources::Display);
    auto attrs = displayNode.get_child(Resources::DisplayAttrs);

    RegisterDisplay::Response result;
    result.status.code = static_cast<RegisterDisplay::Response::Status::Code>(attrs.get<int>(Resources::Status));
    result.status.message = attrs.get<std::string>(Resources::StatusMessage);
    if (result.status.code == RegisterDisplay::Response::Status::Code::Ready)
    {
        result.playerSettings.collectInterval().setValue(displayNode.get<int>(Settings::CollectInterval));
        result.playerSettings.statsEnabled().setValue(displayNode.get<bool>(Settings::StatsEnabled));
        result.playerSettings.xmrNetworkAddress().setValue(displayNode.get<std::string>(Settings::XmrNetworkAddress));
        int width = static_cast<int>(displayNode.get<double>(Settings::Width));
        int height = static_cast<int>(displayNode.get<double>(Settings::Height));
        result.playerSettings.size().setValue(width, height);
        int x = static_cast<int>(displayNode.get<double>(Settings::XPos));
        int y = static_cast<int>(displayNode.get<double>(Settings::YPos));
        result.playerSettings.position().setValue(x, y);
        result.playerSettings.logLevel().setValue(displayNode.get<std::string>(Settings::LogLevel));
        result.playerSettings.screenshotInterval().setValue(displayNode.get<int>(Settings::ScreenShotRequestInterval));
        result.playerSettings.embeddedServerPort().setValue(
            displayNode.get<unsigned short>(Settings::EmbeddedServerPort));
        result.playerSettings.preventSleep().setValue(displayNode.get<bool>(Settings::PreventSleep));
        result.playerSettings.displayName().setValue(displayNode.get<std::string>(Settings::DisplayName));
    }

    return result;
}

std::string RegisterDisplay::Request::string() const
{
    return toSoapString(Resources::Name,
                        clientCode,
                        clientType,
                        clientVersion,
                        displayName,
                        macAddress,
                        xmrChannel,
                        xmrPubKey,
                        serverKey,
                        hardwareKey);
}
