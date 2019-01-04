#ifndef PLAYERSETTINGS_HPP
#define PLAYERSETTINGS_HPP

#include <string>

struct PlayerSettings
{
    int collectInterval;
    std::string downloadStartWindow;
    std::string downloadEndWindow;
    bool statsEnabled;
    std::string xmrNetworkAddress;
    double sizeX;
    double sizeY;
    double offsetX;
    double offsetY;
    std::string logLevel;
    bool shellCommandsEnabled;
    bool modifiedLayoutsEnabled;
    int maxConcurrentDownloads;
    bool statusLayoutUpdate;
    int screenshotInterval;
    int screenshotSize;
    int maxLogFilesUploads;
    int embeddedServerPort;
    bool preventSleep;
    std::string displayName;
    bool screenshotRequested;
};

#endif // PLAYERSETTINGS_HPP
