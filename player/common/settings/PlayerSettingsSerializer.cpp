#include "PlayerSettingsSerializer.hpp"
#include "networking/xmds/Resources.hpp"

void PlayerSettingsSerializer::loadFrom(const FilePath& file, PlayerSettings& settings)
{
    loadFromImpl(file, settings.width, settings.height, settings.x, settings.y, settings.logLevel, settings.displayName,
                 settings.preventSleep, settings.statsEnabled, settings.screenshotSize, settings.collectInterval,
                 settings.downloadEndWindow, settings.xmrNetworkAddress, settings.embeddedServerPort,
                 settings.maxLogFilesUploads, settings.screenshotInterval, settings.statusLayoutUpdate,
                 settings.downloadStartWindow, settings.screenshotRequested, settings.shellCommandsEnabled,
                 settings.maxConcurrentDownloads, settings.modifiedLayoutsEnabled);
}

void PlayerSettingsSerializer::loadFrom(const ptree_node& node, PlayerSettings& settings)
{
    namespace Settings = XmdsResources::RegisterDisplay::Settings;

    settings.collectInterval = node.get<int>(Settings::CollectInterval);
    settings.downloadStartWindow = node.get<std::string>(Settings::DownloadStartWindow);
    settings.downloadEndWindow = node.get<std::string>(Settings::DownloadEndWindow);
    settings.statsEnabled = node.get<bool>(Settings::StatsEnabled);
    settings.xmrNetworkAddress = node.get<std::string>(Settings::XmrNetworkAddress);
    settings.width = static_cast<int>(node.get<double>(Settings::Width));
    settings.height = static_cast<int>(node.get<double>(Settings::Height));
    settings.x = static_cast<int>(node.get<double>(Settings::XPos));
    settings.y = static_cast<int>(node.get<double>(Settings::YPos));
    settings.logLevel = toLogLevelEnum(node.get<std::string>(Settings::LogLevel));
    settings.shellCommandsEnabled = node.get<bool>(Settings::EnableShellCommands);
    settings.modifiedLayoutsEnabled = node.get<bool>(Settings::ExpireModifiedLayouts);
    settings.maxConcurrentDownloads = node.get<int>(Settings::MaxConcurrentDownloads);
    // shellCommandAllowList
    settings.statusLayoutUpdate = node.get<bool>(Settings::SendCurrentLayoutAsStatusUpdate);
    settings.screenshotInterval = node.get<int>(Settings::ScreenShotRequestInterval);
    settings.screenshotSize = node.get<int>(Settings::ScreenShotSize);
    settings.maxLogFilesUploads = node.get<int>(Settings::MaxLogFileUploads);
    settings.embeddedServerPort = node.get<unsigned short>(Settings::EmbeddedServerPort);
    settings.preventSleep = node.get<bool>(Settings::PreventSleep);
    settings.displayName = node.get<std::string>(Settings::DisplayName);
    settings.screenshotRequested = node.get<bool>(Settings::ScreenShotRequested);
}

LoggingLevel PlayerSettingsSerializer::toLogLevelEnum(const std::string& level)
{
    if (level == "trace")
        return LoggingLevel::Trace;
    else if (level == "audit")
        return LoggingLevel::Debug;
    else if (level == "info")
        return LoggingLevel::Info;
    else if (level == "error")
        return LoggingLevel::Error;

    return LoggingLevel::Error;
}

void PlayerSettingsSerializer::saveTo(const FilePath& file, const PlayerSettings& settings)
{
    saveToImpl(file, settings.width, settings.height, settings.x, settings.y, settings.logLevel, settings.displayName,
               settings.preventSleep, settings.statsEnabled, settings.screenshotSize, settings.collectInterval,
               settings.downloadEndWindow, settings.xmrNetworkAddress, settings.embeddedServerPort,
               settings.maxLogFilesUploads, settings.screenshotInterval, settings.statusLayoutUpdate,
               settings.downloadStartWindow, settings.screenshotRequested, settings.shellCommandsEnabled,
               settings.maxConcurrentDownloads, settings.modifiedLayoutsEnabled);
}
