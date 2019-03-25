#include "PlayerSettingsManager.hpp"

#include "model/PlayerSettings.hpp"
#include "utils/FileSystemAdaptor.hpp"

std::istream& operator>> (std::istream& in, LoggingLevel& level)
{
    int temp;
    in >> temp;
    level = static_cast<LoggingLevel>(temp);
    return in;
}

std::ostream& operator<< (std::ostream& out, LoggingLevel level)
{
    return out << static_cast<int>(level);
}

PlayerSettingsManager::PlayerSettingsManager(const FilePath& settingsFile) :
    m_settingsFile(settingsFile), m_filesystem(std::make_unique<FileSystemAdaptor>())
{
}

PlayerSettings PlayerSettingsManager::load()
{
    PlayerSettings settings;
    loadHelper(settings.sizeX,
               settings.sizeY,
               settings.offsetX,
               settings.offsetY,
               settings.logLevel,
               settings.displayName,
               settings.preventSleep,
               settings.statsEnabled,
               settings.screenshotSize,
               settings.collectInterval,
               settings.downloadEndWindow,
               settings.xmrNetworkAddress,
               settings.embeddedServerPort,
               settings.maxLogFilesUploads,
               settings.screenshotInterval,
               settings.statusLayoutUpdate,
               settings.downloadStartWindow,
               settings.screenshotRequested,
               settings.shellCommandsEnabled,
               settings.maxConcurrentDownloads,
               settings.modifiedLayoutsEnabled);
    return settings;
}

void PlayerSettingsManager::update(const PlayerSettings& settings)
{
    updateHelper(settings.sizeX,
                 settings.sizeY,
                 settings.offsetX,
                 settings.offsetY,
                 settings.logLevel,
                 settings.displayName,
                 settings.preventSleep,
                 settings.statsEnabled,
                 settings.screenshotSize,
                 settings.collectInterval,
                 settings.downloadEndWindow,
                 settings.xmrNetworkAddress,
                 settings.embeddedServerPort,
                 settings.maxLogFilesUploads,
                 settings.screenshotInterval,
                 settings.statusLayoutUpdate,
                 settings.downloadStartWindow,
                 settings.screenshotRequested,
                 settings.shellCommandsEnabled,
                 settings.maxConcurrentDownloads,
                 settings.modifiedLayoutsEnabled);
}
