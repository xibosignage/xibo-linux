#pragma once

#include <string>
#include <spdlog/common.h>

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
    spdlog::level::level_enum logLevel;
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
