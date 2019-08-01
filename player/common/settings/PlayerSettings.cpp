#include "PlayerSettings.hpp"

void PlayerSettings::loadFrom(const FilePath& settingsFile)
{
    loadFromImpl(settingsFile,
                 m_width,
                 m_height,
                 m_x,
                 m_y,
                 m_logLevel,
                 m_displayName,
                 m_preventSleep,
                 m_statsEnabled,
                 m_screenshotSize,
                 m_collectInterval,
                 m_downloadEndWindow,
                 m_xmrNetworkAddress,
                 m_embeddedServerPort,
                 m_maxLogFilesUploads,
                 m_screenshotInterval,
                 m_statusLayoutUpdate,
                 m_downloadStartWindow,
                 m_screenshotRequested,
                 m_shellCommandsEnabled,
                 m_maxConcurrentDownloads,
                 m_modifiedLayoutsEnabled);
}

void PlayerSettings::saveTo(const FilePath& settingsFile)
{
    saveToImpl(settingsFile,
               m_width,
               m_height,
               m_x,
               m_y,
               m_logLevel,
               m_displayName,
               m_preventSleep,
               m_statsEnabled,
               m_screenshotSize,
               m_collectInterval,
               m_downloadEndWindow,
               m_xmrNetworkAddress,
               m_embeddedServerPort,
               m_maxLogFilesUploads,
               m_screenshotInterval,
               m_statusLayoutUpdate,
               m_downloadStartWindow,
               m_screenshotRequested,
               m_shellCommandsEnabled,
               m_maxConcurrentDownloads,
               m_modifiedLayoutsEnabled);
}

int PlayerSettings::collectInterval() const
{
    return m_collectInterval;
}

void PlayerSettings::setCollectInterval(int collectInterval)
{
    m_collectInterval = collectInterval;
}

std::string PlayerSettings::downloadStartWindow() const
{
    return m_downloadStartWindow;
}

void PlayerSettings::setDownloadStartWindow(const std::string& downloadStartWindow)
{
    m_downloadStartWindow = downloadStartWindow;
}

std::string PlayerSettings::downloadEndWindow() const
{
    return m_downloadEndWindow;
}

void PlayerSettings::setDownloadEndWindow(const std::string& downloadEndWindow)
{
    m_downloadEndWindow = downloadEndWindow;
}

bool PlayerSettings::statsEnabled() const
{
    return m_statsEnabled;
}

void PlayerSettings::setStatsEnabled(bool statsEnabled)
{
    m_statsEnabled = statsEnabled;
}

std::string PlayerSettings::xmrNetworkAddress() const
{
    return m_xmrNetworkAddress;
}

void PlayerSettings::setXmrNetworkAddress(const std::string& xmrNetworkAddress)
{
    m_xmrNetworkAddress = xmrNetworkAddress;
}

PlayerSettings::Dimensions PlayerSettings::dimensions() const
{
    return Dimensions{m_width, m_height, m_x, m_y};
}

void PlayerSettings::setDimensions(const Dimensions& dimensions)
{
    m_width = dimensions.width;
    m_height = dimensions.height;
    m_x = dimensions.x;
    m_y = dimensions.y;
}

LoggingLevel PlayerSettings::logLevel() const
{
    return m_logLevel;
}

void PlayerSettings::setLogLevel(LoggingLevel logLevel)
{
    m_logLevel = logLevel;
}

bool PlayerSettings::shellCommandsEnabled() const
{
    return m_shellCommandsEnabled;
}

void PlayerSettings::setShellCommandsEnabled(bool shellCommandsEnabled)
{
    m_shellCommandsEnabled = shellCommandsEnabled;
}

bool PlayerSettings::modifiedLayoutsEnabled() const
{
    return m_modifiedLayoutsEnabled;
}

void PlayerSettings::setModifiedLayoutsEnabled(bool modifiedLayoutsEnabled)
{
    m_modifiedLayoutsEnabled = modifiedLayoutsEnabled;
}

int PlayerSettings::maxConcurrentDownloads() const
{
    return m_maxConcurrentDownloads;
}

void PlayerSettings::setMaxConcurrentDownloads(int maxConcurrentDownloads)
{
    m_maxConcurrentDownloads = maxConcurrentDownloads;
}

bool PlayerSettings::statusLayoutUpdate() const
{
    return m_statusLayoutUpdate;
}

void PlayerSettings::setStatusLayoutUpdate(bool statusLayoutUpdate)
{
    m_statusLayoutUpdate = statusLayoutUpdate;
}

int PlayerSettings::screenshotInterval() const
{
    return m_screenshotInterval;
}

void PlayerSettings::setScreenshotInterval(int screenshotInterval)
{
    m_screenshotInterval = screenshotInterval;
}

int PlayerSettings::screenshotSize() const
{
    return m_screenshotSize;
}

void PlayerSettings::setScreenshotSize(int screenshotSize)
{
    m_screenshotSize = screenshotSize;
}

int PlayerSettings::maxLogFilesUploads() const
{
    return m_maxLogFilesUploads;
}

void PlayerSettings::setMaxLogFilesUploads(int maxLogFilesUploads)
{
    m_maxLogFilesUploads = maxLogFilesUploads;
}

unsigned short PlayerSettings::embeddedServerPort() const
{
    return m_embeddedServerPort;
}

void PlayerSettings::setEmbeddedServerPort(unsigned short embeddedServerPort)
{
    m_embeddedServerPort = embeddedServerPort;
}

bool PlayerSettings::preventSleep() const
{
    return m_preventSleep;
}

void PlayerSettings::setPreventSleep(bool preventSleep)
{
    m_preventSleep = preventSleep;
}

std::string PlayerSettings::displayName() const
{
    return m_displayName;
}

void PlayerSettings::setDisplayName(const std::string& displayName)
{
    m_displayName = displayName;
}

bool PlayerSettings::screenshotRequested() const
{
    return m_screenshotRequested;
}

void PlayerSettings::setScreenshotRequested(bool screenshotRequested)
{
    m_screenshotRequested = screenshotRequested;
}
