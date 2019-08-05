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
    auto display = Utils::parseXmlFromString(activationMessage).get_child(Resources::Display);
    auto attrs = display.get_child(Resources::DisplayAttrs);

    RegisterDisplay::Result result;
    result.status.code = static_cast<RegisterDisplay::Result::Status::Code>(attrs.get<int>(Resources::Status));
    result.status.message = attrs.get<std::string>(Resources::StatusMessage);
    if(result.status.code == RegisterDisplay::Result::Status::Code::Ready)
    {
        fillPlayerSettings(result.playerSettings, display);
    }

    return result;
}

void Soap::ResponseParser<RegisterDisplay::Result>::fillPlayerSettings(PlayerSettings& settings, const xml_node& display)
{
    namespace Settings = Resources::Settings;

    settings.setCollectInterval(display.get<int>(Settings::CollectInterval));
    settings.setDownloadStartWindow(display.get<std::string>(Settings::DownloadStartWindow));
    settings.setDownloadStartWindow(display.get<std::string>(Settings::DownloadEndWindow));
    settings.setStatsEnabled(display.get<bool>(Settings::StatsEnabled));
    settings.setXmrNetworkAddress(display.get<std::string>(Settings::XmrNetworkAddress));
    int width = static_cast<int>(display.get<double>(Settings::Width));
    int height = static_cast<int>(display.get<double>(Settings::Height));
    int x = static_cast<int>(display.get<double>(Settings::XPos));
    int y = static_cast<int>(display.get<double>(Settings::YPos));
    settings.setDimensions(PlayerSettings::Dimensions{width, height, x, y});
    settings.setLogLevel(toLogLevelEnum(display.get<std::string>(Settings::LogLevel)));
    settings.setShellCommandsEnabled(display.get<bool>(Settings::EnableShellCommands));
    settings.setModifiedLayoutsEnabled(display.get<bool>(Settings::ExpireModifiedLayouts));
    settings.setMaxConcurrentDownloads(display.get<int>(Settings::MaxConcurrentDownloads));
    //shellCommandAllowList
    settings.setStatusLayoutUpdate(display.get<bool>(Settings::SendCurrentLayoutAsStatusUpdate));
    settings.setScreenshotInterval(display.get<int>(Settings::ScreenShotRequestInterval));
    settings.setScreenshotSize(display.get<int>(Settings::ScreenShotSize));
    settings.setMaxLogFilesUploads(display.get<int>(Settings::MaxLogFileUploads));
    settings.setEmbeddedServerPort(display.get<unsigned short>(Settings::EmbeddedServerPort));
    settings.setPreventSleep(display.get<bool>(Settings::PreventSleep));
    settings.setDisplayName(display.get<std::string>(Settings::DisplayName));
    settings.setScreenshotSize(display.get<bool>(Settings::ScreenShotRequested));
}


LoggingLevel Soap::ResponseParser<RegisterDisplay::Result>::toLogLevelEnum(const std::string& level)
{
    if(level == "trace")
        return LoggingLevel::Trace;
    else if(level == "audit")
        return LoggingLevel::Debug;
    else if(level == "info")
        return LoggingLevel::Info;
    else if(level == "error")
        return LoggingLevel::Error;

    return LoggingLevel::Error;
}
