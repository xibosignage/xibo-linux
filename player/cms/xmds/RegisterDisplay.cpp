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

void RegisterDisplay::Response::parseBody(const XmlNode& node)
{
    auto activationMessage = node.get<std::string>(Resources::ActivationMessage);
    auto displayNode = Parsing::xmlFrom(activationMessage).get_child(Resources::Display);
    auto attrs = displayNode.get_child(Resources::DisplayAttrs);

    status.code = static_cast<RegisterDisplay::Response::Status::Code>(attrs.get<int>(Resources::Status));
    status.message = attrs.get<std::string>(Resources::StatusMessage);
    if (status.code == RegisterDisplay::Response::Status::Code::Ready)
    {
        playerSettings.collectInterval().setValue(displayNode.get<int>(Settings::CollectInterval));
        playerSettings.statsEnabled().setValue(displayNode.get<bool>(Settings::StatsEnabled));
        playerSettings.xmrNetworkAddress().setValue(displayNode.get<std::string>(Settings::XmrNetworkAddress));
        int width = static_cast<int>(displayNode.get<double>(Settings::Width));
        int height = static_cast<int>(displayNode.get<double>(Settings::Height));
        playerSettings.size().setValue(width, height);
        int x = static_cast<int>(displayNode.get<double>(Settings::XPos));
        int y = static_cast<int>(displayNode.get<double>(Settings::YPos));
        playerSettings.position().setValue(x, y);
        playerSettings.logLevel().setValue(displayNode.get<std::string>(Settings::LogLevel));
        playerSettings.screenshotInterval().setValue(displayNode.get<int>(Settings::ScreenShotRequestInterval));
        playerSettings.embeddedServerPort().setValue(displayNode.get<unsigned short>(Settings::EmbeddedServerPort));
        playerSettings.preventSleep().setValue(displayNode.get<bool>(Settings::PreventSleep));
        playerSettings.displayName().setValue(displayNode.get<std::string>(Settings::DisplayName));
    }
}
