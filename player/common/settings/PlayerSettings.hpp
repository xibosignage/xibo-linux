#pragma once

#include "Settings.hpp"
#include "../logger/LoggingLevel.hpp"

class PlayerSettings : public Settings
{
public:
    struct Dimensions
    {
        int width;
        int height;
        int x;
        int y;
    };

    void loadFrom(const FilePath& file) override;
    void saveTo(const FilePath& file) override;

    int collectInterval() const;
    void setCollectInterval(int collectInterval);

    std::string downloadStartWindow() const;
    void setDownloadStartWindow(const std::string& downloadStartWindow);

    std::string downloadEndWindow() const;
    void setDownloadEndWindow(const std::string& downloadEndWindow);

    bool statsEnabled() const;
    void setStatsEnabled(bool statsEnabled);

    std::string xmrNetworkAddress() const;
    void setXmrNetworkAddress(const std::string& xmrNetworkAddress);

    Dimensions dimensions() const;
    void setDimensions(const Dimensions& dimensions);

    LoggingLevel logLevel() const;
    void setLogLevel(LoggingLevel logLevel);

    bool shellCommandsEnabled() const;
    void setShellCommandsEnabled(bool shellCommandsEnabled);

    bool modifiedLayoutsEnabled() const;
    void setModifiedLayoutsEnabled(bool modifiedLayoutsEnabled);

    int maxConcurrentDownloads() const;
    void setMaxConcurrentDownloads(int maxConcurrentDownloads);

    bool statusLayoutUpdate() const;
    void setStatusLayoutUpdate(bool statusLayoutUpdate);

    int screenshotInterval() const;
    void setScreenshotInterval(int screenshotInterval);

    int screenshotSize() const;
    void setScreenshotSize(int screenshotSize);

    int maxLogFilesUploads() const;
    void setMaxLogFilesUploads(int maxLogFilesUploads);

    unsigned short embeddedServerPort() const;
    void setEmbeddedServerPort(unsigned short embeddedServerPort);

    bool preventSleep() const;
    void setPreventSleep(bool preventSleep);

    std::string displayName() const;
    void setDisplayName(const std::string& displayName);

    bool screenshotRequested() const;
    void setScreenshotRequested(bool screenshotRequested);

private:
    Field<int> m_collectInterval{"collectInterval", 900};
    Field<std::string> m_downloadStartWindow{"downloadStartWindow", "00:00"};
    Field<std::string> m_downloadEndWindow{"downloadEndWindow", "00:00"};
    Field<bool> m_statsEnabled{"statsEnabled", false};
    Field<std::string> m_xmrNetworkAddress{"xmrNetworkAddress"};
    Field<int> m_width{"sizeX", 0};
    Field<int> m_height{"sizeY", 0};
    Field<int> m_x{"offsetX", 0};
    Field<int> m_y{"offsetY", 0};
    Field<LoggingLevel> m_logLevel{"logLevel", LoggingLevel::Debug};
    Field<bool> m_shellCommandsEnabled{"shellCommandsEnabled", false};
    Field<bool> m_modifiedLayoutsEnabled{"modifiedLayoutsEnabled", false};
    Field<int> m_maxConcurrentDownloads{"maxConcurrentDownloads", 2};
    Field<bool> m_statusLayoutUpdate{"statusLayoutUpdate", false};
    Field<int> m_screenshotInterval{"screenshotInterval", 0};
    Field<int> m_screenshotSize{"screenshotSize", 0};
    Field<int> m_maxLogFilesUploads{"maxLogFilesUploads", 1};
    Field<unsigned short> m_embeddedServerPort{"embeddedServerPort", 1234};
    Field<bool> m_preventSleep{"preventSleep", false};
    Field<std::string> m_displayName{"displayName"};
    Field<bool> m_screenshotRequested{"screenshotRequested", false};

};

