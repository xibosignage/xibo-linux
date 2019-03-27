#include "PlayerSettingsManager.hpp"

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

void PlayerSettingsManager::load()
{
    loadHelper(m_settings.cms.host,
               m_settings.cms.serverKey,
               m_settings.cms.hardwareKey,
               m_settings.player.sizeX,
               m_settings.player.sizeY,
               m_settings.player.offsetX,
               m_settings.player.offsetY,
               m_settings.player.logLevel,
               m_settings.player.displayName,
               m_settings.player.preventSleep,
               m_settings.player.statsEnabled,
               m_settings.player.screenshotSize,
               m_settings.player.collectInterval,
               m_settings.player.downloadEndWindow,
               m_settings.player.xmrNetworkAddress,
               m_settings.player.embeddedServerPort,
               m_settings.player.maxLogFilesUploads,
               m_settings.player.screenshotInterval,
               m_settings.player.statusLayoutUpdate,
               m_settings.player.downloadStartWindow,
               m_settings.player.screenshotRequested,
               m_settings.player.shellCommandsEnabled,
               m_settings.player.maxConcurrentDownloads,
               m_settings.player.modifiedLayoutsEnabled);
}

void PlayerSettingsManager::update(const Settings& settings)
{    
    updateHelper(settings.cms.host,
                 settings.cms.serverKey,
                 settings.cms.hardwareKey,
                 settings.player.sizeX,
                 settings.player.sizeY,
                 settings.player.offsetX,
                 settings.player.offsetY,
                 settings.player.logLevel,
                 settings.player.displayName,
                 settings.player.preventSleep,
                 settings.player.statsEnabled,
                 settings.player.screenshotSize,
                 settings.player.collectInterval,
                 settings.player.downloadEndWindow,
                 settings.player.xmrNetworkAddress,
                 settings.player.embeddedServerPort,
                 settings.player.maxLogFilesUploads,
                 settings.player.screenshotInterval,
                 settings.player.statusLayoutUpdate,
                 settings.player.downloadStartWindow,
                 settings.player.screenshotRequested,
                 settings.player.shellCommandsEnabled,
                 settings.player.maxConcurrentDownloads,
                 settings.player.modifiedLayoutsEnabled);

    m_settings = settings;
}

void PlayerSettingsManager::updateCmsSettings(const CmsSettings& cms)
{
    update(Settings{cms, m_settings.player});
}

void PlayerSettingsManager::updatePlayerSettings(const PlayerSettings& player)
{
    update(Settings{m_settings.cms, player});
}

PlayerSettings PlayerSettingsManager::playerSettings()
{
    return m_settings.player;
}

CmsSettings PlayerSettingsManager::cmsSettings()
{
    return m_settings.cms;
}
